/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnaryPredicate.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:39:33 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/01 23:06:12 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>

#include "AstNode.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/core/IExprVisitor.hpp"
#include "peg/syntax/UnaryPredicate.hpp"
#include "utils/Diag.hpp"
#include "utils/Input.hpp"

// ============================================================================
// Predicate
// ============================================================================

void	Predicate::accept(IExprVisitor &visitor) const
{
	visitor.visitPredicate(*this);
}

// Perform a look-ahead evaluation without consuming input.
bool	Predicate::parse(PackratParser &parser, AstNode *parent) const
{
	Diag	&errs = parser.diag();
	Input	&in = parser.input();
	size_t	start = in.pos();

	errs.save();
	const bool	ok = parser.eval(_inner, parent);
	errs.restore();
	in.setPos(start);

	return _isAnd ? ok : !ok;
}