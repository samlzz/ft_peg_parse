/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PackratParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:40:11 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 00:16:18 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_PACKRAT_PARSER_HPP__
# define __FTPP_PACKRAT_PARSER_HPP__

# include <stdint.h>

# include "AstNode.hpp"
# include "config.h"
# include "utils/Diag.hpp"
# include "utils/Input.hpp"

// ============================================================================
// PackratParser
// ============================================================================

/**
 * @brief Packrat PEG parser
 *
 * Evaluates grammar expressions using a classic packrat strategy:
 *   - recursive descent
 *   - memo table for linear-time guarantees *not implemented yet)
 */
class PackratParser {

public:
	// ========================================================================
	// Construction
	// ========================================================================

	PackratParser(Input &raw);
	~PackratParser();

	// ========================================================================
	// Methods
	// ========================================================================

	AstNode		*parseRule(Expr *rule);
	bool		eval(const Expr *expr, AstNode *parent);

	// ---- Accessors ----
	const Diag	&diag(void) const	{ return _err; }
	Diag		&diag(void)			{ return _err; }

	const Input	&input(void) const	{ return _input; }
	Input		&input(void)		{ return _input; }

private:
	Input	&_input;
	Diag	_err;

	PackratParser();
	PackratParser(const PackratParser &other);
	PackratParser &operator=(const PackratParser &other);

# if FTPP_DEBUG_PACKRAT
	// ---- Debug attributes ----
	size_t	_evalCount;
	size_t	_cacheHits;
	size_t	_backtrackCount;

public:
	// ---- Debug functions ----

	struct Stats
	{
		size_t	totalEvals;
		size_t	cacheHits;
		size_t	backtrackCount;

		double	cacheHitRate(void) const;
	};

	Stats		getStats(void) const;
	static void	printStats(Stats stats, std::ostream &os = std::cerr);

# endif
};

#endif /* __FTPP_PACKRAT_PARSER_HPP__ */