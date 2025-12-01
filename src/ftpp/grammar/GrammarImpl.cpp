/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GrammarImpl.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 01:12:17 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 00:14:24 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception>
#include <string>

#include "GrammarImpl.hpp"
#include "Grammar.hpp"
#include "FtppException.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/PegLexer.hpp"
#include "peg/PegParser.hpp"
#include "peg/core/Expr.hpp"
#include "peg/visitors/FirstSymbolVisitor.hpp"
#include "peg/visitors/ResolutionVisitor.hpp"
#include "utils/Input.hpp"

// ============================================================================
// Construction: load -> lex -> parse => _rules
// ============================================================================

Grammar::Impl::Impl(const std::string &path, bool checkLeftRecursion)
	: _in(Input::fromFile(path)), _rules()
{
	try
	{
		PegLexer	lex(_in);
		PegParser	grammarParser(lex);
		grammarParser.build(_rules);
		_resolveRefs();
		if (checkLeftRecursion)
			_checkLeftRecursion();
	}
	catch (const FtppException &)
	{
		throw;
	}
	catch (const std::exception &e)
	{
		throw EngineError(e.what());
	}
	catch (...)
	{
		throw EngineError("unknown internal error");
	}
}

Grammar::Impl::~Impl()
{
	deleteVals(_rules);
}

// ============================================================================
// Helper
// ============================================================================

const std::string	&Grammar::Impl::rootRule(void) const
{
	if (_rules.size() == 0)
		throw GrammarError("contain no rules");
	return _rules.begin()->first;
}

// ============================================================================
// Public parse API
// ============================================================================

AstNode	*
Grammar::Impl::parseFile(const std::string &path) const
{
	return parseFile(path, rootRule());
}

AstNode	*
Grammar::Impl::parseFile(const std::string &path, const std::string &rule) const
{
	Input in = Input::fromFile(path);
	return _parseInput(in, rule);
}

AstNode	*
Grammar::Impl::parseString(const std::string &text) const
{
	return parseString(text, rootRule());
}

AstNode	*
Grammar::Impl::parseString(const std::string &text,
						const std::string &rule) const
{
	Input in = Input::fromText(text);
	return _parseInput(in, rule);
}

// ============================================================================
// Core Packrat call
// ============================================================================

AstNode	*
Grammar::Impl::_parseInput(Input &in, const std::string &ruleName) const
{
	t_ExprDict::const_iterator start = _rules.find(ruleName);

	if (start == _rules.end())
		throw GrammarError("error: Rule '" + ruleName + "' not found");
	try
	{
		PackratParser	mainParser(in);
		AstNode			*result = mainParser.parseRule(start->second);
#if FTPP_DEBUG_PACKRAT
		_lastStats = mainParser.getStats();
#endif
		return result;
	}
	catch (const FtppException &)
	{
		throw;
	}
	catch (const std::exception &e)
	{
		throw EngineError(e.what());
	}
	catch (...)
	{
		throw EngineError("unknown internal error");
	}
}

void	Grammar::Impl::_resolveRefs(void)
{
	ResolutionVisitor	visitor(_rules);

	for (t_ExprDict::iterator it = _rules.begin(); it != _rules.end(); ++it)
		visitor.resolve(it->second);
}

// Left recursion detection
void	Grammar::Impl::_checkLeftRecursion(void) const
{
	for (t_ExprDict::const_iterator it = _rules.begin(); it != _rules.end(); ++it)
	{
		std::string			ruleName = it->first;
		FirstSymbolVisitor	visitor(ruleName);

		if (visitor.matches(it->second))
			throw GrammarLeftRecursionError(ruleName);
	}
}