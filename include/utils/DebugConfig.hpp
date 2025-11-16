/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DebugConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 07:54:11 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/16 18:29:22 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUGCONFIG_HPP
# define DEBUGCONFIG_HPP

// Debug Levels
# define PEG_DBG_NONE		0
# define PEG_DBG_ERROR		1
# define PEG_DBG_WARN		2
# define PEG_DBG_INFO		3
# define PEG_DBG_VERBOSE	4
# define PEG_DBG_TRACE		5

# ifndef PEG_DEBUG_LEVEL
#  define PEG_DEBUG_LEVEL PEG_DBG_NONE
# endif

// Fonctionnality flags (can be additionned)
// # define PEG_DEBUG_AST
// # define PEG_DEBUG_PARSER
// # define PEG_DEBUG_CACHE
// # define PEG_DEBUG_GRAMMAR
// # define PEG_DEBUG_LEXER

// flags by default
# if PEG_DEBUG_LEVEL > 0
#  define PEG_DEBUG_AST
#  define PEG_DEBUG_GRAMMAR
#  define PEG_DEBUG_PARSER
# endif

#endif
