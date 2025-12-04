/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatsVisitor.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 00:01:33 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/04 11:43:48 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

#if FTPP_DEBUG_GRAMMAR

# include <iomanip>

# include "peg/core/Expr.hpp"
# include "peg/visitors/DebugVisitors.hpp"
# include "peg/syntax/CombinatorOps.hpp"
# include "peg/syntax/RuleRef.hpp"
# include "peg/syntax/TerminalOps.hpp"
# include "peg/syntax/UnaryActions.hpp"
# include "peg/syntax/UnaryPredicate.hpp"
# include "peg/syntax/UnaryQuantifiers.hpp"

// ============================================================================
// StatsVisitor
// ============================================================================

StatsVisitor::StatsVisitor()
	: _totalNodes(0), _currentDepth(0), _maxDepth(0)
{}

// Record current node and update stats.
void	StatsVisitor::recordNode(const Expr &expr)
{
	_totalNodes++;
	_kindCounts[expr.kind()]++;

	if (_currentDepth > _maxDepth)
		_maxDepth = _currentDepth;
}

// Explore one child and adjust depth counters.
void	StatsVisitor::descend(const Expr *child)
{
	if (!child)
		return;

	_currentDepth++;
	child->accept(*this);
	_currentDepth--;
}

// ============================================================================
// Visit methods 
// ============================================================================

// ---- Terminals ----
void	StatsVisitor::visitLiteral(const Literal &expr)		{ recordNode(expr); }
void	StatsVisitor::visitCharRange(const CharRange &expr)	{ recordNode(expr); }
void	StatsVisitor::visitAny(const Any &expr)				{ recordNode(expr); }

// ---- Combinators ----
void	StatsVisitor::visitSequence(const Sequence &expr)
{
	recordNode(expr);
	for (size_t i = 0; i < expr.childCount(); ++i)
		descend(expr[i]);
}

void	StatsVisitor::visitChoice(const Choice &expr)
{
	recordNode(expr);
	for (size_t i = 0; i < expr.childCount(); ++i)
		descend(expr[i]);
}

// ---- Unary Quantifiers ----
void	StatsVisitor::visitZeroOrMore(const ZeroOrMore &expr)
{
	recordNode(expr);
	descend(expr.inner());
}
void	StatsVisitor::visitOneOrMore(const OneOrMore &expr)
{
	recordNode(expr);
	descend(expr.inner());
}
void	StatsVisitor::visitOptional(const Optional &expr)
{
	recordNode(expr);
	descend(expr.inner());
}

// ---- Unary Predicate ----
void	StatsVisitor::visitPredicate(const Predicate &expr)
{
	recordNode(expr);
	descend(expr.inner());
}

// --- Unary Action ----
void	StatsVisitor::visitCapture(const Capture &expr)
{
	recordNode(expr);
	descend(expr.inner());
}
void	StatsVisitor::visitFatal(const Fatal &expr)
{
	recordNode(expr);
	descend(expr.inner());
}

// ---- RuleRef ----
void	StatsVisitor::visitRuleRef(const RuleRef &expr)
{
	recordNode(expr);
	descend(expr.resolved());
}

// Print statistics summary (counts, depths, types).
void	StatsVisitor::print(std::ostream &os) const
{
	os << "\n╔═══════════════════════════════════════╗\n";
	os << "║     Expression Statistics             ║\n";
	os << "╠═══════════════════════════════════════╣\n";
	os << "║ Total nodes:   " << std::setw(23) << _totalNodes << "║\n";
	os << "║ Max depth:     " << std::setw(23) << _maxDepth << "║\n";
	os << "║                                       ║\n";
	os << "║ Node types:                           ║\n";

	static const char *kindNames[] = {
		"Literal",
		"CharRange",
		"Any",
		"Sequence",
		"Choice",
		"ZeroOrMore",
		"OneOrMore",
		"Optional",
		"Predicate",
		"RuleRef",
		"Capture",
		"Fatal",
	};

	for (std::map<Expr::e_expr_kind, size_t>::const_iterator it = _kindCounts.begin();
			it != _kindCounts.end(); ++it)
	{
		const char	*name = (it->first < 11) ? kindNames[it->first] : "Unknown";
		os << "║   " << std::setw(20)
			<< std::left << name << std::setw(16)
			<< std::right << it->second
			<< "║\n";
	}

	os << "╚═══════════════════════════════════════╝\n";
}

#endif

