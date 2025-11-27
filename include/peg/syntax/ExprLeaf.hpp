/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprLeaf.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:10:24 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/27 17:51:33 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPRLEAF_HPP
# define EXPRLEAF_HPP

# include <string>

# include "peg/grammar/Expr.hpp"
# include "utils/StringUtils.hpp"

// ============================================================================
// ExprLeaf
// ============================================================================

/**
 * @brief Base class for leaf PEG expressions holding a string value.
 *
 * Provides a simple storage for character data such as literals or
 * character ranges. Concrete subclasses implement their matching logic.
 */
class ExprLeaf : public Expr {

protected:
	std::string _value;

public:
	ExprLeaf(enum e_expr_kind kind, const std::string &value)
		: Expr(kind), _value(value)
	{}
	virtual ~ExprLeaf() {}

	// ---- Accessors ----
	const std::string&	value() const					{ return _value; }
	void				setValue(const std::string &v)	{ _value = v; }

# if PEG_DEBUG_ANY
	virtual std::string	debugValue(void) const			{ return escapeStringDisplay(_value); }
# endif

private:
	ExprLeaf();
	ExprLeaf(const ExprLeaf &other);
	ExprLeaf &operator=(const ExprLeaf &other);
};

// ============================================================================
// Literal
// ============================================================================

/**
 * @brief Matches an exact literal string.
 *
 * The constructor receives an escaped representation from the grammar
 * and stores the unescaped content internally.
 */
class Literal : public ExprLeaf {

public:
	Literal(const std::string &escaped)
		: ExprLeaf(K_LITERAL, unescapeString(escaped))
	{}

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor &visitor) const;

# if PEG_DEBUG_ANY
	virtual std::string	debugName(void) const { return "Literal"; }
# endif
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
class CharRange : public ExprLeaf {

public:
	CharRange(const std::string &charset)
		: ExprLeaf(K_CHARRANGE, expandCharSet(charset))
	{}

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor &visitor) const;

# if PEG_DEBUG_ANY
	virtual std::string	debugName(void) const	{ return "CharRange"; }
	virtual std::string	debugValue(void) const	{ return escapeCharSetDisplay(_value); }
# endif
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

# if PEG_DEBUG_ANY
	virtual std::string	debugName(void) const	{ return "CharAny"; }
	virtual std::string	debugValue(void) const	{ return "."; }
# endif
};

#endif

