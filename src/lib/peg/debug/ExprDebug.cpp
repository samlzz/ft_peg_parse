/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprDebug.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 20:44:46 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/26 10:26:49 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/DebugConfig.hpp"

#if PEG_DEBUG_ANY

# include <ostream>
# include <sstream>
# include <string>

# include "peg/Expr.hpp"
# include "peg/debug/ExprVisitors.hpp"
# include "utils/DebugLogger.hpp"

// ============================================================================
// Expr display functions
// ============================================================================

// Return a debug-friendly inline colored representation, optionally colored.
std::string	Expr::debugRepr(bool colored) const
{
	std::ostringstream oss;

	if (!colored)
		oss << debugName() << ": " << debugValue();
	else
		oss << PegDebug::Logger::color(debugName(), COLOR_YELLOW)
			<< ": "
			<< PegDebug::Logger::color(debugValue(), COLOR_GREEN);

	return oss.str();
}

// Use TreeVisitor to perform the print of expressions
void	Expr::printTree(const Expr *root, std::ostream &os, int32_t maxDepth)
{
	if (!root)
	{
		os << "(null)\n";
		return;
	}
	TreeVisitor visitor(os, maxDepth);
	root->accept(visitor);
}

// Use StatsVisitor to collect and print expressions statistiques
void	Expr::printStats(const Expr *root, std::ostream &os)
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

