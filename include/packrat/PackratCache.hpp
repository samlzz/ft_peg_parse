/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PackratCache.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:34:45 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/02 21:46:52 by sliziard         ###   ########.fr       */
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

		MemoKey(): _pos(0), _rule(NULL) {}
		MemoKey(size_t pos, const Expr *rule): _pos(pos), _rule(rule)
		{}
		bool	operator<(const MemoKey &other) const;

		size_t		pos(void)	const	{ return _pos; }
		const Expr	*rule(void)	const	{ return _rule; }
	};

	class MemoEntry {
	private:
		bool		_ok;
		size_t		_nextPos;
		AstNode*	_node;

	public:
		MemoEntry(): _ok(false), _nextPos(0), _node(NULL) {}
		MemoEntry(bool ok, size_t nextPos, const AstNode *og):
			_ok(ok), _nextPos(nextPos), _node(og ? new AstNode(*og) : NULL)
		{}
		MemoEntry(const MemoEntry &other):
			_ok(other.ok()),
			_nextPos(other.nextPos()),
			_node(other._node ? new AstNode(*other._node): NULL)
		{}
		~MemoEntry() { delete _node; }

		MemoEntry	&operator=(const MemoEntry &other);

		bool			ok(void)		const	{ return _ok; }
		size_t			nextPos(void)	const	{ return _nextPos; }
		const AstNode	*node(void)		const	{ return _node; }
		AstNode			*consumeNode(void);
	};

	typedef std::map<MemoKey, MemoEntry> t_Memo;

	PackratCache(): _table() {}
	~PackratCache() {}

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
