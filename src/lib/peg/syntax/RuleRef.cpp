/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RuleRef.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 20:10:09 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 23:44:07 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packrat/PackratParser.hpp"
#include "peg/Expr.hpp"
#include "peg/PegExpr.hpp"

// * Constructors

RuleRef::RuleRef():
	Expr(K_RULEREF), _name(), _resolved(NULL)
{}

RuleRef::RuleRef(const std::string &name):
	Expr(K_RULEREF), _name(name), _resolved(NULL)
{}

RuleRef::~RuleRef() {}

// * Methods

void	RuleRef::resolve(const Expr* e)
{
	_resolved = e;
}

// * Parsing

bool RuleRef::parse(PackratParser &parser, AstNode *&out) const
{
	if (!_resolved)
		throw PackratParser::ParseError("Unresolved rule: " + _name);
	return parser.eval(_resolved, out);
}
