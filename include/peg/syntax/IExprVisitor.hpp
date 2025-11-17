/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IExprVisitor.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 20:28:06 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/17 19:27:12 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IEXPRVISITOR_HPP
# define IEXPRVISITOR_HPP

// Forward declarations
class Literal;
class CharRange;
class Any;
class Sequence;
class Choice;
class ZeroOrMore;
class OneOrMore;
class Optional;
class Predicate;
class RuleRef;
class Capture;

class IExprVisitor {
public:
	virtual ~IExprVisitor() {}
	
	virtual void	visitLiteral(const Literal& expr) = 0;
	virtual void	visitCharRange(const CharRange& expr) = 0;
	virtual void	visitAny(const Any& expr) = 0;
	virtual void	visitSequence(const Sequence& expr) = 0;
	virtual void	visitChoice(const Choice& expr) = 0;
	virtual void	visitZeroOrMore(const ZeroOrMore& expr) = 0;
	virtual void	visitOneOrMore(const OneOrMore& expr) = 0;
	virtual void	visitOptional(const Optional& expr) = 0;
	virtual void	visitPredicate(const Predicate& expr) = 0;
	virtual void	visitRuleRef(const RuleRef& expr) = 0;
	virtual void	visitCapture(const Capture& expr) = 0;
};

#endif
