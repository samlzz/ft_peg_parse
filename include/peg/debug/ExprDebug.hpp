/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprDebug.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 01:38:43 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/25 21:00:00 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPRDEBUG_HPP
# define EXPRDEBUG_HPP

# include "utils/DebugConfig.hpp"

# if PEG_DEBUG_ANY

#  include <iostream>
#  include <ostream>
#  include <stdint.h>

#  include "peg/Expr.hpp"

namespace ExprDebug {

// ============================================================================
// Expr debug helpers
// ============================================================================

/**
 * @brief Print a compact inline representation of an expression.
 *
 * Delegates formatting to PrintVisitor. If expr is NULL, prints "(null)".
 */
void	printExpr(const Expr *expr, std::ostream &os = std::cerr);

/**
 * @brief Print an expression as a tree structure.
 *
 * Uses TreeVisitor to display children with indentation and
 * optional maximum depth truncation.
 */
void	printExprTree(const Expr *expr,
						std::ostream &os = std::cerr,
						int32_t maxDepth = -1);

/**
 * @brief Compute and print statistics about an expression tree.
 *
 * Includes total node count, maximum depth, and frequency of
 * expression kinds.
 */
void	printExprStats(const Expr *expr, std::ostream &os = std::cerr);

} // namespace ExprDebug

# endif // PEG_DEBUG_ANY
#endif

