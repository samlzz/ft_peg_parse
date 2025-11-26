/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrintVisitor.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 23:52:25 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/25 16:06:21 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/DebugConfig.hpp"

#if PEG_DEBUG_ANY

# include "peg/Expr.hpp"
# include "peg/debug/ExprVisitors.hpp"
# include "peg/syntax/ExprContainer.hpp"
# include "peg/syntax/ExprLeaf.hpp"
# include "peg/syntax/ExprUnary.hpp"
# include "peg/syntax/RuleRef.hpp"
# include "utils/DebugLogger.hpp"

namespace ExprDebug {

using namespace PegDebug;

// ============================================================================
// Helpers
// ============================================================================

// Map Expr kind to ANSI color code.
static const char *_kindColor(Expr::e_expr_kind k)
{
	switch (k) {
		case Expr::K_LITERAL:
		case Expr::K_CHARRANGE:
		case Expr::K_ANY:
			return COLOR_YELLOW;

		case Expr::K_SEQUENCE:
		case Expr::K_CHOICE:
			return COLOR_RED;

		case Expr::K_ZERO_OR_MORE:
		case Expr::K_ONE_OR_MORE:
		case Expr::K_OPTIONAL:
			return COLOR_MAGENTA;

		case Expr::K_PREDICATE:
			return COLOR_BLUE;

		case Expr::K_RULEREF:
			return COLOR_GREEN;

		case Expr::K_CAPTURE:
			return COLOR_BOLD_WHITE;

		default:
			return COLOR_RESET;
	}
}

// ============================================================================
// PrintVisitor
// ============================================================================

PrintVisitor::PrintVisitor(std::ostream &os, bool useColors)
	: _os(os), _useColors(useColors)
{}

// Apply ANSI color if enabled.
std::string	PrintVisitor::color(const std::string &text, const char *code) const
{
	if (!_useColors || !code)
		return text;
	return std::string(code) + text + COLOR_RESET;
}

// ---- Macro to generate visit methods ----
# define PRINT_VISIT_IMPL(ClassName) \
	void PrintVisitor::visit##ClassName(const ClassName &expr) \
	{ \
		_os << color(expr.debugName(), _kindColor(expr.kind())) \
			<< " " << expr.debugValue(); \
	}

// Implementations
PRINT_VISIT_IMPL(Literal)
PRINT_VISIT_IMPL(CharRange)
PRINT_VISIT_IMPL(Any)

PRINT_VISIT_IMPL(Sequence)
PRINT_VISIT_IMPL(Choice)

PRINT_VISIT_IMPL(OneOrMore)
PRINT_VISIT_IMPL(ZeroOrMore)
PRINT_VISIT_IMPL(Optional)
PRINT_VISIT_IMPL(Predicate)
PRINT_VISIT_IMPL(Capture)

# undef PRINT_VISIT_IMPL

void	PrintVisitor::visitRuleRef(const RuleRef &expr)
{
	_os << color(expr.debugName(), _kindColor(expr.kind()))
		<< " " << expr.debugValue();

	if (!expr.resolved())
		_os << " " << color("(unresolved!)", COLOR_BOLD_RED);
}

} // namespace ExprDebug

#endif

