/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Grammar.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 19:08:10 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/27 17:53:47 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
#include <set>
#include <string>

#include "peg/grammar/Expr.hpp"
#include "peg/grammar/Grammar.hpp"
#include "peg/syntax/ExprContainer.hpp"
#include "peg/syntax/ExprUnary.hpp"
#include "peg/syntax/RuleRef.hpp"

// ============================================================================
// Constructors / assignment
// ============================================================================

Grammar::Grammar(t_ExprDict &rules) : _rules(), _start()
{
	// Transfer ownership
	_rules.swap(rules);
	rules.clear();

	// Resolve rule references
	resolveRefs();

	// Auto-select first rule as start rule
	if (!_rules.empty())
		_start = _rules.begin()->first;
}

Grammar	&Grammar::operator=(Grammar &other)
{
	if (this != &other)
	{
		deleteVals(_rules);
		_rules.swap(other._rules);
		other._rules.clear();
		_start = other._start;
	}
	return *this;
}

// ============================================================================
// Public methods
// ============================================================================

Expr	*Grammar::get(const std::string &name) const
{
	t_ExprDict::const_iterator it = _rules.find(name);
	if (it == _rules.end())
		return NULL;
	return it->second;
}

// ============================================================================
// Rule reference resolution
// ============================================================================

static void	resolveExpr(Expr *expr, const t_ExprDict &rules)
{
	if (!expr)
		return;

	switch (expr->kind())
	{
	case Expr::K_LITERAL:
	case Expr::K_CHARRANGE:
	case Expr::K_ANY:
		return;

	case Expr::K_SEQUENCE:
	case Expr::K_CHOICE:
	{
		const ExprContainer *c = static_cast<ExprContainer *>(expr);
		for (t_ExprList::const_iterator it = c->begin(); it != c->end(); ++it)
			resolveExpr(*it, rules);
		break;
	}

	case Expr::K_ZERO_OR_MORE:
	case Expr::K_ONE_OR_MORE:
	case Expr::K_OPTIONAL:
	case Expr::K_PREDICATE:
	case Expr::K_CAPTURE:
	case Expr::K_FATAL:
	{
		ExprUnary *u = static_cast<ExprUnary *>(expr);
		resolveExpr(u->inner(), rules);
		break;
	}

	case Expr::K_RULEREF:
	{
		RuleRef *r = static_cast<RuleRef *>(expr);
		if (r->resolved())
			break;

		std::string ruleName = r->name();
		if (ruleName[0] == '@')
			ruleName = ruleName.substr(1);

		t_ExprDict::const_iterator it = rules.find(ruleName);
		if (it == rules.end())
			throw Grammar::GrammarResolutionError(ruleName);

		r->resolve(it->second);
		break;
	}
	}
}

void	Grammar::resolveRefs(void)
{
	for (t_ExprDict::iterator it = _rules.begin(); it != _rules.end(); ++it)
		resolveExpr(it->second, _rules);
}

// ============================================================================
// Left recursion detection
// ============================================================================

static bool	firstSymbolIs(
	Expr *expr,
	const std::string &target,
	std::set<const Expr *> &visited)
{
	if (!expr || visited.count(expr))
		return false;

	visited.insert(expr);

	switch (expr->kind())
	{
	case Expr::K_RULEREF:
	{
		RuleRef *r = static_cast<RuleRef *>(expr);
		return r->name() == target;
	}

	case Expr::K_SEQUENCE:
	{
		Sequence *s = static_cast<Sequence *>(expr);
		if (!s->elems().empty())
			return firstSymbolIs(s->elems().front(), target, visited);
		break;
	}

	case Expr::K_CHOICE:
	{
		Choice *c = static_cast<Choice *>(expr);
		for (t_ExprList::const_iterator it = c->begin(); it != c->end(); ++it)
			if (firstSymbolIs(*it, target, visited))
				return true;
		break;
	}

	case Expr::K_ZERO_OR_MORE:
	case Expr::K_ONE_OR_MORE:
	case Expr::K_OPTIONAL:
	case Expr::K_PREDICATE:
	case Expr::K_CAPTURE:
	case Expr::K_FATAL:
	{
		ExprUnary *u = static_cast<ExprUnary *>(expr);
		return firstSymbolIs(u->inner(), target, visited);
	}

	default:
		break;
	}

	return false;
}

void	Grammar::checkLeftRecursion(void) const
{
	for (t_ExprDict::const_iterator it = _rules.begin(); it != _rules.end(); ++it)
	{
		std::string ruleName = it->first;
		std::set<const Expr *> visited;

		if (firstSymbolIs(it->second, ruleName, visited))
			throw GrammarLeftRecursionError(ruleName);
	}
}

