/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   level.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 19:13:11 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/29 22:53:16 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEVEL_HPP
# define LEVEL_HPP

#include <ostream>

namespace ft_log
{

/**
 * @brief Logging severity level.
 */
enum e_log_level
{
	LOG_TRACE = 0,
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR,
	LOG_OFF
};

std::ostream	&operator<<(std::ostream &os, e_log_level lvl);

} // namespace ft_log

#endif

