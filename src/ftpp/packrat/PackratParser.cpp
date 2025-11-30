/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PackratParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:58:58 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/30 00:23:19 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// IWYU pragma: keep

#include "ft_log/ft_log.hpp"

#include "AstNode.hpp"
#include "PackratParser.hpp"
#include "peg/core/Expr.hpp"
#include "utils/Input.hpp"
#include "utils/StringUtils.hpp"

// ============================================================================
// Construction
// ============================================================================


PackratParser::PackratParser(const std::string &path, const Grammar &pegGrammar,
								bool checkLeftRecursion)
	: _input(Input::fromFile(path)), _grammar(pegGrammar), _err()
#if FTPP_DEBUG_PACKRAT
		, _evalCount(0), _cacheHits(0), _backtrackCount(0)
#endif
{
	if (checkLeftRecursion)
		_grammar.checkLeftRecursion();
}

// ============================================================================
// parseRule: public entry point
// ============================================================================

void	PackratParser::parseRule(const std::string &rootRuleName, AstNode *&out)
{
	const Expr *start = _grammar.get(rootRuleName);
	if (!start)
		throw ParseError("root rule ' " + rootRuleName + "' not found in grammar");

	ft_log::log(FTPP_LOG_PACKRAT, ft_log::LOG_INFO)
		<< "Begin parsing with rule : " << rootRuleName << std::endl;

	AstNode		*root = new AstNode("_root");
	const bool	ok = eval(start, root);

	if (!ok || !_input.eof())
	{
		delete root;
		throw ParseError(_err.formatError(_input, true));
	}
	if (root->children().size() < 2)
	{
		if (root->children().size() == 1)
			out = root->popChild();
		delete root;
	}
	else
	{
		ft_log::log("peg.packrat.parser", ft_log::LOG_WARN)
			<< "Multiples nodes detected at root, added one 'ftpp_root' node at root"
			<< std::endl;
		out = root;
	}
}

// ============================================================================
// eval: main packrat logic (memo + dispatch)
// ============================================================================

bool	PackratParser::eval(const Expr *expr, AstNode *parent)
{
# if FTPP_DEBUG_PACKRAT
	_evalCount++;
# endif

	size_t	pos = _input.pos();
	bool	ok;
	{
		ft_log::LogScope _(FTPP_LOG_PACKRAT, expr->debugRepr().c_str());
		ok = expr->parse(*this, parent);
	}
	if (ok)
		ft_log::indentedLog(FTPP_LOG_PACKRAT)
			<< "[OK] nextPos=" + toString(_input.pos()) + "\n";
	else
	{
		ft_log::indentedLog(FTPP_LOG_PACKRAT) << "[FAIL]\n";
		_input.setPos(pos);
# if FTPP_DEBUG_PACKRAT
		_backtrackCount++;
# endif
	}

	return ok;
}

