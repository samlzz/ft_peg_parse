/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CombinatorOps.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:37:18 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/01 22:59:36 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AstNode.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/core/IExprVisitor.hpp"
#include "peg/syntax/CombinatorOps.hpp"

// ============================================================================
// Sequence
// ============================================================================

void Sequence::accept(IExprVisitor &visitor) const
{
	visitor.visitSequence(*this);
}

// Evaluate each sub-expression in order. Stops on first failure.
bool Sequence::parse(PackratParser &parser, AstNode *parent) const
{
	for (size_t i = 0; i < _elems.size(); ++i)
	{
		if (!parser.eval(_elems[i], parent))
			return false;
	}
	return true;
}

// ============================================================================
// Choice
// ============================================================================

void Choice::accept(IExprVisitor &visitor) const
{
	visitor.visitChoice(*this);
}

// Try each alternative expression until one succeeds.
// Restores input position before each attempt.
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
	parser.diag().update(start, "one of the alternatives", Diag::PRIO_LOW);
	return false;
}