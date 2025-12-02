/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprDebug.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 20:44:46 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 00:30:44 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

#if FTPP_DEBUG_GRAMMAR

# include <ostream>

# include "peg/core/Expr.hpp"
# include "peg/visitors/DebugVisitors.hpp"

// ============================================================================
// Expr debug functions
// ============================================================================

// Use TreeVisitor to perform the print of expressions
void	Expr::dumpTree(
	const Expr *root, std::ostream &os, int32_t maxDepth, bool expandRuleRefs
)
{
	if (!root)
	{
		os << "(null)\n";
		return;
	}
	TreeVisitor visitor(os, maxDepth, expandRuleRefs);
	root->accept(visitor);
}

// Use StatsVisitor to collect and print expressions statistiques
void	Expr::dumpStats(const Expr *root, std::ostream &os)
{
	if (!root)
	{
		os << "No expression to analyze\n";
		return;
	}
	StatsVisitor visitor;
	root->accept(visitor);
	visitor.print(os);
}

#endif

