/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreeVisitor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 23:56:47 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/25 21:20:10 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/DebugConfig.hpp"

#if PEG_DEBUG_ANY

# include "peg/debug/ExprVisitors.hpp"
# include "peg/syntax/ExprContainer.hpp"
# include "peg/syntax/ExprLeaf.hpp"
# include "peg/syntax/ExprUnary.hpp"
# include "peg/syntax/RuleRef.hpp"

namespace ExprDebug {

// ============================================================================
// TreeVisitor
// ============================================================================

TreeVisitor::TreeVisitor(std::ostream &os, int32_t maxDepth, bool useColors)
	: _os(os), _depth(0), _maxDepth(maxDepth),
	  _prefix(""), _isLast(true), _useColors(useColors)
{}

// Apply ANSI color if enabled.
std::string TreeVisitor::color(const std::string &text, const char *code) const
{
	if (!_useColors || !code)
		return text;
	return std::string(code) + text + "\033[0m";
}

// Print the current node with prefix and branch symbols.
void	TreeVisitor::printNode(const Expr &expr)
{
	_os << _prefix;

	if (_depth > 0)
		_os << (_isLast ? "└─ " : "├─ ");

	PrintVisitor printer(_os, _useColors);
	expr.accept(printer);

	_os << "\n";
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

} // namespace ExprDebug

#endif

