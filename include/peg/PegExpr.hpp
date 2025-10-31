/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PegExpr.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:56:54 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 12:24:26 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PEGEXPR_HPP
# define PEGEXPR_HPP

# include "./Expr.hpp"

class Literal: public Expr {

private:
	std::string	_value;

	Literal(const Literal &other);
	Literal	&operator=(const Literal &other);

public:
	Literal();
	Literal(const std::string &val);
	virtual ~Literal();

	virtual bool	parse(Input &in, AstNode *&node) const;
};

class CharRange: public Expr {

private:
	std::string	_charset;

	CharRange(const CharRange &other);
	CharRange	&operator=(const CharRange &other);

public:
	CharRange();
	CharRange(const std::string &charset);
	virtual ~CharRange();

	virtual bool	parse(Input &in, AstNode *&node) const;
};

class Sequence: public Expr {

private:
	ExprList	_elems;

	Sequence(const Sequence &other);
	Sequence	&operator=(const Sequence &other);

public:
	Sequence();
	Sequence(const ExprList &elems);
	virtual ~Sequence();

	virtual bool	parse(Input &in, AstNode *&node) const;
};

class Choice: public Expr {

private:
	ExprList	_opts;

	Choice(const Choice &other);
	Choice	&operator=(const Choice &other);

public:
	Choice();
	Choice(const ExprList &options);
	virtual ~Choice();

	virtual bool	parse(Input &in, AstNode *&node) const;
};

class ZeroOrMore: public Expr {

private:
	Expr	*_inner;

	ZeroOrMore(const ZeroOrMore &other);
	ZeroOrMore	&operator=(const ZeroOrMore &other);

public:
	ZeroOrMore();
	ZeroOrMore(Expr *e);
	virtual ~ZeroOrMore();

	virtual bool	parse(Input &in, AstNode *&node) const;
};

class OneOrMore: public Expr {

private:
	Expr	*_inner;

	OneOrMore(const OneOrMore &other);
	OneOrMore	&operator=(const OneOrMore &other);

public:
	OneOrMore();
	OneOrMore(Expr *e);
	virtual ~OneOrMore();

	virtual bool	parse(Input &in, AstNode *&node) const;
};

class Optional: public Expr {

private:
	Expr	*_inner;

	Optional(const Optional &other);
	Optional	&operator=(const Optional &other);

public:
	Optional();
	Optional(Expr *e);
	virtual ~Optional();

	virtual bool	parse(Input &in, AstNode *&node) const;
};

class Predicate: public Expr {

private:
	Expr	*_inner;
	bool	_isAnd; // ? if true AndPredicate (&) else NotPredicate (!)

	Predicate(const Predicate &other);
	Predicate	&operator=(const Predicate &other);

public:
	Predicate();
	Predicate(Expr *e, bool isAnd);
	virtual ~Predicate();

	virtual bool	parse(Input &in, AstNode *&node) const;
};

class RuleRef: public Expr {

private:
	std::string	_name;
	const Expr	*_resolved;

	RuleRef(const RuleRef &other);
	RuleRef	&operator=(const RuleRef &other);

public:
	RuleRef();
	RuleRef(const std::string &name);
	virtual ~RuleRef();

	void			resolve(const Expr* e);
	virtual bool	parse(Input &in, AstNode *&node) const;
};

class Capture: public Expr {

private:
	Expr	*_inner;
	std::string	_tag;

	Capture(const Capture &other);
	Capture	&operator=(const Capture &other);

public:
	Capture();
	Capture(Expr *e, const std::string &tag);
	virtual ~Capture();

	virtual bool	parse(Input &in, AstNode *&node) const;
};


#endif
