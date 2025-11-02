/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Grammar.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:32:37 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/02 17:22:41 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAMMAR_HPP
# define GRAMMAR_HPP

# include <stdint.h>
# include <string>

# include "./Expr.hpp"
# include "utils/PegException.hpp"

# define PRINT_CALLER

class Grammar {

private:
	t_ExprDict	_rules;
	std::string	_start;

	Grammar(const Grammar& other);
	Grammar& operator=(const Grammar& other);

	// ? call by constructor
	void	resolveRefs(void);

public:
	class GrammarError: public PegException {
	public:
		GrammarError(const std::string &msg)
			: PegException(msg)
		{}
	};

	class GrammarResolutionError : public GrammarError {
	public:
		GrammarResolutionError(const std::string& rule)
			: GrammarError("Unresolved rule reference: " + rule)
		{}
	};
	class GrammarLeftRecursionError : public GrammarError {
	public:
		GrammarLeftRecursionError(const std::string& rule)
			: GrammarError("Left recursion detected in rule: " + rule)
		{}
	};

	// Consume rules
	Grammar(t_ExprDict &rules);
	~Grammar();

	Expr	*get(const std::string &name) const;
	void	checkLeftRecursion(void) const;
};

#endif
