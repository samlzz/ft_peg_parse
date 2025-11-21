/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AstNode.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 19:47:11 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/20 17:01:34 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASTNODE_HPP
# define ASTNODE_HPP

# include <cstddef>
# include <iostream>
# include <map>
# include <stdint.h>
# include <string>
# include <vector>

# include "peg/Expr.hpp"
# include "utils/PegException.hpp"
# include "utils/DebugConfig.hpp"

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
	AstNode(const AstNode& other);
	~AstNode() { deleteAll(_children); }

	AstNode& operator=(const AstNode& other);

	// --- Tree Management ---
	void	addChild(AstNode *child);
	AstNode	*popChild(void);
	void	stealChildren(AstNode &stolen);

	// --- Accessors ---
	const std::string	&type(void) const					{ return _type; }
	void				setType(const std::string &type)	{ _type = type; }

	void				setSpan(size_t start, size_t end);

	// --- Attribute Management ---
	void		setAttr(const std::string &key, const std::string &val);
	bool		hasAttr(const std::string &key) const;
	std::string	getAttr(const std::string &key, const std::string &def) const;

	const std::vector<AstNode *>				&children(void) const	{ return _children; }
	const std::map<std::string, std::string>	&attrs(void) const		{ return _attrs; }

// ---- Debug ----
# if PEG_DEBUG_AST
	struct PrintOptions {
		bool showSpan;
		bool showAttributes;
		bool compactMode;
		int maxDepth;
		std::string indentStr;
		
		PrintOptions() :
			showSpan(true),
			showAttributes(true),
			compactMode(false),
			maxDepth(-1),
			indentStr("  ") {}
	};
	
	void		print(std::ostream& os = std::cerr,
				const PrintOptions& opts = PrintOptions()) const;
	
	static void	PrintTree(const AstNode *root, std::ostream& os = std::cerr,
					const PrintOptions& opts = PrintOptions(), int currentDepth = 0);
	
	size_t		nodeCount(void) const;
	size_t		maxDepth(void) const;

	friend std::ostream& operator<<(std::ostream& os, const AstNode& node);
# endif
};

#endif
