/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PegParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 01:53:21 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/04 13:05:31 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#include "peg/PegParser.hpp"
#include "peg/Expr.hpp"
#include "peg/Grammar.hpp"
#include "peg/PegLexer.hpp"
#include "peg/syntax/ExprContainer.hpp"
#include "peg/syntax/ExprUnary.hpp"
#include "peg/syntax/ExprLeaf.hpp"
#include "peg/syntax/RuleRef.hpp"

PegParser::PegParser(const std::string &grammar_path):
	_lex(grammar_path), _tokens(), _rules(), _err()
{}
PegParser::~PegParser() {}

Expr	*PegParser::parsePrimary(void)
{
	PegLexer::Token	tk = _lex.next();

	switch (tk.type)
	{
	case PegLexer::T_LITERAL:
		return new Literal(tk.val);
	case PegLexer::T_CHARRANGE:
		return new CharRange(tk.val);
	case PegLexer::T_ID: {
		if (_lex.match(PegLexer::T_COLON))
			return new Capture(parseChoice(), tk.val, true);
		return new RuleRef(tk.val);
	}
	case PegLexer::T_LPAREN: {
		Expr	*expr = parseChoice();
		if (!_lex.match(PegLexer::T_RPAREN))
		{
			delete expr;
			throw PegParserError("Expected ')'");
		}
		return expr;
	}
	default:
		throw PegParserError("Unexpected token in primary");
	}
}

Expr	*PegParser::parseSuffix(void)
{
	Expr	*expr = parsePrimary();
	
	while (true)
	{
		PegLexer::Token	tk = _lex.peek();
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

Expr	*PegParser::parsePrefix(void)
{
	if (_lex.match(PegLexer::T_NOT))
		return new Predicate(parsePrefix(), false);
	if (_lex.match(PegLexer::T_AND))
		return new Predicate(parsePrefix(), true);
	return parseSuffix();
}

Expr	*PegParser::parseSequence(void)
{
	t_ExprList		seq;
	PegLexer::Token	tk;

	while (true)
	{
		PegLexer::Token	tk = _lex.peek();
		if (tk.type == PegLexer::T_RPAREN
			|| tk.type == PegLexer::T_SLASH
			|| tk.type == PegLexer::T_END)
			break;
		seq.push_back(parsePrefix());
	}
	if (seq.size() == 1)
		return seq[0];
	return new Sequence(seq);
}

Expr	*PegParser::parseChoice(void)
{
	t_ExprList	choices;

	choices.push_back(parseSequence());
	while (_lex.match(PegLexer::T_SLASH))
		choices.push_back(parseSequence());
	if (choices.size() == 1)
		return choices[0];
	return new Choice(choices);
}

void	PegParser::parseRule(void)
{
	PegLexer::Token	id = _lex.next();
	if (id.type != PegLexer::T_ID)
		throw PegParserError("Expected rule name");

	bool		captureRule = (id.val[0] == '@');
	std::string	ruleName = captureRule ? id.val.substr(1) : id.val;

	if (!_lex.match(PegLexer::T_ASSIGN))
		throw PegParserError("Expected assignement after rule name");

	Expr *expr = parseChoice();
	if (captureRule)
		expr = new Capture(expr, ruleName);
	if (_rules.find(ruleName) == _rules.end())
		_rules[ruleName] = expr;
	else
	{
		delete expr;
		throw PegParserError("Duplicate rule for identifier '" + ruleName + "'");
	}
}

void	PegParser::parseGrammar(Grammar &out)
{
	PegLexer::Token	tk;

	while ((tk = _lex.peek()).type != PegLexer::T_END)
		parseRule();
	if (_rules.empty())
		throw PegParserError("Empty grammar file");
	Grammar	tmp(_rules);
	out = tmp;
}
