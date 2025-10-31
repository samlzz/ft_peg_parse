/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Choice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 19:46:39 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 23:57:32 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packrat/PackratParser.hpp"
#include "peg/Expr.hpp"
#include "peg/PegExpr.hpp"

// * Constructors

Choice::Choice(): Expr(K_CHOICE), _opts()
{}

Choice::Choice(const ExprList &options): Expr(K_CHOICE), _opts(options)
{}

Choice::~Choice()
{
	deleteAll(_opts);
}

// * Parsing

bool Choice::parse(PackratParser &parser, AstNode *&out) const
{
	Input	&in = parser.input();
	size_t	start = in.pos();
	AstNode	*child;

	for (size_t i = 0; i < _opts.size(); ++i)
	{
		in.setPos(start);
		child = NULL;
		if (parser.eval(_opts[i], child))
		{
			out = child;
			return true;
		}
	}
	parser.diag().update(start, "expected at least one valid choice");
	out = NULL;
	return false;
}
