/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprLeaf.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:10:24 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/02 19:26:40 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPRLEAF_HPP
# define EXPRLEAF_HPP

# include <string>

# include "peg/Expr.hpp"

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
	Literal(const std::string &val): ExprLeaf(K_LITERAL, val)
	{}

	virtual bool	parse(PackratParser &parser, AstNode *&out) const;
};

class CharRange: public ExprLeaf {

private:
	void	expandCharset(void);

public:
	CharRange(const std::string &charset): ExprLeaf(K_CHARRANGE, charset)
	{}

	virtual bool	parse(PackratParser &parser, AstNode *&out) const;
};

#endif

