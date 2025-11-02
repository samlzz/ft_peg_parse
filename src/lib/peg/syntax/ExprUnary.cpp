/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprUnary.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:39:33 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/02 20:55:46 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


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
	if (childrens.empty())
	{
		out = NULL;
		return true;
	}

	out = new AstNode(childrens);
	return true;
}

bool OneOrMore::parse(PackratParser &parser, AstNode *&out) const
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
	if (childrens.empty())
	{
		parser.diag().update(in.pos(), "expected one or more repetitions");
		out = NULL;
		return false;
	}
	out = new AstNode(childrens);
	return true;
}

bool Optional::parse(PackratParser &parser, AstNode *&out) const
{
	AstNode *child = NULL;

	if (!parser.eval(_inner, child))
	{
		out = NULL;
		return true;
	}
	out = child;
	return true;
}

bool Predicate::parse(PackratParser &parser, AstNode *&out) const
{
	Input	&in = parser.input();
	size_t	start = in.pos();
	AstNode	*child = NULL;

	bool	ok = parser.eval(_inner, child);
	if (child)
		delete child;

	in.setPos(start);
	out = NULL;
	return _isAnd ? ok : !ok;
}

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