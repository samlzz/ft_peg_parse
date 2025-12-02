/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprTerminalBase.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 12:35:00 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 13:31:11 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "peg/core/ExprTerminalBase.hpp"

// ============================================================================
// Construction / Destruction
// ============================================================================

ExprTerminalBase::ExprTerminalBase(enum e_expr_kind kind, const std::string &value)
	: Expr(kind), _value(value)
{}

ExprTerminalBase::~ExprTerminalBase() {}

// ============================================================================
// Accessors
// ============================================================================

void				ExprTerminalBase::setValue(const std::string &v)
{
	_value = v;
}

const std::string&	ExprTerminalBase::value() const
{
	return _value;
}
