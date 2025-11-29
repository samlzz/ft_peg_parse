/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogOp.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:32:14 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/29 22:53:21 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGOP_HPP
# define LOGOP_HPP

# include <ostream>
# include <string>

# include "level.hpp"

// ============================================================================
// ft_log - public helper API
// ============================================================================

namespace ft_log
{

// ---------------------------------------------------------------------------
// Runtime query and logging helpers
// ---------------------------------------------------------------------------

/**
 * @brief Check if logging is enabled for the category and level.
 */
bool			enabled(const std::string &category,
				e_log_level level = LOG_TRACE);

/**
 * @brief Get the output stream for the given category and level.
 */
std::ostream	&log(const std::string &category,
				e_log_level level = LOG_TRACE);

/**
 * @brief Behave same as log, except indentation based on LogScope::indentLevel
 */
std::ostream	&indentedLog(const std::string &category,
				e_log_level level = LOG_TRACE);

/**
 * @brief Return a colorized version of the given text.
 *
 * If color output is disabled, the plain text is returned.
 */
std::string		color(const std::string &text, const char *ansiCode);

} // namespace ft_log

#endif
