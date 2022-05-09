/*
 * lex.h
 *
*/

#ifndef LEX_H_
#define LEX_H_

#include <string>
#include <iostream>
#include <map>
using namespace std;

//Definition of all the possible token types
enum Token {
		// keywords
	PROGRAM, PRINT, READ, INTEGER, END, IF, THEN, REAL, CHAR,

		// an identifier
	IDENT,

		// an integer, real, and string constant
	ICONST, RCONST, SCONST,

		// the operators, parens, semicolon
	PLUS, MINUS, MULT, DIV, ASSOP, LPAREN, RPAREN, COMA,
	EQUAL, LTHAN, CONCAT, COLON,
		// any error returns this token
	ERR,

		// when completed (EOF), return this token
	DONE
};

static map<Token,string> tokenPrint = {
		{PROGRAM, "PROGRAM"},
		{READ, "READ"},
		{INTEGER, "INTEGER"},
		{REAL, "REAL"},
		{CHAR, "CHAR"},
		{ PRINT, "PRINT" },
		{ IF, "IF" },
		{ END, "END" },
		{THEN, "THEN"},

		{ IDENT, "IDENT" },

		{ ICONST, "ICONST" },
		{ RCONST, "RCONST" },
		{ SCONST, "SCONST" },

		{ PLUS, "PLUS" },
		{ MINUS, "MINUS" },
		{ MULT, "MULT" },
		{ DIV, "DIV" },
		{ ASSOP, "ASSOP" },
		{ LPAREN, "LPAREN" },
		{ RPAREN, "RPAREN" },

		{ COLON, "COLON" },
		{COMA, "COMA" },
		{ EQUAL, "EQUAL" },
		{ LTHAN, "LTHAN" },

		{ CONCAT, "CONCAT" },

		{ ERR, "ERR" },

		{ DONE, "DONE" },
};

static map<string,Token> kwmap = {
		{"PROGRAM", PROGRAM},
		{"READ", READ},
		{ "INTEGER", INTEGER},
		{ "REAL", REAL},
		{ "CHAR", CHAR},
		{ "PRINT", PRINT },
		{ "IF", IF },
		{"THEN", THEN},
		{ "END", END },
};

//Class definition of LexItem
class LexItem {
	Token	token;
	string	lexeme;
	int	lnum;

public:
	LexItem() {
		token = ERR;
		lnum = -1;
	}
	LexItem(Token token, string lexeme, int line) {
		this->token = token;
		this->lexeme = lexeme;
		this->lnum = line;
	}

	bool operator==(const Token token) const { return this->token == token; }
	bool operator!=(const Token token) const { return this->token != token; }

	Token	GetToken() const { return token; }
	string	GetLexeme() const { return lexeme; }
	int	GetLinenum() const { return lnum; }
};



extern ostream& operator<<(ostream& out, const LexItem& tok);
extern LexItem id_or_kw(const string& lexeme, int linenum);
extern LexItem getNextToken(istream& in, int& linenum);


#endif /* LEX_H_ */
