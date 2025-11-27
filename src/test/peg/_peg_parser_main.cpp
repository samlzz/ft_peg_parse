#include <iostream>
#include "helper_TmpFile.hpp"
#include "peg/PegParser.hpp"
#include "peg/grammar/Grammar.hpp"
#include "test.h"

int main() {
	sep("PegParser + Grammar");

	const std::string pegContent =
		"@rule <- \"hello\" / \"hi\"\n"
		"expr <- word:(\"a\" / \"b\")+\n";

	TmpPegFile file(pegContent, "parser_test.peg");
	Grammar grammar;

	try {
		PegParser parser(file.path());
		parser.parseGrammar(grammar);
		check(grammar.get("rule") != NULL, "rule resolved in grammar");
		check(grammar.get("expr") != NULL, "expr resolved in grammar");
		std::cout << "✅ Grammar built successfully\n";
	} catch (const std::exception &e) {
		std::cerr << "❌ Exception: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}

