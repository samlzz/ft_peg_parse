#include <iostream>
#include "helper_TmpFile.hpp"
#include "peg/PegLexer.hpp"
#include "test.h"

int main() {
	sep("PegLexer - tokenize PEG grammar");

	const std::string pegContent =
		"@rule <- \"hello\" / \"hi\"\n"
		"expr <- word:(\"a\" / \"b\")+\n";
	TmpPegFile file(pegContent, "lexer_test.peg");

	PegLexer lex(file.path());
	PegLexer::Token tk;
	int count = 0;
	while ((tk = lex.next()).type != PegLexer::T_END) {
		std::cout << tk.val << " ";
		count++;
	}
	std::cout << std::endl;

	check(count > 0, "Tokens read successfully");
	std::cout << "✅ PegLexer tests done\n";
	return 0;
}

