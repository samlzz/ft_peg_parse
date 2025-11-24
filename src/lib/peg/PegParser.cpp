/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PegParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 01:53:21 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/24 12:36:33 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#include "peg/Expr.hpp"
#include "peg/Grammar.hpp"
#include "peg/PegLexer.hpp"
#include "peg/PegParser.hpp"
#include "peg/syntax/ExprContainer.hpp"
#include "peg/syntax/ExprUnary.hpp"
#include "peg/syntax/ExprLeaf.hpp"
#include "peg/syntax/RuleRef.hpp"
#include "utils/DebugLogger.hpp"

PegParser::PegParser(const std::string &grammar_path):
	_lex(grammar_path), _rules()
{}

static inline bool _isRuleBegin(PegLexer lex)
{
	PegLexer::Token	tk = lex.next();

	return (lex.peek().type == PegLexer::T_ASSIGN);
}

Expr	*PegParser::parsePrimary(void)
{
	PEG_LOG_PARSER_FN("parsePrimary");
	if (_isRuleBegin(_lex))
		return NULL;
	PegLexer::Token	tk = _lex.next();

	switch (tk.type)
	{
	case PegLexer::T_LITERAL:
		return new Literal(tk.val);
	case PegLexer::T_CHARRANGE:
		return new CharRange(tk.val);
	case PegLexer::T_DOT:
		return new Any();
	case PegLexer::T_ID: {
		if (_lex.match(PegLexer::T_COLON))
		{
			Expr	*expr = parseChoice();
			return new Capture(expr, tk.val, true);
		}
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
	PEG_LOG_PARSER_FN("parseSuffix");
	Expr	*expr = parsePrimary();
	if (!expr)
		return NULL;

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
	PEG_LOG_PARSER_FN("parsePrefix");
	if (_lex.match(PegLexer::T_NOT))
		return new Predicate(parsePrefix(), false);
	if (_lex.match(PegLexer::T_AND))
		return new Predicate(parsePrefix(), true);
	return parseSuffix();
}

Expr	*PegParser::parseSequence(void)
{
	PEG_LOG_PARSER_FN("parseSequence");
	t_ExprList		seq;
	Expr			*e;
	PegLexer::Token	tk;

	while (true)
	{
		PegLexer::Token	tk = _lex.peek();
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

Expr	*PegParser::parseChoice(void)
{
	PEG_LOG_PARSER_FN("ParseChoice");
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
	PEG_LOG_PARSER_FN("parseRule");
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

	PEG_LOG_INFO_C(PARSER, "PegParser", 
		"Parsed rule " + ruleName + " " + expr->debugRepr()
	);
	if (_rules.find(ruleName) != _rules.end())
	{
		delete expr;
		throw PegParserError("Duplicate rule for identifier '" + ruleName + "'");
	}
	_rules[ruleName] = expr;
}

void	PegParser::parseGrammar(Grammar &out)
{
	PEG_LOG_PARSER_FN("parseGrammar");
	PegLexer::Token	tk;

	while ((tk = _lex.peek()).type != PegLexer::T_END)
	{
		if (_lex.match(PegLexer::T_EOL))
			continue;
		parseRule();
	}
	if (_rules.empty())
		throw PegParserError("Empty grammar file");
	Grammar	tmp(_rules);
	out = tmp;
}
