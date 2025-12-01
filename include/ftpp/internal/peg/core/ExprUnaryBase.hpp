/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprUnaryBase.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:10:28 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/01 11:55:11 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPR_UNARY_BASE_HPP
# define EXPR_UNARY_BASE_HPP

# include <cstddef>

# include "peg/core/Expr.hpp"

// ============================================================================
// ExprUnaryBase (abstract helper class)
// ============================================================================

/**
 * @brief Base class for unary PEG expressions.
 *
 * Wraps a single inner expression and defines a consistent child interface.
 * Concrete subclasses implement repetition, optionality, predicates,
 * or capture behaviors.
 */
class ExprUnaryBase : public Expr {

private:
	ExprUnaryBase();
	ExprUnaryBase(const ExprUnaryBase &other);
	ExprUnaryBase &operator=(const ExprUnaryBase &other);

protected:
	Expr *_inner;

public:
	ExprUnaryBase(enum e_expr_kind kind, Expr *inner)
		: Expr(kind), _inner(inner)
	{}
	virtual ~ExprUnaryBase() { delete _inner; }

	// ---- Accessors ----
	Expr*				inner()				{ return _inner; }
	const Expr*			inner() const		{ return _inner; }
	void				setInner(Expr *e)	{ replaceOne(e, _inner); }

	// ---- Child overrides ----
	virtual size_t		childCount(void) const		{ return _inner ? 1 : 0; }
	virtual Expr		*child(size_t idx) const	{ return idx == 0 ? _inner : NULL; }
};

#endif