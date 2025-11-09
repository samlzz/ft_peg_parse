/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprLeaf.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:10:24 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/09 11:25:50 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPRLEAF_HPP
# define EXPRLEAF_HPP

# include <string>

# include "peg/Expr.hpp"
#include "utils/StringUtils.hpp"

class ExprLeaf : public Expr {
private:
	ExprLeaf();
	ExprLeaf(const ExprLeaf &other);
	ExprLeaf &operator=(const ExprLeaf &other);

protected:
	std::string _value;

public:
	ExprLeaf(enum e_expr_kind kind, const std::string &value)
		: Expr(kind), _value(value)
	{}
	virtual ~ExprLeaf() {}

	const std::string&	value() const					{ return _value; }
	void				setValue(const std::string &v)	{ _value = v; }
};

class Literal: public ExprLeaf {

public:
	Literal(const std::string &escaped):
		ExprLeaf(K_LITERAL, unescapeString(escaped))
	{}

	virtual bool	parse(PackratParser &parser, AstNode *parent) const;
};

class CharRange: public ExprLeaf {

public:
	CharRange(const std::string &charset):
		ExprLeaf(K_CHARRANGE, expandCharSet(charset))
	{}

	virtual bool	parse(PackratParser &parser, AstNode *parent) const;
};

class Any: public Expr {

public:
	Any(): Expr(K_ANY) {}

	virtual bool parse(PackratParser &parser, AstNode *parent) const;
};

#endif

