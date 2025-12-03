/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar_stats.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 14:11:38 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/03 15:05:27 by sliziard         ###   ########.fr       */
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
** Display rule-level statistics (usage count, recursion detection,
** min/max length estimation, etc.). This is useful for debugging PEG
** grammars and optimizing packrat performance.
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

		// Raw grammar structure
		grammar.printGrammar();

		// Statistics for root rule "config"
		grammar.printRuleStats("config");
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return (0);
}
