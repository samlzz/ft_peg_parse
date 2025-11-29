/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 22:47:37 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/29 23:13:06 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ostream>

#include "internal/Logger.hpp"
#include "LogConfig.hpp"
#include "LogScope.hpp"

// ============================================================================
// Global configuration API
// ============================================================================

namespace ft_log
{

void	setLevel(e_log_level level)
{
	Logger::instance().setLevel(level);
}

e_log_level	getLevel(void)
{
	return (Logger::instance().level());
}

void	enableCategory(const std::string &category)
{
	Logger::instance().enableCategory(category);
}

void	disableCategory(const std::string &category)
{
	Logger::instance().disableCategory(category);
}

bool	isCategoryEnabled(const std::string &category)
{
	return (Logger::instance().categoryEnabled(category));
}

void	setOutputStream(std::ostream &os)
{
	Logger::instance().setStream(os);
}

void	setColorEnabled(bool enabled)
{
	Logger::instance().setUseColor(enabled);
}

bool	isColorEnabled(void)
{
	return (Logger::instance().useColor());
}

void	setShowLevel(bool enabled)
{
	Logger::instance().setShowLvl(enabled);
}

bool	isShowLevelEnabled(void)
{
	return (Logger::instance().showLvl());
}

// ============================================================================
// Runtime query and logging helpers
// ============================================================================

bool	enabled(const std::string &category, e_log_level level)
{
	return (Logger::instance().enabled(category, level));
}

std::ostream	&log(const std::string &category, e_log_level level)
{
	Logger	&instance = Logger::instance();

	if (instance.enabled(category, level))
		return (instance.stream());
	return Logger::_nullStream;
}

std::ostream	&indentedLog(const std::string &category, e_log_level level)
{
	Logger	&instance = Logger::instance();

	if (!instance.enabled(category, level))
		return Logger::_nullStream;

	std::ostream	&logStream = instance.stream();
	int32_t			currIndent = LogScope::indentLevel();
	for (int32_t i = 0; i < currIndent; ++i)
		logStream << FT_LOG_SCOPE_INDENT_VAL;
	return logStream;
}

std::string	color(const std::string &text, const char *ansiCode)
{
	return (Logger::instance().colorize(text, ansiCode));
}

} // namespace ft_log

