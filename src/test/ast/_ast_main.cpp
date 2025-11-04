#include <iostream>
#include "ast/AstNode.hpp"
#include "test.h"

int main() {
	sep("AstNode - basic tree and attributes");

	AstNode *root = new AstNode("root");
	AstNode *child1 = new AstNode("child1");
	AstNode *child2 = new AstNode("child2");

	root->addChild(child1);
	root->addChild(child2);
	root->setAttr("name", "server");

	check(root->children().size() == 2, "root has two children");
	check(root->hasAttr("name"), "attribute exists");
	check(root->getAttr("name", "") == "server", "attribute value correct");

	AstNode copy(*root);
	check(copy.children().size() == 2, "copy constructor deep-copies children");
	check(copy.getAttr("name", "") == "server", "attributes copied correctly");

	delete root;
	std::cout << "✅ AstNode tests done\n";
	return 0;
}

