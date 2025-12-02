/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseRepr.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 11:47:33 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/01 11:50:13 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <string>

#include "ft_log/AnsiColor.hpp"
#include "ft_log/LogOp.hpp"
#include "peg/core/Expr.hpp"
#include "peg/core/ExprCombinatorBase.hpp"

// ============================================================================
// Expr
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

// ============================================================================
// ExprContainer
// ============================================================================

// Generic debug function
std::string ExprCombinatorBase::reprValue(void) const
{
	std::ostringstream	oss;

	oss << "(" << _elems.size() << " elem" << (_elems.size() > 1 ? "s" : "") << ")";
	return oss.str();
}
