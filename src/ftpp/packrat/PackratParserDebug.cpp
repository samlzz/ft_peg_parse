/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PackratParserDebug.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:14:35 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/28 21:48:30 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DebugConfig.hpp"

#if PEG_DEBUG_PACKRAT

# include <iomanip>

# include "PackratParser.hpp"
# include "peg/core/Expr.hpp"

// ============================================================================
// Statistics
// ============================================================================

PackratParser::Stats	PackratParser::getStats(void) const
{
	Stats s;

	s.totalEvals = _evalCount;
	s.cacheHits = _cacheHits;
	s.backtrackCount = _backtrackCount;
	return s;
}

void	PackratParser::resetStats(void)
{
	_evalCount = 0;
	_cacheHits = 0;
	_backtrackCount = 0;
}

void	PackratParser::printStats(Stats stats, std::ostream &os)
{
	os << "\n╔═══════════════════════════════════════╗\n";
	os <<   "║         PackratParser Statistics      ║\n";
	os <<   "╠═══════════════════════════════════════╣\n";
	os <<   "║ Total evaluations: " << std::setw(18) << stats.totalEvals     << " ║\n";
	os <<   "║ Cache hits:        " << std::setw(18) << stats.cacheHits      << " ║\n";
	os <<   "║ Cache hit rate:    " << std::setw(16) << std::fixed
		 << std::setprecision(1) << (stats.cacheHitRate() * 100) << "% ║\n";
	os <<   "║ Backtracks:        " << std::setw(18) << stats.backtrackCount << " ║\n";
	os <<   "╚═══════════════════════════════════════╝\n";
}

#endif

