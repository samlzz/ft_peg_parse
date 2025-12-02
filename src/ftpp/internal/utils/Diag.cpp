/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Diag.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 16:10:00 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 13:25:58 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

#include "utils/Diag.hpp"
#include "utils/Input.hpp"

// ============================================================================
// Expectation
// ============================================================================

bool Diag::Expectation::operator<(const Expectation &other) const
{
	if (priority != other.priority)
		return priority > other.priority;
	return message < other.message;
}

bool Diag::Expectation::operator==(const Expectation &other) const
{
	return priority == other.priority
		&& message == other.message;
}

// ============================================================================
// Checkpoint management
// ============================================================================

void	Diag::save(void)
{
	_checkpoints.push(Checkpoint(_expectations, _farthest));
}

void	Diag::restore(void)
{
	if (_checkpoints.empty())
		return;

	const Checkpoint &cp = _checkpoints.top();
	_expectations = cp.expectations;
	_farthest = cp.farthest;
	
	_checkpoints.pop();
}

void	Diag::commit(void)
{
	if (_checkpoints.empty())
		return;

	_checkpoints.pop();
}

// ============================================================================
// Diag
// ============================================================================

// ========================================================================
// Construction / Assignment
// ========================================================================

Diag::Diag(): _expectations(), _checkpoints(), _farthest(0) {}
Diag::Diag(const Diag &other)
		: _expectations(other._expectations)
		, _checkpoints(other._checkpoints)
		, _farthest(other._farthest) {}

Diag::~Diag() {}

Diag	&Diag::operator=(const Diag &other)
{
	if (this != &other)
	{
		_expectations = other._expectations;
		_farthest = other._farthest;
	}
	return *this;
}

// ============================================================================
// Update & merge
// ============================================================================

void	Diag::add_expectation(const std::string &msg, enum e_priority prio)
{
	for (size_t i = 0; i < _expectations.size(); ++i)
	{
		if (_expectations[i].message == msg)
		{
			if (prio > _expectations[i].priority)
				_expectations[i].priority = prio;
			return;
		}
	}
	
	_expectations.push_back(Expectation(msg, prio));
}

// New farthest position: reset expectations
// Same position: add to expectations
// Less farther position: ignore (older error)
void	Diag::update(size_t pos, const std::string &expected, enum e_priority prio)
{
	if (pos > _farthest)
	{
		_farthest = pos;
		_expectations.clear();
		add_expectation(expected, prio);
	}
	else if (pos == _farthest)
		add_expectation(expected, prio);
}

void	Diag::reset(void)
{
	_farthest = 0;
	_expectations.clear();
}

// Getter
size_t	Diag::farthest(void) const	{ return _farthest; }

// ============================================================================
// Formatting helpers
// ============================================================================

// Sort by priority (descending), then alphabetically
void	Diag::deduplicate_expectations(void)
{
	if (_expectations.empty())
		return;

	std::sort(_expectations.begin(), _expectations.end());

	std::vector<Expectation> unique;

	unique.push_back(_expectations[0]);
	for (size_t i = 1; i < _expectations.size(); ++i)
	{
		if (_expectations[i].message != _expectations[i-1].message)
			unique.push_back(_expectations[i]);
	}
	_expectations = unique;
}

static inline void	_group_by_priority(
	const std::vector<Diag::Expectation> &src,
	std::vector<Diag::Expectation> groups[4]
)
{
	for (size_t i = 0; i < src.size(); ++i)
	{
		const Diag::Expectation &e = src[i];
		groups[e.priority].push_back(e);
	}
}

static inline const std::vector<Diag::Expectation> *
_select_best_group(std::vector<Diag::Expectation> groups[4])
{
	if (!groups[Diag::PRIO_CRITICAL].empty())	return &groups[Diag::PRIO_CRITICAL];
	if (!groups[Diag::PRIO_HIGH].empty())		return &groups[Diag::PRIO_HIGH];
	if (!groups[Diag::PRIO_MEDIUM].empty())		return &groups[Diag::PRIO_MEDIUM];
	if (!groups[Diag::PRIO_LOW].empty())		return &groups[Diag::PRIO_LOW];
	return NULL;
}

static inline std::string _format_group(const std::vector<Diag::Expectation> &v)
{
	std::ostringstream oss;
	oss << "expected ";

	size_t max_show = (v.size() > 3) ? 3 : v.size();

	for (size_t i = 0; i < max_show; ++i)
	{
		if (i > 0)
		{
			if (i == max_show - 1)
				oss << " or ";
			else
				oss << ", ";
		}
		oss << v[i].message;
	}

	if (v.size() > 3)
		oss << " (or " << (v.size() - 3) << " other alternatives)";
	return oss.str();
}

std::string Diag::format_expectations(void) const
{
	if (_expectations.empty())
		return "unexpected input";

	std::vector<Expectation> groups[4];
	_group_by_priority(_expectations, groups);

	const std::vector<Expectation> *best = _select_best_group(groups);
	if (!best || best->empty())
		return "unexpected input";

	return _format_group(*best);
}

// ============================================================================
// Error formatting with mark
// ============================================================================

static inline void	_append_ctx(const Input &in, size_t farthest, std::ostringstream &oss)
{
	const size_t	rad = 20;
	std::string		ctx = in.context(farthest, rad);

	if (!ctx.empty())
	{
		size_t		pos = std::min(farthest, rad);

		if (ctx.size() < pos)
			pos = ctx.size();
		oss << "\n" << ctx.substr(0, pos)
			<< " >>>" << ctx.substr(pos);
	}
}

std::string	Diag::formatError(const Input &in, bool withCtx) const
{
	std::ostringstream	oss;
	Input				tmp(in);

	tmp.setPos(_farthest);
	oss << "error at line " << tmp.line()
		<< ", column " << tmp.column() << ": ";

	Diag mutable_copy = *this;
	mutable_copy.deduplicate_expectations();
	oss << mutable_copy.format_expectations();

	if (withCtx)
		_append_ctx(in, _farthest, oss);

	return oss.str();
}
