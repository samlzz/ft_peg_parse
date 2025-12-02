/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprCombinatorBase.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:55:00 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/03 12:00:00 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>

#include "peg/core/ExprCombinatorBase.hpp"

// ============================================================================
// Construction / Destruction
// ============================================================================

ExprCombinatorBase::ExprCombinatorBase(enum e_expr_kind kind)
	: Expr(kind), _elems()
{}

ExprCombinatorBase::ExprCombinatorBase(enum e_expr_kind kind, const t_ExprList &elems)
	: Expr(kind), _elems(elems)
{}

ExprCombinatorBase::~ExprCombinatorBase()
{
	deleteAll(_elems);
}

// ============================================================================
// Element access
// ============================================================================

Expr		*
ExprCombinatorBase::operator[](size_t index)		{ return _elems[index]; }
const Expr	*
ExprCombinatorBase::operator[](size_t index) const	{ return _elems[index]; }

size_t		
ExprCombinatorBase::childCount() const				{ return _elems.size(); }
const Expr	*
ExprCombinatorBase::child(size_t index) const		{ return _elems[index]; }

// ============================================================================
// Container access
// ============================================================================

const t_ExprList	&
ExprCombinatorBase::elems() const	{ return _elems; }
t_ExprList			&
ExprCombinatorBase::elems()			{ return _elems; }

t_ExprList::const_iterator	
ExprCombinatorBase::begin() const	{ return _elems.begin(); }

t_ExprList::const_iterator	
ExprCombinatorBase::end() const		{ return _elems.end(); }

// ============================================================================
// Representation
// ============================================================================

std::string	ExprCombinatorBase::reprValue(void) const
{
	std::ostringstream oss;

	oss << "(" << _elems.size() << " elem" << (_elems.size() > 1 ? "s" : "") << ")";
	return oss.str();
}
