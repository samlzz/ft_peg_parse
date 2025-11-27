/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AstNode.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 19:47:11 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/27 17:49:38 by sliziard         ###   ########.fr       */
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

# include "peg/grammar/Expr.hpp"
# include "utils/PegException.hpp"
# include "utils/DebugConfig.hpp"

// ============================================================================
// Span
// ============================================================================

/**
 * @brief Represents a byte range inside the parsed input.
 *
 * Used by AstNode to store matched input bounds.
 */
struct Span {
	size_t	start;
	size_t	end;

	Span(): start(0), end(0) {}
	Span(const Span &other): start(other.start), end(other.end) {}

	size_t	length(void) const { return (end - start); }
};

// ============================================================================
// AstNode
// ============================================================================

/**
 * @brief A node of the PEG Abstract Syntax Tree (AST).
 *
 * Each instance stores a type, optional attributes, children,
 * and the matched input span. Nodes own their children.
 */
class AstNode {

private:
	std::string							_type;
	std::map<std::string, std::string>	_attrs;
	std::vector<AstNode *>				_children;
	Span								_span;

	void	replaceChildren(const std::vector<AstNode *> &src);

public:

	// ========================================================================
	// Error classe
	// ========================================================================

	class AstNodeAttrEmptyKey : public PegException {
	public:
		AstNodeAttrEmptyKey()
			: PegException("AstNode: empty attribute key") {}
	};

	// ========================================================================
	// Construction / Assignment
	// ========================================================================

	AstNode() {}
	AstNode(const std::string &type): _type(type) {}
	AstNode(const AstNode &other);

	~AstNode() { deleteAll(_children); }

	AstNode &operator=(const AstNode &other);

	// ========================================================================
	// Methods
	// ========================================================================

	// ---- Tree management ----
	void		addChild(AstNode *child);
	AstNode		*popChild(void);
	void		stealChildren(AstNode &stolen);

	// ---- Accessors ----
	const std::string	&type(void) const					{ return _type; }
	void				setType(const std::string &type)	{ _type = type; }

	const std::vector<AstNode *>			&children(void) const	{ return _children; }
	const std::map<std::string, std::string>	&attrs(void) const	{ return _attrs; }

	void				setSpan(size_t start, size_t end);

	// ---- Attribute management ----
	void		setAttr(const std::string &key, const std::string &val);
	bool		hasAttr(const std::string &key) const;
	std::string	getAttr(const std::string &key, const std::string &def) const;

// ---- Debug functions ---
# if PEG_DEBUG_AST
	struct PrintOptions {
		bool		showSpan;
		bool		showAttributes;
		bool		compactMode;
		int32_t		maxDepth;
		std::string	indentStr;

		PrintOptions()
			: showSpan(true), showAttributes(true),
			compactMode(false), maxDepth(-1), indentStr("  ") {}
	};

	void		print(std::ostream &os = std::cerr, int32_t depth = 0,
					const PrintOptions &opts = PrintOptions()) const;

	static void	PrintTree(const AstNode *node, std::ostream &os = std::cerr,
					int32_t currentDepth = 0,
					const PrintOptions &opts = PrintOptions());

	size_t		nodeCount(void) const;
	size_t		maxDepth(void) const;

	friend std::ostream &operator<<(std::ostream &os, const AstNode &node);
# endif

};

#endif

