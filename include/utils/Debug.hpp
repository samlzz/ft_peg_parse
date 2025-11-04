/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Debug.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 14:17:07 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/04 14:49:02 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_HPP
#define DEBUG_HPP

# ifndef DEBUG_LEVEL
#  define DEBUG_LEVEL 0
# endif

# if DEBUG_LEVEL > 0
#  include <iostream>
# endif

# include <string>

# include "peg/PegLexer.hpp"

enum e_debug_level {
	DBG_NONE = 0,
	DBG_ERROR = 1,
	DBG_INFO = 2,
	DBG_VERBOSE = 3
};

static inline void dbg_print(e_debug_level lvl, const std::string &prefix, const std::string &msg)
{
# if DEBUG_LEVEL > 0
	if (lvl <= DEBUG_LEVEL)
		std::cerr << prefix << msg << std::endl;
# else
	(void)lvl; (void)prefix; (void)msg;
# endif
}

// ===============================
// === FONCTIONS UTILITAIRES ===
// ===============================

# if DEBUG_LEVEL > 0

void printToken(const class PegLexer::Token &tk);
void printExprTree(const class Expr *e, int depth = 0);
void printEvalTrace(const class Expr *e, size_t pos, bool entering);
void printCacheHit(const class Expr *e, size_t pos);
void printCacheSet(const class Expr *e, size_t pos, bool ok, size_t next);
void printAstTree(const class AstNode *node, int depth = 0);

# else

// ? make it inline when DEBUG_LEVEL is 0 to let compiler remove them

inline void printToken(const struct PegLexer::Token &) {}
inline void printExprTree(const class Expr *, int = 0) {}
inline void printEvalTrace(const class Expr *, size_t, bool) {}
inline void printCacheHit(const class Expr *, size_t) {}
inline void printCacheSet(const class Expr *, size_t, bool, size_t) {}
inline void printAstTree(const class AstNode *, int = 0) {}

# endif

#endif

