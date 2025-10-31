/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OneOrMore.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 19:55:12 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 23:42:21 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>

#include "ast/AstNode.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/Expr.hpp"
#include "peg/PegExpr.hpp"

// * Constructors

OneOrMore::OneOrMore(): Expr(K_ONE_OR_MORE), _inner(NULL)
{}

OneOrMore::OneOrMore(Expr *inner): Expr(K_ONE_OR_MORE), _inner(inner)
{}

OneOrMore::~OneOrMore()
{
	if (_inner)
		delete _inner;
	_inner = 0;
}

// * Parsing

bool OneOrMore::parse(PackratParser &parser, AstNode *&out) const
{
	Input	&in = parser.input();
	size_t	last = in.pos();
	AstNode	*child = NULL;
	std::vector<AstNode *>	childrens;

	while (parser.eval(_inner, child))
	{
		if (child)
			childrens.push_back(child);
		child = NULL;
		if (in.pos() == last)
			break;
		last = in.pos();
	}
	if (childrens.empty())
	{
		parser.diag().update(in.pos(), "expected one or more repetitions");
		out = NULL;
		return false;
	}
	out = new AstNode(childrens);
	return true;
}
