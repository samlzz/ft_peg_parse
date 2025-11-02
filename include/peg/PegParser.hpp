/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PegParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:32:07 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/02 17:22:41 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PEGPARSER_HPP
# define PEGPARSER_HPP

# include <stdint.h>

# include "./PegLexer.hpp"
# include "./Expr.hpp"
# include "peg/Grammar.hpp"
# include "utils/Diag.hpp"

# define PRINT_CALLER

class PegParser {

private:
	PegLexer						_lex;
	std::vector<PegLexer::Token>	_tokens;
	t_ExprDict						_rules;
	Diag							_err;

	PegParser(const PegParser& other);
	PegParser& operator=(const PegParser& other);

	Expr	*parseRule(void);
	Expr	*parseExpression(void);
	Expr	*parseSequence(void);
	Expr	*parsePrefix(void);
	Expr	*parsePrimary(void);

public:
	PegParser(const std::string &grammar_path);
	~PegParser();

	Grammar	parseGrammar(void);
};

#endif
