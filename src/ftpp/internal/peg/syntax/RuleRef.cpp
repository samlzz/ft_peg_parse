/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RuleRef.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 20:10:09 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 14:00:08 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AstNode.hpp"
#include "FtppException.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/core/IExprVisitor.hpp"
#include "peg/syntax/RuleRef.hpp"

// ============================================================================
// RuleRef
// ============================================================================

void RuleRef::accept(IExprVisitor &visitor) const
{
	visitor.visitRuleRef(*this);
}

// Delegate parsing to the resolved expression.
// Fails with a parse error if the reference has not been resolved.
bool RuleRef::parse(PackratParser &parser, AstNode *parent) const
{
	if (!_resolved)
		throw ConfigError("Unresolved rule: " + _name);

	return parser.eval(_resolved, parent);
}

// ---- Children introspection ----

size_t	RuleRef::childCount(void) const
{
	return _resolved ? 1 : 0;
}

const Expr	*RuleRef::child(size_t idx) const
{
	return idx == 0 ? _resolved : NULL;
}