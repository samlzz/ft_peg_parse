/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PackratCache.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 16:39:31 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/02 21:46:48 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packrat/PackratCache.hpp"
#include "ast/AstNode.hpp"
#include "peg/Expr.hpp"
#include <cstddef>

// ---- Memo Structs -----

// ---- MemoKey

bool PackratCache::MemoKey::operator<(const MemoKey& o) const
{
	return (_rule < o._rule) || (_rule == o._rule && _pos < o._pos);
}

// ---- MemoEntry

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

AstNode			*PackratCache::MemoEntry::consumeNode(void)
{
	AstNode	*tmp = _node;
	_node = NULL;
	return tmp;
}

// ---- PackratCache ----

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