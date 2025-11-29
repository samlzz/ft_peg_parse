/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogConfig.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 22:36:11 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/29 23:13:00 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_CONFIG_HPP
# define LOG_CONFIG_HPP

# include <string>
# include <ostream>

# include "level.hpp"

// ============================================================================
// ft_log - public configuration API
// ============================================================================

namespace ft_log
{

// ---------------------------------------------------------------------------
// Global configuration
// ---------------------------------------------------------------------------

/**
 * @brief Set the global minimum log level.
 *
 * Log with a level below this threshold will be ignored.
 */
void		setLevel(e_log_level threshold);

/**
 * @brief Get the current global log level.
 */
e_log_level	getLevel(void);

/**
 * @brief Enable a logging category (e.g. "peg.parser").
 */
void		enableCategory(const std::string &category);

/**
 * @brief Disable a logging category.
 */
void		disableCategory(const std::string &category);

/**
 * @brief Check if a category is currently enabled.
 */
bool		isCategoryEnabled(const std::string &category);

/**
 * @brief Set the output stream used for logging (default: std::cerr).
 */
void		setOutputStream(std::ostream &os);

/**
 * @brief Enable or disable ANSI color output globally.
 */
void		setColorEnabled(bool enabled);

/**
 * @brief Check whether ANSI color output is enabled.
 */
bool		isColorEnabled(void);

/**
 * @brief Enable or disable level prefixing for log messages.
 */
void		setShowLevel(bool enabled);

/**
 * @brief Check whether level prefixing is currently enabled.
 */
bool		isShowLevelEnabled(void);

} // namespace ft_log

#endif // LOG_CONFIG_HPP

