/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprDebug.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 20:44:46 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/18 01:45:46 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if PEG_DEBUG_LEVEL > 0

# include <ostream>

# include "peg/Expr.hpp"
# include "peg/debug/ExprVisitors.hpp"

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

#endif