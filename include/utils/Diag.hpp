/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Diag.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:06:54 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/25 20:45:05 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIAG_HPP
# define DIAG_HPP

# include <cstddef>
# include <stdint.h>
# include <string>

# include "utils/Input.hpp"

// ============================================================================
// Diag
// ============================================================================

/**
 * @brief Diagnostic accumulator for PEG parsing errors.
 *
 * Stores:
 *   - the farthest input position reached before failing
 *   - the expected token or rule at that position.
 *
 * Used by PackratParser to build precise error messages.
 */
class Diag {

private:
	std::string	_expected;
	size_t		_farthest;

	void	concat_expectation(const std::string &same_far_expct);

public:

	// ========================================================================
	// Construction / Assignment
	// ========================================================================

	Diag(): _expected(), _farthest(0) {}
	Diag(const Diag &other): _expected(other._expected), _farthest(other._farthest) {}

	Diag &operator=(const Diag &other);

	~Diag() {}

	// ========================================================================
	// Methods
	// ========================================================================

	// ---- Update and merge ----
	void		update(size_t pos, const std::string &expected);
	void		merge(const Diag &other);
	void		reset(void);

	// ---- Accessors ----
	size_t				farthest(void) const	{ return _farthest; }
	const std::string	&expected(void) const	{ return _expected; }

	/**
	 * @brief Format the diagnostic message as a human-readable error.
	 */
	std::string			formatError(const Input &in, bool with_ctx) const;
};

#endif

