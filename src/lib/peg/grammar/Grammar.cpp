/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Grammar.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 19:08:10 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/27 18:16:16 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
#include <string>

#include "peg/grammar/Expr.hpp"
#include "peg/grammar/Grammar.hpp"
#include "peg/grammar/FirstSymbolVisitor.hpp"
#include "peg/grammar/ResolutionVisitor.hpp"

// ============================================================================
// Constructors / assignment
// ============================================================================

Grammar::Grammar(t_ExprDict &rules) : _rules(), _start()
{
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

// Rule reference resolution
void	Grammar::resolveRefs(void)
{
	ResolutionVisitor	visitor(_rules);

	for (t_ExprDict::iterator it = _rules.begin(); it != _rules.end(); ++it)
		visitor.resolve(it->second);
}

// Left recursion detection
void	Grammar::checkLeftRecursion(void) const
{
	for (t_ExprDict::const_iterator it = _rules.begin(); it != _rules.end(); ++it)
	{
		std::string			ruleName = it->first;
		FirstSymbolVisitor	visitor(ruleName);

		if (visitor.matches(it->second))
			throw GrammarLeftRecursionError(ruleName);
	}
}

