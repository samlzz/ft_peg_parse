/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PackratParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:58:58 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/02 21:49:04 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packrat/PackratParser.hpp"
#include "peg/Expr.hpp"
#include "ast/AstNode.hpp"
#include <cstddef>

// ---- parseRule: public entry point
bool PackratParser::parseRule(const std::string &rootRuleName, AstNode *&out)
{
	out = NULL;
	const Expr	*start = _grammar.get(rootRuleName);
	if (!start)
		throw ParseError("Unknown start rule: " + rootRuleName);

	const bool	ok = eval(start, out);
	if (!ok)
		throw ParseError(_err.formatError(_input, true));
	return ok;
}

// ---- private helpers method for eval
bool	PackratParser::retrieveExpr(const Expr *e, size_t pos, AstNode *&out)
{
	PackratCache::MemoEntry m = _memo.get(e, pos);

	_input.setPos(m.nextPos());
	if (m.node())
		out = new AstNode(*m.node());
	return m.ok();
}

// ----- eval: main of packrat logic (memo + dispatch)
bool PackratParser::eval(const Expr *expr, AstNode *&out)
{
	out = NULL;
	const size_t pos = _input.pos();

	if (_memo.has(expr, pos))
		return retrieveExpr(expr, pos, out);

	AstNode					*tmp = NULL;
	const bool				ok = expr->parse(*this, tmp);
	PackratCache::MemoEntry	store(ok, _input.pos(), tmp);

	_memo.set(expr, pos, store);

	if (ok)
		out = tmp;
	else if (tmp)
		delete tmp;
	return ok;
}