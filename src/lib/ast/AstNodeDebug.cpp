/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AstNodeDebug.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 08:36:10 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/21 20:08:22 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/DebugLogger.hpp"

#if PEG_DEBUG_AST

# include <ostream>

# include "ast/AstNode.hpp"

// ---- Display ----

void AstNode::print(std::ostream& os, int32_t depth, const PrintOptions& opts) const
{
	os << PegDebug::Logger::color(_type, "\033[1;36m");
	
	if (opts.showSpan && _span.length() > 0)
		os << " [" << _span.start << ":" << _span.end << "]";
	
	if (opts.showAttributes && !_attrs.empty())
	{
		os << "\n";
		for (int32_t i = 0 ; i < depth + 2; ++i)
			os << opts.indentStr;
		os << "attrs:\n";

		std::map<std::string, std::string>::const_iterator it = _attrs.begin();
		for ((void)it; it != _attrs.end(); ++it)
		{
			for (int32_t i = 0 ; i < depth + 3; ++i)
				os << opts.indentStr;

			os << it->first << "="
				<< PegDebug::Logger::color("\"" + it->second + "\"", "\033[33m")
				<< "\n";
		}

		for (int32_t i = 0 ; i < depth; ++i)
			os << opts.indentStr;
	}
	
	if (!_children.empty() && !opts.compactMode)
		os << " (" << _children.size() << " children)";
}

void AstNode::PrintTree(const AstNode *node, std::ostream& os,
						int32_t currentDepth, const PrintOptions& opts)
{
	for (int i = 0; i < currentDepth; ++i)
		os << opts.indentStr;
	if (currentDepth > 0)
		os << "├─ ";
	
	node->print(os, currentDepth, opts);
	os << "\n";
	
	if (opts.maxDepth >= 0 && currentDepth >= opts.maxDepth)
	{
		if (!node->children().empty())
		{
			for (int i = 0; i <= currentDepth; ++i)
				os << opts.indentStr;
			os << "└─ ... (" << node->nodeCount() - 1 << " more nodes)\n";
		}
		return;
	}
	
	for (size_t i = 0; i < node->children().size(); ++i)
		PrintTree(node->children()[i], os, currentDepth + 1, opts);
}

std::ostream& operator<<(std::ostream& os, const AstNode& node)
{
	node.print(os);
	return os;
}

size_t AstNode::nodeCount() const
{
	size_t count = 1;
	for (size_t i = 0; i < _children.size(); ++i)
		count += _children[i]->nodeCount();
	return count;
}

size_t AstNode::maxDepth() const
{
	if (_children.empty())
		return 1;
	size_t	maxChildDepth = 0;
	size_t	d;
	for (size_t i = 0; i < _children.size(); ++i)
	{
		d = _children[i]->maxDepth();
		if (d > maxChildDepth)
			maxChildDepth = d;
	}
	return 1 + maxChildDepth;
}

#endif