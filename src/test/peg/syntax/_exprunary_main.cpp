#include <iostream>
#include "peg/syntax/ExprUnary.hpp"
#include "peg/syntax/ExprLeaf.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/grammar/Grammar.hpp"
#include "ast/AstNode.hpp"
#include "utils/Input.hpp"
#include "test.h"

int main() {
	sep("ExprUnary - ZeroOrMore / OneOrMore / Optional / Predicate / Capture");

	// Base grammar + input helpers
	Grammar g;
	AstNode *node = NULL;

	// --------------------------
	// ZeroOrMore: should accept zero or many 'a'
	// --------------------------
	{
		Input in = Input::fromText("aaaa");
		PackratParser parser(in, g);

		Expr *litA = new Literal("a");
		ZeroOrMore zeroOrMore(litA);

		bool ok = zeroOrMore.parse(parser, node);
		check(ok, "ZeroOrMore accepts multiple repetitions");
		check(in.eof(), "ZeroOrMore consumed all input");
	}

	{
		Input in = Input::fromText("");
		PackratParser parser(in, g);

		Expr *litA = new Literal("a");
		ZeroOrMore zeroOrMore(litA);
		bool ok = zeroOrMore.parse(parser, node);
		check(ok, "ZeroOrMore accepts zero occurrences");
	}

	// --------------------------
	// OneOrMore: requires at least one
	// --------------------------
	{
		Input in = Input::fromText("aa");
		PackratParser parser(in, g);
		Expr *litA = new Literal("a");
		OneOrMore oneOrMore(litA);
		bool ok = oneOrMore.parse(parser, node);
		check(ok, "OneOrMore accepts 2 repetitions");
		check(in.eof(), "All consumed");
	}

	{
		Input in = Input::fromText("");
		PackratParser parser(in, g);
		Expr *litA = new Literal("a");
		OneOrMore oneOrMore(litA);
		bool ok = oneOrMore.parse(parser, node);
		check(!ok, "OneOrMore fails on zero repetitions");
	}

	// --------------------------
	// Optional: accepts 0 or 1
	// --------------------------
	{
		Input in = Input::fromText("a");
		PackratParser parser(in, g);
		Expr *litA = new Literal("a");
		Optional opt(litA);
		bool ok = opt.parse(parser, node);
		check(ok, "Optional accepts one match");
		check(in.eof(), "Consumed one token");
	}

	{
		Input in = Input::fromText("");
		PackratParser parser(in, g);
		Expr *litA = new Literal("a");
		Optional opt(litA);
		bool ok = opt.parse(parser, node);
		check(ok, "Optional accepts zero matches");
		check(in.pos() == 0, "Input untouched when no match");
	}

	// --------------------------
	// Predicate AND / NOT
	// --------------------------
	{
		Input in = Input::fromText("a");
		PackratParser parser(in, g);

		Expr *litA = new Literal("a");
		Predicate andPred(litA, true);
		bool ok = andPred.parse(parser, node);
		check(ok, "AND predicate succeeds when match succeeds");
		check(in.pos() == 0, "AND predicate does not consume input");
	}

	{
		Input in = Input::fromText("b");
		PackratParser parser(in, g);

		Expr *litA = new Literal("a");
		Predicate notPred(litA, false);
		bool ok = notPred.parse(parser, node);
		check(ok, "NOT predicate succeeds when inner fails");
		check(in.pos() == 0, "NOT predicate does not consume input");
	}

	// --------------------------
	// Capture (node + property)
	// --------------------------
	{
		Input in = Input::fromText("abc");
		PackratParser parser(in, g);
		AstNode *out = NULL;

		Expr *inner = new Literal("a");
		Capture captureNode(inner, "rule");
		bool ok = captureNode.parse(parser, out);

		check(ok, "Capture node parses correctly");
		check(out != NULL, "AST node created");
		check(out->type() == "rule", "AST node tag correct");
		delete out;
	}

	{
		Input in = Input::fromText("abc");
		PackratParser parser(in, g);
		AstNode *out = new AstNode("parent");

		Expr *inner = new Literal("a");
		Capture captureProp(inner, "val", true);
		bool ok = captureProp.parse(parser, out);
		check(ok, "Capture property parses correctly");
		check(out->hasAttr("val"), "Property stored in parent");
		delete out;
	}

	std::cout << "✅ ExprUnary tests done\n";
	return 0;
}

