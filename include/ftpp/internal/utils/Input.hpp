/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Input.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:34:40 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 13:00:17 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_INPUT_HPP__
# define __FTPP_INPUT_HPP__

# include <cstddef>
# include <stdint.h>
# include <string>

// ============================================================================
// Input
// ============================================================================

/**
 * @brief Lightweight UTF-8 agnostic text reader for PEG parsing.
 *
 * Provides controlled random-access on string or file data.
 * Exposes:
 *   - sequential reading
 *   - literal matching
 *   - position tracking (line/column)
 *   - context extraction for diagnostics
 */
class Input {

private:
	std::string	_data;
	size_t		_pos;

	Input();
	Input(const std::string &data);

public:
	// ========================================================================
	// Construction / Assignment
	// ========================================================================

	static Input	fromFile(const std::string &path);
	static Input	fromText(const std::string &text);

	Input(const Input &other);
	~Input();

	Input	&operator=(const Input &other);

	// ========================================================================
	// Core functionality
	// ========================================================================

	char		operator*() const;				// Current char
	char		operator[](size_t idx) const;	// Char at index relative to current pos
	Input		&operator++();					// Advance one char

	// ---- Reading ----
	bool		eof(void) const;
	char		peek(void) const;
	char		get(void);
	bool		match(const std::string &literal);

	// ---- Position ----
	size_t		pos(void) const;
	void		setPos(size_t newPos);

	size_t		remaining(void) const;
	size_t		line(void) const;
	size_t		column(void) const;

	// ---- Contextual extraction ----
	std::string	substr(size_t start, size_t end) const;
	std::string	context(size_t pos, size_t radius) const;

	// ---- Traverse until condition ----
	void		skipUntil(bool (*until)(char c), std::string *skipped = NULL);
};

#endif /* __FTPP_INPUT_HPP__ */