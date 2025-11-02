/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RuleRef.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 20:10:09 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/02 19:45:11 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "peg/syntax/RuleRef.hpp"
#include "ast/AstNode.hpp"
#include "packrat/PackratParser.hpp"

bool RuleRef::parse(PackratParser &parser, AstNode *&out) const
{
	if (!_resolved)
		throw PackratParser::ParseError("Unresolved rule: " + _name);
	return parser.eval(_resolved, out);
}
