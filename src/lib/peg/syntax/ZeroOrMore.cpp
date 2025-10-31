/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ZeroOrMore.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 19:52:13 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 23:44:24 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>

#include "ast/AstNode.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/Expr.hpp"
#include "peg/PegExpr.hpp"

// * Constructors

ZeroOrMore::ZeroOrMore(): Expr(K_ZERO_OR_MORE), _inner(NULL)
{}

ZeroOrMore::ZeroOrMore(Expr *inner): Expr(K_ZERO_OR_MORE), _inner(inner)
{}

ZeroOrMore::~ZeroOrMore()
{
	if (_inner)
		delete _inner;
	_inner = 0;
}

// * Parsing

bool ZeroOrMore::parse(PackratParser &parser, AstNode *&out) const
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
		out = NULL;
		return true;
	}

	out = new AstNode(childrens);
	return true;
}
