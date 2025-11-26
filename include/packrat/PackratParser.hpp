/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PackratParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:40:11 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/26 16:20:36 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PACKRATPARSER_HPP
# define PACKRATPARSER_HPP

# include <stdint.h>

# include "ast/AstNode.hpp"
# include "peg/Grammar.hpp"
# include "utils/DebugConfig.hpp"
# include "utils/Diag.hpp"
# include "utils/Input.hpp"
# include "utils/PegException.hpp"

# if PEG_DEBUG_PACKRAT
#  include "utils/DebugLogger.hpp"
# endif

// ============================================================================
// PackratParser
// ============================================================================

/**
 * @brief Packrat PEG parser with memoization.
 *
 * Evaluates grammar expressions using a classic packrat strategy:
 *   - recursive descent
 *   - memo table for linear-time guarantees
 *   - optional debug tracing
 */
class PackratParser {

private:
	Input			_input;
	const Grammar	&_grammar;
	Diag			_err;

	PackratParser(const PackratParser &other);
	PackratParser &operator=(const PackratParser &other);

	bool	retrieveExpr(const Expr *e, size_t pos, AstNode *parent);

	// ---- Debug attributes ----
# if PEG_DEBUG_PACKRAT
	int32_t	_traceDepth;
	bool	_traceEnabled;

	size_t	_evalCount;
	size_t	_cacheHits;
	size_t	_backtrackCount;

	void	_traceEnter(const Expr *expr, const Input &in);
	void	_traceExit(const Expr *expr, const Input &in, bool success);
# endif

public:

	// ========================================================================
	// Error classes
	// ========================================================================

	class ParseError : public PegException {
	public:
		ParseError(const std::string &rule)
			: PegException("Parse failed: " + rule) {}
	};

	// ========================================================================
	// Construction
	// ========================================================================

	PackratParser(const std::string &path, const Grammar &pegGrammar):
		_input(Input::fromFile(path)), _grammar(pegGrammar), _err()
# if PEG_DEBUG_PACKRAT
		, _traceDepth(0), _traceEnabled(true)
		, _evalCount(0), _cacheHits(0), _backtrackCount(0)
	{
		PegDebug::Logger::resetIndent();
		PegDebug::Logger::setIndentValue("│ ");
	}
# else
	{}
# endif

	~PackratParser() {}

	// ========================================================================
	// Methods
	// ========================================================================

	void	parseRule(const std::string &rootRuleName, AstNode *&out);
	bool	eval(const Expr *expr, AstNode *parent);

	// ---- Accessors ----
	const Diag	&diag(void) const	{ return _err; }
	Diag		&diag(void)			{ return _err; }

	const Input	&input(void) const	{ return _input; }
	Input		&input(void)		{ return _input; }

	// ---- Debug functions ----
# if PEG_DEBUG_PACKRAT

	void	enableTrace(bool enable = true) { _traceEnabled = enable; }
	bool	isTraceEnabled(void) const	{ return _traceEnabled; }

	struct Stats {
		size_t	totalEvals;
		size_t	cacheHits;
		size_t	backtrackCount;

		double	cacheHitRate(void) const {
			return totalEvals > 0 ? (double)cacheHits / totalEvals : 0.0;
		}
	};

	Stats		getStats(void) const;
	void		resetStats(void);
	static void	printStats(Stats stats, std::ostream &os = std::cerr);

# endif

};

#endif

