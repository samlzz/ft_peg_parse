/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatsVisitor.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 00:01:33 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/27 17:56:41 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/DebugConfig.hpp"

#if PEG_DEBUG_ANY

# include <iomanip>

# include "peg/grammar/Expr.hpp"
# include "peg/grammar/ExprDebugVisitors.hpp"
# include "peg/syntax/ExprContainer.hpp"
# include "peg/syntax/ExprLeaf.hpp"
# include "peg/syntax/ExprUnary.hpp"
# include "peg/syntax/RuleRef.hpp"

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

// ---- Macro to generate visit methods ----
# define STATS_VISIT_IMPL(ClassName) \
	void StatsVisitor::visit##ClassName(const ClassName &expr) \
	{ \
		recordNode(expr); \
		for (size_t i = 0; i < expr.childCount(); ++i) \
			descend(expr.child(i)); \
	}

// Implementations
STATS_VISIT_IMPL(Literal)
STATS_VISIT_IMPL(CharRange)
STATS_VISIT_IMPL(Any)

STATS_VISIT_IMPL(Sequence)
STATS_VISIT_IMPL(Choice)

STATS_VISIT_IMPL(ZeroOrMore)
STATS_VISIT_IMPL(OneOrMore)
STATS_VISIT_IMPL(Optional)
STATS_VISIT_IMPL(Predicate)
STATS_VISIT_IMPL(Capture)
STATS_VISIT_IMPL(Fatal)

# undef STATS_VISIT_IMPL

void	StatsVisitor::visitRuleRef(const RuleRef &expr)
{
	recordNode(expr);
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
		"Literal", "CharRange", "Sequence", "Choice",
		"ZeroOrMore", "OneOrMore", "Optional", "Predicate",
		"RuleRef", "Capture", "Any"
	};

	for (std::map<Expr::e_expr_kind, size_t>::const_iterator it = _kindCounts.begin();
			it != _kindCounts.end(); ++it)
	{
		const char	*name = (it->first < 11) ? kindNames[it->first] : "Unknown";
		os << "║   " << std::setw(20)
			<< std::left << name << std::setw(15)
			<< std::right << it->second
			<< "║\n";
	}

	os << "╚═══════════════════════════════════════╝\n";
}

#endif

