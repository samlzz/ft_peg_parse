/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PegLexer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:27:44 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/20 14:01:14 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PEGLEXER_HPP
# define PEGLEXER_HPP

#include <ostream>
# include <stdint.h>
# include <string>

# include "utils/Input.hpp"
# include "utils/PegException.hpp"
# include "utils/DebugConfig.hpp"

class PegLexer {

public:
	enum e_tk_type {
		T_ID, T_LITERAL,
		T_SLASH, T_STAR, T_PLUS, T_QMARK,
		T_AND, T_NOT,
		T_LPAREN, T_RPAREN,
		T_CHARRANGE, T_DOT,
		T_COLON, T_ASSIGN,
		T_EOL, T_END
	};

	struct Token {
		enum e_tk_type	type;
		std::string		val;
# if defined(PEG_DEBUG_PEGPARSER) || defined(PEG_DEBUG_GRAMMAR)
		std::string	debugType(void) const;
		std::string	debugRepr(bool colored = true) const;

		friend std::ostream &operator<<(std::ostream &os, const Token &tk)
			{ return os << tk.debugRepr(); }
# endif
	};

	class PegLexerError : public PegException {
	public:
		PegLexerError(const std::string& msg) : PegException("Grammar lexer: " + msg)
		{}
	};

	PegLexer(const std::string &grammar_path);
	PegLexer(const PegLexer& other);
	~PegLexer() {}

	Token	peek(void);
	Token	next(void);
	bool	match(enum e_tk_type type);
	void	skipWhitespaces(void);

	const Input	&input(void) const	{ return _input; }
	Input		&input(void)		{ return _input; }

private:
	Input	_input;
	Token	_peeked;
	bool	_hasPeeked;

	PegLexer& operator=(const PegLexer& other);

	Token	lexLiteral(void);
	Token	lexIdentifier(void);
	Token	lexCharRange(void);
	Token	lexOp(char c);
	Token	lexOne(void);
};

#endif
