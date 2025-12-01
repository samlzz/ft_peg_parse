/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FtppException.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 18:41:12 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/01 23:32:09 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_EXCEPTION_HPP__
# define __FTPP_EXCEPTION_HPP__

# include <stdexcept>
# include <string>

// ============================================================================
// FtppException (base class)
// ============================================================================

/**
 * @brief Base exception class for all ftpp errors.
 *
 * All exceptions thrown by the library derive from this class so that users may
 * catch FttpException to intercept all error cases, or catch more precise
 * subclasses to handle specific categories of failure.
 */
class FtppException : public std::runtime_error
{
public:
	FtppException(const std::string &msg) : std::runtime_error(msg) {}
	virtual ~FtppException() throw() {}
};

// ============================================================================
// EngineError
// ============================================================================

/**
 * @brief Errors originating from internal components of the library:
 *        - Input (I/O, unexpected EOF)
 *        - AST / structural issues
 *        - Any low-level failure unrelated to the user's grammar or config
 */
class EngineError : public FtppException
{
public:
	EngineError(const std::string &msg): FtppException(msg) {}
};

// ============================================================================
// GrammarError
// ============================================================================

/**
 * @brief Errors in the user's PEG grammar.
 *
 * These occur when loading or resolving the grammar file: unresolved rule refs,
 * invalid syntax, left recursion, malformed expressions, etc.
 */
class GrammarError : public FtppException
{
public:
	GrammarError(const std::string &msg): FtppException("Grammar " + msg) {}
};

/**
 * @brief A reference inside the PEG grammar points to a non-existing rule.
 */
class GrammarResolutionError : public GrammarError
{
public:
	GrammarResolutionError(const std::string &rule)
		: GrammarError("error: Unresolved rule reference: " + rule) {}
};

/**
 * @brief A grammar rule is left-recursive, which PEG cannot accept.
 */
class GrammarLeftRecursionError : public GrammarError
{
public:
	GrammarLeftRecursionError(const std::string &rule)
		: GrammarError("error: Left recursion detected in rule: " + rule) {}
};

// ============================================================================
// ConfigError
// ============================================================================

/**
 * @brief The user's configuration file does not conform to the grammar.
 */
class ConfigError : public FtppException
{
public:
	ConfigError(const std::string &msg)
		: FtppException("Config parse failed: " + msg) {}
};

#endif /* __FTPP_EXCEPTION_HPP__ */

