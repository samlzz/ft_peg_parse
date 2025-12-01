/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnaryQuantifiers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:39:33 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/01 23:07:47 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>

#include "AstNode.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/core/IExprVisitor.hpp"
#include "peg/syntax/UnaryQuantifiers.hpp"
#include "utils/Diag.hpp"
#include "utils/Input.hpp"

// ============================================================================
// Helper to handle diagnostics in while
// ============================================================================

static inline bool	_child_ok(PackratParser &parser, AstNode *parent, Expr *inner)
{
	Diag &errs = parser.diag();

	errs.save();
	if (parser.eval(inner, parent))
	{
		errs.commit();
		return true;
	}
	errs.restore();
	return false;
}

// ============================================================================
// ZeroOrMore
// ============================================================================

void	ZeroOrMore::accept(IExprVisitor &visitor) const
{
	visitor.visitZeroOrMore(*this);
}

// Repeatedly evaluate the inner expression until it stops consuming input.
bool	ZeroOrMore::parse(PackratParser &parser, AstNode *parent) const
{
	Input	&in = parser.input();
	size_t	last = in.pos();

	while (_child_ok(parser, parent, _inner))
	{	
		if (in.pos() == last)
			break;
		last = in.pos();
	}
	return true;
}

// ============================================================================
// OneOrMore
// ============================================================================

void	OneOrMore::accept(IExprVisitor &visitor) const
{
	visitor.visitOneOrMore(*this);
}

// Evaluate at least once, then behave like ZeroOrMore.
bool	OneOrMore::parse(PackratParser &parser, AstNode *parent) const
{
	Input	&in = parser.input();
	size_t	last = in.pos();

	if (!parser.eval(_inner, parent))
	{
		parser.diag().update(in.pos(),
			"at least one occurence", Diag::PRIO_MEDIUM);
		return false;
	}

	while (_child_ok(parser, parent, _inner))
	{	
		if (in.pos() == last)
			break;
		last = in.pos();
	}
	return true;
}

// ============================================================================
// Optional
// ============================================================================

void	Optional::accept(IExprVisitor &visitor) const
{
	visitor.visitOptional(*this);
}

// Try the inner expression but never fail.
bool	Optional::parse(PackratParser &parser, AstNode *parent) const
{
	Diag	&errs = parser.diag();

	errs.save();
	parser.eval(_inner, parent);
	errs.restore();
	return true;
}