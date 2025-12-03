/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AstNode.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 19:47:11 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/03 14:16:09 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_ASTNODE_HPP__
# define __FTPP_ASTNODE_HPP__

# include <cstddef>
# include <map>
# include <stdint.h>
# include <string>
# include <vector>

# include "config.h"

// Debug only
# if FTPP_DEBUG_AST
#  include <iostream>
# endif

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

	Span();
	Span(const Span &other);

	size_t	length(void) const;
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
	// Construction / Assignment
	// ========================================================================

	AstNode();
	AstNode(const std::string &type);
	AstNode(const AstNode &other);

	~AstNode();

	AstNode &operator=(const AstNode &other);

	// ========================================================================
	// Methods
	// ========================================================================

	// ---- Tree management ----
	void		addChild(AstNode *child);
	AstNode		*popChild(void);
	void		stealChildren(AstNode &stolen);

	// ---- Accessors ----
	const std::string	&type(void) const;
	void				setType(const std::string &type);

	void				setSpan(size_t start, size_t end);

	const std::vector<AstNode *>				&children(void) const;
	const std::map<std::string, std::string>	&attrs(void) const;

	// ---- Attribute management ----
	void		setAttr(const std::string &key, const std::string &val);
	bool		hasAttr(const std::string &key) const;
	std::string	getAttr(const std::string &key, const std::string &def) const;

// ---- Debug functions ---
# if FTPP_DEBUG_AST
	struct PrintOptions {
		bool		showSpan;
		bool		showAttributes;
		bool		compactMode;
		int32_t		maxDepth;
		std::string	indentStr;

		PrintOptions();
	};

	void		print(std::ostream &os = std::cerr, int32_t depth = 0,
					const PrintOptions &opts = PrintOptions()) const;

	static void	printTree(const AstNode *node, std::ostream &os = std::cerr,
					int32_t currentDepth = 0,
					const PrintOptions &opts = PrintOptions());

	size_t		nodeCount(void) const;
	size_t		maxDepth(void) const;
# endif // FTPP_DEBUG_AST
};

# if FTPP_DEBUG_AST
std::ostream &operator<<(std::ostream &os, const AstNode &node);
# endif

#endif /* __FTPP_ASTNODE_HPP__ */