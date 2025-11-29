/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LogScope.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 00:00:39 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/29 19:01:23 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal/Logger.hpp"
#include "LogScope.hpp"

namespace ft_log {

// ============================================================================
// Indentation (static statement)
// ============================================================================

int32_t	LogScope::_indentLvl = 0;

int32_t	LogScope::indentLevel(void)		{ return _indentLvl; }
void	LogScope::resetIndent(void)		{ _indentLvl = 0; }

// ============================================================================
// Private helper
// ============================================================================

void	LogScope::_logIndent(Logger &instance)
{
	for (int32_t i = 0; i < _indentLvl; ++i)
		instance.stream() << FT_LOG_SCOPE_INDENT_VAL;
}

// ============================================================================
// Enter/Exit print
// ============================================================================

LogScope::LogScope(const char *category, const char *name, e_log_level level)
	: _category(category), _name(name)
	, _level(level), _enabled(false)
{
	Logger	&instance = Logger::instance();

	if (!instance.enabled(_category, _level))
		return;
	_enabled = true;

	_logIndent(instance);
	instance.stream()
		<< instance.colorize("enter ", FT_LOG_SCOPE_ENTER_COLOR)
		<< instance.colorize(_name, FT_LOG_SCOPE_NAME_COLOR)
		<< "\n";

	_indentLvl++;
}

LogScope::~LogScope(void)
{
	if (!_enabled)
		return;
	Logger	&instance = Logger::instance();
	
	if (_indentLvl > 0)
		_indentLvl--;
	_logIndent(instance);

	instance.stream()
		<< instance.colorize("exit ", FT_LOG_SCOPE_EXIT_COLOR)
		<< instance.colorize(_name, FT_LOG_SCOPE_NAME_COLOR)
		<< "\n";

}

} // namespace ft_log

