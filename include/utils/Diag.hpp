/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Diag.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:06:54 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/26 16:24:04 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIAG_HPP
# define DIAG_HPP

# include <cstddef>
# include <stdint.h>
# include <string>
# include <vector>

# include "utils/Input.hpp"

// ============================================================================
// Diag
// ============================================================================

/**
 * @brief Diagnostic accumulator for PEG parsing errors.
 *
 * Improved error reporting with:
 *   - Priority levels for expectations
 *   - Deduplication and grouping
 *   - Contextual single-line error display
 *   - Rule name tracking for better context
 */
class Diag {

public:
	// Priority levels for error messages
	enum e_priority {
		PRIO_LOW      = 0,
		PRIO_MEDIUM   = 1,
		PRIO_HIGH     = 2,
		PRIO_CRITICAL = 3
	};

	struct Expectation {
		std::string		message;
		enum e_priority	priority;
		std::string		rule_context;

		Expectation(const std::string &msg, enum e_priority prio,
					const std::string &ctx = "")
			: message(msg), priority(prio), rule_context(ctx) {}
		
		bool operator<(const Expectation &other) const;
		bool operator==(const Expectation &other) const;
	};

private:

	// ---- Attributes ----
	std::vector<Expectation>		_expectations;
	size_t							_farthest;
	std::string						_current_rule;

	void		add_expectation(const std::string &msg, enum e_priority prio);
	void		deduplicate_expectations(void);
	std::string	format_expectations(void) const;

public:

	// ========================================================================
	// Construction / Assignment
	// ========================================================================

	Diag(): _expectations(), _farthest(0), _current_rule() {}
	Diag(const Diag &other)
		: _expectations(other._expectations),
			_farthest(other._farthest),
			_current_rule(other._current_rule) {}

	Diag &operator=(const Diag &other);

	~Diag() {}

	// ========================================================================
	// Methods
	// ========================================================================

	// ---- Update and reset ----
	void		update(size_t pos, const std::string &expected, enum e_priority prio);
	void		reset(void);
	
	// ---- Rule context tracking ----
	void		enter_rule(const std::string &rule_name);
	void		exit_rule(void);


	// ---- Accessors ----
	size_t				farthest(void) const	{ return _farthest; }
	const std::string	&current_rule(void) const { return _current_rule; }

	/**
	 * @brief Format the diagnostic message as a human-readable error.
	 * @return Formatted error message with single-line context
	 */
	std::string	formatError(const Input &in, bool withCtx) const;
};

#endif

