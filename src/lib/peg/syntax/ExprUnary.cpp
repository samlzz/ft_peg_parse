/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprUnary.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:39:33 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/25 15:46:17 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
#include <string>

#include "ast/AstNode.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/syntax/ExprUnary.hpp"
#include "peg/syntax/IExprVisitor.hpp"
#include "utils/Input.hpp"

// ============================================================================
// ExprUnary
// ============================================================================

// Replace the current inner expression, deleting the previous one.
void ExprUnary::setInner(Expr *e)
{
	if (_inner != e)
	{
		delete _inner;
		_inner = e;
	}
}

// ============================================================================
// ZeroOrMore
// ============================================================================

void ZeroOrMore::accept(IExprVisitor &visitor) const
{
	visitor.visitZeroOrMore(*this);
}

// Repeatedly evaluate the inner expression until it stops consuming input.
bool ZeroOrMore::parse(PackratParser &parser, AstNode *parent) const
{
	Input	&in = parser.input();
	size_t	last = in.pos();

	while (parser.eval(_inner, parent))
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

void OneOrMore::accept(IExprVisitor &visitor) const
{
	visitor.visitOneOrMore(*this);
}

// Evaluate at least once, then behave like ZeroOrMore.
bool OneOrMore::parse(PackratParser &parser, AstNode *parent) const
{
	Input	&in = parser.input();
	size_t	last = in.pos();
	size_t	occ = 0;

	while (parser.eval(_inner, parent))
	{
		occ++;
		if (in.pos() == last)
			break;
		last = in.pos();
	}
	if (!occ)
	{
		parser.diag().update(in.pos(), "expected one or more repetitions");
		return false;
	}
	return true;
}

// ============================================================================
// Optional
// ============================================================================

void Optional::accept(IExprVisitor &visitor) const
{
	visitor.visitOptional(*this);
}

// Try the inner expression but never fail.
bool Optional::parse(PackratParser &parser, AstNode *parent) const
{
	parser.eval(_inner, parent);
	return true;
}

// ============================================================================
// Predicate
// ============================================================================

void Predicate::accept(IExprVisitor &visitor) const
{
	visitor.visitPredicate(*this);
}

// Perform a look-ahead evaluation without consuming input.
bool Predicate::parse(PackratParser &parser, AstNode *parent) const
{
	Input	&in = parser.input();
	size_t	start = in.pos();

	const bool	ok = parser.eval(_inner, parent);
	in.setPos(start);

	return _isAnd ? ok : !ok;
}

// ============================================================================
// Capture
// ============================================================================

void Capture::accept(IExprVisitor &visitor) const
{
	visitor.visitCapture(*this);
}

// Parse a property capture: store substring directly into parent attributes.
bool Capture::parseProperty(PackratParser &parser, AstNode *parent) const
{
	if (!parent)
		throw PackratParser::ParseError("Property capture outside of node context");

	Input	&in = parser.input();
	size_t	start = in.pos();

	{
		AstNode	tmp;
		if (!parser.eval(_inner, &tmp))
			return false;
		parent->stealChildren(tmp);
	}

	parent->setAttr(_tag, in.substr(start, in.pos()));
	return true;
}

// Standard capture: build a child AST node with inner parse content.
bool Capture::parse(PackratParser &parser, AstNode *parent) const
{
	if (_isProp)
		return parseProperty(parser, parent);

	Input	&in = parser.input();
	size_t	start = in.pos();
	AstNode	*me = new AstNode(_tag);

	if (!parser.eval(_inner, me))
	{
		in.setPos(start);
		delete me;
		return false;
	}
	me->setSpan(start, in.pos());
	parent->addChild(me);
	return true;
}

