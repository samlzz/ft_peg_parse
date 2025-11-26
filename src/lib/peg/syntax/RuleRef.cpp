/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RuleRef.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 20:10:09 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/26 10:40:51 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "peg/syntax/RuleRef.hpp"
#include "ast/AstNode.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/IExprVisitor.hpp"

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
		throw PackratParser::ParseError("Unresolved rule: " + _name);

	return parser.eval(_resolved, parent);
}

