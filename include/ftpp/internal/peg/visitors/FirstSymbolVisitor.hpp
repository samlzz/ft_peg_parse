/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FirstSymbolVisitor.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 20:45:00 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 12:33:37 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_FIRST_SYMBOL_VISITOR_HPP__
# define __FTPP_FIRST_SYMBOL_VISITOR_HPP__

# include <set>
# include <string>

# include "peg/core/Expr.hpp"
# include "peg/core/IExprVisitor.hpp"

// ============================================================================
// FirstSymbolVisitor
// ============================================================================

/**
 * @brief Detects whether the first symbol of an expression is a given rule name.
 *
 * Traverses the expression structure to check if the first reachable
 * RuleRef encountered matches the target name. Used for left-recursion detection.
 */
class FirstSymbolVisitor : public IExprVisitor {

private:
	const std::string		&_target;
	std::set<const Expr *>	_visited;
	bool					_match;

	// ---- Helpers ----
	void	visitNode(const Expr *expr);

	FirstSymbolVisitor();
	FirstSymbolVisitor &operator=(const FirstSymbolVisitor &other);

public:
	// ---- Construction ----
	FirstSymbolVisitor(const std::string &target);

	// ---- API ----
	bool			matches(const Expr *expr);

	// ---- Visitor methods ----
	virtual void	visitLiteral(const Literal &);
	virtual void	visitCharRange(const CharRange &);
	virtual void	visitAny(const Any &);

	virtual void	visitSequence(const Sequence &expr);
	virtual void	visitChoice(const Choice &expr);

	virtual void	visitZeroOrMore(const ZeroOrMore &expr);
	virtual void	visitOneOrMore(const OneOrMore &expr);
	virtual void	visitOptional(const Optional &expr);
	virtual void	visitPredicate(const Predicate &expr);

	virtual void	visitRuleRef(const RuleRef &expr);
	virtual void	visitCapture(const Capture &expr);
	virtual void	visitFatal(const Fatal &expr);
};

#endif /* __FTPP_FIRST_SYMBOL_VISITOR_HPP__ */