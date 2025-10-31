/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Input.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:34:40 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 14:10:57 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_HPP
# define INPUT_HPP

# include <cstddef>
# include <stdint.h>
# include <string>

# include "./PegException.hpp"

# define PRINT_CALLER

class Input {

private:
	std::string	_data;
	size_t		_pos;

	static const char	*WHITESPACES;

public:
	class InputUnexpectedEof : public PegException {
	public:
		InputUnexpectedEof() : PegException("Input: unexpected EOF")
		{}
		InputUnexpectedEof(const std::string &msg) : PegException("Input: " + msg)
		{}
	};

	Input();
	Input(const std::string &text);
	Input(const char *file_path);
	Input(const Input& other);
	~Input();

	Input&	operator=(const Input& other);
	char	operator*() const;
	Input	&operator++();

	bool		eof(void) const;

	char		peek(void) const;
	char		get(void);								/* in case of eof : throw an expection */
	bool		match(const std::string &literal);		/* in case of eof: return '\0' */

	size_t		pos(void) const;
	void		setPos(size_t newPos);

	size_t		remaining(void) const;
	size_t		line(void) const;
	size_t		column(void) const;

	std::string	substr(size_t start, size_t end) const;
	std::string	context(size_t pos, size_t radius) const;
	void		skipWhitespace(void);					/* Only skip ASCII whitespace (no support of LANG_C) */
};

#endif
