/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RuleRef.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:32:54 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/20 16:29:36 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RULEREF_HPP
# define RULEREF_HPP

# include <cstddef>
# include <string>

# include "peg/Expr.hpp"

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

	virtual size_t		childCount(void) const		{ return _resolved ? 1 : 0; }
	virtual const Expr	*child(size_t idx) const	{ return idx == 0 ? _resolved : NULL; }
# if PEG_DEBUG_ANY
	virtual std::string	debugName(void) const	{ return "RuleRef"; }
	virtual std::string debugValue(void) const	{ return _name; }
# endif
};

#endif