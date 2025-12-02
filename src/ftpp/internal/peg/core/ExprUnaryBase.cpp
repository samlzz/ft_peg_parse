/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprUnaryBase.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 12:35:00 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 13:42:52 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "peg/core/ExprUnaryBase.hpp"

// ============================================================================
// Construction / Destruction
// ============================================================================

ExprUnaryBase::ExprUnaryBase(enum e_expr_kind kind, Expr *inner)
	: Expr(kind), _inner(inner)
{}

ExprUnaryBase::~ExprUnaryBase()
{
	delete _inner;
}

// ============================================================================
// Accessors
// ============================================================================

Expr*		ExprUnaryBase::inner()			{ return _inner; }
const Expr*	ExprUnaryBase::inner() const	{ return _inner; }

void	ExprUnaryBase::setInner(Expr *e)
{
	if (_inner != e)
	{
		delete _inner;
		_inner = e;
	}
}

// ============================================================================
// Child interface
// ============================================================================

size_t	ExprUnaryBase::childCount(void) const
{
	return _inner ? 1 : 0;
}

Expr*	ExprUnaryBase::child(size_t idx) const
{
	return (idx == 0 ? _inner : NULL);
}

