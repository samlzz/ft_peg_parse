/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TreeVisitor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 23:56:47 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/04 11:41:55 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

#if FTPP_DEBUG_GRAMMAR

# include <ft_log/LogOp.hpp>

# include "peg/syntax/CombinatorOps.hpp"
# include "peg/syntax/RuleRef.hpp"
# include "peg/syntax/TerminalOps.hpp"
# include "peg/syntax/UnaryActions.hpp"
# include "peg/syntax/UnaryPredicate.hpp"
# include "peg/syntax/UnaryQuantifiers.hpp"
# include "peg/visitors/DebugVisitors.hpp"

// ============================================================================
// TreeVisitor
// ============================================================================

TreeVisitor::TreeVisitor(std::ostream &os, int32_t maxDepth, bool expandRuleRefs)
	: _os(os), _depth(0), _maxDepth(maxDepth)
	, _prefix(""), _isLast(true), _expandRef(expandRuleRefs)
{}

// Print the current node with prefix and branch symbols.
void	TreeVisitor::printNode(const Expr &expr)
{
	_os << _prefix;

	if (_depth > 0)
		_os << (_isLast ? "└─ " : "├─ ");

	_os << expr.repr() << "\n";
}

// Visit all children, respecting maxDepth and prefixes.
void	TreeVisitor::visitChildren(const Expr &parent)
{
	// Reached maximum depth
	if (_maxDepth >= 0 && _depth >= _maxDepth)
	{
		_os << _prefix
			<< (_isLast ? "   " : "│  ")
			<< ft_log::color("...", "\033[2m") << "\n";
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

// ============================================================================
// Visit methods
// ============================================================================

// ---- Terminals ----
void	TreeVisitor::visitLiteral(const Literal &expr)
{
	printNode(expr);
	visitChildren(expr);
}
void	TreeVisitor::visitCharRange(const CharRange &expr)
{
	printNode(expr);
	visitChildren(expr);
}
void	TreeVisitor::visitAny(const Any &expr)
{
	printNode(expr);
	visitChildren(expr);
}

// ---- Combinators ----
void	TreeVisitor::visitSequence(const Sequence &expr)
{
	printNode(expr);
	visitChildren(expr);
}

void	TreeVisitor::visitChoice(const Choice &expr)
{
	printNode(expr);
	visitChildren(expr);
}

// ---- Unary Quantifiers ----
void	TreeVisitor::visitZeroOrMore(const ZeroOrMore &expr)
{
	printNode(expr);
	visitChildren(expr);
}
void	TreeVisitor::visitOneOrMore(const OneOrMore &expr)
{
	printNode(expr);
	visitChildren(expr);
}
void	TreeVisitor::visitOptional(const Optional &expr)
{
	printNode(expr);
	visitChildren(expr);
}

// ---- Unary Predicate ----
void	TreeVisitor::visitPredicate(const Predicate &expr)
{
	printNode(expr);
	visitChildren(expr);
}

// --- Unary Action ----
void	TreeVisitor::visitCapture(const Capture &expr)
{
	printNode(expr);
	visitChildren(expr);
}
void	TreeVisitor::visitFatal(const Fatal &expr)
{
	printNode(expr);
	visitChildren(expr);
}

// ---- RuleRef ----
void	TreeVisitor::visitRuleRef(const RuleRef &expr)
{
	printNode(expr);

	if (!_expandRef || !expr.resolved())
		return;

	_os << _prefix << (_isLast ? "   " : "│  ")
		<< ft_log::color("↳ resolves to:", "\033[2m") << "\n";

	std::string oldPrefix = _prefix;
	_prefix += (_isLast ? "   " : "│  ");
	descend(expr.resolved(), true);
	_prefix = oldPrefix;
}

#endif

