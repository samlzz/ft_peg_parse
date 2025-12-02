/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TerminalOps.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:27:31 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/01 23:05:46 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#include "FtppException.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/core/IExprVisitor.hpp"
#include "utils/Diag.hpp"
#include "utils/StringUtils.hpp"
#include "peg/syntax/TerminalOps.hpp"

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
		throw ConfigError("Empty literal not allowed");

	Input &in = parser.input();
	(void)parent;

	if (in.eof() || !in.match(_value))
	{
		parser.diag().update(in.pos(),
			escapeStringDisplay(_value), Diag::PRIO_HIGH);
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
	if (_value.empty())
		throw ConfigError("Empty character class not allowed");

	Input	&in = parser.input();
	char	c = in.peek();
	(void)parent;

	if (in.eof() || _value.find(c) == std::string::npos)
	{
		parser.diag().update(in.pos(),
			escapeCharSetDisplay(_value), Diag::PRIO_HIGH);
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
		parser.diag().update(in.pos(), "any character", Diag::PRIO_LOW);
		return false;
	}

	in.get();
	return true;
}