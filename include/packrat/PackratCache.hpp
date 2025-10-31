/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PackratCache.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 17:34:45 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 12:36:35 by sliziard         ###   ########.fr       */
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
private:

	struct MemoKey {
		size_t		pos;
		const Expr	*rule;

		MemoKey(): pos(0), rule(NULL) {};
		bool operator<(const MemoKey& o) const
		{
			return (rule < o.rule) || (rule == o.rule && pos < o.pos);
		}
	};

	struct MemoEntry {
		bool		ok;
		size_t		nextPos;
		AstNode*	node;

		MemoEntry(): ok(false), nextPos(0), node(NULL) {};
	};

	std::map<MemoKey, MemoEntry>	_table;

	PackratCache(const PackratCache& other);
	PackratCache& operator=(const PackratCache& other);

public:

	PackratCache();
	~PackratCache();

	bool		has(const Expr *rule, size_t pos) const;
	MemoEntry	get(const Expr *rule, size_t pos) const;
	void		set(const Expr *rule, size_t pos, const MemoEntry &entry);
};

#endif
