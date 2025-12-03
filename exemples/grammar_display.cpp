/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar_display.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 14:11:38 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/03 15:04:32 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** This define improves LSP/IDE completion for debug symbols.
**
** IMPORTANT:
**   This only affects the example compilation.
**   To actually enable grammar debug features, libftpp.a must be
**   compiled with: -DFTPP_DEBUG_GRAMMAR=1
*/
#define FTPP_DEBUG_GRAMMAR 1

#include <iostream>
#include "Grammar.hpp"

/*
** Display the PEG grammar (all rules defined in .peg) without
** expanding RuleRef, then display the fully expanded version of the
** root rule "config".
**
** This lets users understand:
**   - the declared structure of the grammar
**   - the effective structure after resolving references
**
** Usage:
**   ./grammar_display grammar.peg input.conf
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
		Grammar	grammar(av[1], true);

		// Print user-defined grammar (structure only)
		grammar.printGrammar();

		std::cerr << "\n=========================\n\n";

		// Print fully expanded root rule
		grammar.printRule("config");
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return (0);
}
