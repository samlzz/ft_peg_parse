/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PegLexer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:27:44 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 23:34:12 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PEGLEXER_HPP
# define PEGLEXER_HPP

# include <stdint.h>
# include <string>

# include "utils/Input.hpp"
# include "utils/PegException.hpp"

# define PRINT_CALLER

class PegLexer {

private:
	Input	&_input;

	PegLexer(const PegLexer& other);
	PegLexer& operator=(const PegLexer& other);

public:
	enum e_tk_type {
		T_ID,
		T_LITERAL,
		T_SLASH,
		T_STAR,
		T_PLUS,
		T_QMARK,
		T_AND,
		T_NOT,
		T_LPAREN,
		T_RPAREN,
		T_ASSIGN,
		T_END
	};

	struct Token {
		enum e_tk_type	type;
		std::string		val;
	};

	class PegLexerError : public PegException {
	public:
		PegLexerError(const std::string& msg) : PegException("Lexer: " + msg)
		{}
	};

	PegLexer(const std::string &grammar_path);
	~PegLexer();

	void	setSrc(const std::string &src);

	Token	peek(void) const;
	Token	next(void);
	bool	match(enum e_tk_type type);
	void	skipWhitespaces(void);
};

#endif
