/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Diag.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:06:54 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 17:51:06 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIAG_HPP
# define DIAG_HPP

# include <cstddef>
# include <stdint.h>
# include <string>

# include "utils/Input.hpp"

/**
 * Diagnostic helper used by the Packrat parser to track and report parsing errors.
 * 
 * A Diag instance stores two information:
 *   - the farthest position reached in the input before a parsing failure occurred
 *   - the expected token(s) or rule(s) at that position
*/
class Diag {

private:
	std::string	_expected;
	size_t		_farthest;

	Diag(const Diag& other);
	Diag& operator=(const Diag& other);

	void	concat_expectation(const std::string &same_far_expct);

public:
	// --- Construction ---
	Diag();
	~Diag();

	// --- Update & combination ---
	void				update(size_t pos, const std::string &expected);
	void				merge(const Diag &other);
	void				reset(void);

	// --- Accessors ---
	size_t				farthest(void) const;
	const std::string	&expected(void) const;
	std::string			formatError(const Input& in, bool with_ctx) const;
};

#endif
