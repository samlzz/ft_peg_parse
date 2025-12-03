/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_parse_stats.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 14:11:38 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/03 15:03:53 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** This define improves LSP/IDE completion for debug symbols.
**
** IMPORTANT:
**   This only affects the example compilation.
**   To actually activate packrat debug, libftpp.a must be
**   compiled with: -DFTPP_DEBUG_PACKRAT=1
**
** When this flag is enabled, the packrat memoization engine records
** per-rule and per-node statistics that allow detailed inspection of:
**   - total parse calls per rule
**   - memoization hit/miss ratio
**   - backtracking behavior
**   - recursion/branch exploration depth
*/
#define FTPP_DEBUG_PACKRAT 1

#define FTPP_DEBUG_PACKRAT 1

#include <iostream>
#include "Grammar.hpp"
#include "AstNode.hpp"

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

		grammar.printPackratStats();

		delete ast;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return (0);
}
