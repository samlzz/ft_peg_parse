/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AstNode.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:40:26 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/02 20:02:33 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASTNODE_HPP
# define ASTNODE_HPP

# include <cstddef>
# include <map>
# include <stdint.h>
# include <string>
# include <vector>

#include "peg/Expr.hpp"
# include "utils/PegException.hpp"

/**
 * @struct Span
 * Represents a text range inside the parsed input.
*/
struct Span {
	size_t	start;
	size_t	end;

	Span(): start(0), end(0) {}
	Span(const Span &other): start(other.start), end(other.end)
	{}

	size_t	length(void) const { return (end - start); }
};

/**
 * @class AstNode
 * 
 * Represents a node in the Abstract Syntax Tree (AST) produced by the PEG parser.
 * Each node corresponds to a grammar rule or capture.
 * 
 * The AST is hierarchical:
 *   - every node has a type (usually the rule name or capture tag)
 *   - it can own multiple child nodes
 *   - it can carry attributes (key/value pairs) for literal or semantic data
 * 
 * This structure is designed to be generic and reusable across different grammars.
*/
class AstNode {

private:
	std::string							_type;
	std::map<std::string, std::string>	_attrs;
	std::vector<AstNode *>				_children;		// < Owned
	Span								_span;			// < Offset positions in input

	void	replaceChildren(const std::vector<AstNode *> &src);

public:

	/**
	 * @class AstNodeAttrEmptyKey
	 * Exception thrown when trying to insert an attribute with an empty key.
	 * Indicates a logical error in grammar or AST construction.
	*/
	class AstNodeAttrEmptyKey : public PegException {
	public:
		AstNodeAttrEmptyKey()
			: PegException("AstNode: empty attribute key") {}
	};

	// --- Construction / Destruction ---
	AstNode() {}
	AstNode(const std::string &type): _type(type) {}
	AstNode(std::vector<AstNode *> &childrens);
	AstNode(const AstNode& other);
	~AstNode() { deleteAll(_children); }

	AstNode& operator=(const AstNode& other);

	// --- Tree Management ---
	void	addChild(AstNode *child);
	void	stealChildren(AstNode &stolen);

	// --- Attribute Management ---
	void		setAttr(const std::string &key, const std::string &val);
	bool		hasAttr(const std::string &key) const;
	std::string	getAttr(const std::string &key, const std::string &def) const;
	
	// --- Accessors ---
	const std::string	&type(void) const					{ return _type; }
	void				setType(const std::string &type)	{ _type = type; }

	void				setSpan(size_t start, size_t end);

	const std::vector<AstNode *>	&children(void) const	{ return _children; }
};

#endif
