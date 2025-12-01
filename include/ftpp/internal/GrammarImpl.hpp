/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GrammarImpl.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 01:05:11 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/01 22:53:47 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_GRAMMAR_IMPL_HPP__
# define __FTPP_GRAMMAR_IMPL_HPP__

# include <string>

# include "AstNode.hpp"
# include "Grammar.hpp"
# include "peg/core/Expr.hpp"
# include "utils/Input.hpp"

// ============================================================================
// GrammarImpl (internal engine)
// ============================================================================

class Grammar::Impl {

public:

	Impl(const std::string &path, bool checkLeftRecursion);
	~Impl();

	// --------------------------------------------------------------------
	// Parsing API
	// --------------------------------------------------------------------

	AstNode				*parseFile(const std::string &path) const;
	AstNode				*parseFile(const std::string &path,
									const std::string &ruleName) const;

	AstNode				*parseString(const std::string &text) const;
	AstNode				*parseString(const std::string &text,
									const std::string &ruleName) const;

	const std::string	&rootRule(void) const;

private:

	// --------------------------------------------------------------------
	// Internal fields
	// --------------------------------------------------------------------

	Input		_in;
	t_ExprDict	_rules;

	// ---- Core parse logic ----
	AstNode	*_parseInput(Input &in, const std::string &ruleName) const;

	void	_resolveRefs(void);
	void	_checkLeftRecursion(void)	const;

	Impl();
	Impl(const Impl &);
	Impl &operator=(const Impl &);
};

#endif /* __FTPP_GRAMMAR_IMPL_HPP__ */