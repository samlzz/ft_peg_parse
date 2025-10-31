/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Input.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:34:40 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 12:28:29 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_HPP
# define INPUT_HPP

# include <cstddef>
# include <stdint.h>
# include <string>

# define PRINT_CALLER

class Input {

private:
	std::string	_data;
	size_t		_pos;

public:
	Input();
	Input(const std::string &text);
	Input(const char *file_path);
	Input(const Input& other);
	~Input();

	Input& operator=(const Input& other);

	bool		match(const std::string &literal);
	char		get(void);
	char		peek(void) const;
	bool		eof(void) const;

	size_t		pos(void) const;
	void		setPos(size_t newPos);

	std::string	substr(size_t start, size_t end) const;

	size_t		remaining(void) const;
	size_t		line(void) const;
	size_t		column(void) const;
};

#endif
