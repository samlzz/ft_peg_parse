/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FistSymbolVisitor.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 20:45:00 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/01 12:17:30 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "peg/visitors/FirstSymbolVisitor.hpp"
#include "peg/syntax/CombinatorOps.hpp"
#include "peg/syntax/RuleRef.hpp"
#include "peg/syntax/UnaryActions.hpp"
#include "peg/syntax/UnaryPredicate.hpp"
#include "peg/syntax/UnaryQuantifiers.hpp"

// ============================================================================
// Helpers
// ============================================================================

void FirstSymbolVisitor::visitNode(const Expr *expr)
{
	if (_match || !expr || _visited.count(expr))
		return;

	_visited.insert(expr);
	expr->accept(*this);
}

// ============================================================================
// Construction
// ============================================================================

FirstSymbolVisitor::FirstSymbolVisitor(const std::string &target)
	: _target(target), _visited(), _match(false)
{}

// ============================================================================
// Public API
// ============================================================================

bool FirstSymbolVisitor::matches(const Expr *expr)
{
	_visited.clear();
	_match = false;

	visitNode(expr);
	return _match;
}

// ============================================================================
// Visitor Methods
// ============================================================================

void FirstSymbolVisitor::visitLiteral(const Literal &) {}
void FirstSymbolVisitor::visitCharRange(const CharRange &) {}
void FirstSymbolVisitor::visitAny(const Any &) {}

void FirstSymbolVisitor::visitSequence(const Sequence &expr)
{
	if (!expr.elems().empty())
		visitNode(expr.elems().front());
}

void FirstSymbolVisitor::visitChoice(const Choice &expr)
{
	for (t_ExprList::const_iterator it = expr.begin();
			!_match && it != expr.end(); ++it)
		visitNode(*it);
}

void FirstSymbolVisitor::visitZeroOrMore(const ZeroOrMore &expr)
{
	visitNode(expr.inner());
}

void FirstSymbolVisitor::visitOneOrMore(const OneOrMore &expr)
{
	visitNode(expr.inner());
}

void FirstSymbolVisitor::visitOptional(const Optional &expr)
{
	visitNode(expr.inner());
}

void FirstSymbolVisitor::visitPredicate(const Predicate &expr)
{
	visitNode(expr.inner());
}

void FirstSymbolVisitor::visitRuleRef(const RuleRef &expr)
{
	_match = (expr.name() == _target);
}

void FirstSymbolVisitor::visitCapture(const Capture &expr)
{
	visitNode(expr.inner());
}

void FirstSymbolVisitor::visitFatal(const Fatal &expr)
{
	visitNode(expr.inner());
}

