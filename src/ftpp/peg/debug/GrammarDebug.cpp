/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GrammarDebug.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:42:07 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/01 11:49:23 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

#if FTPP_DEBUG_GRAMMAR

# include <iomanip>

# include "Grammar.hpp"
# include "peg/core/Expr.hpp"

// ============================================================================
// Grammar debug display
// ============================================================================

void	Grammar::print(std::ostream &os, const PrintOptions &opts) const
{
	os << "\n╔═══════════════════════════════════════════════════════════╗\n";
	os << "║                      PEG Grammar                          ║\n";
	os << "╠═══════════════════════════════════════════════════════════╣\n";
	os << "║ Start rule: " << std::setw(46) << std::left << _start << "║\n";
	os << "║ Total rules: " << std::setw(45) << _rules.size() << "║\n";
	os << "╚═══════════════════════════════════════════════════════════╝\n\n";

	for (t_ExprDict::const_iterator it = _rules.begin(); it != _rules.end(); ++it)
	{
		os << "\n┌─ Rule: " << it->first;
		if (it->first == _start)
			os << " (START)";
		os << "\n";

		if (opts.expandRules)
			Expr::dumpTree(it->second, os, opts.maxDepth);
		else
			os << it->second->repr();

		os << "└─────────────────────────────────\n";
	}
}

void	Grammar::printRule(const std::string &ruleName,
						std::ostream &os,
						const PrintOptions &opts) const
{
	const Expr *expr = get(ruleName);

	if (!expr)
	{
		os << "Rule '" << ruleName << "' not found\n";
		return;
	}

	os << "\nRule: " << ruleName << "\n";
	Expr::dumpTree(expr, os, opts.maxDepth);
}

void	Grammar::printStats(std::ostream &os) const
{
	os << "\n╔═══════════════════════════════════════╗\n";
	os << "║        Grammar Statistics             ║\n";
	os << "╠═══════════════════════════════════════╣\n";
	os << "║ Total rules: " << std::setw(20) << _rules.size() << "║\n";
	os << "║                                       ║\n";
	os << "║ Expression types:                     ║\n";

	if (!_start.empty())
		Expr::dumpStats(get(_start), os);
	else
		os << "║ (no start rule)                       ║\n";

	os << "╚═══════════════════════════════════════╝\n";
}

#endif // FTPP_DEBUG_GRAMMAR

