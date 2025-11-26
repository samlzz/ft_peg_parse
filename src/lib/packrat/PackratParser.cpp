/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PackratParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:58:58 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/26 10:54:38 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>

#include "packrat/PackratParser.hpp"
#include "peg/Expr.hpp"
#include "ast/AstNode.hpp"
#include "utils/DebugLogger.hpp"
#include "utils/DebugConfig.hpp"
#include "utils/Input.hpp"

// ============================================================================
// parseRule: public entry point
// ============================================================================

void	PackratParser::parseRule(const std::string &rootRuleName, AstNode *&out)
{
	out = NULL;
	const Expr *start = _grammar.get(rootRuleName);
	if (!start)
		throw ParseError("at rule '" + rootRuleName
				+ "': " + _err.formatError(_input, true));

	PEG_LOG_TRACE_C(PACKRAT, "PackratParser",
		"Begin parsing with rule :" + rootRuleName);

	AstNode		*root = new AstNode(rootRuleName);
	const bool	ok = eval(start, root);

	if (!ok || !_input.eof())
	{
		delete root;
		throw ParseError(_err.formatError(_input, true));
	}
	if (root->children().size() < 2)
	{
		if (root->children().size() == 1)
			out = root->popChild();
		delete root;
	}
	else
		out = root;
}

// ============================================================================
// eval: main packrat logic (memo + dispatch)
// ============================================================================

bool	PackratParser::eval(const Expr *expr, AstNode *parent)
{
	size_t pos = _input.pos();

# if PEG_DEBUG_PACKRAT
	_evalCount++;
	_traceEnter(expr, _input);
# endif

	// TODO: retrieve from cache

	const bool ok = expr->parse(*this, parent);

# if PEG_DEBUG_PACKRAT
	_traceExit(expr, _input, ok);
	if (!ok)
		_backtrackCount++;
# endif

	// TODO: append to cache

	if (!ok)
		_input.setPos(pos);
	return ok;
}

