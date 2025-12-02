/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExprTerminalBase.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:10:24 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 12:27:18 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FTPP_EXPR_TERMINAL_BASE_HPP__
# define __FTPP_EXPR_TERMINAL_BASE_HPP__

# include <string>

# include "peg/core/Expr.hpp"

// ============================================================================
// ExprTerminalBase (abstract helper class)
// ============================================================================

/**
 * @brief Base class for leaf PEG expressions holding a string value.
 *
 * Provides a simple storage for character data such as literals or
 * character ranges. Concrete subclasses implement their matching logic.
 */
class ExprTerminalBase : public Expr {

protected:
	std::string _value;

public:
	ExprTerminalBase(enum e_expr_kind kind, const std::string &value)
		: Expr(kind), _value(value)
	{}
	virtual ~ExprTerminalBase() {}

	// ---- Accessors ----
	const std::string&	value() const					{ return _value; }
	void				setValue(const std::string &v)	{ _value = v; }

private:
	ExprTerminalBase();
	ExprTerminalBase(const ExprTerminalBase &other);
	ExprTerminalBase &operator=(const ExprTerminalBase &other);
};

#endif /* __FTPP_EXPR_TERMINAL_BASE_HPP__ */