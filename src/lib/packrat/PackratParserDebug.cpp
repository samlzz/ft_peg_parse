/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PackratParserDebug.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:14:35 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/19 14:08:59 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/DebugLogger.hpp"

#ifdef PEG_DEBUG_PARSER

# include <iomanip>
# include <sstream>

# include "utils/DebugLogger.hpp"
# include "packrat/PackratParser.hpp"
# include "peg/Expr.hpp"

void	PackratParser::_traceEnter(const Expr* expr, size_t pos)
{
	if (!_traceEnabled)
		return;
	
	std::ostringstream	oss;
	oss << "├─→ " << expr << " @" << pos;
	
	PEG_LOG_TRACE("PARSER", oss.str());
	PegDebug::Logger::indent();
}

void	PackratParser::_traceExit(const Expr* expr, size_t pos, bool success)
{
	if (!_traceEnabled)
		return;
	
	PegDebug::Logger::unindent();
	std::ostringstream	oss;

	oss << "└─" << (success ? "✓" : "✗") << " " << expr << " @" << pos;
	PEG_LOG_TRACE("PARSER", oss.str());
}

PackratParser::Stats 	PackratParser::getStats(void) const
{
	Stats	resp;

	resp.backtrackCount = _backtrackCount;
	resp.cacheHits = _cacheHits;
	resp.totalEvals = _evalCount;
	return resp;
}

void	PackratParser::resetStats(void)
{
	_backtrackCount = 0;
	_cacheHits = 0;
	_evalCount = 0;
}

void	PackratParser::printStats(Stats stats, std::ostream& os)
{
	os << "\n╔═══════════════════════════════════════╗\n";
	os << "║	  PackratParser Statistics		 ║\n";
	os << "╠═══════════════════════════════════════╣\n";
	os << "║ Total evaluations: " << std::setw(18) << stats.totalEvals << " ║\n";
	os << "║ Cache hits:		" << std::setw(18) << stats.cacheHits << " ║\n";
	os << "║ Cache hit rate:	" << std::setw(16) << std::fixed
		<< std::setprecision(1) << (stats.cacheHitRate() * 100) << "% ║\n";
	os << "║ Backtracks:		" << std::setw(18) << stats.backtrackCount << " ║\n";
	os << "╚═══════════════════════════════════════╝\n";
}

#endif
