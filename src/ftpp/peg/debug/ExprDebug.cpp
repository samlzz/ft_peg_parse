/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprDebug.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 20:44:46 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/30 02:11:14 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"
#include "ft_log/AnsiColor.hpp"
#include "ft_log/LogOp.hpp"

#if FTPP_DEBUG_EXPR

# include <ostream>
# include <sstream>
# include <string>

# include "peg/core/Expr.hpp"
# include "peg/visitors/DebugVisitors.hpp"

// ============================================================================
// Expr display functions
// ============================================================================

// Return a debug-friendly inline colored representation
std::string	Expr::repr(void) const
{
	std::ostringstream oss;

	oss << ft_log::color(reprKind(), FT_LOG_COLOR_YELLOW)
		<< ": "
		<< ft_log::color(reprValue(), FT_LOG_COLOR_GREEN);

	return oss.str();
}

// Use TreeVisitor to perform the print of expressions
void	Expr::dumpTree(const Expr *root, std::ostream &os, int32_t maxDepth)
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

