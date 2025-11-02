/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PackratParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:40:11 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/02 17:38:42 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PACKRATPARSER_HPP
# define PACKRATPARSER_HPP

# include <stdint.h>

# include "PackratCache.hpp"
# include "ast/AstNode.hpp"
# include "peg/Grammar.hpp"
# include "utils/Diag.hpp"
# include "utils/Input.hpp"
# include "utils/PegException.hpp"

# define PRINT_CALLER

class PackratParser {

private:
	Input			&_input;
	const Grammar	&_grammar;
	PackratCache	_memo;
	Diag			_err;

	PackratParser(const PackratParser& other);
	PackratParser& operator=(const PackratParser& other);

	bool	retrieveExpr(const Expr *e, size_t pos, AstNode *&out);

public:
	class ParseError : public PegException {
	public:
		ParseError(const std::string& rule)
			: PegException("Parse failed: " + rule)
		{}
	};

	PackratParser(Input &in, const Grammar &pegGrammar);
	~PackratParser();

	bool		parseRule(const std::string &rootRuleName, AstNode *&out);
	bool		eval(const Expr *expr, AstNode *&out);

	const Diag	&diag(void) const;
	Diag		&diag(void);
	const Input	&input(void) const;
	Input		&input(void);

	void		resetDiag(void);
	void		resetMemo(void);
};

#endif
