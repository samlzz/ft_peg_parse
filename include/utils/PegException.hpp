/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PegException.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 19:10:43 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/25 19:10:45 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PEGEXCEPTION_HPP
# define PEGEXCEPTION_HPP

# include <stdexcept>
# include <string>

// ============================================================================
// PegException
// ============================================================================

/**
 * @brief Base exception used across PEG parsing components.
 *
 * All parser, lexer and grammar errors derive from this class.
 */
class PegException : public std::runtime_error {

public:
	PegException(const std::string &msg) : std::runtime_error(msg) {}
	virtual ~PegException() throw() {}
};

#endif

