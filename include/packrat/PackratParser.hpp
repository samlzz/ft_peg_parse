/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PackratParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:40:11 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 12:21:52 by sliziard         ###   ########.fr       */
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

public:
	class ParseError : public PegException {
	public:
		ParseError(const std::string& rule)
			: PegException("Parse failed on rule: " + rule)
		{}
	};

	PackratParser(Input &in, const Grammar &pegGrammar);
	~PackratParser();

	bool		parseRule(const std::string &rootRuleName, AstNode *&node);
	bool		eval(const Expr *expr, AstNode *&node);

	const Diag	&diag(void) const;
	void		resetDiag(void);
	void		resetMemo(void);
};

#endif
