/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnaryPredicate.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:10:28 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 12:31:37 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_UNARY_PREDICATE_HPP__
# define __FTPP_UNARY_PREDICATE_HPP__

# include <string>

# include "peg/core/Expr.hpp"
# include "peg/core/ExprUnaryBase.hpp"

// ============================================================================
// Predicate
// ============================================================================

/**
 * @brief Look-ahead predicate (positive or negative).
 *
 * Evaluates the inner expression without consuming input.
 * `isAnd()` determines whether the result is taken as-is (&) or negated (!).
 */
class Predicate : public ExprUnaryBase {

private:
	bool _isAnd;

public:
	Predicate(Expr *inner, bool isAnd)
		: ExprUnaryBase(K_PREDICATE, inner), _isAnd(isAnd)
	{}
	virtual ~Predicate() {}

	bool				isAnd(void) const { return _isAnd; }

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor &visitor) const;

	virtual std::string	reprKind(void) const	{ return "Predicate"; }
	virtual std::string	reprValue(void) const	{ return _isAnd ? "&" : "!"; }
};

#endif /* __FTPP_UNARY_PREDICATE_HPP__ */