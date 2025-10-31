/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CharRange.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 19:45:09 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 23:56:41 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#include "packrat/PackratParser.hpp"
#include "peg/Expr.hpp"
#include "peg/PegExpr.hpp"

// * Constructors

CharRange::CharRange(): Expr(K_CHARRANGE), _charset()
{}

CharRange::CharRange(const std::string &charset):
	Expr(K_CHARRANGE), _charset(charset)
{}

CharRange::~CharRange() {}

// * Parsing

bool CharRange::parse(PackratParser &parser, AstNode *&out) const
{
	Input &in = parser.input();
	out = NULL;

	if (in.eof())
	{
		parser.diag().update(
			in.pos(),
			"unexpected EOF (expected [" + _charset + "])"
		);
		return false;
	}
	char c = in.peek();
	if (_charset.find(c) == std::string::npos)
	{
		parser.diag().update(
			in.pos(),
			std::string("unexpected '") + c + "' (expected [" + _charset + "])"
		);
		return false;
	}
	in.get();
	return true;
}
