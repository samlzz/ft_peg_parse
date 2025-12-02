/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResolutionVisitor.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 20:17:00 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 12:33:52 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_RESOLUTION_VISITOR_HPP__
# define __FTPP_RESOLUTION_VISITOR_HPP__

# include <set>

# include "peg/core/Expr.hpp"
# include "peg/core/IExprVisitor.hpp"

// ============================================================================
// ResolutionVisitor
// ============================================================================

/**
 * @brief Visitor used to resolve RuleRef nodes to actual grammar expressions.
 *
 * Performs a DFS over the expression tree and resolves rule references
 * by consulting the rule dictionary provided to the visitor.
 */
class ResolutionVisitor : public IExprVisitor {

private:
	const t_ExprDict		 &_rules;
	std::set<const Expr *>	_visited;

	ResolutionVisitor();
	ResolutionVisitor &operator=(const ResolutionVisitor &other);
	// ---- Helpers ----
	void	visitChild(const Expr *child);

public:
	// ---- Construction ----
	ResolutionVisitor(const t_ExprDict &rules);

	// ---- API ----
	void			resolve(Expr *root);

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

#endif /* __FTPP_RESOLUTION_VISITOR_HPP__ */