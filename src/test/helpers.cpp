/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 18:49:11 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 18:50:45 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <string>
#include <iostream>

void	sep(const std::string &title)
{
	std::cout << "\n=== " << title << " ===" << std::endl;
}

void	check(bool condition, const std::string &msg)
{
	if (!condition)
	{
		std::cerr << "❌ FAIL: " << msg << std::endl;
		std::exit(1);
	}
	std::cout << "✅ " << msg << std::endl;
}
