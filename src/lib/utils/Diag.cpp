/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Diag.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 16:10:00 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/04 18:04:21 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <string>

#include "utils/Diag.hpp"
#include "utils/Input.hpp"

// ---- Operators ----

Diag& Diag::operator=(const Diag& other)
{
	if (this != &other)
	{
		_expected = other._expected;
		_farthest = other._farthest;
	}
	return *this;
}

// --- Update & combination ---

void	Diag::concat_expectation(const std::string &same_far_expct)
{
	if (_expected.find(same_far_expct) != std::string::npos)
		return ;
	if (!_expected.empty())
		_expected += ", ";
	_expected += same_far_expct;
}

void	Diag::update(size_t pos, const std::string &expected)
{
	if (pos > _farthest)
	{
		_farthest = pos;
		_expected = expected;
	}
	else if (pos == _farthest)
		concat_expectation(expected);
}

void	Diag::merge(const Diag &other)
{
	update(other._farthest, other._expected);
}

void	Diag::reset(void)
{
	_farthest = 0;
	_expected.clear();
}

// --- Accessors ---

std::string	Diag::formatError(const Input &in, bool with_ctx) const
{
	std::ostringstream	oss;
	Input				tmp(in);

	tmp.setPos(_farthest);
	oss << "error at line " << tmp.line() << ", column " << tmp.column();
	if (!_expected.empty())
		oss << " " << _expected;

	if (with_ctx)
	{
		std::string ctx = in.context(_farthest, 10);
		if (!ctx.empty())
			oss << " before \"" << ctx << "\"";
	}
	return oss.str();
}