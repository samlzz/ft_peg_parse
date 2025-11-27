/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PackratParserDebug.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:14:35 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/27 18:27:28 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/DebugConfig.hpp"

#if PEG_DEBUG_PACKRAT

# include <iomanip>
# include <sstream>

# include "utils/DebugLogger.hpp"
# include "utils/Input.hpp"
# include "packrat/PackratParser.hpp"
# include "peg/grammar/Expr.hpp"

// ============================================================================
// Construction
// ============================================================================


PackratParser::PackratParser(const std::string &path, const Grammar &pegGrammar,
								bool checkLeftRecursion)
	: _input(Input::fromFile(path)), _grammar(pegGrammar), _err()
		, _traceDepth(0), _traceEnabled(true)
		, _evalCount(0), _cacheHits(0), _backtrackCount(0)
{
	PegDebug::Logger::resetIndent();
	PegDebug::Logger::setIndentValue("│ ");
	if (checkLeftRecursion)
		_grammar.checkLeftRecursion();
}

// ============================================================================
// Helpers
// ============================================================================

static inline std::string _getPos(const Input &in)
{
	std::ostringstream oss;
	oss << "[Ln " << in.line() << ", Col " << in.column() << "]";
	return oss.str();
}

// ============================================================================
// Trace entry
// ============================================================================

void	PackratParser::_traceEnter(const Expr *expr, const Input &in)
{
	if (!_traceEnabled)
		return;

	std::ostringstream oss;

	oss << "├─"
		<< PegDebug::Logger::color("→ ", COLOR_CYAN)
		<< *expr << " "
		<< PegDebug::Logger::color(_getPos(in), COLOR_BLUE);

	PegDebug::Logger::log(PegDebug::LOG_TRACE, "PackratParser", oss.str());
	PegDebug::Logger::indent();
}

// ============================================================================
// Trace exit
// ============================================================================

void	PackratParser::_traceExit(const Expr *expr, const Input &in, bool success)
{
	if (!_traceEnabled)
		return;

	PegDebug::Logger::unindent();
	std::ostringstream oss;

	oss << "└─";
	if (success)
		oss << PegDebug::Logger::color("✓", COLOR_GREEN);
	else
		oss << PegDebug::Logger::color("✗", COLOR_RED);

	oss << " " << *expr
		<< PegDebug::Logger::color(_getPos(in), COLOR_BLUE);

	PegDebug::Logger::log(PegDebug::LOG_TRACE, "PackratParser", oss.str());
}

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

