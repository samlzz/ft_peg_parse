/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DebugVisitors.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 10:16:49 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 12:32:10 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_DEBUGVISITORS_HPP__
# define __FTPP_DEBUGVISITORS_HPP__

# include "config.h"

# if FTPP_DEBUG_GRAMMAR

#  include <iostream>
#  include <ostream>
#  include <string>
#  include <map>

#  include "peg/core/Expr.hpp"
#  include "peg/core/IExprVisitor.hpp"

// ============================================================================
// TreeVisitor
// ============================================================================

/**
 * @brief Visitor displaying expressions as a tree structure.
 *
 * Recursively prints children with indentation, branches, and optional
 * depth restriction. Uses ASCII tree markers and optional coloring.
 */
class TreeVisitor : public IExprVisitor {

private:
	std::ostream	&_os;
	int				_depth;
	int				_maxDepth;
	std::string		_prefix;
	bool			_isLast;
	bool			_expandRef;

	void		printNode(const Expr &expr);
	void		visitChildren(const Expr &parent);
	void		descend(const Expr *child, bool isLast);

public:
	TreeVisitor(std::ostream &os = std::cerr,
				int32_t maxDepth = -1, bool expandRuleRefs = true);

	virtual void	visitLiteral(const Literal &expr);
	virtual void	visitCharRange(const CharRange &expr);
	virtual void	visitAny(const Any &expr);
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

// ============================================================================
// StatsVisitor
// ============================================================================

/**
 * @brief Visitor collecting structural statistics on an expression tree.
 *
 * Tracks total node count, maximum depth reached, and counts per
 * expression kind. Children are fully explored recursively.
 */
class StatsVisitor : public IExprVisitor {

private:
	size_t								_totalNodes;
	size_t								_currentDepth;
	size_t								_maxDepth;
	std::map<Expr::e_expr_kind, size_t>	_kindCounts;

	void	recordNode(const Expr &expr);
	void	descend(const Expr *child);

public:
	StatsVisitor();

	// ---- Accessors ----
	size_t	totalNodes() const { return _totalNodes; }
	size_t	maxDepth() const	{ return _maxDepth; }
	const std::map<Expr::e_expr_kind, size_t> &
			kindCounts() const	{ return _kindCounts; }

	// ---- Output ----
	void	print(std::ostream &os = std::cerr) const;

	// ---- Visit overrides ----
	virtual void	visitCharRange(const CharRange &expr);
	virtual void	visitAny(const Any &expr);
	virtual void	visitSequence(const Sequence &expr);
	virtual void	visitChoice(const Choice &expr);
	virtual void	visitZeroOrMore(const ZeroOrMore &expr);
	virtual void	visitOneOrMore(const OneOrMore &expr);
	virtual void	visitLiteral(const Literal &expr);
	virtual void	visitOptional(const Optional &expr);
	virtual void	visitPredicate(const Predicate &expr);
	virtual void	visitRuleRef(const RuleRef &expr);
	virtual void	visitCapture(const Capture &expr);
	virtual void	visitFatal(const Fatal &expr);
};

# endif // FTPP_DEBUG_GRAMMAR

#endif /* __FTPP_DEBUGVISITORS_HPP__ */