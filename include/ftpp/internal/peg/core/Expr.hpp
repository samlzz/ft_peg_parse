/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Expr.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:40:29 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 14:19:16 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_EXPR_HPP__
# define __FTPP_EXPR_HPP__

# include <cstddef>
# include <map>
# include <stdint.h>
# include <string>
# include <vector>

# include "config.h"

// debug only
# if FTPP_DEBUG_GRAMMAR
#  include <iostream>
# endif

// ============================================================================
// Forward declarations
// ============================================================================
class PackratParser;
class AstNode;
class IExprVisitor;

// ============================================================================
// Expr base abstract class
// ============================================================================

/**
 * @brief Abstract base class for all PEG expressions.
 *
 * Provides a polymorphic interface for parsing rules and visiting
 * grammar nodes. Each subclass implements `parse()` and `accept()`.
*/
class Expr {

private:
	Expr();
	Expr(const Expr &other);
	Expr	&operator=(const Expr &other);

public:
	enum e_expr_kind {
		K_LITERAL,
		K_CHARRANGE,
		K_ANY,
		K_SEQUENCE,
		K_CHOICE,
		K_ZERO_OR_MORE,
		K_ONE_OR_MORE,
		K_OPTIONAL,
		K_PREDICATE,
		K_RULEREF,
		K_CAPTURE,
		K_FATAL
	};

protected:
	const enum e_expr_kind	_kind;

	Expr(enum e_expr_kind kind);

public:

	virtual ~Expr();

	virtual bool			parse(PackratParser &parser, AstNode *parent) const = 0;
	virtual void			accept(IExprVisitor &visitor) const = 0;

	// ---- Accessors ----
	enum e_expr_kind		kind(void) const;
	virtual size_t			childCount(void) const;
	virtual const Expr *	child(size_t) const;

	// ---- Representation ----
	virtual std::string		reprKind(void) const = 0;
	virtual std::string		reprValue(void) const;

	std::string				repr(void) const;

# if FTPP_DEBUG_GRAMMAR
	// ---- Debug functions ---

	static void	dumpTree(const Expr *root, std::ostream &os = std::cerr,
							int32_t maxDepth = -1, bool expandRuleRefs = true);
	static void	dumpStats(const Expr *root, std::ostream &os = std::cerr);
# endif // FTPP_DEBUG_GRAMMAR
};

// ============================================================================
// Helpers
// ============================================================================

// ----- Expr containers types ----

typedef std::vector<Expr *> 			t_ExprList;
typedef std::map<std::string, Expr *>	t_ExprDict;

// ---- Delete containers contents ----
// - - (work on any ptr container) - -

template <typename T>
void	deleteAll(std::vector<T *> &list)
{
	for (size_t i = 0; i < list.size(); ++i)
		delete list[i];
	list.clear();
}

template <typename K, typename V>
void	deleteVals(std::map<K, V*> &dict)
{
	typename std::map<K, V*>::iterator	it;

	for (it = dict.begin(); it != dict.end(); ++it)
	{
		if (it->second)
		{
			delete it->second;
			it->second = NULL;
		}
	}
	dict.clear();
}

#endif /* __FTPP_EXPR_HPP__ */