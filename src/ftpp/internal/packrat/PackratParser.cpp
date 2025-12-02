/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PackratParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:58:58 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/01 22:57:50 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AstNode.hpp"
#include "FtppException.hpp"
#include "ft_log/ft_log.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/core/Expr.hpp"
#include "utils/Input.hpp"

// ============================================================================
// Construction / Destruction
// ============================================================================

PackratParser::PackratParser(Input &raw): _input(raw), _err()
#if FTPP_DEBUG_PACKRAT
	, _evalCount(0), _cacheHits(0), _backtrackCount(0)
#endif
{}

PackratParser::~PackratParser() {}

// ============================================================================
// parseRule: public entry point
// ============================================================================

AstNode	*PackratParser::parseRule(Expr *rule)
{
	AstNode		*root = new AstNode("_root");
	bool		ok;

	try
	{
		ok = eval(rule, root);
	}
	catch (...)
	{
		delete root;
		throw;
	}

	if (!ok || !_input.eof())
	{
		delete root;
		throw ConfigError(_err.formatError(_input, true));
	}
	if (root->children().size() < 2)
	{
		AstNode	*tmp = root->popChild();
		delete root;
		root = tmp;
	}
	else
		ft_log::log("peg.packrat.parser", ft_log::LOG_WARN)
			<< "Multiples nodes detected at root, added one '_root' node."
			<< std::endl;
	return root;
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
		ft_log::LogScope _(FTPP_LOG_PACKRAT, expr->repr().c_str());
		ok = expr->parse(*this, parent);
	}
	if (ok)
		ft_log::indentedLog(FTPP_LOG_PACKRAT)
			<< "[OK] nextPos=" <<_input.pos() << '\n';
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