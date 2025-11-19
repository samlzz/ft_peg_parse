/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PackratParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:40:11 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/19 14:23:18 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PACKRATPARSER_HPP
# define PACKRATPARSER_HPP

# include <stdint.h>

# include "PackratCache.hpp"
# include "ast/AstNode.hpp"
# include "peg/Grammar.hpp"
# include "utils/DebugLogger.hpp"
# include "utils/Diag.hpp"
# include "utils/Input.hpp"
# include "utils/PegException.hpp"

# define PRINT_CALLER

class PackratParser {

private:
	Input			&_input;
	const Grammar	&_grammar;
	PackratCache	_memo;
	Diag			_err;

	PackratParser(const PackratParser& other);
	PackratParser& operator=(const PackratParser& other);

	bool	retrieveExpr(const Expr *e, size_t pos, AstNode *parent);

#ifdef PEG_DEBUG_PARSER
	// trace state
	int32_t	_traceDepth;
	bool	_traceEnabled;
	
	// statistics
	size_t	_evalCount;
	size_t	_cacheHits;
	size_t	_backtrackCount;
	
	void _traceEnter(const Expr* expr, size_t pos);
	void _traceExit(const Expr* expr, size_t pos, bool success);
#endif

public:
	class ParseError : public PegException {
	public:
		ParseError(const std::string& rule)
			: PegException("Parse failed: " + rule)
		{}
	};

	PackratParser(Input &in, const Grammar &pegGrammar):
		_input(in), _grammar(pegGrammar), _memo(), _err()
# ifdef PEG_DEBUG_PARSER
		, _traceDepth(0), _traceEnabled(false)
		, _evalCount(0), _cacheHits(0), _backtrackCount(0)
	{
		PegDebug::Logger::resetIndent();
		PegDebug::Logger::setIndentValue("│ ");
	}
# else
	{}
# endif
	~PackratParser() {};

	void		parseRule(const std::string &rootRuleName, AstNode *&out);
	bool		eval(const Expr *expr, AstNode *parent);

	const Diag	&diag(void) const	{ return _err; }
	Diag		&diag(void)			{ return _err; }
	const Input	&input(void) const	{ return _input; }
	Input		&input(void)		{ return _input; }

	void		resetDiag(void)		{ _err.reset(); }
	void		resetMemo(void)		{ _memo.reset(); }

#ifdef PEG_DEBUG_PARSER
	void	enableTrace(bool enable = true) { _traceEnabled = enable; }
	bool	isTraceEnabled() const			{ return _traceEnabled; }
	
	struct Stats {
		size_t totalEvals;
		size_t cacheHits;
		size_t backtrackCount;
		
		double cacheHitRate(void) const
		{
			return totalEvals > 0 ? (double)cacheHits / totalEvals : 0.0;
		}
	};
	
	Stats 		getStats(void) const;
	void		resetStats(void);
	static void	printStats(Stats stats, std::ostream& os = std::cerr);
#endif
};

#endif
