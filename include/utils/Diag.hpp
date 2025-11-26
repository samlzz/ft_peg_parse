/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Diag.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:06:54 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/26 17:14:46 by sliziard         ###   ########.fr       */
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

		Expectation(const std::string &msg, enum e_priority prio)
			: message(msg), priority(prio) {}
		
		bool operator<(const Expectation &other) const;
		bool operator==(const Expectation &other) const;
	};

private:

	// ---- Attributes ----
	std::vector<Expectation>		_expectations;
	size_t							_farthest;

	void		add_expectation(const std::string &msg, enum e_priority prio);
	void		deduplicate_expectations(void);
	std::string	format_expectations(void) const;

public:

	// ========================================================================
	// Construction / Assignment
	// ========================================================================

	Diag(): _expectations(), _farthest(0) {}
	Diag(const Diag &other)
		: _expectations(other._expectations),
			_farthest(other._farthest) {}

	Diag &operator=(const Diag &other);

	~Diag() {}

	// ========================================================================
	// Methods
	// ========================================================================

	// ---- Update and reset ----
	void		update(size_t pos, const std::string &expected, enum e_priority prio);
	void		reset(void);
	
	// ---- Accessors ----
	size_t				farthest(void) const	{ return _farthest; }

	/**
	 * @brief Format the diagnostic message as a human-readable error.
	 * @return Formatted error message with single-line context
	 */
	std::string	formatError(const Input &in, bool withCtx) const;
};

#endif

