/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Expr.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:55:00 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 13:49:11 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <string>

#include "ft_log/AnsiColor.hpp"
#include "ft_log/LogOp.hpp"

#include "peg/core/Expr.hpp"

// ============================================================================
// Expr
// ============================================================================

Expr::Expr(enum e_expr_kind kind): _kind(kind) {}
Expr::~Expr() {}

// Accessors
enum Expr::e_expr_kind	Expr::kind(void) const			{ return _kind; }
size_t					Expr::childCount(void) const	{ return 0; }
const Expr				*Expr::child(size_t) const		{ return NULL; }

// Representation
std::string	Expr::reprValue(void) const
{
	return "";
}

// Return a debug-friendly inline colored representation
std::string	Expr::repr(void) const
{
	std::ostringstream oss;

	oss << ft_log::color(reprKind(), FT_LOG_COLOR_YELLOW)
		<< ": "
		<< ft_log::color(reprValue(), FT_LOG_COLOR_GREEN);

	return oss.str();
}
