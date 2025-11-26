/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 10:35:09 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/26 14:27:41 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdio>
#include <sstream>
#include <stdint.h>
#include <cstddef>
#include <string>

#include "utils/StringUtils.hpp"

// ============================================================================
// Escape helpers
// ============================================================================

static inline std::string	_escapeCharDisplay(unsigned char c)
{
	switch (c)
	{
		case '\n': return "\\n";
		case '\r': return "\\r";
		case '\t': return "\\t";
		case '\\': return "\\\\";
		case '"':  return "\\\"";

		default:
			if (c < 32 || c >= 127)
			{
				char buf[6];
				std::sprintf(buf, "\\x%02X", c);
				return buf;
			}
			return std::string(1, c);
	}
}

std::string	escapeStringDisplay(const std::string &str, size_t maxLen)
{
	std::ostringstream oss;

	oss << "'";
	for (size_t i = 0; i < str.size() && i < maxLen; ++i)
		oss << _escapeCharDisplay(str[i]);

	if (str.size() > maxLen)
		oss << "...";
	oss << "'";
	return oss.str();
}

std::string	escapeCharSetDisplay(const std::string &set, size_t maxSegments)
{
	std::ostringstream	oss;
	size_t				count = 0;
	size_t				seg = 0;

	oss << "[";
	for (size_t i = 0; i < set.size() && seg < maxSegments; )
	{
		uint8_t	start = static_cast<uint8_t>(set[i]);
		uint8_t	end   = start;
		size_t	j;

		for (j = i + 1; j < set.size()
				&& static_cast<uint8_t>(set[j]) == end + 1; ++j)
			end = static_cast<uint8_t>(set[j]);

		if (seg++ > 0)
			oss << " ";

		oss << _escapeCharDisplay(start);
		if (start != end)
			oss << '-' << _escapeCharDisplay(end);

		count += (end - start + 1);
		i = j;
	}
	if (set.size() > count)
		oss << "...";
	oss << "]";
	return oss.str();
}

// ============================================================================
// Unescape helpers
// ============================================================================

static inline char	_unescapeChar(const std::string &s, size_t &i)
{
	if (s[i] != '\\' || i + 1 >= s.size())
		return s[i];

	char c = s[++i];
	switch (c)
	{
	case 'n':  return '\n';
	case 't':  return '\t';
	case 'r':  return '\r';
	case '\\': return '\\';
	case '\'': return '\'';
	case '"':  return '"';
	case '0':  return '\0';
	default:   return c;
	}
}

std::string	unescapeString(const std::string &raw)
{
	std::ostringstream oss;

	for (size_t i = 0; i < raw.size(); ++i)
		oss << _unescapeChar(raw, i);
	return oss.str();
}

// ============================================================================
// Charset expansion
// ============================================================================

static inline void	_appendRange(std::ostringstream &dst, char beg, char end)
{
	if (static_cast<uint8_t>(beg) <= static_cast<uint8_t>(end))
	{
		for (uint8_t ch = beg; ch <= end; ++ch)
			dst << ch;
	}
	else
		dst << beg << "-" << end;
}

static inline std::string	_inverseChars(const std::string &src)
{
	std::ostringstream dst;

	for (int32_t i = 0; i < 256; ++i)
	{
		char c = static_cast<char>(i);
		if (src.find(c) == std::string::npos)
			dst << c;
	}
	return dst.str();
}

std::string	expandCharSet(const std::string &raw)
{
	std::ostringstream	resp;
	bool				negate;
	char				c;

	negate = (!raw.empty() && raw[0] == '^');
	for (size_t i = size_t(negate); i < raw.size(); ++i)
	{
		c = _unescapeChar(raw, i);

		if (i + 2 < raw.size()
			&& raw[i + 1] == '-' && (!i || raw[i - 1] != '\\'))
		{
			i += 2;
			_appendRange(resp, c, _unescapeChar(raw, i));
		}
		else
			resp << c;
	}

	if (negate)
		return _inverseChars(resp.str());
	return resp.str();
}

