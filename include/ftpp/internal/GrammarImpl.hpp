/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GrammarImpl.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 01:05:11 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/03 15:32:44 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_GRAMMAR_IMPL_HPP__
# define __FTPP_GRAMMAR_IMPL_HPP__

# include <string>

# include "AstNode.hpp"
# include "Grammar.hpp"
# include "peg/core/Expr.hpp"
# include "utils/Input.hpp"

# if FTPP_DEBUG_PACKRAT
# include "packrat/PackratParser.hpp"
# endif

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

	// ========================================================================
	// Debug API
	// ========================================================================

# if FTPP_DEBUG_GRAMMAR
	void	debugPrintGrammar(std::ostream &os) const;
	void	debugPrintRule(const std::string &ruleName,
							std::ostream &os) const;
	void	debugPrintRuleStats(const std::string &ruleName,
								std::ostream &os) const;
# endif

# if FTPP_DEBUG_PACKRAT
	void	debugPrintPackratStats(std::ostream &os) const;
# endif

private:

	// --------------------------------------------------------------------
	// Internal fields
	// --------------------------------------------------------------------

	Input							_in;
	t_ExprDict						_rules;

# if FTPP_DEBUG_PACKRAT
	mutable PackratParser::Stats	_lastStats;
# endif

	// ---- Core parse logic ----
	AstNode	*_parseInput(Input &in, const std::string &ruleName) const;

	void	_resolveRefs(void);
	void	_checkLeftRecursion(void)	const;

	Impl();
	Impl(const Impl &);
	Impl &operator=(const Impl &);
};

#endif /* __FTPP_GRAMMAR_IMPL_HPP__ */