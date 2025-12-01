/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Grammar.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:32:37 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/01 22:53:31 by sliziard         ###   ########.fr       */
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

	explicit Grammar(const std::string &grammarPath, bool checkLeftRecursion = true);
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