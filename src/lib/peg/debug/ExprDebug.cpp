/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprDebug.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 20:44:46 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/20 16:34:24 by sliziard         ###   ########.fr       */
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

namespace ExprDebug {

void	printExpr(const Expr* expr, std::ostream &os)
{
	if (!expr)
	{
		os << "(null)";
		return;
	}
	PrintVisitor visitor(os);
	expr->accept(visitor);
}

void	printExprTree(const Expr* expr, std::ostream &os, int maxDepth)
{
	if (!expr)
	{
		os << "(null)\n";
		return;
	}
	TreeVisitor visitor(os, maxDepth);
	expr->accept(visitor);
}

void	printExprStats(const Expr* expr, std::ostream &os)
{
	if (!expr)
	{
		os << "No expression to analyze\n";
		return;
	}
	StatsVisitor visitor;
	expr->accept(visitor);
	visitor.print(os);
}

} // end namespace ExprDebug

std::string	Expr::debugRepr(bool colored) const
{
	std::ostringstream	oss;

	if (!colored)
		oss << debugName() << ": " << debugValue();
	else
		oss << PegDebug::Logger::color(debugName(), COLOR_YELLOW)
			<< ": "
			<< PegDebug::Logger::color(debugValue(), COLOR_GREEN);
	return oss.str();
}
#endif