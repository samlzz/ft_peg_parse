/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Expr.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:40:29 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/01 11:45:58 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPR_HPP
# define EXPR_HPP

# include <cstddef>
# include <map>
# include <stdint.h>
# include <string>
# include <vector>

# include "config.h"

# if FTPP_DEBUG_EXPR
#  include <iostream>
# endif

// ============================================================================
// Forward declarations
// ============================================================================
class PackratParser;
class AstNode;
class IExprVisitor;

// ============================================================================
// Expr abstract class
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

	Expr(enum e_expr_kind kind): _kind(kind) {}

public:

	virtual ~Expr() {}

	virtual bool			parse(PackratParser &parser, AstNode *parent) const = 0;
	virtual void			accept(IExprVisitor &visitor) const = 0;

	// ---- Accessors ----
	enum e_expr_kind		kind(void) const		{ return _kind; }
	virtual size_t			childCount(void) const	{ return 0; }
	virtual const Expr *	child(size_t) const		{ return NULL; }

	// ---- Representation ----
	/**
	 * @brief Returns the symbolic name of the expression kind (literal, choice...).
	 */
	virtual std::string		reprKind(void) const = 0;
	/**
	 * @brief Returns the optional value associated with the expression.
	 */
	virtual std::string		reprValue(void) const	{ return ""; }
	/**
	 * @brief Returns a compact representation used for logging and tracing.
	 */
	std::string				repr(void) const;

# if FTPP_DEBUG_EXPR
	// ---- Debug functions ---

	/**
	 * @brief Dumps the expression tree into a human-readable structure.
	 */
	static void	dumpTree(const Expr *root, std::ostream &os = std::cerr,
							int32_t maxDepth = -1);
	/**
	 * @brief Dumps aggregated statistics about an expression tree.
	 */
	static void	dumpStats(const Expr *root, std::ostream &os = std::cerr);
# endif
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

/// ---- Replace unary ptr ----

template <typename T>
void	replaceOne(T *srcPtr, T *&destPtr)
{
	if (srcPtr != destPtr)
	{
		if (destPtr)
			delete destPtr;
		destPtr = srcPtr;
	}
}

#endif