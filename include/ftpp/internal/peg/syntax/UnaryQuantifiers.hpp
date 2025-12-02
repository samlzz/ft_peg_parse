/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnaryQuantifiers.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:10:28 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 12:31:06 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_UNARY_QUANTIFIERS_HPP__
# define __FTPP_UNARY_QUANTIFIERS_HPP__

# include <string>

# include "peg/core/Expr.hpp"
# include "peg/core/ExprUnaryBase.hpp"

// ============================================================================
// ZeroOrMore
// ============================================================================

/**
 * @brief Matches zero or more repetitions of the inner expression.
 */
class ZeroOrMore : public ExprUnaryBase {

public:
	ZeroOrMore(Expr *inner) : ExprUnaryBase(K_ZERO_OR_MORE, inner) {}
	virtual ~ZeroOrMore() {}

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor &visitor) const;

	virtual std::string	reprKind(void) const	{ return "ZeroOrMore"; }
};

// ============================================================================
// OneOrMore
// ============================================================================

/**
 * @brief Matches one or more repetitions of the inner expression.
 */
class OneOrMore : public ExprUnaryBase {

public:
	OneOrMore(Expr *inner) : ExprUnaryBase(K_ONE_OR_MORE, inner) {}
	virtual ~OneOrMore() {}

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor &visitor) const;

	virtual std::string	reprKind(void) const	{ return "OneOrMore"; }
};

// ============================================================================
// Optional
// ============================================================================

/**
 * @brief Matches the inner expression if present, but never fails.
 */
class Optional : public ExprUnaryBase {

public:
	Optional(Expr *inner) : ExprUnaryBase(K_OPTIONAL, inner) {}
	virtual ~Optional() {}

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor &visitor) const;

	virtual std::string	reprKind(void) const	{ return "Optional"; }
};

#endif /* __FTPP_UNARY_QUANTIFIERS_HPP__ */