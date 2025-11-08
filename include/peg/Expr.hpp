/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Expr.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:40:29 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/07 20:24:34 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPR_HPP
# define EXPR_HPP

# include <map>
# include <stdint.h>
# include <string>
# include <vector>

class PackratParser;
class AstNode;

class Expr {

private:
	Expr(const Expr &other);
	Expr	&operator=(const Expr &other);

public:
	enum e_expr_kind {
		K_LITERAL,
		K_CHARRANGE,
		K_ANY,
		K_SEQUENCE,
		K_CHOICE,
		K_ZERO_OR_MORE,
		K_ONE_OR_MORE,
		K_OPTIONAL,
		K_PREDICATE,
		K_RULEREF,
		K_CAPTURE,
	};

	virtual ~Expr() {}

	enum e_expr_kind	kind(void) const	{ return _kind; }
	virtual bool		parse(PackratParser &parser, AstNode *parent) const = 0;

protected:
	const enum e_expr_kind	_kind;

	Expr(enum e_expr_kind kind): _kind(kind) {}
};

typedef std::vector<Expr *> 			t_ExprList;
typedef std::map<std::string, Expr *>	t_ExprDict;

template <typename T>
void	deleteAll(std::vector<T *> &list)
{
	for (size_t i = 0; i < list.size(); ++i)
		delete list[i];
	list.clear();
}

template <typename K, typename V>
void	deleteVals(std::map<K, V*> &dict)
{
	typename std::map<K, V*>::iterator	it;

	for (it = dict.begin(); it != dict.end(); ++it)
	{
		if (it->second)
		{
			delete it->second;
			it->second = NULL;
		}
	}
	dict.clear();
}

#endif
