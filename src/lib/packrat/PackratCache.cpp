/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PackratCache.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:39:31 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/02 17:47:40 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packrat/PackratCache.hpp"
#include "ast/AstNode.hpp"
#include "peg/Expr.hpp"
#include <cstddef>

// ---- Memo Structs -----

// ---- MemoKey
PackratCache::MemoKey::MemoKey():
	_pos(0), _rule(NULL)
{}

PackratCache::MemoKey::MemoKey(size_t p, const Expr *r):
	_pos(p), _rule(r)
{}

bool PackratCache::MemoKey::operator<(const MemoKey& o) const
{
	return (_rule < o._rule) || (_rule == o._rule && _pos < o._pos);
}

size_t	PackratCache::MemoKey::pos(void)		const	{ return _pos; }

const Expr	*PackratCache::MemoKey::rule(void)	const	{ return _rule; }

// ---- MemoEntry
PackratCache::MemoEntry::MemoEntry():
	_ok(false), _nextPos(0), _node(NULL)
{}

PackratCache::MemoEntry::MemoEntry(bool ok, size_t nextPos, const AstNode *og):
	_ok(ok), _nextPos(nextPos), _node(og ? new AstNode(*og): NULL)
{}

PackratCache::MemoEntry::MemoEntry(const MemoEntry &other):
	_ok(other._ok),
	_nextPos(other._nextPos),
	_node(other._node ? new AstNode(*other._node) : NULL)
{}

PackratCache::MemoEntry::~MemoEntry()
{
	if (_node)
		delete _node;
}

PackratCache::MemoEntry	&PackratCache::MemoEntry::operator=(const MemoEntry &other)
{
	if (this != &other)
	{
		_ok = other._ok;
		_nextPos = other._nextPos;
		if (_node)
		{
			delete _node;
			_node = NULL;
		}
		if (other._node)
			_node = new AstNode(*other._node);
	}
	return (*this);
}

bool	PackratCache::MemoEntry::ok(void)		const	{ return _ok; }

size_t PackratCache::MemoEntry::nextPos(void)	const	{ return _nextPos; }

const AstNode	*PackratCache::MemoEntry::node(void) const
{
	return _node;
}
AstNode			*PackratCache::MemoEntry::consumeNode(void)
{
	AstNode	*tmp = _node;
	_node = NULL;
	return tmp;
}

// ---- PackratCache ----

PackratCache::PackratCache(): _table()
{}
PackratCache::~PackratCache()
{}

bool PackratCache::has(const Expr *rule, size_t pos) const
{
	MemoKey key(pos, rule);
	return _table.find(key) != _table.end();
}

PackratCache::MemoEntry PackratCache::get(const Expr *rule, size_t pos) const
{
	MemoKey					key(pos, rule);
	t_Memo::const_iterator	it = _table.find(key);

	if (it == _table.end())
		return MemoEntry();
	return it->second;
}

void PackratCache::set(const Expr *rule, size_t pos, const MemoEntry &entry)
{
	MemoKey key(pos, rule);
	_table[key] = entry;
}

void PackratCache::reset(void)
{
	_table.clear();
}