/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Predicate.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 19:57:18 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 23:43:16 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast/AstNode.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/Expr.hpp"
#include "peg/PegExpr.hpp"

// * Constructors

Predicate::Predicate(Expr *inner, bool isAndPredicate):
	Expr(K_PREDICATE), _inner(inner), _isAnd(isAndPredicate)
{}

Predicate::~Predicate()
{
	if (_inner)
		delete _inner;
	_inner = 0;
}

// * Parsing

bool Predicate::parse(PackratParser &parser, AstNode *&out) const
{
	Input	&in = parser.input();
	size_t	start = in.pos();
	AstNode	*child = NULL;

	bool	ok = parser.eval(_inner, child);
	if (child)
		delete child;

	in.setPos(start);
	out = NULL;
	return _isAnd ? ok : !ok;
}
