/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AstNode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 17:55:13 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 18:41:52 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast/AstNode.hpp"
#include <string>

// *Constructors
AstNode::AstNode(): _type(), _attrs(), _children(), _span()
{}

AstNode::AstNode(const std::string &type):
	_type(type), _attrs(), _children(), _span()
{}

AstNode::AstNode(const AstNode& other):
	_type(other.type()),
	_attrs(other._attrs),
	_children(other._children),
	_span(other._span)
{}

// *Destructor
AstNode::~AstNode()
{
	for (size_t i = 0; i < _children.size(); ++i)
		delete _children[i];
}

// *Operators

AstNode&	AstNode::operator=(const AstNode& other)
{
	if (this != &other)
	{
		_type = other.type();
		_attrs = other._attrs;
		_children = other._children;
		_span.start = other._span.start;
		_span.end = other._span.end;
	}
	return *this;
}

//* Methods

void	AstNode::addChild(AstNode *child)
{
	if (child)
		_children.push_back(child);
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
