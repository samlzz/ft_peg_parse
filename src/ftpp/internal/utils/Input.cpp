/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Input.cpp                                          :+:      :+:         :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 12:46:56 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/26 20:45:26 by sliziard         ###   ########.fr       */
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
#include "FtppException.hpp"

// ========================================================================
// Construction / Destruction
// ========================================================================

// ---- Private ----
Input::Input(): _data(), _pos(0) {}
Input::Input(const std::string &data): _data(data), _pos(0) {}

// --- Public ----
Input::Input(const Input &other): _data(other._data), _pos(other._pos) {}
Input::~Input() {}

Input	Input::fromFile(const std::string &path)
{
	std::ifstream file(path.c_str(),std::ios::in | std::ios::binary);

	if (!file.is_open())
		throw EngineError("Input: failed to open file '" + path + "'");

	std::ostringstream buf;
	buf << file.rdbuf();
	if (file.bad())
		throw EngineError("Input: error while reading file '" + path + "'");

	return Input(buf.str());
}

Input	Input::fromText(const std::string &text)
{
	return Input(text);
}

// ============================================================================
// Operators
// ============================================================================

Input	&Input::operator=(const Input &other)
{
	if (this != &other)
	{
		_pos  = other._pos;
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

// ============================================================================
// Methods
// ============================================================================

// ---- Reading ----

bool	Input::eof(void) const
{
	return (_data.empty() || _pos >= _data.size());
}

char	Input::get(void)
{
	if (eof())
		throw EngineError("Input: unexpected EOF");
	return _data[_pos++];
}

char	Input::peek(void) const
{
	if (eof())
		return '\0';
	return _data[_pos];
}

bool	Input::match(const std::string &literal)
{
	size_t len = literal.size();

	if (eof()
		|| _pos + len > _data.size()
		|| std::strncmp(_data.c_str() + _pos, literal.c_str(), len)
	)
		return false;

	_pos += len;
	return true;
}

// ---- Position ----

// - - Accessors - -
size_t	Input::pos(void) const			{ return _pos; }
void	Input::setPos(size_t newPos)	{ _pos = newPos; }

size_t	Input::remaining(void) const
{
	if (_pos >= _data.size())
		return 0;
	return _data.size() - _pos;
}

size_t	Input::line(void) const
{
	size_t ln = 1;
	for (size_t i = 0; i < _pos; ++i)
		if (_data[i] == '\n')
			++ln;
	return ln;
}

size_t	Input::column(void) const
{
	size_t lastLn = _data.rfind('\n', _pos == 0 ? 0 : _pos - 1);
	if (lastLn == std::string::npos)
		return _pos + 1;
	return _pos - lastLn;
}

// ---- Contextual extraction ----
std::string	Input::substr(size_t start, size_t end) const
{
	return _data.substr(start, end - start);
}

std::string	Input::context(size_t pos, size_t radius) const
{
	size_t size  = _data.size();
	size_t start = (pos > radius ? pos - radius : 0);
	size_t end   = (pos + radius < size ? pos + radius : size);

	return substr(start, end);
}

void Input::skipUntil(bool (*until)(char), std::string *skipped)
{
	while (!eof())
	{
		char c = peek();

		if (until(c))
			break;
		if (skipped)
			*skipped += c;
		get();
	}
}
