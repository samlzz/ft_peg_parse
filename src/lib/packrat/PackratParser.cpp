/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PackratParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:58:58 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/05 17:29:12 by sliziard         ###   ########.fr       */
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
	{
		AstNode	*clone = new AstNode(*m.node());
		appendNode(clone, out);
	}
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

	AstNode		*produced = NULL;
	const bool	ok = expr->parse(*this, produced);

	PackratCache::MemoEntry	store(ok, _input.pos(), produced);
	_memo.set(expr, pos, store);
	printCacheSet(expr, pos, ok, _input.pos());

	printEvalTrace(expr, out, _input.pos(), false);

	if (ok)
	{
		if (produced)
			appendNode(produced, out);
	}
	else
	{
		_input.setPos(pos);
		delete produced;
	}
	return ok;
}