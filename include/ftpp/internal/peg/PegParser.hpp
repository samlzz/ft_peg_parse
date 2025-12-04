/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PegParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:32:07 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/03 18:04:04 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_PEG_PARSER_HPP__
# define __FTPP_PEG_PARSER_HPP__

# include <stdint.h>

# include "./PegLexer.hpp"
# include "peg/core/Expr.hpp"

// ============================================================================
// PegParser
// ============================================================================

/**
 * @brief Parses a PEG grammar file into an Expr-based Grammar object.
 *
 * Uses PegLexer to tokenize the grammar file and builds a rule dictionary
 * composed of Expr trees. Supports choices, sequences, unary operators,
 * captures, named rules.
 */
class PegParser {

public:
	// ========================================================================
	// Construction / Destruction
	// ========================================================================

	PegParser(PegLexer &lexer);
	~PegParser();

	// ========================================================================
	// Parsing API
	// ========================================================================

	/**
	 * @brief Parse the grammar file and store result in grammar.
	 *
	 * Builds Expr trees for each rule, mapped to it's name.
	 */
	void	build(t_ExprDict &grammar);

private:
	PegLexer	&_lex;

	PegParser();
	PegParser(const PegParser &other);
	PegParser &operator=(const PegParser &other);

	// ---- Internal parsing steps ----
	void	parseRule(t_ExprDict &rules);
	Expr	*parseChoice(void);
	Expr	*parseSequence(void);
	Expr	*parsePrefix(void);
	Expr	*parseSuffix(void);
	Expr	*parsePrimary(void);
	Expr	*parseIdentifier(std::string &id, const PegLexer::State &fallback);
	Expr	*parseSubExpr(void);

};

#endif /* __FTPP_PEG_PARSER_HPP__ */