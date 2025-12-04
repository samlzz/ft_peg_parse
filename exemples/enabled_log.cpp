/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enabled_log.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 10:12:00 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/04 08:54:28 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Example: enable ftpp internal logging using ft_log.
**
** This shows how to:
**   - configure the global log level
**   - enable ftpp logging categories (lexer, parser, ...)
**   - optionally enable level prefixes and colors
**   - emit user-level log messages
**
** IMPORTANT:
**   Logging is always compiled in ftpp.
**   However, nothing is printed unless:
**     1) the category is enabled (ft_log::enableCategory)
**     2) the level is >= current global level (ft_log::setLevel)
*/

#include "config.h"
#include <iostream>
#include <ft_log/ft_log.hpp>
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
		// -----------------------------------------------------------------
		// 1) Configure ft_log global behavior
		// -----------------------------------------------------------------

		// Print everything from TRACE and above (TRACE, DEBUG, INFO, ...)
		ft_log::setLevel(ft_log::LOG_TRACE);

		// Show the log level prefix (e.g. [INFO], [DEBUG], ...)
		ft_log::setShowLevel(true);

		// Disabled ANSI color output (activate by default)
		// ft_log::setColorEnabled(false);

		// Redirect logs to output stream (writes to std::cerr by default)
		ft_log::setOutputStream(std::cout);

		// -----------------------------------------------------------------
		// 2) Enable ftpp logging categories
		//    (see config.h for the full list of categories)
		// -----------------------------------------------------------------

		ft_log::enableCategory(FTPP_LOG_LEXER);
		ft_log::enableCategory(FTPP_LOG_PARSER);
		// Packrat log can be enabled in the same way:
		// ft_log::enableCategory(FTPP_LOG_PACKRAT);

		// -----------------------------------------------------------------
		// 3) Optional: user-defined logging category
		// -----------------------------------------------------------------

		ft_log::enableCategory("user.example");

		ft_log::log("user.example", ft_log::LOG_INFO)
			<< "[ftpp] starting parsing with logging enabled\n";

		// -----------------------------------------------------------------
		// 4) Use ftpp normally: logs will be emitted by internal code
		// -----------------------------------------------------------------

		// Load grammar from .peg file
		Grammar grammar(av[1], true);

		// Parse the input file starting from the rule "config"
		AstNode *ast = grammar.parseFile(av[2], "config");

		ft_log::log("user.example", ft_log::LOG_DEBUG)
			<< "[ftpp] parse succeeded" << std::endl;

		delete ast;

		ft_log::log("user.example", ft_log::LOG_INFO)
			<< "[ftpp] done." << std::endl;
	}
	catch (const std::exception &e)
	{
		// In case of error, we still log an explicit message
		ft_log::log("user.example", ft_log::LOG_ERROR)
			<< "[ftpp] exception: " << e.what() << "\n";

		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}

