/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DebugLogger.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 07:59:53 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/16 19:53:08 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUGLOGGER_HPP
# define DEBUGLOGGER_HPP

# include "utils/DebugConfig.hpp"

# if PEG_DEBUG_LEVEL > 0

#  include <sstream>
#  include <iostream>
#  include <string>
#  include <stdint.h>

namespace PegDebug {

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

template <typename T>
std::string toString(T value)
{
	std::ostringstream	oss;
	oss << value;
	return oss.str();
}

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
