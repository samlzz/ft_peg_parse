/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DebugLogger.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 07:59:53 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/17 23:47:58 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUGLOGGER_HPP
# define DEBUGLOGGER_HPP

# include "utils/DebugConfig.hpp"

# if PEG_DEBUG_LEVEL > 0

#  include <iostream>
#  include <string>
#  include <stdint.h>

namespace PegDebug {

#define COLOR_RESET			"\033[0m"
#define COLOR_BOLD			"\033[1m"
#define COLOR_DIM			"\033[2m"

#define COLOR_RED			"\033[31m"
#define COLOR_GREEN			"\033[32m"
#define COLOR_YELLOW		"\033[33m"
#define COLOR_BLUE			"\033[34m"
#define COLOR_MAGENTA		"\033[35m"
#define COLOR_CYAN			"\033[36m"
#define COLOR_WHITE			"\033[37m"

#define COLOR_BOLD_RED		"\033[1;31m"
#define COLOR_BOLD_GREEN	"\033[1;32m"
#define COLOR_BOLD_YELLOW	"\033[1;33m"
#define COLOR_BOLD_BLUE		"\033[1;34m"
#define COLOR_BOLD_MAGENTA	"\033[1;35m"
#define COLOR_BOLD_CYAN		"\033[1;36m"
#define COLOR_BOLD_WHITE	"\033[1;37m"

enum LogLevel {
	LOG_ERROR = PEG_DBG_ERROR,
	LOG_WARN = PEG_DBG_WARN,
	LOG_INFO = PEG_DBG_INFO,
	LOG_VERBOSE = PEG_DBG_VERBOSE,
	LOG_TRACE = PEG_DBG_TRACE
};

class Logger {
private:
	static std::ostream	*_output;
	static int32_t		_indentLevel;
	static bool			_useColors;
	
public:
	static void		setOutput(std::ostream& os)	{ _output = &os; }
	static void		setColorMode(bool enabled)	{ _useColors = enabled; }
	
	static void		indent()		{ _indentLevel++; }
	static void		unindent()		{ if (_indentLevel > 0) _indentLevel--; }
	static int32_t	indentLevel()	{ return _indentLevel; }
	static void		resetIndent()	{ _indentLevel = 0; }
	
	static void		log(LogLevel level, const std::string& category,
					const std::string& message);
	
	static std::string	color(const std::string& text, const char* colorCode);
};

class ScopedIndent
{
public:
	ScopedIndent()	{ Logger::indent(); }
	~ScopedIndent()	{ Logger::unindent(); }
};

}

// Debug Macros
# define PEG_LOG(level, category, msg) \
	do { \
		if (level <= PEG_DEBUG_LEVEL) \
			PegDebug::Logger::log((PegDebug::LogLevel)(level), category, msg); \
	} while(0)

# define PEG_LOG_ERROR(cat, msg)	PEG_LOG(PEG_DBG_ERROR, cat, msg)
# define PEG_LOG_WARN(cat, msg)		PEG_LOG(PEG_DBG_WARN, cat, msg)
# define PEG_LOG_INFO(cat, msg)		PEG_LOG(PEG_DBG_INFO, cat, msg)
# define PEG_LOG_VERBOSE(cat, msg)	PEG_LOG(PEG_DBG_VERBOSE, cat, msg)
# define PEG_LOG_TRACE(cat, msg)	PEG_LOG(PEG_DBG_TRACE, cat, msg)

# else
// Mode release : empty macros
#  define PEG_LOG(level, category, msg)	do {} while(0)
#  define PEG_LOG_ERROR(cat, msg)		do {} while(0)
#  define PEG_LOG_WARN(cat, msg)		do {} while(0)
#  define PEG_LOG_INFO(cat, msg)		do {} while(0)
#  define PEG_LOG_VERBOSE(cat, msg)		do {} while(0)
#  define PEG_LOG_TRACE(cat, msg)		do {} while(0)
# endif

#endif
