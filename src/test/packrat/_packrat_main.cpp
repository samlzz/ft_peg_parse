#include <iostream>
#include "peg/PegParser.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/Grammar.hpp"
#include "ast/AstNode.hpp"
#include "utils/Input.hpp"
#include "helper_TmpFile.hpp"
#include "test.h"

int main() {
	sep("PackratParser - full PEG execution");

	const std::string pegContent =
		"@config <- \"hello\" name:[a-zA-Z]+\n";

	TmpPegFile file(pegContent, "packrat_test.peg");

	Grammar grammar;
	PegParser parser(file.path());
	parser.parseGrammar(grammar);

	Input input = Input::fromText("hello world");
	PackratParser exec(input, grammar);
	AstNode *ast = NULL;

	try {
		bool ok = exec.parseRule("config", ast);
		check(ok, "Root rule parsed successfully");
		check(ast != NULL, "AST node created");
		check(ast->hasAttr("name"), "Captured property set");
		std::cout << "Captured name: " << ast->getAttr("name", "") << std::endl;
	} catch (const std::exception &e) {
		std::cerr << "❌ Exception: " << e.what() << std::endl;
		return 1;
	}
	std::cout << "✅ Packrat full test done\n";
	return 0;
}

