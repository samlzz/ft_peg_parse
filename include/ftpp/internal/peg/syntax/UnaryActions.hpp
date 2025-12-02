/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnaryActions.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:10:28 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 12:30:30 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_UNARY_ACTIONS_HPP__
# define __FTPP_UNARY_ACTIONS_HPP__

# include <string>

# include "peg/core/Expr.hpp"
# include "peg/core/ExprUnaryBase.hpp"

// ============================================================================
// Capture
// ============================================================================

/**
 * @brief Captures the matched substring or builds a new AST subtree.
 *
 * If used in property mode, stores the matched text under the provided key.
 * Otherwise, wraps the inner result in a newly created AstNode.
 */
class Capture : public ExprUnaryBase {

private:
	std::string	_tag;
	bool		_isProp;

	bool				parseProperty(PackratParser &parser, AstNode *out) const;

public:
	Capture(Expr *inner, const std::string &tag, bool isProperty = false)
		: ExprUnaryBase(K_CAPTURE, inner), _tag(tag), _isProp(isProperty)
	{}
	virtual ~Capture() {}

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor &visitor) const;

	const std::string&	tag(void) const			{ return _tag; }
	bool			 	isProperty(void) const	{ return _isProp; }

	virtual std::string	reprKind(void) const	{ return "Capture"; }
	virtual std::string	reprValue(void) const	{ return (_isProp ? ": " : "@") + _tag; }
};

// ============================================================================
// Fatal
// ============================================================================

/**
 * @brief Fatal expression: commits to a parsing path.
 * 
 * If the inner expression fails after a FatalExpr has been entered,
 * parsing stops immediately with the current diagnostic state.
 */
class Fatal : public ExprUnaryBase {

public:
	Fatal(Expr *inner) : ExprUnaryBase(K_FATAL, inner) {}
	virtual ~Fatal() {}

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor &visitor) const;

	virtual std::string	reprKind(void) const	{ return "Fatal"; }
};

#endif /* __FTPP_UNARY_ACTIONS_HPP__ */