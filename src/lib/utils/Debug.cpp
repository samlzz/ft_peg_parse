/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Debug.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 15:39:45 by sliziard          #+#    #+#             */
/*   Updated: 2025/11/05 11:46:58 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if DEBUG_LEVEL > 0

#include <cstddef>
#include <iostream>
#include <string>

#include "ast/AstNode.hpp"
#include "peg/Expr.hpp"
#include "peg/PegLexer.hpp"
#include "peg/syntax/ExprContainer.hpp"
#include "peg/syntax/ExprLeaf.hpp"
#include "peg/syntax/ExprUnary.hpp"
#include "peg/syntax/RuleRef.hpp"

// ==============================
// ** Peg Lexer (Tokens print) **
// ==============================

const char* tokenName(PegLexer::e_tk_type type)
{
	switch (type)
	{
		case PegLexer::T_ID: return "ID";
		case PegLexer::T_LITERAL: return "LITERAL";
		case PegLexer::T_SLASH: return "SLASH";
		case PegLexer::T_STAR: return "STAR";
		case PegLexer::T_PLUS: return "PLUS";
		case PegLexer::T_QMARK: return "QMARK";
		case PegLexer::T_AND: return "AND";
		case PegLexer::T_NOT: return "NOT";
		case PegLexer::T_LPAREN: return "LPAREN";
		case PegLexer::T_RPAREN: return "RPAREN";
		case PegLexer::T_CHARRANGE: return "CHARRANGE";
		case PegLexer::T_COLON: return "COLON";
		case PegLexer::T_ASSIGN: return "ASSIGN";
		case PegLexer::T_EOL: return "EOL";
		case PegLexer::T_END: return "END";
		default: return "UNKNOWN";
	}
}

void printToken(const PegLexer::Token& tk)
{
	std::cerr << "[LEXER] " << tokenName(tk.type)
			<< "('" << (tk.val == "\n" ? "\\n" : tk.val) << "')" << std::endl;
}

// ============================
// ** PegParser (Expr print) **
// ============================

const char* exprKindName(Expr::e_expr_kind k)
{
	switch (k)
	{
		case Expr::K_LITERAL: return "Literal";
		case Expr::K_CHARRANGE: return "CharRange";
		case Expr::K_SEQUENCE: return "Sequence";
		case Expr::K_CHOICE: return "Choice";
		case Expr::K_ZERO_OR_MORE: return "ZeroOrMore";
		case Expr::K_ONE_OR_MORE: return "OneOrMore";
		case Expr::K_OPTIONAL: return "Optional";
		case Expr::K_PREDICATE: return "Predicate";
		case Expr::K_RULEREF: return "RuleRef";
		case Expr::K_CAPTURE: return "Capture";
		default: return "Unknown";
	}
}

const std::string exprValue(const Expr *e)
{
	if (!e) return "";

	switch (e->kind())
	{
		case Expr::K_LITERAL:
		case Expr::K_CHARRANGE:
			return static_cast<const ExprLeaf *>(e)->value();
		case Expr::K_SEQUENCE:
		case Expr::K_CHOICE:
		{
			const ExprContainer	*c = static_cast<const ExprContainer *>(e);
			std::string val("[ ");
			for (size_t i = 0; i < c->size(); i++)
			{
				if (i != 0 || i != c->size() - 1)
					val += ", ";
				val += exprValue(c->elems()[i]);
			}
			return val + " ]";
		}
		case Expr::K_CAPTURE:
			return static_cast<const Capture *>(e)->tag();
		case Expr::K_ZERO_OR_MORE:
		case Expr::K_ONE_OR_MORE:
		case Expr::K_OPTIONAL:
		case Expr::K_PREDICATE:
			return "( " + exprValue(static_cast<const ExprUnary *>(e)->inner()) + " )";
		case Expr::K_RULEREF:
		{
			const RuleRef *r = static_cast<const RuleRef *>(e);
			return r->name() + ": " + exprValue(r->resolved());
		}
	}
	return "nAn";
}

void printExprTree(const Expr* e, int depth = 0)
{
	if (!e) return;
	std::string indent(depth * 2, ' ');
	std::cerr << indent << "- " << exprKindName(e->kind());
	if (e->kind() == Expr::K_LITERAL || e->kind() == Expr::K_CHARRANGE)
		std::cerr << " \"" << static_cast<const Literal*>(e)->value() << "\"";
	else if (e->kind() == Expr::K_RULEREF)
		std::cerr << " -> " << static_cast<const RuleRef*>(e)->name();
	else if (e->kind() == Expr::K_CAPTURE)
		std::cerr << " (" << static_cast<const Capture *>(e)->tag() << ")";
	std::cerr << std::endl;

	if (e->kind() == Expr::K_SEQUENCE || e->kind() == Expr::K_CHOICE)
	{
		const ExprContainer* c = static_cast<const ExprContainer*>(e);
		for (size_t i = 0; i < c->size(); ++i)
			printExprTree((*c)[i], depth + 1);
	}
	else if (e->kind() == Expr::K_ZERO_OR_MORE || e->kind() == Expr::K_ONE_OR_MORE ||
			e->kind() == Expr::K_OPTIONAL || e->kind() == Expr::K_PREDICATE || e->kind() == Expr::K_CAPTURE)
	{
		const ExprUnary* u = static_cast<const ExprUnary*>(e);
		printExprTree(u->inner(), depth + 1);
	}
}

// =========================================
// ** Packrat (print parse calls & cache) **
// =========================================

void printEvalTrace(const Expr* e, const AstNode *parent, size_t pos, bool entering)
{
	std::cerr << "[PACKRAT] " << (entering ? "→ Enter " : "← Exit  ")
			<< exprKindName(e->kind())
			<< " { " << exprValue(e) << " } "
			<< (parent ? "his parent: " + parent->type() : "")
			<< " @pos=" << pos << std::endl;
}

void printCacheHit(const Expr* e, size_t pos)
{
	std::cerr << "[CACHE] hit  " << exprKindName(e->kind())
			<< " @pos=" << pos << std::endl;
}
void printCacheSet(const Expr* e, size_t pos, bool ok, size_t next)
{
	std::cerr << "[CACHE] set  " << exprKindName(e->kind())
			<< " @pos=" << pos << " ok=" << ok
			<< " next=" << next << std::endl;
}

// =========================
// ** AstNode (print AST) **
// =========================

void printAstTree(const AstNode* node, int depth = 0)
{
	if (!node) return;
	std::string indent(depth * 2, ' ');
	std::cout << indent << "- " << node->type();
	if (!node->children().empty())
		std::cout << " (" << node->children().size() << " children)";
	std::cout << std::endl;

	// afficher les attributs
	for (std::map<std::string, std::string>::const_iterator it = node->attrs().begin();
		it != node->attrs().end(); ++it)
		std::cout << indent << "   @" << it->first << " = " << it->second << std::endl;

	for (size_t i = 0; i < node->children().size(); ++i)
		printAstTree(node->children()[i], depth + 1);
}

#endif
