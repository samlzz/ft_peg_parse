/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprContainer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:37:18 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/02 19:39:21 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast/AstNode.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/syntax/ExprContainer.hpp"

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

bool Choice::parse(PackratParser &parser, AstNode *&out) const
{
	Input	&in = parser.input();
	size_t	start = in.pos();
	AstNode	*child;

	for (size_t i = 0; i < _elems.size(); ++i)
	{
		in.setPos(start);
		child = NULL;
		if (parser.eval(_elems[i], child))
		{
			out = child;
			return true;
		}
	}
	parser.diag().update(start, "expected at least one valid choice");
	out = NULL;
	return false;
}