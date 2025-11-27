#include <iostream>
#include "peg/syntax/ExprContainer.hpp"
#include "peg/syntax/ExprUnary.hpp"
#include "peg/syntax/ExprLeaf.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/grammar/Grammar.hpp""
#include "test.h"

int main() {
	sep("ExprContainer / ExprUnary - sequence, choice, repetition");

	Input in = Input::fromText("aaab");
	Grammar g;
	PackratParser parser(in, g);
	AstNode *node = NULL;

	Expr *a = new Literal("a");
	Expr *b = new Literal("b");

	ZeroOrMore* repA = new ZeroOrMore(a);
	Sequence seq;
	seq.add(repA);
	seq.add(b);

	check(seq.parse(parser, node), "Sequence + ZeroOrMore parses 'aaab'");

	Input in2 = Input::fromText("x");
	PackratParser parser2(in2, g);
	Choice choice;
	choice.add(new Literal("a"));
	choice.add(new Literal("x"));
	check(choice.parse(parser2, node), "Choice parses one of alternatives");

	std::cout << "✅ ExprContainer tests done\n";
	return 0;
}

