/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sequence.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 19:42:47 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/02 17:22:30 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>

#include "ast/AstNode.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/Expr.hpp"
#include "peg/PegExpr.hpp"

// * Constructors

Sequence::Sequence(): Expr(K_SEQUENCE), _elems()
{}

Sequence::Sequence(const t_ExprList &elements): Expr(K_SEQUENCE), _elems(elements)
{}

Sequence::~Sequence()
{
	deleteAll(_elems);
}

// * Parsing

bool Sequence::parse(PackratParser &parser, AstNode *&out) const
{
	AstNode *child = NULL;
	std::vector<AstNode*> childrens;

	out = NULL;
	for (size_t i = 0; i < _elems.size(); ++i)
	{
		if (!parser.eval(_elems[i], child))
		{
			deleteAll(childrens);
			return false;
		}
		if (child)
			childrens.push_back(child);
		child = NULL;
	}
	if (childrens.empty())
		return true;

	out = new AstNode(childrens);
	return true;
}
