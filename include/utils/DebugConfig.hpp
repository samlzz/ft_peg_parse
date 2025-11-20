/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DebugConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 07:54:11 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/20 13:57:12 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUGCONFIG_HPP
# define DEBUGCONFIG_HPP

// ---- Debug Levels ----

# define PEG_DBG_NONE		0
# define PEG_DBG_ERROR		1
# define PEG_DBG_WARN		2
# define PEG_DBG_INFO		3
# define PEG_DBG_VERBOSE	4
# define PEG_DBG_TRACE		5

# ifndef PEG_DEBUG_LEVEL
#  define PEG_DEBUG_LEVEL PEG_DBG_NONE
# endif

// ---- All flags supported ----
// comment to disable

# if PEG_DEBUG_LEVEL > 0
// #  define PEG_DEBUG_PEGPARSER
#  define PEG_DEBUG_GRAMMAR
#  define PEG_DEBUG_PARSER
// #  define PEG_DEBUG_CACHE
#  define PEG_DEBUG_AST
# endif

#endif
