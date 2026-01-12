/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringUtils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 10:35:09 by sliziard          #+#    #+#             */
/*   Updated: 2026/01/12 13:22:24 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_STRING_UTILS_HPP__
# define __FTPP_STRING_UTILS_HPP__

# include <cstdio>
# include <stdint.h>
# include <cstddef>
# include <string>

// ============================================================================
// Escape helpers
// ============================================================================

/**
 * @brief Escape a string up to maxLen for human display.
 */
std::string	escapeStringDisplay(const std::string &str, size_t maxLen = 30);
/**
 * @brief Escape a character set represented as a raw string.
 */
std::string	escapeCharSetDisplay(const std::string &set, size_t maxSegments = 15);

// ============================================================================
// Unescape helpers
// ============================================================================

/**
 * @brief Fully unescape a string literal, rewriting classic escapes.
 */
std::string	unescapeString(const std::string &raw);
/**
 * @brief Expand a character set definition (e.g. "a-zA-Z") into raw chars.
 * Expand range (-), negate (^ start) and classic escapes
 */
std::string	expandCharSet(const std::string &raw);

#endif /* __FTPP_STRING_UTILS_HPP__ */