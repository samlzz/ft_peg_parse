/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GrammarImplDebug.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 00:58:12 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 12:13:17 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

#if FTPP_DEBUG_GRAMMAR || FTPP_DEBUG_PACKRAT

# include <ostream>

# include "GrammarImpl.hpp"
# include "peg/core/Expr.hpp"
# include "packrat/PackratParser.hpp"

// ============================================================================
// GrammarImpl debug functions
// ============================================================================

# if FTPP_DEBUG_GRAMMAR

void	Grammar::Impl::debugPrintGrammar(std::ostream &os) const
{
	for (t_ExprDict::const_iterator it = _rules.begin();
			it != _rules.end(); ++it)
	{
		os << "=== Rule: " << it->first << " ===\n";
		Expr::dumpTree(it->second, os, -1, false);
		os << "\n";
	}
}

void	Grammar::Impl::debugPrintRule(const std::string &ruleName,
										std::ostream &os) const
{
	t_ExprDict::const_iterator it = _rules.find(ruleName);
	if (it == _rules.end())
	{
		os << "Rule not found: " << ruleName << "\n";
		return;
	}
	Expr::dumpTree(it->second, os);
}

void	Grammar::Impl::debugPrintRuleStats(const std::string &ruleName,
											std::ostream &os) const
{
	t_ExprDict::const_iterator it = _rules.find(ruleName);
	if (it == _rules.end())
	{
		os << "Rule not found: " << ruleName << "\n";
		return;
	}
	Expr::dumpStats(it->second, os);
}

# endif // FTPP_DEBUG_GRAMMAR


# if FTPP_DEBUG_PACKRAT

void	Grammar::Impl::debugPrintPackratStats(std::ostream &os) const
{
	PackratParser::printStats(_lastStats, os);
}

# endif // FTPP_DEBUG_PACKRAT

#endif /* FTPP_DEBUG_GRAMMAR || FTPP_DEBUG_PACKRAT */

