/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PegLexer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:27:44 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/30 01:17:11 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PEGLEXER_HPP
# define PEGLEXER_HPP

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
	// Constructors / Destructor
	// ========================================================================

	PegLexer(const std::string &grammar_path);
	PegLexer(const PegLexer &other);
	~PegLexer() {}

	// ========================================================================
	// Methods
	// ========================================================================

	// ---- Main operations ----
	Token	peek(void);
	Token	next(void);
	bool	match(enum e_tk_type type);
	void	skipWhitespaces(void);

	// ---- Input accessors ----
	const Input	&input(void) const	{ return _input; }

private:

	Input	_input;
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

#endif

