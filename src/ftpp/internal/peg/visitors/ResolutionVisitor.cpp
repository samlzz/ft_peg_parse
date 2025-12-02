/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResolutionVisitor.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 20:17:00 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/01 22:24:10 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FtppException.hpp"
#include "peg/visitors/ResolutionVisitor.hpp"
#include "peg/syntax/CombinatorOps.hpp"
#include "peg/syntax/UnaryActions.hpp"
#include "peg/syntax/UnaryPredicate.hpp"
#include "peg/syntax/UnaryQuantifiers.hpp"
#include "peg/syntax/RuleRef.hpp"

// ============================================================================
// Helpers
// ============================================================================

void ResolutionVisitor::visitChild(const Expr *child)
{
	if (!child || _visited.count(child))
		return;

	_visited.insert(child);
	child->accept(*this);
}

// ============================================================================
// Construction
// ============================================================================

ResolutionVisitor::ResolutionVisitor(const t_ExprDict &rules)
	: _rules(rules), _visited()
{}

// ============================================================================
// Public API
// ============================================================================

void ResolutionVisitor::resolve(Expr *root)
{
	visitChild(root);
}

// ============================================================================
// Visitor Methods
// ============================================================================

void ResolutionVisitor::visitLiteral(const Literal &) {}
void ResolutionVisitor::visitCharRange(const CharRange &) {}
void ResolutionVisitor::visitAny(const Any &) {}

void ResolutionVisitor::visitSequence(const Sequence &expr)
{
	for (t_ExprList::const_iterator it = expr.begin(); it != expr.end(); ++it)
		visitChild(*it);
}

void ResolutionVisitor::visitChoice(const Choice &expr)
{
	for (t_ExprList::const_iterator it = expr.begin(); it != expr.end(); ++it)
		visitChild(*it);
}

void ResolutionVisitor::visitZeroOrMore(const ZeroOrMore &expr)
{
	visitChild(expr.inner());
}

void ResolutionVisitor::visitOneOrMore(const OneOrMore &expr)
{
	visitChild(expr.inner());
}

void ResolutionVisitor::visitOptional(const Optional &expr)
{
	visitChild(expr.inner());
}

void ResolutionVisitor::visitPredicate(const Predicate &expr)
{
	visitChild(expr.inner());
}

void ResolutionVisitor::visitRuleRef(const RuleRef &expr)
{
	RuleRef &mutableExpr = const_cast<RuleRef &>(expr);

	if (mutableExpr.resolved())
		return;

	std::string ruleName = mutableExpr.name();
	if (!ruleName.empty() && ruleName[0] == '@')
		ruleName = ruleName.substr(1);

	t_ExprDict::const_iterator it = _rules.find(ruleName);
	if (it == _rules.end())
		throw GrammarResolutionError(ruleName);

	mutableExpr.resolve(it->second);
}

void ResolutionVisitor::visitCapture(const Capture &expr)
{
	visitChild(expr.inner());
}

void ResolutionVisitor::visitFatal(const Fatal &expr)
{
	visitChild(expr.inner());
}

