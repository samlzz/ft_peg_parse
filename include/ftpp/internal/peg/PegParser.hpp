/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PegParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:32:07 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/01 23:19:37 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PEG_PARSER_HPP
# define PEG_PARSER_HPP

# include <stdint.h>

# include "./PegLexer.hpp"
# include "peg/core/Expr.hpp"
# include "Grammar.hpp"

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

private:
	PegLexer	_lex;
	t_ExprDict	_rules;

	PegParser(const PegParser &other);
	PegParser &operator=(const PegParser &other);

	// ---- Internal parsing steps ----
	void	parseRule(void);
	Expr	*parseChoice(void);
	Expr	*parseSequence(void);
	Expr	*parsePrefix(void);
	Expr	*parseSuffix(void);
	Expr	*parsePrimary(void);
	Expr	*parseIdentifier(std::string &id);
	Expr	*parseSubExpr(void);

public:
	// ========================================================================
	// Construction
	// ========================================================================

	PegParser(const std::string &grammar_path);
	~PegParser() { deleteVals(_rules); }

	// ========================================================================
	// Parsing API
	// ========================================================================

	/**
	 * @brief Parse the grammar file and store result in out.
	 *
	 * Builds Expr trees for each rule and outputs a resolved Grammar.
	 */
	void	parseGrammar(Grammar &out);
};

#endif
