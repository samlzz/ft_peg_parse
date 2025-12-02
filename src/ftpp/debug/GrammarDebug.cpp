/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GrammarDebug.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 12:20:30 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 12:21:29 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

#if FTPP_DEBUG_GRAMMAR || FTPP_DEBUG_PACKRAT

# include "Grammar.hpp"
# include "GrammarImpl.hpp"

// ============================================================================
// Public debug API (thin delegation to Impl)
// ============================================================================

# if FTPP_DEBUG_GRAMMAR

void	Grammar::printRule(const std::string &rule, std::ostream &os) const
{
	_impl->debugPrintRule(rule, os);
}

void	Grammar::printGrammar(std::ostream &os) const
{
	_impl->debugPrintGrammar(os);
}

void	Grammar::printRuleStats(const std::string &rule, std::ostream &os) const
{
	_impl->debugPrintRuleStats(rule, os);
}

# endif

# if FTPP_DEBUG_PACKRAT

void	Grammar::printPackratStats(std::ostream &os) const 
{
	_impl->debugPrintPackratStats(os);
}

# endif

#endif