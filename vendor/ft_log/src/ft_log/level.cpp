/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   level.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 21:37:11 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/29 22:53:25 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ostream>

#include "level.hpp"
#include "AnsiColor.hpp"

namespace ft_log
{

const char	*e_levelLabel(e_log_level lvl)
{
	switch (lvl)
	{
		case LOG_TRACE: return "TRACE";
		case LOG_DEBUG: return "DEBUG";
		case LOG_INFO:  return "INFO";
		case LOG_WARN:  return "WARN";
		case LOG_ERROR: return "ERROR";
		case LOG_OFF:   return "";
	}
}

const char	*e_levelColor(e_log_level lvl)
{
	switch (lvl)
	{
		case LOG_TRACE: return FT_LOG_COLOR_BLACK;
		case LOG_DEBUG: return FT_LOG_COLOR_GREEN;
		case LOG_INFO:  return FT_LOG_COLOR_CYAN;
		case LOG_WARN:  return FT_LOG_COLOR_YELLOW;
		case LOG_ERROR: return FT_LOG_COLOR_RED;
		case LOG_OFF:   return FT_LOG_COLOR_RESET;
	}
	return FT_LOG_COLOR_RESET;
}

std::ostream	&operator<<(std::ostream &os, e_log_level lvl)
{
	return os << e_levelLabel(lvl);
}

}
