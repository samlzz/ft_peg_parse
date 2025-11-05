/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprUnary.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:39:33 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/03 15:47:20 by sliziard         ###   ########.fr       */
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
	AstNode	*child = NULL;
	std::vector<AstNode *>	childrens;

	while (parser.eval(_inner, child))
	{
		if (child)
			childrens.push_back(child);
		child = NULL;
		if (in.pos() == last)
			break;
		last = in.pos();
	}
	if (!childrens.empty())
		appendNode(new AstNode(childrens), out);
	return true;
}

bool OneOrMore::parse(PackratParser &parser, AstNode *&out) const
{
	Input	&in = parser.input();
	size_t	last = in.pos();
	AstNode	*child = NULL;
	size_t	occ = 0;
	std::vector<AstNode *>	childrens;

	while (parser.eval(_inner, child))
	{
		occ++;
		if (child)
			childrens.push_back(child);
		child = NULL;
		if (in.pos() == last)
			break;
		last = in.pos();
	}
	if (!occ)
	{
		parser.diag().update(in.pos(), "expected one or more repetitions");
		return false;
	}
	if (!childrens.empty())
		appendNode(new AstNode(childrens), out);
	return true;
}

bool Optional::parse(PackratParser &parser, AstNode *&out) const
{
	AstNode	*child = NULL;

	parser.eval(_inner, child);
	if (child)
		appendNode(child, out);
	return true;
}

bool Predicate::parse(PackratParser &parser, AstNode *&out) const
{
	Input	&in = parser.input();
	size_t	start = in.pos();
	AstNode	*child = NULL;

	bool	ok = parser.eval(_inner, child);
	if (child)
		appendNode(child, out);

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

	if (!parser.eval(_inner, me))
		{
		in.setPos(start);
		delete me;
		return false;
	}
	me->setSpan(start, in.pos());
	appendNode(me, out);
	return true;
}