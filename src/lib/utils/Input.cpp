/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Input.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 12:46:56 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/03 01:51:25 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>

#include "utils/Input.hpp"

const char * Input::WHITESPACES = " \n\t\r";

Input	Input::fromFile(const std::string &path)
{
	std::ifstream	file(path.c_str(), std::ios::in | std::ios::binary);

	if (!file.is_open())
		throw FileOpenError(
			"Input: failed to open file '" + std::string(path) + "'"
		);
	
	std::ostringstream buf;
	buf << file.rdbuf();
	if (file.bad())
		throw FileOpenError(
			"Input: error while reading file '" + std::string(path) + "'"
		);

	return Input(buf.str());
}

Input	Input::fromText(const std::string &text)
{
	return (Input(text));
}

// *Operators

Input& Input::operator=(const Input& other)
{
	if (this != &other)
	{
		_pos = other._pos;
		_data = other._data;
	}
	return *this;
}

char	Input::operator*() const
{
	return peek();
}

char	Input::operator[](size_t index) const
{
	return _data[_pos + index];
}

Input	&Input::operator++()
{
	++_pos;
	return *this;
}

//* Methods

bool		Input::eof(void) const
{
	return (_data.empty() || _pos >= _data.size());
}

bool		Input::match(const std::string &literal)
{
	size_t	len = literal.size();

	if (eof() || _pos + len > _data.size() ||
		std::strncmp(_data.c_str() + _pos, literal.c_str(), len))
		return false;

	_pos += len;
	return true;
}

char		Input::get(void)
{
	if (eof())
		throw InputUnexpectedEof();
	return _data[_pos++];
}

char		Input::peek(void) const
{
	if (eof())
		return '\0';
	return _data[_pos];
}

size_t		Input::remaining(void) const
{
	size_t	size = _data.size();
	if (_pos >= size) return (0);
	return size - _pos;
}

size_t		Input::line(void) const
{
	size_t	i = 0;
	size_t	ln_count = 1;

	while (i < _pos)
	{
		if (_data[i] == '\n')
			++ln_count;
		++i;
	}
	return (ln_count);
}

size_t		Input::column(void) const
{
	size_t	lst_ln;
	
	lst_ln = _data.rfind('\n', _pos == 0 ? 0 : _pos - 1);
	if (lst_ln == std::string::npos)
		return _pos + 1;
	return _pos - lst_ln;
}

std::string	Input::substr(size_t start, size_t end) const
{
	return _data.substr(start, end - start);
}

std::string	Input::context(size_t pos, size_t radius) const
{
	size_t	start;
	size_t	end;
	size_t	size = _data.size();

	if (pos > radius)
		start = pos - radius;
	else
		start = 0;
	if (pos + radius < size)
		end = pos + radius;
	else
		end = size;
	return substr(start, end);
}

void	Input::skipUntil(bool (*until)(char), std::string *skipped)
{
	while (!eof())
	{
		if (until(peek()))
			break;
		if (skipped)
			skipped += get();
	}
}
