/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CombinatorOps.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 18:54:44 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 13:52:31 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_COMBINATOR_OPS_HPP__
# define __FTPP_COMBINATOR_OPS_HPP__

# include "peg/core/Expr.hpp"
# include "peg/core/ExprCombinatorBase.hpp"

// ============================================================================
// Sequence
// ============================================================================

/**
 * @brief Ordered concatenation of sub-expressions.
 *
 * All elements must match sequentially for the sequence to succeed.
 */
class Sequence : public ExprCombinatorBase {

public:
	Sequence(): ExprCombinatorBase(K_SEQUENCE) {}
	Sequence(const t_ExprList &elements)
		: ExprCombinatorBase(K_SEQUENCE, elements) {}

	virtual ~Sequence() {}

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor &visitor) const;

	virtual std::string	reprKind(void) const { return "Sequence"; }
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
class Choice : public ExprCombinatorBase {

public:
	Choice(): ExprCombinatorBase(K_CHOICE) {}
	Choice(const t_ExprList &options)
		: ExprCombinatorBase(K_CHOICE, options) {}

	virtual ~Choice() {}

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor &visitor) const;

	virtual std::string	reprKind(void) const { return "Choice"; }
};

#endif /* __FTPP_COMBINATOR_OPS_HPP__ */