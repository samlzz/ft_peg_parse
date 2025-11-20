/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TokenDebug.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 12:53:15 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/20 14:02:04 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/DebugConfig.hpp"

#if defined(PEG_DEBUG_PEGPARSER) || defined(PEG_DEBUG_GRAMMAR)

# include "utils/DebugLogger.hpp"
# include "peg/PegLexer.hpp"

std::string PegLexer::Token::debugType(void) const
{
	switch (type)
	{
		case T_ID:         return "ID";
		case T_LITERAL:    return "LITERAL";
		case T_CHARRANGE:  return "CHARRANGE";

		case T_SLASH:      return "SLASH";
		case T_STAR:       return "STAR";
		case T_PLUS:       return "PLUS";
		case T_QMARK:      return "QMARK";

		case T_AND:        return "AND";
		case T_NOT:        return "NOT";

		case T_LPAREN:     return "LPAREN";
		case T_RPAREN:     return "RPAREN";

		case T_DOT:        return "DOT";
		case T_COLON:      return "COLON";
		case T_ASSIGN:     return "ASSIGN";

		case T_EOL:        return "EOL";
		case T_END:        return "END";

		default:
			return "UNKNOWN";
	}
}

std::string PegLexer::Token::debugRepr(bool colored) const
{
	using namespace PegDebug;

	std::string typeStr = debugType();

	if (type == T_EOL)
	{
		if (colored)
			return Logger::color(typeStr, COLOR_YELLOW);
		return typeStr;
	}
	std::string valStr = val;

	if (colored)
	{
		typeStr = Logger::color(typeStr, COLOR_YELLOW);
		if (!valStr.empty())
			valStr = Logger::color("\"" + valStr + "\"", COLOR_GREEN);
	}
	else
		valStr = "\"" + valStr + "\"";
	return typeStr + " " + valStr;
}

#endif
