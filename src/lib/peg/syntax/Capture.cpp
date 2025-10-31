/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Capture.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 21:14:44 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/01 00:12:38 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>

#include "ast/AstNode.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/Expr.hpp"
#include "peg/PegExpr.hpp"

// * Constructors

Capture::Capture():
	Expr(K_CAPTURE), _inner(NULL), _tag()
{}

Capture::Capture(Expr *inner, const std::string &tag):
	Expr(K_CAPTURE), _inner(inner), _tag(tag)
{}

Capture::~Capture()
{
	if (_inner)
		delete _inner;
	_inner = 0;
}

// * Parsing

bool Capture::parse(PackratParser &parser, AstNode *&out) const
{
	Input	&in = parser.input();
	size_t	start = in.pos();
	AstNode	*innerNode = NULL;
	AstNode	*me = NULL;

	if (!parser.eval(_inner, innerNode))
	{
		if (innerNode) delete innerNode;
		out = NULL;
		return false;
	}

	me = new AstNode(_tag);
	if (innerNode)
	{
		if (innerNode->type().empty())
		{
			me->stealChildren(*innerNode);
			delete innerNode;
		}
		else
			me->addChild(innerNode);
	}
	me->setSpan(start, in.pos());

	out = me;
	return true;
}
