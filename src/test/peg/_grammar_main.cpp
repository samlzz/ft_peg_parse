#include <iostream>
#include "peg/grammar/Grammar.hpp"
#include "peg/syntax/RuleRef.hpp"
#include "test.h"

int main() {
	sep("Grammar - left recursion detection");

	t_ExprDict rules;
	rules["A"] = new RuleRef("A"); // direct recursion

	try {
		Grammar g(rules);
		g.checkLeftRecursion();
		std::cerr << "❌ Left recursion not detected" << std::endl;
		return 1;
	} catch (const Grammar::GrammarLeftRecursionError &e) {
		std::cout << "✅ Left recursion correctly detected\n";
	}
	return 0;
}

