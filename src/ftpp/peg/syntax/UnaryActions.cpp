/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnaryActions.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:39:33 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/01 23:07:38 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>

#include "AstNode.hpp"
#include "FtppException.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/core/IExprVisitor.hpp"
#include "peg/syntax/UnaryActions.hpp"
#include "utils/Input.hpp"

// ============================================================================
// Capture
// ============================================================================

void	Capture::accept(IExprVisitor &visitor) const
{
	visitor.visitCapture(*this);
}

// Parse a property capture: store substring directly into parent attributes.
bool	Capture::parseProperty(PackratParser &parser, AstNode *parent) const
{
	if (!parent)
		throw ConfigError("Property capture outside of node context");

	Input	&in = parser.input();
	size_t	start = in.pos();

	{
		AstNode	tmp;
		if (!parser.eval(_inner, &tmp))
		{
			return false;
		}
		parent->stealChildren(tmp);
	}

	parent->setAttr(_tag, in.substr(start, in.pos()));
	return true;
}

// Standard capture: build a child AST node with inner parse content.
bool	Capture::parse(PackratParser &parser, AstNode *parent) const
{
	if (_isProp)
		return parseProperty(parser, parent);

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
	parent->addChild(me);
	return true;
}

// ============================================================================
// Fatal
// ============================================================================

void	Fatal::accept(IExprVisitor &visitor) const
{
	visitor.visitFatal(*this);
}

// Fatal failure: throw immediately with current diagnostic
bool	Fatal::parse(PackratParser &parser, AstNode *parent) const
{
	if (!parser.eval(_inner, parent))
	{
		throw ConfigError(
			parser.diag().formatError(parser.input(), true)
		);
	}
	return true;
}