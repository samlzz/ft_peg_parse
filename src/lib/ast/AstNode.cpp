/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AstNode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 17:55:13 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 23:59:49 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
#include <string>
#include <vector>

#include "ast/AstNode.hpp"
#include "peg/Expr.hpp"

// *Constructors
AstNode::AstNode(): _type(), _attrs(), _children(), _span()
{}

AstNode::AstNode(const std::string &type):
	_type(type), _attrs(), _children(), _span()
{}

AstNode::AstNode(std::vector<AstNode *> &childrens):
	_type(), _attrs(), _children(), _span()
{
	_children.swap(childrens);
	childrens.clear();
}

AstNode::AstNode(const AstNode& other):
	_type(other.type()),
	_attrs(other._attrs),
	_children(),
	_span(other._span)
{
	replaceChildren(other._children);
}

// *Destructor
AstNode::~AstNode()
{
	deleteAll(_children);
}

// *Operators

AstNode&	AstNode::operator=(const AstNode& other)
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

//* Methods

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

void	AstNode::stealChildren(AstNode &stolen)
{
	std::vector<AstNode *>tmp;

	tmp.swap(stolen._children);
	stolen._children.clear();
	_children.insert(_children.end(), tmp.begin(), tmp.end());
}

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
	return (it->second);
}

const std::string	&AstNode::type(void) const
{
	return _type;
}

void	AstNode::setType(const std::string &type)
{
	_type = type;
}

const std::vector<AstNode *>	&AstNode::children(void) const
{
	return _children;
}

void	AstNode::setSpan(size_t start, size_t end)
{
	_span.start = start;
	_span.end = end;
}