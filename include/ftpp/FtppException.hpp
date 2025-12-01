/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtppException.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 19:10:43 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/01 19:59:15 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_EXCEPTION_HPP__
# define __FTPP_EXCEPTION_HPP__

# include <stdexcept>
# include <string>

// ============================================================================
// FtppException
// ============================================================================

/**
 * @brief Base exception used across PEG parsing components.
 *
 * All parser, lexer and grammar errors derive from this class.
 */
class FtppException : public std::runtime_error {

public:
	FtppException(const std::string &msg) : std::runtime_error(msg) {}
	virtual ~FtppException() throw() {}
};

#endif /* __FTPP_EXCEPTION_HPP__ */