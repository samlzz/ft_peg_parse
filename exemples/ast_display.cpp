/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_display.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 14:11:38 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/03 15:04:48 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** This define improves LSP/IDE completion for debug symbols.
**
** IMPORTANT:
**   This only affects the example compilation.
**   To actually enable AST debug features, libftpp.a must be
**   compiled with: -DFTPP_DEBUG_AST=1
*/
#define FTPP_DEBUG_AST 1

#include <iostream>
#include "Grammar.hpp"
#include "AstNode.hpp"

/*
** Example: parse a file using a PEG grammar and print the generated AST.
** 
**    ./parse_and_print_ast mygrammar.peg input.conf
**
** Grammar grammar(grammarPath, true):
**     - grammarPath: file path of the PEG grammar
**     - "true": enable check of left recursion rules
**
** Calling grammar.parseFile(file, rootRule):
**     - Parses the file using the rule "config" (defined in the .peg)
**     - Returns a heap-allocated AST root node
*/

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
		// Load grammar from .peg file
		Grammar	grammar(av[1], true);

		// Parse the input file starting from the rule "config"
		AstNode	*ast = grammar.parseFile(av[2], "config");

		// Pretty print the AST tree (depth-indented)
		AstNode::printTree(ast);

		delete ast;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return (0);
}
