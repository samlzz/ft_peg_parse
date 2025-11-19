/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringUtils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 10:35:09 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/19 15:25:53 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRINGUTILS_HPP
# define STRINGUTILS_HPP

# include <sstream>
# include <stdint.h>
# include <cstddef>
# include <string>

inline std::string	escapeStringDisplay(const std::string &str, size_t maxLen = 40)
{
	std::string	result;
	size_t		len = 0;
	
	for (size_t i = 0; i < str.size() && len < maxLen; ++i, ++len)
	{
		switch (str[i]) {
			case '\n': result += "\\n"; break;
			case '\r': result += "\\r"; break;
			case '\t': result += "\\t"; break;
			case '\\': result += "\\\\"; break;
			case '"':  result += "\\\""; break;
			default:
				result += str[i];
				break;
		}
	}
	
	if (str.size() > maxLen)
		result += "...";
	
	return result;
}

inline char unescapeChar(const std::string &s, size_t &i)
{
	if (s[i] != '\\' || i + 1 >= s.size())
		return s[i];

	char c = s[++i];
	switch (c)
	{
		case 'n':	return '\n';
		case 't':	return '\t';
		case 'r':	return '\r';
		case '\\':	return '\\';
		case '\'':	return '\'';
		case '"':	return '"';
		case '0':	return '\0';
		default:	return c;
	}
}

inline std::string unescapeString(const std::string &raw)
{
	std::string	resp;
	
	for (size_t i = 0; i < raw.size(); ++i)
		resp += unescapeChar(raw, i);
	return resp;
}

static inline void	_appendRange(std::string &dst, char beg, char end)
{
	if (static_cast<uint8_t>(beg) <= static_cast<uint8_t>(end))
	{
		for (uint8_t ch = beg; ch <= end; ++ch)
			dst += ch;
	}
	else
		dst += std::string() + beg + '-' + end;
}

inline std::string expandCharSet(const std::string &raw)
{
	std::string	resp;
	char		c;

	for (size_t i = 0; i < raw.size(); ++i)
	{
		c = unescapeChar(raw, i);
		if (i + 2 < raw.size()
			&& raw[i + 1] == '-' && (!i || raw[i - 1] != '\\'))
		{
			i += 2;
			_appendRange(resp, c, unescapeChar(raw, i));
		}
		else
			resp += c;
	}
	return resp;
}

template <typename T>
std::string toString(T value)
{
	std::ostringstream	oss;
	oss << value;
	return oss.str();
}

#endif