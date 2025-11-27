/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _expr_classes_main.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 00:30:11 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/27 17:53:41 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "ast/AstNode.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/grammar/Grammar.hpp""
#include "peg/syntax/ExprLeaf.hpp"
#include "peg/syntax/ExprContainer.hpp"
#include "peg/syntax/ExprUnary.hpp"
#include "peg/syntax/RuleRef.hpp"
#include "utils/Input.hpp"
#include "test.h"

int main(void)
{
	try {
		/* ========================== BASIC TERMINALS ========================== */
		sep("Literal");
		{
			Input in = Input::fromText("abc");
			Grammar g;
			PackratParser parser(in, g);
			Literal lit("ab");

			AstNode *n = NULL;
			bool ok = lit.parse(parser, n);
			check(ok && parser.input().pos() == 2, "Literal 'ab' matched correctly");
			check(!parser.input().match("ab"), "Input advanced correctly");
		}

		sep("CharRange");
		{
			Input in = Input::fromText("z");
			Grammar g;
			PackratParser parser(in, g);
			CharRange range("abcxyz");

			AstNode *n = NULL;
			bool ok = range.parse(parser, n);
			check(ok && parser.input().pos() == 1, "CharRange matched single char");
		}

		/* ========================== SEQUENCE / CHOICE ======================== */
		sep("Sequence");
		{
			Input in = Input::fromText("abc");
			Grammar g;
			PackratParser parser(in, g);

			t_ExprList seqElems;
			seqElems.push_back(new Literal("a"));
			seqElems.push_back(new Literal("b"));
			Sequence seq(seqElems);

			AstNode *n = NULL;
			bool ok = seq.parse(parser, n);
			check(ok && parser.input().pos() == 2, "Sequence 'a' 'b' matched");
			delete n;
		}

		sep("Choice");
		{
			Input in = Input::fromText("dog");
			Grammar g;
			PackratParser parser(in, g);

			t_ExprList opts;
			opts.push_back(new Literal("cat"));
			opts.push_back(new Literal("dog"));
			opts.push_back(new Literal("mouse"));
			Choice ch(opts);

			AstNode *n = NULL;
			bool ok = ch.parse(parser, n);
			check(ok && parser.input().pos() == 3, "Choice matched correct alternative");
			delete n;
		}

		/* ======================= QUANTIFIERS (*, +, ?) ======================= */
		sep("ZeroOrMore");
		{
			Input in = Input::fromText("aaaX");
			Grammar g;
			PackratParser parser(in, g);
			ZeroOrMore zm(new Literal("a"));

			AstNode *n = NULL;
			bool ok = zm.parse(parser, n);
			check(ok && parser.input().pos() == 3, "ZeroOrMore consumed all 'a'");
			delete n;
		}

		sep("OneOrMore");
		{
			Input in = Input::fromText("aaaaaX");
			Grammar g;
			PackratParser parser(in, g);
			OneOrMore om(new Literal("a"));

			AstNode *n = NULL;
			bool ok = om.parse(parser, n);
			std::cout << "Now pos: " << parser.input().pos() << " | ok: " << ok << std::endl;
			check(ok && parser.input().pos() == 5, "OneOrMore consumed all 'a'");
			delete n;
		}

		sep("Optional");
		{
			Input in = Input::fromText("abc");
			Grammar g;
			PackratParser parser(in, g);
			Optional opt(new Literal("ab"));

			AstNode *n = NULL;
			bool ok = opt.parse(parser, n);
			check(ok && parser.input().pos() == 2, "Optional matched literal");
			delete n;

			// case where optional fails
			Input in2 = Input::fromText("xyz");
			PackratParser parser2(in2, g);
			Optional opt2(new Literal("ab"));
			AstNode *n2 = NULL;
			ok = opt2.parse(parser2, n2);
			check(ok && in2.pos() == 0 && n2 == NULL, "Optional gracefully failed");
		}

		/* ============================= PREDICATES ============================ */
		sep("Predicate & and !");
		{
			Input in = Input::fromText("abc");
			Grammar g;
			PackratParser parser(in, g);

			Predicate andPred(new Literal("a"), true);
			Predicate notPred(new Literal("x"), false);

			AstNode *n = NULL;
			bool ok1 = andPred.parse(parser, n);
			check(ok1 && parser.input().pos() == 0, "And-predicate succeeded without consuming");

			bool ok2 = notPred.parse(parser, n);
			check(ok2 && parser.input().pos() == 0, "Not-predicate succeeded on mismatch");
		}

		/* ============================== CAPTURE ============================== */
		sep("Capture");
		{
			Input in = Input::fromText("abc");
			Grammar g;
			PackratParser parser(in, g);

			Capture cap(new Literal("ab"), "pair");
			AstNode *n = NULL;
			bool ok = cap.parse(parser, n);
			check(ok && n != NULL && n->type() == "pair", "Capture created node 'pair'");
			delete n;
		}

		/* ============================== RULE REF ============================= */
		sep("RuleRef");
		{
			Input in = Input::fromText("hello");
			Grammar g;
			PackratParser parser(in, g);

			Literal *hello = new Literal("hello");
			RuleRef rule("hello_rule");
			rule.resolve(hello);

			AstNode *n = NULL;
			bool ok = rule.parse(parser, n);
			check(ok && parser.input().pos() == 5, "RuleRef correctly delegated to Literal");
			delete n;
			delete hello;
		}

		std::cout << "\n🎉 All PEG expression tests passed successfully!" << std::endl;
	}
	catch (const std::exception &e) {
		std::cerr << "❌ Exception: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}

