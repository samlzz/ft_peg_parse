/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Optional.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 19:56:40 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 23:42:33 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packrat/PackratParser.hpp"
#include "peg/Expr.hpp"
#include "peg/PegExpr.hpp"

// * Constructors

Optional::Optional(): Expr(K_OPTIONAL), _inner(NULL)
{}

Optional::Optional(Expr *inner): Expr(K_OPTIONAL), _inner(inner)
{}

Optional::~Optional()
{
	if (_inner)
		delete _inner;
	_inner = NULL;
}

// * Parsing

bool Optional::parse(PackratParser &parser, AstNode *&out) const
{
	AstNode *child = NULL;

	if (!parser.eval(_inner, child))
	{
		out = NULL;
		return true;
	}
	out = child;
	return true;
}
