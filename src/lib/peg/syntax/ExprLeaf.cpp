/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprLeaf.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:27:31 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/06 17:35:03 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
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

inline char unescapeChar(const std::string &s, size_t &i)
{
	if (s[i] != '\\' || i + 1 >= s.size())
		return s[i];

	char c = s[++i];
	switch (c)
	{
		case 'n': return '\n';
		case 't': return '\t';
		case 'r': return '\r';
		case '\\': return '\\';
		case '\'': return '\'';
		case '"': return '"';
		case '0': return '\0';
		default:  return c; // Si non reconnu, on retourne le caractère tel quel
	}
}

static inline void	_appendRange(std::string &dst, char beg, char end)
{
	if (static_cast<uint8_t>(beg) <= static_cast<uint8_t>(end))
	{
		for (uint8_t ch = beg; ch <= end; ++ch)
			dst += ch;
	}
	else
		dst += std::string() + beg + '-' + end;
}

std::string CharRange::expandCharset(const std::string &raw)
{
	std::string	resp;
	char		c;

	for (size_t i = 0; i < raw.size(); ++i)
	{
		c = unescapeChar(raw, i);
		if (i + 2 < raw.size()
			&& raw[i + 1] == '-' && (!i || raw[i - 1] != '\\'))
		{
			i += 2;
			_appendRange(resp, c, unescapeChar(raw, i));
		}
		else
			resp += c;
	}
	return resp;
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