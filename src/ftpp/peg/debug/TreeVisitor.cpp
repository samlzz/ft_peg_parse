/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreeVisitor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 23:56:47 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/30 01:43:21 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

#if FTPP_DEBUG_EXPR

# include "peg/visitors/DebugVisitors.hpp"
# include "peg/syntax/ExprContainer.hpp"
# include "peg/syntax/ExprLeaf.hpp"
# include "peg/syntax/ExprUnary.hpp"
# include "peg/syntax/RuleRef.hpp"

// ============================================================================
// TreeVisitor
// ============================================================================

TreeVisitor::TreeVisitor(std::ostream &os, int32_t maxDepth)
	: _os(os), _depth(0), _maxDepth(maxDepth),
		_prefix(""), _isLast(true)
{}

// Print the current node with prefix and branch symbols.
void	TreeVisitor::printNode(const Expr &expr)
{
	_os << _prefix;

	if (_depth > 0)
		_os << (_isLast ? "└─ " : "├─ ");

	_os << expr.debugRepr(_useColors) << "\n";
}

// Visit all children, respecting maxDepth and prefixes.
void	TreeVisitor::visitChildren(const Expr &parent)
{
	// Reached maximum depth
	if (_maxDepth >= 0 && _depth >= _maxDepth)
	{
		_os << _prefix
			<< (_isLast ? "   " : "│  ")
			<< color("...", "\033[2m") << "\n";
		return;
	}

	size_t count = parent.childCount();
	if (count == 0)
		return;

	std::string oldPrefix = _prefix;
	_prefix += (_isLast ? "   " : "│  ");

	for (size_t i = 0; i < count; ++i)
	{
		const Expr *child = parent.child(i);
		if (child)
			descend(child, i == count - 1);
	}

	_prefix = oldPrefix;
}

// Recursively call the visitor on child nodes, updating prefix state.
void	TreeVisitor::descend(const Expr *child, bool isLast)
{
	if (!child)
		return;

	_depth++;
	bool oldIsLast = _isLast;
	_isLast = isLast;

	child->accept(*this);

	_isLast = oldIsLast;
	_depth--;
}

// ---- Macro to generate visit methods ----
# define TREE_VISIT_IMPL(ClassName) \
	void TreeVisitor::visit##ClassName(const ClassName &expr) \
	{ \
		printNode(expr); \
		visitChildren(expr); \
	}

// Implementations
TREE_VISIT_IMPL(Literal)
TREE_VISIT_IMPL(CharRange)
TREE_VISIT_IMPL(Any)

TREE_VISIT_IMPL(Sequence)
TREE_VISIT_IMPL(Choice)

TREE_VISIT_IMPL(ZeroOrMore)
TREE_VISIT_IMPL(OneOrMore)
TREE_VISIT_IMPL(Optional)
TREE_VISIT_IMPL(Predicate)
TREE_VISIT_IMPL(Capture)
TREE_VISIT_IMPL(Fatal)

# undef TREE_VISIT_IMPL

void	TreeVisitor::visitRuleRef(const RuleRef &expr)
{
	printNode(expr);

	if (!expr.resolved())
		return;

	_os << _prefix << (_isLast ? "   " : "│  ")
		<< color("↳ resolves to:", "\033[2m") << "\n";

	std::string oldPrefix = _prefix;
	_prefix += (_isLast ? "   " : "│  ");
	descend(expr.resolved(), true);
	_prefix = oldPrefix;
}

#endif

