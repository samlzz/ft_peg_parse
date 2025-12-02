/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Grammar.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 23:22:01 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/02 12:20:03 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Grammar.hpp"
#include "GrammarImpl.hpp"

// ============================================================================
// Construction / Destruction
// ============================================================================

Grammar::Grammar(const std::string &grammarPath, bool checkLeftRecursion): _impl(NULL)
{
	try
	{
		_impl = new Impl(grammarPath, checkLeftRecursion);
	}
	catch (...)
	{
		delete _impl;
		_impl = NULL;
		throw;
	}
}

Grammar::~Grammar()
{
	delete _impl;
	_impl = NULL;
}

// ============================================================================
// Public parsing API (thin delegation to Impl)
// ============================================================================

AstNode	*
Grammar::parseFile(const std::string &path) const
{
	return _impl->parseFile(path);
}

AstNode	*
Grammar::parseFile(const std::string &path, const std::string &ruleName) const
{
	return _impl->parseFile(path, ruleName);
}

AstNode	*
Grammar::parseString(const std::string &input) const
{
	return _impl->parseString(input);
}

AstNode	*
Grammar::parseString(const std::string &input, const std::string &ruleName) const
{
	return _impl->parseString(input, ruleName);
}
