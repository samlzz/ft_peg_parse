/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprCombinatorBase.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 18:54:44 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 13:50:07 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_EXPR_COMBINATORS_BASE_HPP__
# define __FTPP_EXPR_COMBINATORS_BASE_HPP__

# include "peg/core/Expr.hpp"

// ============================================================================
// ExprCombinatorBase (abstract helper class)
// ============================================================================

/**
 * @brief Base class for PEG expressions that contain multiple child nodes.
 *
 * Provides generic storage and iteration over a list of expressions.
 */
class ExprCombinatorBase : public Expr {

private:
	ExprCombinatorBase();
	ExprCombinatorBase(const ExprCombinatorBase &other);
	ExprCombinatorBase &operator=(const ExprCombinatorBase &other);

protected:
	t_ExprList _elems;

public:
	ExprCombinatorBase(enum e_expr_kind kind);
	ExprCombinatorBase(enum e_expr_kind kind, const t_ExprList &elems);

	virtual ~ExprCombinatorBase();

	// ---- Element access ----
	Expr						*operator[](size_t index);
	const Expr					*operator[](size_t index) const;

	virtual size_t				childCount() const;
	virtual const Expr			*child(size_t index) const;

	// ---- Container getters ----
	const t_ExprList&			elems() const;
	t_ExprList&					elems();

	t_ExprList::const_iterator	begin() const;
	t_ExprList::const_iterator	end() const;

	virtual std::string			reprValue(void) const;
};

#endif /* __FTPP_EXPR_COMBINATORS_BASE_HPP__ */