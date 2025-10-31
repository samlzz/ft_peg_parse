/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Expr.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 22:31:38 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 19:25:54 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "peg/Expr.hpp"

Expr::Expr(enum e_expr_kind kind): _kind(kind)
{}

enum Expr::e_expr_kind Expr::kind(void) const
{
	return _kind;
}

Expr::~Expr() {}

