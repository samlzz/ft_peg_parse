/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PackratParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:58:58 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/05 14:59:04 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>

#include "packrat/PackratParser.hpp"
#include "peg/Expr.hpp"
#include "ast/AstNode.hpp"
#include "utils/Debug.hpp"

// ---- parseRule: public entry point
void	PackratParser::parseRule(const std::string &rootRuleName, AstNode *&out)
{
	out = NULL;
	const Expr	*start = _grammar.get(rootRuleName);
	if (!start)
		throw ParseError("at rule '" + rootRuleName + "': " + _err.formatError(_input, true));

	const bool	ok = eval(start, out);
	if (!ok || !_input.eof())
		throw ParseError(_err.formatError(_input, true));
}

// ---- privates helpers

bool	PackratParser::retrieveExpr(const Expr *e, size_t pos, AstNode *&out)
{
	PackratCache::MemoEntry m = _memo.get(e, pos);

	_input.setPos(m.nextPos());
	if (m.node())
		appendNode(new AstNode(*m.node()), out);
	return m.ok();
}

// ----- eval: main of packrat logic (memo + dispatch)
bool PackratParser::eval(const Expr *expr, AstNode *&out)
{
	const size_t	pos = _input.pos();

	if (_memo.has(expr, pos))
	{
		printCacheHit(expr, pos);
		return retrieveExpr(expr, pos, out);
	}

	printEvalTrace(expr, out, pos, true);

	AstNode					*local = out;
	const bool				ok = expr->parse(*this, local);

	printEvalTrace(expr, out, _input.pos(), false);

	PackratCache::MemoEntry	store(ok, _input.pos(), local);
	_memo.set(expr, pos, store);
	printCacheSet(expr, pos, ok, _input.pos());

	if (ok)
	{
		if (local && local != out)
			appendNode(local, out);
	}
	else
	{
		_input.setPos(pos);
		if (local && local != out)
			delete local;
	}
	return ok;
}