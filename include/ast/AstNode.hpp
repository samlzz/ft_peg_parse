/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AstNode.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:40:26 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 12:35:49 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASTNODE_HPP
# define ASTNODE_HPP

# include <cstddef>
# include <map>
# include <stdint.h>
# include <string>
# include <vector>

# define PRINT_CALLER

struct Span {
	size_t	start;
	size_t	end;

	Span(): start(0), end(0) {}

	size_t	length(void) const { return (end - start); }
};

class AstNode {

private:
	std::string							_type;
	std::map<std::string, std::string>	_attrs;
	std::vector<AstNode *>				_children;
	Span								span;

public:
	AstNode();
	AstNode(const std::string &type);
	AstNode(const AstNode& other);
	~AstNode();

	AstNode& operator=(const AstNode& other);

	void		addChild(AstNode *child);
	void		setAttr(const std::string &key, const std::string &val);
	std::string	getAttr(const std::string &key) const;
	
	const std::string				&type(void) const;
	const std::vector<AstNode *>	&children(void) const;
};

#endif
