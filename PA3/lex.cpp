/*
 * lex.cpp
 *
 */

#include <cctype>
#include <map>
#include "lex.h"

using std::map;
using namespace std;

LexItem id_or_kw(const string& lexeme, int linenum)
{
	Token tt = IDENT;

	auto kIt = kwmap.find(lexeme);
	if( kIt != kwmap.end() )
		tt = kIt->second;

	return LexItem(tt, lexeme, linenum);
}

LexItem
getNextToken(istream& in, int& linenum)
{
	enum TokState { START, INID, INSTRING, ININT, INREAL, INCOMMENT } lexstate = START;
	string lexeme;
	char ch, nextch, nextchar, nextch1;
	int strtype = 0;//0 for double quotes and 1 for single quotes


    while(in.get(ch)) {
		switch( lexstate ) {
		case START:
			if( ch == '\n' )
				linenum++;

			if( isspace(ch) )
				continue;

			lexeme = ch;

			if( isalpha(ch) ) {
				lexeme = toupper(ch);
				lexstate = INID;
			}
			else if( ch == '"' ) {
				lexstate = INSTRING;

			}
			else if( ch == '\'' ) {
				strtype = 1;
				lexstate = INSTRING;

			}
			else if( isdigit(ch) ) {
				lexstate = ININT;
			}
			else if( ch == '!' ) {
				lexstate = INCOMMENT;
			}

			else {
				Token tt = ERR;
				switch( ch ) {
				case '+':
					tt = PLUS;
                    break;

				case '-':
					tt = MINUS;
                    break;

				case '*':
					tt = MULT;
					break;
				case '/':
					nextch1 = in.peek();
					if(nextch1 == '/'){
						in.get(ch);
						tt = CONCAT;
						break;
					}
					tt = DIV;
					break;
				case '=':
					nextchar = in.peek();
					if(nextchar == '='){
						in.get(ch);
						tt = EQUAL;
						break;
					}
					tt = ASSOP;
					break;
				case '(':
					tt = LPAREN;
					break;
				case ')':
					tt = RPAREN;
					break;

				case ':':
					tt = COLON;
					break;
				case ',':
					tt = COMA;
					break;
				case '<':
					tt = LTHAN;
					break;

				case '.':
					nextch = in.peek();
					if(isdigit(nextch)){
						lexstate = INREAL;
						continue;
					}
					else {
						lexeme += nextch;
						return LexItem(ERR, lexeme, linenum);
						cout << "Here what?" << endl;
					}

				}

				return LexItem(tt, lexeme, linenum);
			}
			break;

		case INID:
			if( isalpha(ch) || isdigit(ch) ) {
				ch = toupper(ch);
				lexeme += ch;
			}
			else {
				in.putback(ch);
				return id_or_kw(lexeme, linenum);
			}
			break;

		case INSTRING:

			if( ch == '\n' ) {
				return LexItem(ERR, lexeme, linenum);
			}
			lexeme += ch;
			if( ch == '"' && strtype == 0) {
				lexeme = lexeme.substr(1, lexeme.length()-2);
				return LexItem(SCONST, lexeme, linenum);
			}
			else if (ch == '\'' && strtype == 1){
				lexeme = lexeme.substr(1, lexeme.length()-2);
				return LexItem(SCONST, lexeme, linenum);
			}

			break;

		case ININT:
			if( isdigit(ch) ) {
				lexeme += ch;
			}
			else if(ch == '.') {
				lexstate = INREAL;
				in.putback(ch);

			}
			else {
				in.putback(ch);
				return LexItem(ICONST, lexeme, linenum);
			}
			break;

		case INREAL:
			if( ch == '.' && isdigit(in.peek()) ) {
				lexeme += ch;

			}
			else if(isdigit(ch)){
				lexeme += ch;
			}
			else if(ch == '.' && !isdigit(in.peek())){
				return LexItem(ERR, lexeme, linenum);
			}
			else {
				in.putback(ch);
				return LexItem(RCONST, lexeme, linenum);
			}
			break;


		case INCOMMENT:
			if( ch == '\n' ) {
                ++linenum;
				lexstate = START;
			}
			break;
		}

	}//end of while loop

	if( in.eof() )
		return LexItem(DONE, "", linenum);
	return LexItem(ERR, "some strange I/O error", linenum);
}

ostream& operator<<(ostream& out, const LexItem& tok) {
	Token tt = tok.GetToken();
	out << tokenPrint[ tt ];
	if( tt == IDENT || tt == ICONST || tt == SCONST || tt == RCONST || tt == ERR ) {
		out << "(" << tok.GetLexeme() << ")";
	}
	return out;
}



