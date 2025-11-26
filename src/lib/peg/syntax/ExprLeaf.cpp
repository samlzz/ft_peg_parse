/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprLeaf.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:27:31 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/26 10:40:26 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#include "packrat/PackratParser.hpp"
#include "peg/IExprVisitor.hpp"
#include "peg/syntax/ExprLeaf.hpp"

// ============================================================================
// Literal
// ============================================================================

void Literal::accept(IExprVisitor &visitor) const
{
	visitor.visitLiteral(*this);
}

// Match the exact literal string stored in _value.
bool Literal::parse(PackratParser &parser, AstNode *parent) const
{
	if (_value.empty())
		throw PackratParser::ParseError("Empty literal not allowed");

	Input &in = parser.input();
	(void)parent;

	if (in.eof())
	{
		parser.diag().update(
			in.pos(),
			"unexpected EOF (expected \"" + _value + "\")"
		);
		return false;
	}
	if (!in.match(_value))
	{
		parser.diag().update(in.pos(), "expected \"" + _value + "\"");
		return false;
	}
	return true;
}

// ============================================================================
// CharRange
// ============================================================================

void CharRange::accept(IExprVisitor &visitor) const
{
	visitor.visitCharRange(*this);
}

// Match a single character belonging to the expanded charset.
bool CharRange::parse(PackratParser &parser, AstNode *parent) const
{
	Input &in = parser.input();
	(void)parent;

	if (in.eof())
	{
		parser.diag().update(
			in.pos(),
			"unexpected EOF (expected [" + _value + "])"
		);
		return false;
	}

	char c = in.peek();
	if (_value.empty() || _value.find(c) == std::string::npos)
	{
		parser.diag().update(
			in.pos(),
			std::string("unexpected '") + c + "' (expected [" + _value + "])"
		);
		return false;
	}

	in.get();
	return true;
}

// ============================================================================
// Any
// ============================================================================

void Any::accept(IExprVisitor &visitor) const
{
	visitor.visitAny(*this);
}

// Match any single character, failing only on EOF.
bool Any::parse(PackratParser &parser, AstNode *parent) const
{
	Input &in = parser.input();
	(void)parent;

	if (in.eof())
	{
		parser.diag().update(
			in.pos(),
			"unexpected EOF (expected any character)"
		);
		return false;
	}

	in.get();
	return true;
}

