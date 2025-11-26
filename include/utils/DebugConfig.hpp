/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DebugConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 15:07:16 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/26 10:59:12 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUGCONFIG_HPP
# define DEBUGCONFIG_HPP

// ============================================================================
// Debug Levels
// ============================================================================
# define PEG_DBG_NONE		0
# define PEG_DBG_ERROR		1
# define PEG_DBG_WARN		2
# define PEG_DBG_INFO		3
# define PEG_DBG_VERBOSE	4
# define PEG_DBG_TRACE		5

// ============================================================================
// Global Debug Level
// ============================================================================
# ifndef PEG_DEBUG_LEVEL
#  define PEG_DEBUG_LEVEL PEG_DBG_NONE
# endif

// ============================================================================
// Component-specific Debug Flags
// These can be individually enabled/disabled regardless of PEG_DEBUG_LEVEL
// ============================================================================

// Master switch: if 0, all component flags are ignored
# if PEG_DEBUG_LEVEL > 0
#  define PEG_DEBUG_ENABLED 1
# else
#  define PEG_DEBUG_ENABLED 0
# endif

// Component flags (comment out to disable specific components)
# if PEG_DEBUG_ENABLED

// Uncomment the components you want to debug:
// #  define PEG_DEBUG_LEXER		1
// #  define PEG_DEBUG_PARSER		1
// #  define PEG_DEBUG_GRAMMAR		1
// #  define PEG_DEBUG_PACKRAT		1
// #  define PEG_DEBUG_CACHE		1 // TODO: packrat cache doesn't implemented for now
#  define PEG_DEBUG_AST			1

# endif // PEG_DEBUG_ENABLED

// ============================================================================
// Helper macros to check if a component is enabled
// ============================================================================

# ifndef PEG_DEBUG_LEXER
#  define PEG_DEBUG_LEXER	0
# endif
// display`next` and `peek` call and curr _peeked token
// need & HAD Token debug methods

# ifndef PEG_DEBUG_PARSER
#  define PEG_DEBUG_PARSER	0
# endif
// call PEG_LOG_PARSER_FN in all his methods
// one info in parseRule => expr->DbgRepr()
// need Expr debug methods (not in this component)

# ifndef PEG_DEBUG_GRAMMAR
#  define PEG_DEBUG_GRAMMAR	0
# endif
// just provide his print functions that prints:
// either a rule, either all rule, either rules stats
// need Expr debug methods and ExprDebug namespace

# ifndef PEG_DEBUG_PACKRAT
#  define PEG_DEBUG_PACKRAT	0
# endif
// print a before and after each Expr *::parse call
// show the content of curr Expr at each time
// need Expr debug methods

# ifndef PEG_DEBUG_CACHE
#  define PEG_DEBUG_CACHE	0
# endif
// not implemented for now

# ifndef PEG_DEBUG_AST
#  define PEG_DEBUG_AST		0
# endif
// display nothing whitout user call
// juste provide prints (curr node, a full tree) methods
// no debug deps

// ============================================================================
// Composite flag for debug features used in multiple components
// ============================================================================
# define PEG_DEBUG_ANY (PEG_DEBUG_LEXER || PEG_DEBUG_PARSER \
						|| PEG_DEBUG_GRAMMAR || PEG_DEBUG_PACKRAT \
						|| PEG_DEBUG_CACHE || PEG_DEBUG_AST)

#endif