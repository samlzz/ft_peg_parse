#include <iostream>
#include "peg/syntax/ExprLeaf.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/Grammar.hpp"
#include "test.h"

int main() {
	sep("ExprLeaf - Literal and CharRange");

	Input in1 = Input::fromText("hello");
	Grammar g;
	PackratParser parser(in1, g);

	AstNode *node = NULL;
	Literal lit("hello");
	check(lit.parse(parser, node) == true, "Literal matches exact string");

	Input in2 = Input::fromText("a");
	PackratParser parser2(in2, g);
	CharRange range("abc");
	check(range.parse(parser2, node) == true, "CharRange matches allowed char");

	Input in3 = Input::fromText("z");
	PackratParser parser3(in3, g);
	check(range.parse(parser3, node) == false, "CharRange fails on invalid char");

	std::cout << "✅ ExprLeaf tests done\n";
	return 0;
}

