/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PegParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 01:53:21 by sliziard          #+#    #+#             */
/*   Updated: 2025/12/01 12:12:39 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#include "config.h"
#include "ft_log/LogScope.hpp"
#include "Grammar.hpp"
#include "peg/core/Expr.hpp"
#include "peg/PegLexer.hpp"
#include "peg/PegParser.hpp"
#include "peg/syntax/CombinatorOps.hpp"
#include "peg/syntax/RuleRef.hpp"
#include "peg/syntax/TerminalOps.hpp"
#include "peg/syntax/UnaryActions.hpp"
#include "peg/syntax/UnaryPredicate.hpp"
#include "peg/syntax/UnaryQuantifiers.hpp"

// ============================================================================
// Constructors
// ============================================================================

PegParser::PegParser(const std::string &grammar_path)
	: _lex(grammar_path), _rules()
{}

// ============================================================================
// Primary
// ============================================================================

// ---- Helpers ----

static inline bool	_isRuleBegin(PegLexer lex)
{
	PegLexer::Token tk = lex.next();
	return (lex.peek().type == PegLexer::T_ASSIGN);
}

Expr	*PegParser::parseIdentifier(std::string &id)
{
	if (!_lex.match(PegLexer::T_COLON))
		return new RuleRef(id);

	return new Capture(parsePrefix(), id, true);
}


Expr	*PegParser::parseSubExpr(void)
{
	Expr	*expr = parseChoice();

	if (!_lex.match(PegLexer::T_RPAREN))
	{
		delete expr;
		throw PegParserError("Expected ')'");
	}
	return expr;
}

Expr	*PegParser::parsePrimary(void)
{
	ft_log::LogScope _(FTPP_LOG_PARSER, "parsePrimary");

	if (_isRuleBegin(_lex))
		return NULL;

	PegLexer::Token tk = _lex.next();

	switch (tk.type)
	{
	case PegLexer::T_LITERAL:
		return new Literal(tk.val);

	case PegLexer::T_CHARRANGE:
		return new CharRange(tk.val);

	case PegLexer::T_DOT:
		return new Any();

	case PegLexer::T_ID:
		return parseIdentifier(tk.val);

	case PegLexer::T_LPAREN:
		return parseSubExpr();

	default:
		throw PegParserError("Unexpected token: " + tk.val);
	}
}

// ============================================================================
// Suffix
// ============================================================================

Expr	*PegParser::parseSuffix(void)
{
	ft_log::LogScope _(FTPP_LOG_PARSER, "parseSuffix");

	Expr *expr = parsePrimary();
	if (!expr)
		return NULL;

	while (true)
	{
		PegLexer::Token tk = _lex.peek();
		switch (tk.type)
		{
		case PegLexer::T_STAR:
			_lex.next();
			expr = new ZeroOrMore(expr);
			continue;

		case PegLexer::T_PLUS:
			_lex.next();
			expr = new OneOrMore(expr);
			continue;

		case PegLexer::T_QMARK:
			_lex.next();
			expr = new Optional(expr);
			continue;

		default:
			break;
		}
		break;
	}
	return expr;
}

// ============================================================================
// Prefix
// ============================================================================

Expr	*PegParser::parsePrefix(void)
{
	ft_log::LogScope _(FTPP_LOG_PARSER, "parsePrefix");

	if (_lex.match(PegLexer::T_AND))
		return new Predicate(parsePrefix(), true);
	if (_lex.match(PegLexer::T_TILD))
		return new Fatal(parsePrefix());
	return parseSuffix();
}

// ============================================================================
// Sequence
// ============================================================================

Expr	*PegParser::parseSequence(void)
{
	ft_log::LogScope _(FTPP_LOG_PARSER, "parseSequence");

	t_ExprList seq;
	Expr *e;

	while (true)
	{
		PegLexer::Token tk = _lex.peek();

		if (tk.type == PegLexer::T_RPAREN
			|| tk.type == PegLexer::T_SLASH
			|| tk.type == PegLexer::T_END)
			break;

		if (_lex.match(PegLexer::T_EOL))
			continue;

		e = parsePrefix();
		if (!e)
			break;
		seq.push_back(e);
	}

	if (seq.size() == 1)
		return seq[0];

	return new Sequence(seq);
}

// ============================================================================
// Choice
// ============================================================================

Expr	*PegParser::parseChoice(void)
{
	ft_log::LogScope _(FTPP_LOG_PARSER, "parseChoice");

	t_ExprList choices;

	choices.push_back(parseSequence());
	while (_lex.match(PegLexer::T_SLASH))
		choices.push_back(parseSequence());

	if (choices.size() == 1)
		return choices[0];

	return new Choice(choices);
}

// ============================================================================
// Rule
// ============================================================================

void	PegParser::parseRule(void)
{
	ft_log::LogScope _(FTPP_LOG_PARSER, "parseRule");

	PegLexer::Token id = _lex.next();
	if (id.type != PegLexer::T_ID)
		throw PegParserError("Expected rule name");

	bool		captureRule = (id.val[0] == '@');
	std::string	ruleName = captureRule ? id.val.substr(1) : id.val;

	if (!_lex.match(PegLexer::T_ASSIGN))
		throw PegParserError("Expected assignement after rule name");

	Expr *expr = parseChoice();
	if (captureRule)
		expr = new Capture(expr, ruleName);

	if (_rules.find(ruleName) != _rules.end())
	{
		delete expr;
		throw PegParserError("Duplicate rule for identifier '" + ruleName + "'");
	}

	_rules[ruleName] = expr;
}

// ============================================================================
// Grammar
// ============================================================================

void	PegParser::parseGrammar(Grammar &out)
{
	ft_log::LogScope _(FTPP_LOG_PARSER, "parseGrammar");

	PegLexer::Token tk;

	while ((tk = _lex.peek()).type != PegLexer::T_END)
	{
		if (_lex.match(PegLexer::T_EOL))
			continue;
		parseRule();
	}

	if (_rules.empty())
		throw PegParserError("Empty grammar file");

	Grammar tmp(_rules);
	out = tmp;
}

