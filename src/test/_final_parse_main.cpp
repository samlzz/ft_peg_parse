
#include <iostream>

#include "peg/PegParser.hpp"
#include "packrat/PackratParser.hpp"
#include "peg/grammar/Grammar.hpp"
#include "ast/AstNode.hpp"


int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr 
			<< "Usage: ./" 
			<< av[0] 
			<< " <grammar_file_path>.peg <file_to_parse>" 
			<< std::endl;
		return 2;
	}
	try 
	{
		PegParser	parser(av[1]);
		Grammar		grammar;

		parser.parseGrammar(grammar);
		PackratParser	exec(av[2], grammar);
		AstNode			*output = NULL;
		exec.parseRule("config", output);
		std::cout << "Final AST:" << std::endl;
		AstNode::PrintTree(output);
		delete output;
	} catch (const std::exception &e)
	{
		std::cerr << "Exception:" << e.what() << std::endl;
		return 1;
	}
	return (0);
}