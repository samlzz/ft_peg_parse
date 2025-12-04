/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PackratParserDebug.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:14:35 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/04 08:31:18 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

#if FTPP_DEBUG_PACKRAT

# include <iomanip>
# include <ostream>

# include "packrat/PackratParser.hpp"
# include "peg/core/Expr.hpp"

// ============================================================================
// Stats struct
// ============================================================================

double	PackratParser::Stats::cacheHitRate(void) const
{
	if (totalEvals > 0)
		return (double)cacheHits / totalEvals;
	return 0.0;
}

// ============================================================================
// PackratParser Statistics
// ============================================================================

PackratParser::Stats	PackratParser::getStats(void) const
{
	Stats s;

	s.totalEvals = _evalCount;
	s.cacheHits = _cacheHits;
	s.backtrackCount = _backtrackCount;
	return s;
}

void	PackratParser::printStats(Stats stats, std::ostream &os)
{
	os << "\n╔═══════════════════════════════════════╗\n";
	os <<   "║         PackratParser Statistics      ║\n";
	os <<   "╠═══════════════════════════════════════╣\n";
	os <<   "║ Total evaluations: " << std::setw(18) << stats.totalEvals     << " ║\n";
	os <<   "║ Cache hits:        " << std::setw(18) << stats.cacheHits      << " ║\n";
	os <<   "║ Cache hit rate:    " << std::setw(17) << std::fixed
		 << std::setprecision(1) << (stats.cacheHitRate() * 100) << "% ║\n";
	os <<   "║ Backtracks:        " << std::setw(18) << stats.backtrackCount << " ║\n";
	os <<   "╚═══════════════════════════════════════╝\n";
}

#endif

