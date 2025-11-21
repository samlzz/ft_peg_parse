/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PackratParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:58:58 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/20 17:55:00 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>

#include "packrat/PackratParser.hpp"
#include "peg/Expr.hpp"
#include "ast/AstNode.hpp"
#include "utils/DebugLogger.hpp"
#include "utils/DebugConfig.hpp"
#include "utils/Input.hpp"

// ---- parseRule: public entry point
void	PackratParser::parseRule(const std::string &rootRuleName, AstNode *&out)
{
	out = NULL;
	const Expr	*start = _grammar.get(rootRuleName);
	if (!start)
		throw ParseError("at rule '"
				+ rootRuleName + "': "
				+ _err.formatError(_input, true));

	PEG_LOG_TRACE_C(PACKRAT, "PackratParser", "Begin parsing with rule :" + rootRuleName);
	AstNode		*root = new AstNode(rootRuleName);
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
		out = root;
}

// ---- privates helpers

bool	PackratParser::retrieveExpr(const Expr *e, size_t pos, AstNode *parent)
{
	PackratCache::MemoEntry m = _memo.get(e, pos);

	_input.setPos(m.nextPos());
	if (m.node())
	{
		AstNode	*clone = new AstNode(*m.node());
		parent->addChild(clone);
	}
	return m.ok();
}

// ----- eval: main of packrat logic (memo + dispatch)
bool PackratParser::eval(const Expr *expr, AstNode *parent)
{
	const Input	save(_input);

#if PEG_DEBUG_PACKRAT
	_evalCount++;
	_traceEnter(expr, save);
#endif

	// TODO: retrieve from cache

	const bool	ok = expr->parse(*this, parent);

#if PEG_DEBUG_PACKRAT
	_traceExit(expr, save, ok);
	if (!ok)
		_backtrackCount++;
#endif

	// TODO: append to cache

	if (!ok)
		_input.setPos(save.pos());
	return ok;
}