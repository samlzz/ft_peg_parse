/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Input.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:34:40 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/03 01:51:05 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_HPP
# define INPUT_HPP

# include <cstddef>
# include <stdint.h>
# include <string>

# include "./PegException.hpp"

/**
 * Lightweight text reader used by the PEG parser.
 * 
 * Provides controlled random-access over a string or file input.
 * Used by PegLexer and PackratParser to read and match characters.
 * Only whitespaces present in member constant WHITESPACES (" \n\t\r" by default) are skipped.
*/
class Input {

private:
	std::string	_data;
	size_t		_pos;

	static const char	*WHITESPACES;

	Input(): _data(), _pos(0) {}
	Input(const std::string &data): _data(data), _pos(0) {}

public:
	// Exception thrown when unable to open file
	class FileOpenError : public PegException {
	public:
		FileOpenError() : PegException("Input: unexpected EOF")
		{}
		FileOpenError(const std::string &msg) : PegException("Input: " + msg)
		{}
	};

	// Exception thrown when reading past end of input
	class InputUnexpectedEof : public PegException {
	public:
		InputUnexpectedEof() : PegException("Input: unexpected EOF")
		{}
		InputUnexpectedEof(const std::string &msg) : PegException("Input: " + msg)
		{}
	};

	// --- Construction ---
	static Input	fromFile(const std::string &path);
	static Input	fromText(const std::string &text);
	Input(const Input& other): _data(other._data), _pos(other._pos)
	{}
	~Input() {}

	Input&	operator=(const Input& other);
	char	operator*() const;				// Current char
	char	operator[](size_t index) const;	// Access char by index from curr pos
	Input	&operator++();					// Advance one char

	// --- Core methods ---
	bool		eof(void) const;			// End of input reached
	char		peek(void) const;			// Look current char
	char		get(void);					// Get current char or throw
	bool		match(const std::string &literal);

	size_t		pos(void) const			{ return _pos; }
	void		setPos(size_t newPos)	{ _pos = newPos; }

	size_t		remaining(void) const;
	size_t		line(void) const;
	size_t		column(void) const;

	std::string	substr(size_t start, size_t end) const;
	std::string	context(size_t pos, size_t radius) const;	// Look text around pos
	void		skipUntil(bool (*until)(char c), std::string *skipped = NULL);
};

#endif
