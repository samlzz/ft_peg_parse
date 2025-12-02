/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Diag.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:06:54 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 14:54:07 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_DIAG_HPP__
# define __FTPP_DIAG_HPP__

# include <cstddef>
# include <stack>
# include <stdint.h>
# include <string>
# include <vector>

// ============================================================================
// Forward declarations
// ============================================================================
class Input;

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
	struct Checkpoint {
		std::vector<Expectation>	expectations;
		size_t						farthest;

		Checkpoint(const std::vector<Expectation> &exp, size_t far)
			: expectations(exp), farthest(far) {}
	};

	// ---- Attributes ----
	std::vector<Expectation>		_expectations;
	std::stack<Checkpoint>			_checkpoints;
	size_t							_farthest;

	void		add_expectation(const std::string &msg, enum e_priority prio);
	void		deduplicate_expectations(void);
	std::string	format_expectations(void) const;

public:

	// ========================================================================
	// Construction / Assignment
	// ========================================================================

	Diag();
	Diag(const Diag &other);
	Diag &operator=(const Diag &other);

	~Diag();

	// ========================================================================
	// Checkpoint management
	// ========================================================================
	/**
	 * @brief Save current diagnostic state.
	 */
	void		save(void);
	/**
	 * @brief Restore to last checkpoint (discard errors from failed attempt).
	 */
	void		restore(void);
	/**
	 * @brief Commit checkpoint (keep errors, remove save point).
	 */
	void		commit(void);

	// ========================================================================
	// Methods
	// ========================================================================

	// ---- Update and reset ----
	void		update(size_t pos, const std::string &expected, enum e_priority prio);
	void		reset(void);
	
	// ---- Accessors ----
	size_t		farthest(void) const;

	/**
	 * @brief Format the diagnostic message as a human-readable error.
	 * @return Formatted error message with single-line context
	 */
	std::string	formatError(const Input &in, bool withCtx) const;
};

#endif /* __FTPP_DIAG_HPP__ */