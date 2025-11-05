/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprContainer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:37:18 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/05 12:37:14 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast/AstNode.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/syntax/ExprContainer.hpp"

bool Sequence::parse(PackratParser &parser, AstNode *&out) const
{
	for (size_t i = 0; i < _elems.size(); ++i)
	{
		if (!parser.eval(_elems[i], out))
			return false;
	}
	return true;
}

bool Choice::parse(PackratParser &parser, AstNode *&out) const
{
	Input	&in = parser.input();
	size_t	start = in.pos();

	for (size_t i = 0; i < _elems.size(); ++i)
	{
		in.setPos(start);
		if (parser.eval(_elems[i], out))
			return true;
	}
	parser.diag().update(start, "expected at least one valid choice");
	return false;
}