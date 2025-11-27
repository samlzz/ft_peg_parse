/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GrammarDebug.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:42:07 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/27 17:53:37 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/DebugConfig.hpp"

#if PEG_DEBUG_GRAMMAR

# include <iomanip>

# include "peg/grammar/Grammar.hpp"
# include "peg/grammar/Expr.hpp"

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
			Expr::printTree(it->second, os, opts.maxDepth);
		else
			os << it->second->debugRepr(opts.colored);

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
	Expr::printTree(expr, os, opts.maxDepth);
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
		Expr::printStats(get(_start), os);
	else
		os << "║ (no start rule)                       ║\n";

	os << "╚═══════════════════════════════════════╝\n";
}

#endif // PEG_DEBUG_GRAMMAR

