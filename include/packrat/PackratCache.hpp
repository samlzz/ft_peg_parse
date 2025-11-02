/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PackratCache.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:34:45 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/02 17:45:31 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PACKRATCACHE_HPP
# define PACKRATCACHE_HPP

# include <cstddef>
# include <map>
# include <stdint.h>

# include "ast/AstNode.hpp"
# include "peg/Expr.hpp"

# define PRINT_CALLER

class PackratCache {
public:

	class MemoKey {
	private:
		size_t		_pos;
		const Expr	*_rule;

	public:

		MemoKey();
		MemoKey(size_t pos, const Expr *rule);
		bool	operator<(const MemoKey &other) const;

		size_t		pos(void)	const;
		const Expr	*rule(void)	const;
	};

	class MemoEntry {
	private:
		bool		_ok;
		size_t		_nextPos;
		AstNode*	_node;

	public:
		MemoEntry();
		MemoEntry(bool ok, size_t nextPos, const AstNode *og);
		MemoEntry(const MemoEntry &other);
		~MemoEntry();

		MemoEntry	&operator=(const MemoEntry &other);

		bool			ok(void)		const;
		size_t			nextPos(void)	const;
		const AstNode	*node(void)		const;
		AstNode			*consumeNode(void);
	};

	typedef std::map<MemoKey, MemoEntry> t_Memo;

	PackratCache();
	~PackratCache();

	bool		has(const Expr *rule, size_t pos) const;
	MemoEntry	get(const Expr *rule, size_t pos) const;
	void		set(const Expr *rule, size_t pos, const MemoEntry &entry);

	void		reset(void);

private:

	std::map<MemoKey, MemoEntry>	_table;

	PackratCache(const PackratCache& other);
	PackratCache& operator=(const PackratCache& other);

};

#endif
