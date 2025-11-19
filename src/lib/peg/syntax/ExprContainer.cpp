/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprContainer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:37:18 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/19 14:11:43 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>

#include "ast/AstNode.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/syntax/IExprVisitor.hpp"
#include "peg/syntax/ExprContainer.hpp"

#if PEG_DEBUG_LEVEL > 0
std::string ExprContainer::debugValue(void) const
{
	std::ostringstream	oss;

	oss << "(" << _elems.size() << " elem" << (_elems.size() > 1 ? "s" : "") << ")";
	return oss.str();
}
#endif

void Sequence::accept(IExprVisitor &visitor) const
{
	visitor.visitSequence(*this);
}

bool Sequence::parse(PackratParser &parser, AstNode *parent) const
{
	for (size_t i = 0; i < _elems.size(); ++i)
	{
		if (!parser.eval(_elems[i], parent))
			return false;
	}
	return true;
}

void Choice::accept(IExprVisitor &visitor) const
{
	visitor.visitChoice(*this);
}

bool Choice::parse(PackratParser &parser, AstNode *parent) const
{
	Input	&in = parser.input();
	size_t	start = in.pos();

	for (size_t i = 0; i < _elems.size(); ++i)
	{
		AstNode	tmp;
		in.setPos(start);
		if (parser.eval(_elems[i], &tmp))
		{
			parent->stealChildren(tmp);
			return true;
		}
	}
	parser.diag().update(start, "expected at least one valid choice");
	return false;
}