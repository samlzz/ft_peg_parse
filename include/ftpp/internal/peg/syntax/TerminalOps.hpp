/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TerminalOps.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:10:24 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 12:29:59 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_TERMINAL_OPS_HPP__
# define __FTPP_TERMINAL_OPS_HPP__

# include <string>

# include "peg/core/Expr.hpp"
# include "peg/core/ExprTerminalBase.hpp"
# include "utils/StringUtils.hpp"

// ============================================================================
// Literal
// ============================================================================

/**
 * @brief Matches an exact literal string.
 *
 * The constructor receives an escaped representation from the grammar
 * and stores the unescaped content internally.
 */
class Literal : public ExprTerminalBase {

public:
	Literal(const std::string &escaped)
		: ExprTerminalBase(K_LITERAL, unescapeString(escaped))
	{}

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor &visitor) const;

	virtual std::string	reprKind(void) const	{ return "Literal"; }
	virtual std::string reprValue(void) const	{ return escapeStringDisplay(_value); }
};

// ============================================================================
// CharRange
// ============================================================================

/**
 * @brief Matches a single character belonging to a computed charset.
 *
 * The constructor expands a compact charset representation (e.g. a-z)
 * into a fully enumerated string of allowed characters.
 */
class CharRange : public ExprTerminalBase {

public:
	CharRange(const std::string &charset)
		: ExprTerminalBase(K_CHARRANGE, expandCharSet(charset))
	{}

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor &visitor) const;

	virtual std::string	reprKind(void) const	{ return "CharRange"; }
	virtual std::string	reprValue(void) const	{ return escapeCharSetDisplay(_value); }
};

// ============================================================================
// Any
// ============================================================================

/**
 * @brief Matches any single character except EOF.
 */
class Any : public Expr {

public:
	Any() : Expr(K_ANY) {}

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor &visitor) const;

	virtual std::string	reprKind(void) const	{ return "CharAny"; }
	virtual std::string	reprValue(void) const	{ return "."; }
};

#endif /* __FTPP_TERMINAL_OPS_HPP__ */