/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Diag.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:06:54 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 11:28:12 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIAG_HPP
# define DIAG_HPP

# include <cstddef>
# include <stdint.h>
# include <string>

# include "utils/Input.hpp"

# define PRINT_CALLER

class Diag {

private:
	size_t		_farthest;
	std::string	_expected;

	Diag(const Diag& other);
	Diag& operator=(const Diag& other);

public:
	Diag();
	~Diag();

	void				update(size_t pos, const std::string &expected);
	void				merge(const Diag &other);
	void				reset(void);

	size_t				farthest(void) const;
	const std::string	&expected(void) const;
	std::string			formatError(const Input& in) const;
};

#endif
