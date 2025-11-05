/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprUnary.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:39:33 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/05 16:33:22 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>

#include "ast/AstNode.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/syntax/ExprUnary.hpp"
#include "utils/Input.hpp"

void	ExprUnary::setInner(Expr *e)
{
	if (_inner != e)
	{
		delete _inner;
		_inner = e;
	}
}

bool ZeroOrMore::parse(PackratParser &parser, AstNode *&out) const
{
	Input	&in = parser.input();
	size_t	last = in.pos();

	while (parser.eval(_inner, out))
	{
		if (in.pos() == last)
			break;
		last = in.pos();
	}
	return true;
}

bool OneOrMore::parse(PackratParser &parser, AstNode *&out) const
{
	Input	&in = parser.input();
	size_t	last = in.pos();
	size_t	occ = 0;

	while (parser.eval(_inner, out))
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

bool Optional::parse(PackratParser &parser, AstNode *&out) const
{
	parser.eval(_inner, out);
	return true;
}

bool Predicate::parse(PackratParser &parser, AstNode *&out) const
{
	Input	&in = parser.input();
	size_t	start = in.pos();

	const bool	ok = parser.eval(_inner, out);
	in.setPos(start);
	return _isAnd ? ok : !ok;
}

bool	Capture::parseProperty(PackratParser &parser, AstNode *parent) const
{
	if (!parent)
		throw PackratParser::ParseError("Proprety outside of a node context");
	Input	&in = parser.input();
	size_t	start = in.pos();
	AstNode	*dummy = NULL;

	if (!parser.eval(_inner, dummy))
		return false;
	bool	hasChilds = (dummy && (!dummy->children().empty() || !dummy->type().empty()));
	delete dummy;
	if (hasChilds)
		throw PackratParser::ParseError("A property cannot have subnodes");

	parent->setAttr(_tag, in.substr(start, in.pos()));
	return (true);
}

bool Capture::parse(PackratParser &parser, AstNode *&out) const
{
	if (_isProp)
		return parseProperty(parser, out);

	Input	&in = parser.input();
	size_t	start = in.pos();
	AstNode	*me = new AstNode(_tag);
	AstNode	*childCtx = me;

	if (!parser.eval(_inner, childCtx))
	{
		in.setPos(start);
		delete me;
		return false;
	}
	me->setSpan(start, in.pos());
	out = me;
	return true;
}