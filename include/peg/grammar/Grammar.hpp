/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Grammar.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:32:37 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/26 10:34:41 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAMMAR_HPP
# define GRAMMAR_HPP

# include <stdint.h>
# include <string>

# include "./Expr.hpp"
# include "utils/DebugConfig.hpp"
# include "utils/PegException.hpp"

# if PEG_DEBUG_GRAMMAR
#  include <iostream>
# endif

// ============================================================================
// Grammar
// ============================================================================

/**
 * @brief Container for PEG rules with reference resolution.
 *
 * Holds an associative table of rule names → Expr trees. Automatically resolves
 * RuleRef nodes on construction or assignment.
 */
class Grammar {

private:
	t_ExprDict	_rules;
	std::string	_start;

	Grammar(const Grammar &other);
	Grammar &operator=(const Grammar &other);

	/**
	 * @brief Resolve all rule references inside all expressions.
	 *
	 * Called automatically by constructors and assignment. Throws
	 * GrammarResolutionError on unresolved rule names.
	 */
	void	resolveRefs(void);

public:

	// ========================================================================
	// Error classes
	// ========================================================================

	class GrammarError : public PegException {
	public:
		GrammarError(const std::string &msg)
			: PegException("Grammar error:" + msg) {}
	};

	class GrammarResolutionError : public GrammarError {
	public:
		GrammarResolutionError(const std::string &rule)
			: GrammarError("Unresolved rule reference: " + rule) {}
	};

	class GrammarLeftRecursionError : public GrammarError {
	public:
		GrammarLeftRecursionError(const std::string &rule)
			: GrammarError("Left recursion detected in rule: " + rule) {}
	};

	// ========================================================================
	// Constructors / Destructor
	// ========================================================================

	Grammar() {}

	/**
	 * @brief Construct from an existing rule table, transferring ownership.
	 */
	Grammar(t_ExprDict &rules);

	// Behave like copy constructor
	Grammar &operator=(Grammar &other);

	~Grammar() { deleteVals(_rules); }

	// ========================================================================
	// Methods
	// ========================================================================

	// ---- Basic operations ----

	void	setFirstRule(const std::string &ruleName) { _start = ruleName; }

	/**
	 * @brief Retrieve a rule expression by name.
	 *
	 * @return Expr* or NULL if not found.
	 */
	Expr	*get(const std::string &name) const;

	/**
	 * @brief Detect left recursion in all rules.
	 *
	 * Throws GrammarLeftRecursionError if a rule directly or indirectly
	 * begins with itself.
	 */
	void	checkLeftRecursion(void) const;

// ---- Debug functions ---
# if PEG_DEBUG_GRAMMAR
public:

	struct PrintOptions {
		bool	showResolved;
		bool	expandRules;
		bool	colored;
		int32_t	maxDepth;

		PrintOptions(): showResolved(false), expandRules(true),
				colored(true), maxDepth(-1) {}
	};


	// Pretty-print the entire grammar.
	void print(std::ostream &os = std::cerr,
				const PrintOptions &opts = PrintOptions()) const;

	// Pretty-print one specific rule.
	void printRule(const std::string &ruleName, std::ostream &os = std::cerr,
					const PrintOptions &opts = PrintOptions()) const;

	// Print statistics for the start rule.
	void printStats(std::ostream &os = std::cerr) const;

# endif // PEG_DEBUG_GRAMMAR

};

#endif

