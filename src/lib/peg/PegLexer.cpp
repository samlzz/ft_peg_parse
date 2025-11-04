/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   PegLexer.cpp									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: sliziard <sliziard@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/11/03 00:45:15 by sliziard		  #+#	#+#			 */
/*   Updated: 2025/11/03 01:19:07 by sliziard		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include <cctype>
#include <string>

#include "peg/PegLexer.hpp"
#include "utils/Input.hpp"

// ---- Ctor / Dtor ----

PegLexer::PegLexer(const std::string &grammar_path):
	_input(Input::fromFile(grammar_path)),
	_hasPeeked(false)
{}

PegLexer::~PegLexer() {}

static bool	_until_ln(char c)	{ return c == '\n'; }
static bool	_until_eoid(char c)
{
	return !(c == '@' || c == '_' || std::isalpha(static_cast<uint8_t>(c)));
}

void	PegLexer::skipWhitespaces(void)
{
	char	c;

	while (!_input.eof())
	{
		c = _input.peek();
		if (std::isspace(c))
			++_input;
		else if (c == '#')
			_input.skipUntil(_until_ln);
		else
			break;
	}
}

PegLexer::Token	PegLexer::lexLiteral(void)
{
	char		quote = _input.get();
	std::string	val;

	while (!_input.eof() && _input.peek() != quote)
		val += _input.get();
	if (_input.eof())
		throw PegLexerError("Unterminated string literal");
	_input.get();
	return (Token){T_LITERAL, val};
}

PegLexer::Token	PegLexer::lexIdentifier(void)
{
	std::string	val;
	_input.skipUntil(_until_eoid, &val);
	return (Token){T_ID, val};
}

PegLexer::Token	PegLexer::lexOp(char c)
{
	switch (c)
	{
		case '/': ++_input; return (Token){T_SLASH, "/"}; 
		case '*': ++_input; return (Token){T_STAR, "*"};
		case '+': ++_input; return (Token){T_PLUS, "+"};
		case '?': ++_input; return (Token){T_QMARK, "?"};
		case '!': ++_input; return (Token){T_NOT, "!"};
		case '&': ++_input; return (Token){T_AND, "&"};
		case '(': ++_input; return (Token){T_LPAREN, "("};
		case ')': ++_input; return (Token){T_RPAREN, ")"};
		case ':': ++_input; return (Token){T_COLON, ":"};
		case '<': {
			if (_input[1] == '-')
			{
				++_input; ++_input;
				return (Token){T_ASSIGN, "<-"};
			}
			break;
		}
		default:
			break;
	}
	throw PegLexerError(std::string("Unexpected character: ") + c);
}

PegLexer::Token	PegLexer::lexOne(void)
{
	skipWhitespaces();
	if (_input.eof())
		return (Token){T_END, ""};
	char	c = _input.peek();

	if (c == '"' || c == '\'')
		return lexLiteral();
	else if (c == '@' || c == '_' || std::isalpha(static_cast<uint8_t>(c)))
		return lexIdentifier();
	else
		return lexOp(c);
}

PegLexer::Token	PegLexer::next()
{
	if (_hasPeeked)
	{
		_hasPeeked = false;
		return _peeked;
	}
	_peeked = lexOne();
	return _peeked;
}

PegLexer::Token	PegLexer::peek(void)
{
	if (!_hasPeeked)
	{
		_peeked = lexOne();
		_hasPeeked = true;
	}
	return _peeked;
}

bool	PegLexer::match(enum e_tk_type type)
{
	if (peek().type == type)
	{
		next();
		return true;
	}
	return false;
}