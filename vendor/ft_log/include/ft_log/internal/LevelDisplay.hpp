/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LevelDisplay.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 22:13:43 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/29 22:38:39 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEVEL_DISPLAY_HPP
# define LEVEL_DISPLAY_HPP

# include "level.hpp"

namespace ft_log
{
const char	*e_levelLabel(e_log_level lvl);
const char	*e_levelColor(e_log_level lvl);
}

#endif

