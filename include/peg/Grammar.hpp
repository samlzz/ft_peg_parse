/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Grammar.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:32:37 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/02 20:31:47 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAMMAR_HPP
# define GRAMMAR_HPP

# include <stdint.h>
# include <string>

# include "./Expr.hpp"
# include "utils/PegException.hpp"

class Grammar {

private:
	t_ExprDict	_rules;
	std::string	_start;

	// ? call by constructor
	void	resolveRefs(void);

public:

	class GrammarError: public PegException
	{
	public:
		GrammarError(const std::string &msg)
			: PegException("Grammar error:" + msg) {}
	};

	class GrammarResolutionError: public GrammarError
	{
	public:
		GrammarResolutionError(const std::string& rule)
			: GrammarError("Unresolved rule reference: " + rule) {}
	};

	class GrammarLeftRecursionError: public GrammarError
	{
	public:
		GrammarLeftRecursionError(const std::string& rule)
			: GrammarError("Left recursion detected in rule: " + rule) {}
	};


	// Consume rules or other._rules
	Grammar(t_ExprDict &rules);
	Grammar(Grammar& other) { *this = other; }
	Grammar& operator=(Grammar& other);

	~Grammar() { deleteVals(_rules); }

	void	setFirstRule(const std::string &ruleName) { _start = ruleName; }

	Expr	*get(const std::string &name)	const;
	void	checkLeftRecursion(void)		const;
};

#endif
