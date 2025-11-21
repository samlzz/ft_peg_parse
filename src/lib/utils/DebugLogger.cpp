/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DebugLogger.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 19:27:56 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/19 15:21:07 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/DebugLogger.hpp"

#if PEG_DEBUG_ENABLED

# include <string>
# include <iostream>
# include <sstream>

namespace PegDebug {

std::ostream*	Logger::_output = &std::cerr;
int32_t			Logger::_indentLevel = 0;
std::string		Logger::_indentValue = "  ";
bool			Logger::_useColors = true;

std::string Logger::color(const std::string& text, const char* colorCode)
{
	if (!_useColors || !colorCode)
		return text;

	std::ostringstream	oss;
	oss << colorCode << text << COLOR_RESET;
	return oss.str();
}

static const char* getLevelColor(LogLevel level)
{
	switch (level) {
		case LOG_ERROR:		return COLOR_BOLD_RED;
		case LOG_WARN:		return COLOR_BOLD_YELLOW;
		case LOG_INFO:		return COLOR_BOLD_CYAN;
		case LOG_VERBOSE:	return COLOR_CYAN;
		case LOG_TRACE:		return COLOR_DIM;
		default:
			return COLOR_RESET;
	}
}

static const char* getLevelName(LogLevel level)
{
	switch (level) {
		case LOG_ERROR:		return "ERROR";
		case LOG_WARN:		return "WARN";
		case LOG_INFO:		return "INFO";
		case LOG_VERBOSE:	return "VERB";
		case LOG_TRACE:		return "TRACE";
		default:			return "?????";
	}
}

void Logger::log(LogLevel level, const std::string& category,
				const std::string& message)
{
	if (!_output)
		return;
	
	for (int32_t i = 0; i < _indentLevel; ++i)
		*_output << _indentValue;
	*_output << color(
		std::string("[") + getLevelName(level) + "]",
		getLevelColor(level)
	);
	
	if (!category.empty())
		*_output << " ("
			<< color(category, COLOR_BOLD_WHITE)
			<< ")";
	
	*_output << " " << message << "\n";
	_output->flush();
}


void	Logger::dbg_enter(const std::string &category, const char *fn)
{
	PEG_LOG_TRACE(category,
		Logger::color("Enter", COLOR_RED) + " " +
		Logger::color(fn, COLOR_BLUE)
	);
	Logger::indent();
}

void	Logger::dbg_exit(const std::string &category, const char *fn)
{
	Logger::unindent();
	PEG_LOG_TRACE(category,
		Logger::color("Exit", COLOR_RED) + " " +
		Logger::color(fn, COLOR_BLUE)
	);
}

}

#endif
