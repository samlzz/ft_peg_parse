/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprUnary.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:10:28 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/25 15:43:07 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPRUNARY_HPP
# define EXPRUNARY_HPP

# include <cstddef>
# include <string>

# include "peg/Expr.hpp"

// ============================================================================
// ExprUnary
// ============================================================================

/**
 * @brief Base class for unary PEG expressions.
 *
 * Wraps a single inner expression and defines a consistent child interface.
 * Concrete subclasses implement repetition, optionality, predicates,
 * or capture behaviors.
 */
class ExprUnary : public Expr {

private:
	ExprUnary();
	ExprUnary(const ExprUnary &other);
	ExprUnary &operator=(const ExprUnary &other);

protected:
	Expr *_inner;

public:
	ExprUnary(enum e_expr_kind kind, Expr *inner)
		: Expr(kind), _inner(inner)
	{}
	virtual ~ExprUnary() { delete _inner; }

	// ---- Accessors ----
	Expr*				inner()				{ return _inner; }
	const Expr*			inner() const		{ return _inner; }
	void				setInner(Expr *e);

	// ---- Child overrides ----
	virtual size_t		childCount(void) const		{ return _inner ? 1 : 0; }
	virtual Expr		*child(size_t idx) const	{ return idx == 0 ? _inner : NULL; }
};

// ============================================================================
// ZeroOrMore
// ============================================================================

/**
 * @brief Matches zero or more repetitions of the inner expression.
 */
class ZeroOrMore : public ExprUnary {

public:
	ZeroOrMore(Expr *inner) : ExprUnary(K_ZERO_OR_MORE, inner) {}
	virtual ~ZeroOrMore() {}

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor &visitor) const;

# if PEG_DEBUG_ANY
	virtual std::string	debugName(void) const { return "ZeroOrMore"; }
# endif
};

// ============================================================================
// OneOrMore
// ============================================================================

/**
 * @brief Matches one or more repetitions of the inner expression.
 */
class OneOrMore : public ExprUnary {

public:
	OneOrMore(Expr *inner) : ExprUnary(K_ONE_OR_MORE, inner) {}
	virtual ~OneOrMore() {}

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor &visitor) const;

# if PEG_DEBUG_ANY
	virtual std::string	debugName(void) const { return "OneOrMore"; }
# endif
};

// ============================================================================
// Optional
// ============================================================================

/**
 * @brief Matches the inner expression if present, but never fails.
 */
class Optional : public ExprUnary {

public:
	Optional(Expr *inner) : ExprUnary(K_OPTIONAL, inner) {}
	virtual ~Optional() {}

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor &visitor) const;

# if PEG_DEBUG_ANY
	virtual std::string	debugName(void) const { return "Optional"; }
# endif
};

// ============================================================================
// Predicate
// ============================================================================

/**
 * @brief Look-ahead predicate (positive or negative).
 *
 * Evaluates the inner expression without consuming input.
 * `isAnd()` determines whether the result is taken as-is (&) or negated (!).
 */
class Predicate : public ExprUnary {

private:
	bool _isAnd;

public:
	Predicate(Expr *inner, bool isAnd)
		: ExprUnary(K_PREDICATE, inner), _isAnd(isAnd)
	{}
	virtual ~Predicate() {}

	bool				isAnd(void) const { return _isAnd; }

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor &visitor) const;

# if PEG_DEBUG_ANY
	virtual std::string	debugName(void) const	{ return "Predicate"; }
	virtual std::string	debugValue(void) const	{ return _isAnd ? "&" : "!"; }
# endif
};

// ============================================================================
// Capture
// ============================================================================

/**
 * @brief Captures the matched substring or builds a new AST subtree.
 *
 * If used in property mode, stores the matched text under the provided key.
 * Otherwise, wraps the inner result in a newly created AstNode.
 */
class Capture : public ExprUnary {

private:
	std::string	_tag;
	bool		_isProp;

	bool		parseProperty(PackratParser &parser, AstNode *out) const;

public:
	Capture(Expr *inner, const std::string &tag, bool isProperty = false)
		: ExprUnary(K_CAPTURE, inner), _tag(tag), _isProp(isProperty)
	{}
	virtual ~Capture() {}

	const std::string&	tag(void) const			{ return _tag; }
	bool			 	isProperty(void) const	{ return _isProp; }

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor &visitor) const;

# if PEG_DEBUG_ANY
	virtual std::string	debugName(void) const	{ return "Capture"; }
	virtual std::string	debugValue(void) const	{ return (_isProp ? ": " : "@") + _tag; }
# endif
};

#endif