/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RuleRef.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 20:10:09 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/01 11:54:48 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AstNode.hpp"
#include "PackratParser.hpp"
#include "FtppException.hpp"
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