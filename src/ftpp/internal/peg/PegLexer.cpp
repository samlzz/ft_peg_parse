/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PegLexer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 15:35:07 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/03 17:38:34 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_log/ft_log.hpp>
#include <cctype>
#include <string>

#include "peg/PegLexer.hpp"
#include "FtppException.hpp"
#include "config.h"
#include "utils/Input.hpp"

// ============================================================================
// Constructors
// ============================================================================

PegLexer::PegLexer(Input &rawGrammar)
	: _input(rawGrammar), _peeked(), _hasPeeked(false)
{}

PegLexer::PegLexer(const PegLexer &other)
	: _input(other._input)
	, _peeked(other._peeked), _hasPeeked(other._hasPeeked)
{}

// ============================================================================
// Helpers
// ============================================================================

// Static helpers for scanning
static bool	_until_ln(char c)			{ return c == '\n'; }
static bool	_until_eobracket(char c)	{ return c == ']'; }
static bool	_until_eoid(char c)
{
	return !(c == '_' || std::isalnum(static_cast<uint8_t>(c)));
}

// Skip spaces and comments (starting with '#', until newline).
void PegLexer::skipWhitespaces(void)
{
	char c;

	while (!_input.eof())
	{
		c = _input.peek();
		if (std::isspace(c) && c != '\n')
			++_input;
		else if (c == '#')
			_input.skipUntil(_until_ln);
		else
			break;
	}
}

static inline void	_log_action(
	const std::string &act, const PegLexer::Token &tk
)
{
	ft_log::log(FTPP_LOG_LEXER, ft_log::LOG_TRACE)
		<< ft_log::color(act, FT_LOG_COLOR_MAGENTA)
		<< tk.repr() << std::endl;
}

// ============================================================================
// Tokenizers
// ============================================================================

PegLexer::Token PegLexer::lexLiteral(void)
{
	char quote = _input.get();
	char c;
	std::string val;

	while (!_input.eof() && _input.peek() != quote)
	{
		if (_input.peek() == '\n')
			throw GrammarError("lexing failed: Unexpected newline in string literal");

		c = _input.get();
		val += c;

		if (c == '\\' && !_input.eof())
			val += _input.get();
	}

	if (_input.eof())
		throw GrammarError("lexing failed: Unterminated string literal");

	_input.get();
	return (Token){T_LITERAL, val};
}

PegLexer::Token PegLexer::lexIdentifier(void)
{
	std::string val(1, _input.get());
	_input.skipUntil(_until_eoid, &val);
	return (Token){T_ID, val};
}

PegLexer::Token PegLexer::lexCharRange(void)
{
	std::string buf;

	_input.skipUntil(_until_eobracket, &buf);
	if (!_input.match("]"))
		throw GrammarError("lexing failed: Unterminated character class: missing ']'");

	return (Token){T_CHARRANGE, buf};
}

PegLexer::Token PegLexer::lexOp(char c)
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
		case '[': ++_input; return lexCharRange();
		case '.': ++_input; return (Token){T_DOT, "."};
		case ':': ++_input; return (Token){T_COLON, ":"};
		case '~': ++_input; return (Token){T_TILD, "~"};
		case '<':
		{
			if (_input.match("<-"))
				return (Token){T_ASSIGN, "<-"};
			break;
		}
		default:
			break;
	}
	throw GrammarError(std::string("lexing failed: Unexpected character: ") + c);
}

PegLexer::Token PegLexer::lexOne(void)
{
	skipWhitespaces();

	if (_input.eof())
		return (Token){T_END, ""};

	if (_input.match("\n"))
		return (Token){T_EOL, "\n"};

	char c = _input.peek();

	if (c == '"' || c == '\'')
		return lexLiteral();

	if (c == '@' || c == '_' || std::isalpha(static_cast<uint8_t>(c)))
		return lexIdentifier();

	return lexOp(c);
}

// ============================================================================
// Public token operations
// ============================================================================

PegLexer::Token PegLexer::next(void)
{
	Token curr;

	if (_hasPeeked)
	{
		_hasPeeked = false;
		curr = _peeked;
	}
	else
		curr = lexOne();

	_log_action("next: ", curr);
	return curr;
}

PegLexer::Token PegLexer::peek(void)
{
	if (!_hasPeeked)
	{
		_peeked = lexOne();
		_hasPeeked = true;
	}

	_log_action("peek: ", _peeked);
	return _peeked;
}

bool PegLexer::match(enum e_tk_type type)
{
	if (peek().type == type)
	{
		next();
		return true;
	}
	return false;
}

// ============================================================================
// Tokens representation
// ============================================================================

std::string PegLexer::Token::reprType(void) const
{
	switch (type)
	{
		case T_ID:         return "ID";
		case T_LITERAL:    return "LITERAL";
		case T_CHARRANGE:  return "CHARRANGE";

		case T_SLASH:      return "SLASH";
		case T_STAR:       return "STAR";
		case T_PLUS:       return "PLUS";
		case T_QMARK:      return "QMARK";

		case T_AND:        return "AND";
		case T_NOT:        return "NOT";

		case T_LPAREN:     return "LPAREN";
		case T_RPAREN:     return "RPAREN";

		case T_DOT:        return "DOT";
		case T_COLON:      return "COLON";
		case T_ASSIGN:     return "ASSIGN";

		case T_EOL:        return "EOL";
		case T_END:        return "END";

		default:
			return "UNKNOWN";
	}
}

std::string PegLexer::Token::repr(void) const
{
	std::string	rep = ft_log::color(reprType(), FT_LOG_COLOR_YELLOW);
	if (!val.empty() && type != T_EOL)
		rep += ft_log::color(" \"" + val + "\"", FT_LOG_COLOR_GREEN);

	return rep;
}

// ============================================================================
// Public state operations
// ============================================================================

PegLexer::State	PegLexer::save(void) const
{
	return (State) {
		_input.pos(),
		_hasPeeked,
		_peeked
	};
}

void	PegLexer::restore(const State &st)
{
	_input.setPos(st.pos);
	_hasPeeked = st.hasPeeked;
	_peeked = st.peeked;
}
