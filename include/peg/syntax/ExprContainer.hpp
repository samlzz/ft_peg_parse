/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprContainer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 18:54:44 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/25 15:51:12 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPRCONTAINER_HPP
# define EXPRCONTAINER_HPP

# include "peg/Expr.hpp"

// ============================================================================
// ExprContainer (abstract helper class)
// ============================================================================

/**
 * @brief Base class for PEG expressions that contain multiple child nodes.
 *
 * Provides generic storage and iteration over a list of expressions.
 * Concrete types such as Sequence or Choice extend it and implement
 * their parsing logic.
 */
class ExprContainer : public Expr {

private:
	ExprContainer();
	ExprContainer(const ExprContainer &other);
	ExprContainer &operator=(const ExprContainer &other);

protected:
	t_ExprList _elems;

public:
	ExprContainer(enum e_expr_kind kind)
		: Expr(kind), _elems()
	{}

	ExprContainer(enum e_expr_kind kind, const t_ExprList &elems)
		: Expr(kind), _elems(elems)
	{}

	virtual ~ExprContainer() { deleteAll(_elems); }

	// ---- Element access ----
	Expr				*operator[](size_t index)		{ return _elems[index]; }
	const Expr			*operator[](size_t index) const	{ return _elems[index]; }

	virtual size_t		childCount() const			{ return _elems.size(); }
	virtual const Expr	*child(size_t index) const	{ return _elems[index]; }

	// ---- Container getters ----
	const t_ExprList&			elems() const	{ return _elems; }
	t_ExprList&					elems()			{ return _elems; }

	t_ExprList::const_iterator	begin() const	{ return _elems.begin(); }
	t_ExprList::const_iterator	end() const		{ return _elems.end(); }

	void						add(Expr *e) { if (e) _elems.push_back(e); }

// ---- Debug functions ----
# if PEG_DEBUG_ANY
	virtual std::string	debugValue(void) const;
# endif
};

// ============================================================================
// Sequence
// ============================================================================

/**
 * @brief Ordered concatenation of sub-expressions.
 *
 * All elements must match sequentially for the sequence to succeed.
 */
class Sequence : public ExprContainer {

public:
	Sequence(): ExprContainer(K_SEQUENCE) {}
	Sequence(const t_ExprList &elements): ExprContainer(K_SEQUENCE, elements)
	{}
	virtual ~Sequence() {}

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor &visitor) const;

# if PEG_DEBUG_ANY
	virtual std::string	debugName(void) const { return "Sequence"; }
# endif
};

// ============================================================================
// Choice
// ============================================================================

/**
 * @brief Ordered choice between several alternatives.
 *
 * Tries each option until one succeeds. Does not fail until all
 * alternatives fail.
 */
class Choice : public ExprContainer {

public:
	Choice(): ExprContainer(K_CHOICE) {}
	Choice(const t_ExprList &options): ExprContainer(K_CHOICE, options)
	{}
	virtual ~Choice() {}

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor &visitor) const;

# if PEG_DEBUG_ANY
	virtual std::string	debugName(void) const { return "Choice"; }
# endif
};

#endif

