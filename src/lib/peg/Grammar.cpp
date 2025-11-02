/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   Grammar.cpp										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: sliziard <sliziard@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/11/01 00:38:12 by sliziard		  #+#	#+#			 */
/*   Updated: 2025/11/02 20:39:04 by sliziard		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include <cstddef>
#include <set>
#include <string>

#include "peg/Expr.hpp"
#include "peg/Grammar.hpp"
#include "peg/syntax/ExprContainer.hpp"
#include "peg/syntax/ExprUnary.hpp"
#include "peg/syntax/RuleRef.hpp"

// ---- Ctors ----

Grammar::Grammar(t_ExprDict &rules): _rules(), _start()
{
	_rules.swap(rules);
	rules.clear();
	resolveRefs();
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
	return (*this);
}

// ---- Methods ----

Expr	*Grammar::get(const std::string &name) const
{
	t_ExprDict::const_iterator it = _rules.find(name);

	if (it == _rules.end())
		return (NULL);
	return (it->second);
}

static void	resolveExpr(Expr *expr, const t_ExprDict &rules)
{
	if (!expr)
		return;
	switch (expr->kind())
	{
	case Expr::K_LITERAL:
	case Expr::K_CHARRANGE:
		return;
	case Expr::K_SEQUENCE:
	case Expr::K_CHOICE: {
		const ExprContainer	*c = static_cast<ExprContainer *>(expr);
		for (t_ExprList::const_iterator it = c->begin(); it != c->end(); ++it)
			resolveExpr(*it, rules);
		break;
	}
	case Expr::K_ZERO_OR_MORE:
	case Expr::K_ONE_OR_MORE:
	case Expr::K_OPTIONAL:
	case Expr::K_PREDICATE:
	case Expr::K_CAPTURE: {
		ExprUnary *u = static_cast<ExprUnary *>(expr);
		resolveExpr(u->inner(), rules);
		break;
	}
	case Expr::K_RULEREF: {
		RuleRef *r = static_cast<RuleRef *>(expr);
		t_ExprDict::const_iterator it = rules.find(r->name());
		if (it == rules.end())
			throw Grammar::GrammarResolutionError(r->name());
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

static bool firstSymbolIs(Expr *expr, const std::string &target, std::set<const Expr*> &visited)
{
	if (!expr || visited.count(expr))
		return false;
	visited.insert(expr);
	switch (expr->kind())
	{
	case Expr::K_RULEREF: {
		RuleRef *r = static_cast<RuleRef*>(expr);
		return r->name() == target;
	}
	case Expr::K_SEQUENCE: {
		Sequence *s = static_cast<Sequence*>(expr);
		if (!s->elems().empty())
			return firstSymbolIs(s->elems().front(), target, visited);
		break;
	}
	case Expr::K_CHOICE: {
		Choice *c = static_cast<Choice*>(expr);
		for (t_ExprList::const_iterator it = c->begin(); it != c->end(); ++it)
			if (firstSymbolIs(*it, target, visited))
				return true;
		break;
	}
	case Expr::K_ZERO_OR_MORE:
	case Expr::K_ONE_OR_MORE:
	case Expr::K_OPTIONAL:
	case Expr::K_PREDICATE:
	case Expr::K_CAPTURE: {
		ExprUnary *e = static_cast<ExprUnary *>(expr);
		return firstSymbolIs(e->inner(), target, visited);
	}
	default: break;
	}
	return false;
}

void	Grammar::checkLeftRecursion(void) const
{
	for (t_ExprDict::const_iterator it = _rules.begin(); it != _rules.end(); ++it)
	{
		std::string				ruleName = it->first;
		std::set<const Expr*>	visited;
		if (firstSymbolIs(it->second, ruleName, visited))
			throw GrammarLeftRecursionError(ruleName);
	}
}
