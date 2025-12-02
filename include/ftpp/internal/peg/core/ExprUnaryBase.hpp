/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprUnaryBase.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:10:28 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 13:50:26 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_EXPR_UNARY_BASE_HPP__
# define __FTPP_EXPR_UNARY_BASE_HPP__

# include <cstddef>

# include "peg/core/Expr.hpp"

// ============================================================================
// ExprUnaryBase (abstract helper class)
// ============================================================================

/**
 * @brief Base class for unary PEG expressions.
 *
 * Wraps a single inner expression and defines a consistent child interface.
 */
class ExprUnaryBase : public Expr {

private:
	ExprUnaryBase();
	ExprUnaryBase(const ExprUnaryBase &other);
	ExprUnaryBase &operator=(const ExprUnaryBase &other);

protected:
	Expr *_inner;

public:
	ExprUnaryBase(enum e_expr_kind kind, Expr *inner);
	virtual ~ExprUnaryBase();

	// ---- Accessors ----
	Expr*				inner();
	const Expr*			inner() const;
	void				setInner(Expr *e);

	// ---- Child overrides ----
	virtual size_t		childCount(void) const;
	virtual Expr		*child(size_t idx) const;
};

#endif /* __FTPP_EXPR_UNARY_BASE_HPP__ */