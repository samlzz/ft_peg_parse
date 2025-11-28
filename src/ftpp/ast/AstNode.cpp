/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AstNode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 17:55:13 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/28 13:14:27 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
#include <string>
#include <vector>

#include "AstNode.hpp"
#include "peg/core/Expr.hpp"

// ============================================================================
// Construction
// ============================================================================

AstNode::AstNode(const AstNode &other):
	_type(other.type()),
	_attrs(other._attrs),
	_children(),
	_span(other._span)
{
	replaceChildren(other._children);
}

// ============================================================================
// Assignment
// ============================================================================

AstNode &AstNode::operator=(const AstNode &other)
{
	if (this != &other)
	{
		_type = other.type();
		_attrs = other._attrs;
		_span.start = other._span.start;
		_span.end = other._span.end;
		replaceChildren(other._children);
	}
	return *this;
}

// ============================================================================
// Tree management
// ============================================================================

void	AstNode::replaceChildren(const std::vector<AstNode *> &src)
{
	deleteAll(_children);
	for (size_t i = 0; i < src.size(); ++i)
	{
		if (src[i])
			_children.push_back(new AstNode(*src[i]));
	}
}

void	AstNode::addChild(AstNode *child)
{
	if (child)
		_children.push_back(child);
}

AstNode	*AstNode::popChild(void)
{
	AstNode *resp = _children[0];
	_children.erase(_children.begin());
	return resp;
}

void	AstNode::stealChildren(AstNode &stolen)
{
	_attrs.insert(stolen._attrs.begin(), stolen._attrs.end());
	stolen._attrs.clear();

	if (!stolen._children.size())
		return;

	_children.insert(_children.end(),
		stolen._children.begin(), stolen._children.end());
	stolen._children.clear();
}

// ============================================================================
// Accessors
// ============================================================================

void	AstNode::setSpan(size_t start, size_t end)
{
	_span.start = start;
	_span.end = end;
}

// ============================================================================
// Attribute management
// ============================================================================

void	AstNode::setAttr(const std::string &key, const std::string &val)
{
	if (key.empty())
		throw AstNodeAttrEmptyKey();
	_attrs[key] = val;
}

bool	AstNode::hasAttr(const std::string &key) const
{
	std::map<std::string, std::string>::const_iterator it = _attrs.find(key);
	return (it != _attrs.end());
}

std::string	AstNode::getAttr(const std::string &key, const std::string &def) const
{
	std::map<std::string, std::string>::const_iterator it = _attrs.find(key);
	if (it == _attrs.end())
		return def;
	return it->second;
}

