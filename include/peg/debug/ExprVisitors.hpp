/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprVisitors.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 20:00:00 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/17 20:51:46y sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPRVISITORS_HPP
# define EXPRVISITORS_HPP

# if PEG_DEBUG_LEVEL > 0

#  include <ostream>
#  include <string>
#  include <map>

#  include "peg/syntax/IExprVisitor.hpp"
#  include "peg/Expr.hpp"

namespace ExprDebug {

// ============================================================================
// PrintVisitor
// ============================================================================
class PrintVisitor : public IExprVisitor {
private:
	std::ostream	&_os;
	bool			_useColors;

	std::string		color(const std::string &text, const char *code) const;

public:
	PrintVisitor(std::ostream &os = std::cerr, bool useColors = true);

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
};

// ============================================================================
// TreeVisitor
// ============================================================================
class TreeVisitor : public IExprVisitor {
private:
	std::ostream	&_os;
	int				_depth;
	int				_maxDepth;
	std::string		_prefix;
	bool			_isLast;
	bool			_useColors;

	void		printNode(const Expr &expr);
	void		visitChildren(const Expr &parent);
	void		descend(const Expr *child, bool isLast);
	std::string	color(const std::string &text, const char *code) const;

public:
	TreeVisitor(std::ostream &os = std::cerr, int32_t maxDepth = -1, bool useColors = true);

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
};

// ============================================================================
// StatsVisitor
// ============================================================================
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

	// Accessors
	size_t	totalNodes() const	{ return _totalNodes; }
	size_t	maxDepth() const	{ return _maxDepth; }
	const std::map<Expr::e_expr_kind, size_t>&
			kindCounts() const	{ return _kindCounts; }

	void	print(std::ostream &os = std::cerr) const;

	// Visits
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
};

} // end ExprDebug namespace

# endif
#endif