/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RuleRef.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:32:54 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/17 19:24:14 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RULEREF_HPP
# define RULEREF_HPP

#include <string>

#include "peg/Expr.hpp"

class RuleRef: public Expr {

private:
	std::string	_name;
	const Expr	*_resolved;

	RuleRef();
	RuleRef(const RuleRef &other);
	RuleRef	&operator=(const RuleRef &other);

public:
	RuleRef(const std::string &name):
		Expr(K_RULEREF), _name(name), _resolved(NULL)
	{}
	virtual ~RuleRef() {}

	void				resolve(const Expr* e)	{ _resolved = e; }
	const Expr			*resolved(void) const	{ return _resolved; }
	const std::string	&name(void) const		{ return _name; }

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor& visitor) const;
# if PEG_DEBUG_LEVEL > 0
	virtual std::string	debugName(void) const	{ return "RuleRef"; }
	virtual std::string debugValue(void) const	{ return _name; }
# endif
};

#endif