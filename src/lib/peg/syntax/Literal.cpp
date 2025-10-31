/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Literal.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 19:34:54 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 23:52:32 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packrat/PackratParser.hpp"
#include "peg/Expr.hpp"
#include "peg/PegExpr.hpp"

// * Constructors

Literal::Literal(): Expr(K_LITERAL), _value()
{}

Literal::Literal(const std::string &value): Expr(K_LITERAL), _value(value)
{}

Literal::~Literal() {}

// * Parsing

bool Literal::parse(PackratParser &parser, AstNode *&out) const
{
	if (_value.empty())
		throw PackratParser::ParseError("Empty literal not allowed");
	Input &in = parser.input();
	out = NULL;

	if (in.eof())
	{
		parser.diag().update(in.pos(), "unexpected EOF (expected \"" + _value + "\"");
		return false;
	}
	if (!in.match(_value))
	{
		parser.diag().update(in.pos(), "expected \"" + _value + "\"");
		return false;
	}
	return true;
}
