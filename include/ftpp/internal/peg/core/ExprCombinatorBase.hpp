/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprCombinatorBase.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 18:54:44 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/01 11:25:23 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPR_COMBINATORS_BASE_HPP
# define EXPR_COMBINATORS_BASE_HPP

# include "peg/core/Expr.hpp"

// ============================================================================
// ExprCombinatorBase (abstract helper class)
// ============================================================================

/**
 * @brief Base class for PEG expressions that contain multiple child nodes.
 *
 * Provides generic storage and iteration over a list of expressions.
 * Concrete types such as Sequence or Choice extend it and implement
 * their parsing logic.
 */
class ExprCombinatorBase : public Expr {

private:
	ExprCombinatorBase();
	ExprCombinatorBase(const ExprCombinatorBase &other);
	ExprCombinatorBase &operator=(const ExprCombinatorBase &other);

protected:
	t_ExprList _elems;

public:
	ExprCombinatorBase(enum e_expr_kind kind)
		: Expr(kind), _elems()
	{}

	ExprCombinatorBase(enum e_expr_kind kind, const t_ExprList &elems)
		: Expr(kind), _elems(elems)
	{}

	virtual ~ExprCombinatorBase() { deleteAll(_elems); }

	// ---- Element access ----
	Expr				*operator[](size_t index)		{ return _elems[index]; }
	const Expr			*operator[](size_t index) const	{ return _elems[index]; }

	virtual size_t		childCount() const				{ return _elems.size(); }
	virtual const Expr	*child(size_t index) const		{ return _elems[index]; }

	// ---- Container getters ----
	const t_ExprList&			elems() const			{ return _elems; }
	t_ExprList&					elems()					{ return _elems; }

	t_ExprList::const_iterator	begin() const			{ return _elems.begin(); }
	t_ExprList::const_iterator	end() const				{ return _elems.end(); }

	virtual std::string			reprValue(void) const;
};

#endif