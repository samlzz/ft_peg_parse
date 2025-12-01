/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Grammar.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:32:37 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 00:36:28 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_GRAMMAR_HPP__
# define __FTPP_GRAMMAR_HPP__

# include <string>
# include <stdint.h>

# include "AstNode.hpp"

// ============================================================================
// Grammar (Public API)
// ============================================================================

/**
 * @brief Public interface for ftpp grammars.
 *
 * Loads a PEG grammar from a source file, builds the internal
 * representation, resolves rule references, and provides parsing
 * functions for user configuration files or strings.
 */
class Grammar {

public:
	// ========================================================================
	// Construction / Destruction
	// ========================================================================

	explicit Grammar(
		const std::string &grammarPath, bool checkLeftRecursion = true
	);
	~Grammar();

	// ========================================================================
	// Parsing API
	// ========================================================================

	AstNode	*parseFile(const std::string &path) const;
	AstNode	*parseFile(const std::string &path,
						const std::string &ruleName) const;

	AstNode	*parseString(const std::string &input) const;
	AstNode	*parseString(const std::string &input,
						const std::string &ruleName) const;

	// ========================================================================
	// Debug API
	// ========================================================================

# if FTPP_DEBUG_GRAMMAR
	/**
	 * @brief Print all rules in the grammar.
	 */
	void	printGrammar(std::ostream &os = std::cerr) const;
	/**
	 * @brief Print a specific rule's expression tree.
	 */
	void	printRule(const std::string &ruleName,
					std::ostream &os = std::cerr) const;

	/**
	 * @brief Print statistics for a specific rule.
	 */
	void	printRuleStats(const std::string &ruleName,
					std::ostream &os = std::cerr) const;
# endif

# if FTPP_DEBUG_PACKRAT
	/**
	 * @brief Print statistics collected from the latest Packrat parse.
	 */
	void	printPackratStats(std::ostream &os = std::cerr) const;
# endif

private:

	// ========================================================================
	// PIMPL
	// ========================================================================
	class Impl;
	Impl	*_impl;

	Grammar();
	Grammar(const Grammar &other);
	Grammar &operator=(const Grammar &other);
};

#endif /* __FTPP_GRAMMAR_HPP__ */