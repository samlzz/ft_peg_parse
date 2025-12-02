/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RuleRef.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:32:54 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 14:00:00 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_RULE_REF_HPP__
# define __FTPP_RULE_REF_HPP__

# include <cstddef>
# include <string>

# include "peg/core/Expr.hpp"

// ============================================================================
// RuleRef
// ============================================================================

/**
 * @brief Reference to another named PEG rule.
 *
 * Stores the textual rule name as parsed from the grammar, and is resolved
 * later to point to the corresponding Expr instance (via Grammar::resolveRefs).
 * Parsing delegates entirely to the resolved expression.
 */
class RuleRef : public Expr {

private:
	std::string	_name;
	const Expr	*_resolved;

	RuleRef();
	RuleRef(const RuleRef &other);
	RuleRef &operator=(const RuleRef &other);

public:
	RuleRef(const std::string &name)
		: Expr(K_RULEREF), _name(name), _resolved(NULL)
	{}
	virtual ~RuleRef() {}

	virtual bool		parse(PackratParser &parser, AstNode *parent) const;
	virtual void		accept(IExprVisitor &visitor) const;

	// ---- Accessors ----
	void				resolve(const Expr *e)	{ _resolved = e; }
	const Expr			*resolved(void) const	{ return _resolved; }
	const std::string	&name(void) const		{ return _name; }

	// ---- Children introspection ----
	virtual size_t		childCount(void) const;
	virtual const Expr	*child(size_t idx) const;

	// ---- Representation ----
	virtual std::string	reprKind(void) const	{ return "RuleRef"; }
	virtual std::string	reprValue(void) const	{ return _name; }
};

#endif /* __FTPP_RULE_REF_HPP__ */