/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 01:12:00 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/04 08:51:42 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_CONFIG_HPP__
# define __FTPP_CONFIG_HPP__

/**
 * @brief Compile-time configuration flags for ftpp debugging and logging.
 *
 * This header centralizes all debug switches and log categories exposed by the
 * ftpp library. It is included by all modules that support debug or tracing.
 *
 * Each toggle controls extra debug output compiled into ftpp.
 * Set to 1 to enable, 0 to disable.
 */

// ============================================================================
// Debug feature toggles
// ============================================================================

/**
 * @brief Enable debug output in AstNode (tree dumps, attributes, spans).
 */
# ifndef FTPP_DEBUG_AST
#  define FTPP_DEBUG_AST		0
# endif

/**
 * @brief Enable statistics information of PackratParser in Grammar.
 */
# ifndef FTPP_DEBUG_PACKRAT
#  define FTPP_DEBUG_PACKRAT	0
# endif

/**
 * @brief Enable print and stats functions about expressions in Grammar.
 */
# ifndef FTPP_DEBUG_GRAMMAR
#  define FTPP_DEBUG_GRAMMAR	0
# endif

// ============================================================================
// Log categories (ft_log)
// ============================================================================

/**
 * @brief String identifiers used by ft_log to categorize log messages.
 */
# define FTPP_LOG_LEXER			"ftpp.lexer"
# define FTPP_LOG_PARSER		"ftpp.parser"
# define FTPP_LOG_PACKRAT		"ftpp.packrat"

#endif /* __FTPP_CONFIG_HPP__ */

