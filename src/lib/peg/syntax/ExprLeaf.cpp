/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprLeaf.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:27:31 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/09 10:38:40 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#include "packrat/PackratParser.hpp"
#include "peg/syntax/ExprLeaf.hpp"

bool Literal::parse(PackratParser &parser, AstNode *parent) const
{
	if (_value.empty())
		throw PackratParser::ParseError("Empty literal not allowed");
	Input &in = parser.input();
	(void)parent;

	if (in.eof())
	{
		parser.diag().update(in.pos(), "unexpected EOF (expected \"" + _value + "\")");
		return false;
	}
	if (!in.match(_value))
	{
		parser.diag().update(in.pos(), "expected \"" + _value + "\"");
		return false;
	}
	return true;
}

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