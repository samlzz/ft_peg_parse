/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprUnary.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:10:28 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/21 15:11:41 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPRUNARY_HPP
# define EXPRUNARY_HPP

# include <cstddef>
# include <string>

# include "peg/Expr.hpp"

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
	virtual ~ExprUnary()	{ delete _inner; }

	Expr*				inner()				{ return _inner; }
	const Expr*			inner() const		{ return _inner; }
	void				setInner(Expr *e);

	virtual size_t		childCount(void) const		{ return _inner ? 1 : 0; }
	virtual Expr		*child(size_t idx) const	{ return idx == 0 ? _inner : NULL; }
};

class ZeroOrMore: public ExprUnary {

public:
	ZeroOrMore(Expr *inner): ExprUnary(K_ZERO_OR_MORE, inner)
	{}
	virtual ~ZeroOrMore() {}

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor& visitor) const;
# if PEG_DEBUG_ANY
	virtual std::string	debugName(void) const		{ return "ZeroOrMore"; }
# endif
};

class OneOrMore: public ExprUnary {

public:
	OneOrMore(Expr *inner): ExprUnary(K_ONE_OR_MORE, inner)
	{}
	virtual ~OneOrMore() {}

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor& visitor) const;
# if PEG_DEBUG_ANY
	virtual std::string	debugName(void) const		{ return "OneOrMore"; }
# endif
};

class Optional: public ExprUnary {

public:
	Optional(Expr *inner): ExprUnary(K_OPTIONAL, inner)
	{}
	virtual ~Optional() {}

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor& visitor) const;
# if PEG_DEBUG_ANY
	virtual std::string	debugName(void) const		{ return "Optional"; }
# endif
};

class Predicate: public ExprUnary {

private:
	bool	_isAnd;

public:
	Predicate(Expr *inner, bool isAnd):
		ExprUnary(K_PREDICATE, inner), _isAnd(isAnd)
	{}
	virtual ~Predicate() {}

	bool	isAnd(void) const { return _isAnd; }

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor& visitor) const;
# if PEG_DEBUG_ANY
	virtual std::string	debugName(void) const		{ return "Predicate"; }
	virtual std::string	debugValue(void) const		{ return _isAnd ? "&" : "!"; }
# endif
};

class Capture: public ExprUnary {

private:
	std::string	_tag;
	bool		_isProp;

	bool	parseProperty(PackratParser &parser, AstNode *out) const;

public:
	Capture(Expr *inner, const std::string &tag, bool isProperty = false):
		ExprUnary(K_CAPTURE, inner), _tag(tag), _isProp(isProperty)
	{}
	virtual ~Capture() {}

	const std::string	&tag(void) const		{ return _tag; }
	bool				isProperty(void) const	{ return _isProp; }

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor& visitor) const;
# if PEG_DEBUG_ANY
	virtual std::string	debugName(void) const		{ return "Capture"; }
	virtual std::string	debugValue(void) const		{ return (_isProp ? ": " : "@" ) + _tag; }
# endif
};


#endif