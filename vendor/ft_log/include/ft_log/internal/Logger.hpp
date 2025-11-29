/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 22:33:18 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/29 22:40:27 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <stdint.h>
# include <streambuf>
# include <string>
# include <set>
# include <ostream>

# include "level.hpp"

// ============================================================================
// ft_log::Logger - core logger implementation
// ============================================================================

namespace ft_log
{

/**
 * @brief Core logger singleton.
 *
 * This class holds all runtime logging state:
 *  - global log level,
 *  - static null stream,
 *  - enabled categories,
 *  - output stream,
 *  - use color flag
 *  - show level in log
 *
 * It is not meant to be used directly in most code. Prefer the public
 * facade functions declared in LogConfig.hpp.
 */
class Logger
{
public:
	static Logger	&instance(void);

	// ---------------------------------------------------------------------
	// Configuration
	// ---------------------------------------------------------------------

	void			enableCategory(const std::string &category);
	void			disableCategory(const std::string &category);
	bool			categoryEnabled(
						const std::string &category) const;

	// ---- Accessors ----
	void			setStream(std::ostream &os);
	std::ostream	&stream(void) const;

	void			setUseColor(bool state);
	bool			useColor(void) const;

	void			setLevel(e_log_level level);
	e_log_level		level(void) const;

	void			setShowLvl(bool enabled);
	bool			showLvl(void) const;

	// ---------------------------------------------------------------------
	// Query and logging
	// ---------------------------------------------------------------------

	bool			enabled(const std::string &category,
						e_log_level level) const;

	std::ostream	&log(e_log_level level);

	/**
	 * @brief Colorize text using the given ANSI code.
	 *
	 * If colors are disabled, this returns the plain text unchanged.
	 */
	std::string		colorize(const std::string &text,
						const char *ansiCode) const;

	// exposed for LogConfig or any else wrapper
	static std::ostream	_nullStream;

private:
	Logger(void);
	Logger(const Logger &);
	Logger &operator=(const Logger &);

	class NullBuffer : public std::streambuf
	{
	public:
		int32_t	overflow(int c) { return c; }
	};

	static NullBuffer	_nullBuffer;

	e_log_level				_level;
	std::set<std::string>	_categories;
	std::ostream			*_out;
	bool					_useColor;
	bool					_showLvl;
};

} // namespace ft_log

#endif // FT_LOG_LOGGER_HPP

