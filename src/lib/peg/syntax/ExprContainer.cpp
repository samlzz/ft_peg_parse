/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprContainer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:37:18 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/25 15:45:29 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>

#include "ast/AstNode.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/syntax/IExprVisitor.hpp"
#include "peg/syntax/ExprContainer.hpp"
#include "utils/DebugConfig.hpp"

// ---- Generic debug function ----
#if PEG_DEBUG_ANY
std::string ExprContainer::debugValue(void) const
{
	std::ostringstream	oss;

	oss << "(" << _elems.size() << " elem" << (_elems.size() > 1 ? "s" : "") << ")";
	return oss.str();
}
#endif

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
	parser.diag().update(start, "expected at least one valid choice");
	return false;
}

