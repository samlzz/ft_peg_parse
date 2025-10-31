/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PegExpr.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:56:54 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 21:12:06 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PEGEXPR_HPP
# define PEGEXPR_HPP

# include "./Expr.hpp"

class Literal: public Expr {

private:
	std::string	_value;

	Literal();
	Literal(const Literal &other);
	Literal	&operator=(const Literal &other);

public:
	Literal(const std::string &val);
	virtual ~Literal();

	virtual bool	parse(PackratParser &parser, AstNode *&out) const;
};

class CharRange: public Expr {

private:
	std::string	_charset;

	CharRange();
	CharRange(const CharRange &other);
	CharRange	&operator=(const CharRange &other);

	void	expandCharset(void);

public:
	CharRange(const std::string &charset);
	virtual ~CharRange();

	virtual bool	parse(PackratParser &parser, AstNode *&out) const;
};

class Sequence: public Expr {

private:
	ExprList	_elems;

	Sequence(const Sequence &other);
	Sequence	&operator=(const Sequence &other);

public:
	Sequence();
	Sequence(const ExprList &elements);
	virtual ~Sequence();

	virtual bool	parse(PackratParser &parser, AstNode *&out) const;
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

	virtual bool	parse(PackratParser &parser, AstNode *&out) const;
};

class ZeroOrMore: public Expr {

private:
	Expr	*_inner;

	ZeroOrMore();
	ZeroOrMore(const ZeroOrMore &other);
	ZeroOrMore	&operator=(const ZeroOrMore &other);

public:
	ZeroOrMore(Expr *inner);
	virtual ~ZeroOrMore();

	virtual bool	parse(PackratParser &parser, AstNode *&out) const;
};

class OneOrMore: public Expr {

private:
	Expr	*_inner;

	OneOrMore();
	OneOrMore(const OneOrMore &other);
	OneOrMore	&operator=(const OneOrMore &other);

public:
	OneOrMore(Expr *inner);
	virtual ~OneOrMore();

	virtual bool	parse(PackratParser &parser, AstNode *&out) const;
};

class Optional: public Expr {

private:
	Expr	*_inner;

	Optional();
	Optional(const Optional &other);
	Optional	&operator=(const Optional &other);

public:
	Optional(Expr *inner);
	virtual ~Optional();

	virtual bool	parse(PackratParser &parser, AstNode *&out) const;
};

class Predicate: public Expr {

private:
	Expr	*_inner;
	bool	_isAnd; // ? if true AndPredicate (&) else NotPredicate (!)

	Predicate();
	Predicate(const Predicate &other);
	Predicate	&operator=(const Predicate &other);

public:
	Predicate(Expr *inner, bool isAnd);
	virtual ~Predicate();

	virtual bool	parse(PackratParser &parser, AstNode *&out) const;
};

class RuleRef: public Expr {

private:
	std::string	_name;
	const Expr	*_resolved;

	RuleRef();
	RuleRef(const RuleRef &other);
	RuleRef	&operator=(const RuleRef &other);

public:
	RuleRef(const std::string &name);
	virtual ~RuleRef();

	void			resolve(const Expr* e);
	virtual bool	parse(PackratParser &parser, AstNode *&out) const;
};

class Capture: public Expr {

private:
	Expr	*_inner;
	std::string	_tag;

	Capture();
	Capture(const Capture &other);
	Capture	&operator=(const Capture &other);

public:
	Capture(Expr *inner, const std::string &tag);
	virtual ~Capture();

	virtual bool	parse(PackratParser &parser, AstNode *&out) const;
};


#endif
