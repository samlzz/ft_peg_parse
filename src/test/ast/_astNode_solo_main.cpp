/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _ast_node_main.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 19:12:13 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/04 09:50:16 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cassert>

#include "test.h"
#include "ast/AstNode.hpp"

/**
 * AstNode Tests:
 * - constructors
 * - child management
 * - attributes (set/get/has)
 * - copy and assignment
 * - exception handling
 */
int main(void)
{
	// --------------------------------------------------------------------
	sep("1. Constructors and type()");
	{
		AstNode empty;
		check(empty.type().empty(), "Default constructor creates empty type");

		AstNode typed("server_block");
		check(typed.type() == "server_block", "Type constructor sets type correctly");

		typed.setType("location_block");
		check(typed.type() == "location_block", "setType() updates node type");
	}

	// --------------------------------------------------------------------
	sep("2. Child management");
	{
		AstNode parent("parent");
		AstNode *child1 = new AstNode("child1");
		AstNode *child2 = new AstNode("child2");

		parent.addChild(child1);
		parent.addChild(child2);

		const std::vector<AstNode*>& children = parent.children();
		check(children.size() == 2, "Two children added successfully");
		check(children[0]->type() == "child1", "First child has correct type");
		check(children[1]->type() == "child2", "Second child has correct type");
	}

	// --------------------------------------------------------------------
	sep("3. Attribute management");
	{
		AstNode node("directive");

		node.setAttr("key", "value");
		check(node.hasAttr("key"), "hasAttr() detects existing key");
		check(node.getAttr("key", "default") == "value", "getAttr() retrieves correct value");
		check(node.getAttr("missing", "default") == "default", "getAttr() returns default for missing key");

		node.setAttr("another", "42");
		check(node.hasAttr("another"), "setAttr() can add multiple attributes");
	}

	// --------------------------------------------------------------------
	sep("4. Exception on empty key");
	{
		AstNode node("error_test");
		bool caught = false;
		try {
			node.setAttr("", "oops");
		} catch (const AstNode::AstNodeAttrEmptyKey &e) {
			caught = true;
			std::cout << "Caught exception: " << e.what() << std::endl;
		}
		check(caught, "AstNodeAttrEmptyKey thrown on empty key");
	}

	// --------------------------------------------------------------------
	sep("5. Copy constructor and assignment");
	{
		AstNode a("original");
		a.setAttr("name", "root");
		a.addChild(new AstNode("childA"));
		a.addChild(new AstNode("childB"));

		AstNode b = a; // copy constructor
		check(b.type() == "original", "Copy constructor copies type");
		check(b.hasAttr("name"), "Copy constructor copies attributes");
		check(b.children().size() == 2, "Copy constructor copies child pointers");

		AstNode c("temp");
		c = a; // assignment operator
		check(c.type() == "original", "Assignment operator copies type");
		check(c.hasAttr("name"), "Assignment operator copies attributes");
		check(c.children().size() == 2, "Assignment operator copies child pointers");
	}

	// --------------------------------------------------------------------
	sep("6. Span structure");
	{
		Span s;
		check(s.length() == 0, "Default Span length = 0");

		Span s2;
		s2.start = 5;
		s2.end = 10;
		check(s2.length() == 5, "Span length() returns correct difference");

		Span s3(s2);
		check(s3.start == 5 && s3.end == 10, "Span copy constructor works");
	}

	std::cout << "\n🎉 All AstNode tests passed successfully!\n" << std::endl;
	return 0;
}

