/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogScope.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 22:39:07 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/29 23:44:43 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_SCOPE_HPP
# define LOG_SCOPE_HPP

# include <stdint.h>

# include "AnsiColor.hpp"
# include "level.hpp"

// ============================================================================
// Optional Defines to adapt print
// ============================================================================

// ---- Indentation ----
# ifndef FT_LOG_SCOPE_INDENT_VAL
#  define FT_LOG_SCOPE_INDENT_VAL	"  "
# endif

// ---- Color used ----
# ifndef FT_LOG_SCOPE_ENTER_COLOR
#  define FT_LOG_SCOPE_ENTER_COLOR	FT_LOG_COLOR_RED
# endif
# ifndef FT_LOG_SCOPE_EXIT_COLOR
#  define FT_LOG_SCOPE_EXIT_COLOR	FT_LOG_COLOR_RED
# endif
# ifndef FT_LOG_SCOPE_NAME_COLOR
#  define FT_LOG_SCOPE_NAME_COLOR	FT_LOG_COLOR_BLUE
# endif

// ============================================================================
// Forward declarations
// ============================================================================
class Logger;

// ============================================================================
// ft_log::LogScope - RAII tracing helper
// ============================================================================

namespace ft_log
{

/**
 * @brief RAII helper that logs enter/exit of a scope.
 */
class LogScope
{

public:
	LogScope(const char *category, const char *name, e_log_level level = LOG_TRACE);
	~LogScope(void);

	// ---- Indent management ----
	static int32_t	indentLevel(void);
	static void		resetIndent(void);

private:
	const char		*_category;
	const char		*_name;
	e_log_level		_level;
	bool			_enabled;

	static int32_t	_indentLvl;

	void			_logIndent(Logger &instance);

	LogScope();
	LogScope(const LogScope &other);
	LogScope &operator=(const LogScope &other);
};

} // namespace ft_log

#endif // LOG_SCOPE_HPP

