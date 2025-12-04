/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PegLexer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:27:44 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/03 17:30:33 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_PEG_LEXER_HPP__
# define __FTPP_PEG_LEXER_HPP__

# include <stdint.h>
# include <string>

# include "utils/Input.hpp"

// ============================================================================
// PegLexer
// ============================================================================

/**
 * @brief Lexer for the PEG grammar source file.
 *
 * Produces tokens for identifiers, literals, operators, and character ranges.
 */
class PegLexer {

public:

	// ========================================================================
	// Token definitions
	// ========================================================================

	enum e_tk_type
	{
		T_ID, T_LITERAL,
		T_SLASH, T_STAR, T_PLUS, T_QMARK,
		T_AND, T_NOT, T_TILD,
		T_LPAREN, T_RPAREN,
		T_CHARRANGE, T_DOT,
		T_COLON, T_ASSIGN,
		T_EOL, T_END
	};

	struct Token
	{
		enum e_tk_type	type;
		std::string		val;

		std::string	reprType(void) const;
		std::string repr(void) const;
	};

	// ========================================================================
	// State definitions
	// ========================================================================
	struct State
	{
		size_t	pos;
		bool	hasPeeked;
		Token	peeked;
	};

	// ========================================================================
	// Constructors / Destructor
	// ========================================================================

	PegLexer(Input &rawGrammar);
	PegLexer(const PegLexer &other);
	~PegLexer() {}

	// ========================================================================
	// Methods
	// ========================================================================

	void	skipWhitespaces(void);

	// ---- Tokens operations ----
	Token	peek(void);
	Token	next(void);
	bool	match(enum e_tk_type type);

	// ---- State operations ----
	State	save() const;
	void	restore(const State &st);

private:

	Input	&_input;
	Token	_peeked;
	bool	_hasPeeked;

	PegLexer();
	PegLexer &operator=(const PegLexer &other);

	// ---- Tokenization helpers ----
	Token	lexLiteral(void);
	Token	lexIdentifier(void);
	Token	lexCharRange(void);
	Token	lexOp(char c);
	Token	lexOne(void);
};

#endif /* __FTPP_PEG_LEXER_HPP__ */