/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DebugLogger.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 19:27:56 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/16 19:51:42 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/DebugLogger.hpp"

#if PEG_DEBUG_LEVEL > 0

# include <iostream>
# include <sstream>

namespace PegDebug {

std::ostream*	Logger::_output = &std::cerr;
int32_t			Logger::_indentLevel = 0;
bool			Logger::_useColors = true;

namespace Color {

	const char	*RESET	= "\033[0m";
	const char	*BOLD	= "\033[1m";
	const char	*DIM		 = "\033[2m";
	
	const char	*RED		= "\033[31m";
	const char	*GREEN	= "\033[32m";
	const char	*YELLOW	= "\033[33m";
	const char	*BLUE	= "\033[34m";
	const char	*MAGENTA	= "\033[35m";
	const char	*CYAN	= "\033[36m";
	const char	*WHITE	= "\033[37m";
	
	const char	*BOLD_RED		= "\033[1;31m";
	const char	*BOLD_GREEN		= "\033[1;32m";
	const char	*BOLD_YELLOW		= "\033[1;33m";
	const char	*BOLD_BLUE		= "\033[1;34m";
	const char	*BOLD_MAGENTA	= "\033[1;35m";
	const char	*BOLD_CYAN		= "\033[1;36m";
	const char	*BOLD_WHITE		= "\033[1;37m";

}

std::string Logger::color(const std::string& text, const char* colorCode)
{
	if (!_useColors || !colorCode)
		return text;

	std::ostringstream	oss;
	oss << colorCode << text << Color::RESET;
	return oss.str();
}

static const char* getLevelColor(LogLevel level)
{
	switch (level) {
		case LOG_ERROR:		return Color::BOLD_RED;
		case LOG_WARN:		return Color::BOLD_YELLOW;
		case LOG_INFO:		return Color::BOLD_CYAN;
		case LOG_VERBOSE:	return Color::CYAN;
		case LOG_TRACE:		return Color::DIM;
		default:
			return Color::RESET;
	}
}

static const char* getLevelName(LogLevel level)
{
	switch (level) {
		case LOG_ERROR:		return "ERROR";
		case LOG_WARN:		return "WARN ";
		case LOG_INFO:		return "INFO ";
		case LOG_VERBOSE:	return "VERB ";
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
		*_output << "  ";
	*_output << color(
		std::string("[") + getLevelName(level) + "]",
		getLevelColor(level)
	);
	
	if (!category.empty())
		*_output << " " << color("[" + category + "]", Color::BOLD_WHITE);
	
	*_output << " " << message << "\n";
	_output->flush();
}

}

#endif
