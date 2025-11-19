/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprDebug.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 01:38:43 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/18 01:40:48 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPRDEBUG_HPP
# define EXPRDEBUG_HPP

# if PEG_DEBUG_LEVEL > 0

#  include <ostream>
#  include <stdint.h>

#  include "peg/Expr.hpp"

namespace ExprDebug {
// ============================================================================
// Public entry functions
// ============================================================================

void	printExpr(const Expr *expr, std::ostream &os = std::cerr);
void	printExprTree(const Expr *expr, std::ostream &os = std::cerr, int32_t maxDepth = -1);
void	printExprStats(const Expr *expr, std::ostream &os = std::cerr);

}

# endif
#endif