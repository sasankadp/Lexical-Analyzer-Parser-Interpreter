/* Sasanka Paththameistreege
 * Implementation of Recursive-Descent Parser
 * parseInt.cpp
*/
#include <string.h>
#include "parserInt.h"
static int error_count = 0;
map<string, Value> defVar;
map<string, Token> SymTable;
queue<Value>*ValQue;
map<string,Value>TempsResults;

/*
 * parse.h
 */
#ifndef PARSE_H_
#define PARSE_H_

#include <iostream>

using namespace std;

#include "lex.h"
#include "val.h"

extern bool Prog(istream& in, int& line);
extern bool Stmt(istream& in, int& line);
extern bool Decl(istream& in, int& line);
extern bool PrintStmt(istream& in, int& line,LexItem lt);
extern bool IfStmt(istream& in, int& line);
extern bool ReadStmt(istream& in, int& line);
extern bool IdList(istream& in, int& line, LexItem & tok);
extern bool VarList(istream& in, int& line);
extern bool Var(istream& in, int& line, LexItem & tok);
extern bool AssignStmt(istream& in, int& line);
extern bool ExprList(istream& in, int& line,queue<Value> *valq);
extern bool LogicExpr(istream& in, int& line, Value & retVal);
extern bool Expr(istream& in, int& line, Value & retVal);
extern bool Term(istream& in, int& line, Value & retVal);
extern bool SFactor(istream& in, int& line, Value & retVal);
extern bool Factor(istream& in, int& line, int sign, Value & retVal);
extern int ErrCount();
#endif /* PARSE_H_ */

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;
	}

}

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;

}
//Program is: Prog = PROGRAM IDENT {Decl} {Stmt} END PROGRAM IDENT
bool Prog(istream& in, int& line)
{
	bool dl = false, sl = false;
	LexItem tok = Parser::GetNextToken(in, line);
	//cout << "in Prog" << endl;

	if (tok.GetToken() == PROGRAM) {
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == IDENT) {
			string progName = tok.GetLexeme();
			dl = Decl(in, line);
			if( !dl  )
			{
				ParseError(line, "Incorrect Declaration in Program");
				return false;
			}
            //for(auto const &pair: defVar) {Trace("\n");Trace(pair.first);
            //}

			sl = Stmt(in, line);

			if( !sl  )
			{
				ParseError(line, "Incorrect Statement in program");
				return false;
			}

			tok = Parser::GetNextToken(in, line);

			if (tok.GetToken() == END) {
				tok = Parser::GetNextToken(in, line);

				if (tok.GetToken() == PROGRAM) {
					tok = Parser::GetNextToken(in, line);

					if (tok.GetToken() == IDENT) {
						if(progName!=tok.GetLexeme()){
                            ParseError(line, "Incorrect Program Name");
                            return false;
						}
						else{return true;}

					}
					else
					{
						ParseError(line, "Missing Program Name");
						return false;
					}
				}
				else
				{
					ParseError(line, "Missing PROGRAM at the End");
					return false;
				}
			}
			else
			{
				ParseError(line, "Missing END of Program");
				return false;
			}
		}

	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	return false;
}

//Decl = Type : VarList
//Type = INTEGER | REAL | CHAR
bool Decl(istream& in, int& line) {
	bool status = false;
	LexItem tok;
	//cout << "in Decl" << endl;
	LexItem t = Parser::GetNextToken(in, line);

	if(t == INTEGER || t == REAL || t == CHAR) {
		tok = t;
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == COLON) {
			status = IdList(in, line, t);
			//cout<< tok.GetLexeme() << " " << (status? 1: 0) << endl;
			if (status)
			{
				status = Decl(in, line);
				return status;
			}
		}
		else{
			ParseError(line, "Missing Colon");
			return false;
		}
	}

	Parser::PushBackToken(t);
	return true;
}

//Stmt is either a PrintStmt, ReadStmt, IfStmt, or an AssigStmt
//Stmt = AssigStmt | IfStmt | PrintStmt | ReadStmt
bool Stmt(istream& in, int& line) {
	bool status;
	//cout << "in Stmt" << endl;
	LexItem t = Parser::GetNextToken(in, line);

	switch( t.GetToken() ) {

	case PRINT:


		status = PrintStmt(in, line,t);

		if(status)
			status = Stmt(in, line);
		break;

	case IF:
		status = IfStmt(in, line);
		if(status)
			status = Stmt(in, line);
		break;

	case IDENT:
		Parser::PushBackToken(t);
        status = AssignStmt(in, line);
        /*for(auto const &pair: defVar) {
            cout<<" "<<pair.first);
            cout<<" "<<pair.second;
        }*/
		if(status)
			status = Stmt(in, line);
		break;

	case READ:
		status = ReadStmt(in, line);
		//cout << "status: " << (status? true:false) <<endl;
		if(status)
			status = Stmt(in, line);
		break;

	default:
		Parser::PushBackToken(t);
		return true;
	}

	return status;
}

//PrintStmt:= print, ExpreList
bool PrintStmt(istream& in, int& line,LexItem lt) {
	/*create an empty queue of Value objects.*/
	ValQue = new queue<Value>;
	//bool ex = false ;
	LexItem t;
	//cout << "in PrintStmt" << endl;
	if( (t=Parser::GetNextToken(in, line)) != COMA ) {

		ParseError(line, "Missing a Comma");
		return false;
	}

	bool ex = ExprList(in, line,ValQue);

	if( !ex ) {
		ParseError(line, "Missing expression after print");
		// Empty the queue and delete
		while(!(*ValQue).empty()){
            ValQue->pop();
		}
		delete ValQue;
		return false;
	}
	//Evaluate: print out the list of expressions values
	while (!(*ValQue).empty()) {
        Value nextVal = (*ValQue).front();
        //if(lt.GetLexeme()=="PRINT")
        cout << nextVal;
        ValQue->pop();
    }
     //if(lt.GetLexeme()=="PRINT")
    cout <<endl;
	return ex;
}

//IfStmt:= if (Expr) then {Stmt} END IF
bool IfStmt(istream& in, int& line) {
	bool ex=false ;
	LexItem t;
    Value v;
	//cout << "in IfStmt" << endl;
	if( (t=Parser::GetNextToken(in, line)) != LPAREN ) {

		ParseError(line, "Missing Left Parenthesis");
		return false;
	}

	ex = LogicExpr(in, line,v);
	if( !ex ) {
		ParseError(line, "Missing if statement Logic Expression");
		return false;
	}

	if((t=Parser::GetNextToken(in, line)) != RPAREN ) {

		ParseError(line, "Missing Right Parenthesis");
		return false;
	}

	if((t=Parser::GetNextToken(in, line)) != THEN ) {

		ParseError(line, "Missing THEN");
		return false;
	}

	bool st = Stmt(in, line);
	if( !st ) {
		ParseError(line, "Missing statement for IF");
		return false;
	}

	//Evaluate: execute the if statement
	if((t=Parser::GetNextToken(in, line)) != END ) {

		ParseError(line, "Missing END of IF");
		return false;
	}
	if((t=Parser::GetNextToken(in, line)) != IF ) {

		ParseError(line, "Missing IF at End of IF statement");
		return false;
	}

	return true;
}

bool ReadStmt(istream& in, int& line)
{
	//bool ex = false ;
	LexItem t;
	//cout << "in ReadStmt" << endl;
	if( (t=Parser::GetNextToken(in, line)) != COMA ) {

		ParseError(line, "Missing a Comma");
		return false;
	}

	bool ex = VarList(in, line);

	if( !ex ) {
		ParseError(line, "Missing Variable after Read Statement");
		return false;
	}

	//Evaluate: print out the list of expressions values

	return ex;
}
//IdList:= IDENT {,IDENT}
bool IdList(istream& in, int& line, LexItem & type) {
	bool status = false;
	string identstr;

	LexItem tok = Parser::GetNextToken(in, line);
	if(tok == IDENT)
	{
		//set IDENT lexeme to the type tok value
		identstr = tok.GetLexeme();
		if (defVar.find(identstr)==defVar.end())
		{
			Value v =new Value();
			if(type.GetToken()==INTEGER){
                v.SetType(VINT);
			}
			if(type.GetToken()==REAL){
                v.SetType(VREAL);
			}
			if(type.GetToken()==CHAR){
                v.SetType(VCHAR);
			}
			defVar[identstr] = v;
			SymTable[identstr] = type.GetToken();
		}
		else
		{
			ParseError(line, "Variable Redefinition");
			return false;
		}
	}
	else
	{
		ParseError(line, "Missing Variable");
		return false;
	}

	tok = Parser::GetNextToken(in, line);

	if (tok == COMA) {
		status = IdList(in, line, type);
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}

//VarList
bool VarList(istream& in, int& line)
{
	bool status = false;
	string identstr;
	//cout << "in VarList" << endl;
    LexItem t;
	status = Var(in, line,t);

	if(!status)
	{
		ParseError(line, "Missing Variable");
		return false;
	}

	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == COMA) {
		status = VarList(in, line);
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}

//Var:= ident
bool Var(istream& in, int& line, LexItem &tok)
{
	//called only from the AssignStmt function
	string identstr;
	//cout << "in Var" << endl;
	 tok = Parser::GetNextToken(in, line);

	if (tok == IDENT){
		identstr = tok.GetLexeme();
		if ((defVar.find(identstr))==defVar.end())
		{
			ParseError(line, "Undeclared Variable");
			return false;
		}
		return true;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}

//AssignStmt:= Var = Expr
bool AssignStmt(istream& in, int& line) {
	//cout << "in AssignStmt" << endl;
	bool varstatus = false, status = false;
	LexItem t,left;
    Value v;

	varstatus = Var( in, line,left);
	//cout << "varstatus:" << varstatus << endl;

	if (varstatus){
		t = Parser::GetNextToken(in, line);
		//cout << t.GetLexeme() << endl;
		if (t == ASSOP){
			status = Expr(in, line,v);
			if(!status) {
				ParseError(line, "Missing Expression in Assignment Statment");
				return status;
			}
			//cout << "Value "<<left.GetLexeme();
			Value var = defVar.find(left.GetLexeme())->second;
			if(var.GetType() == VINT) {
                    if(v.GetType() == VINT)  var.SetInt(v.GetInt());
                    if (v.GetType() == VREAL) var.SetInt((int)v.GetReal());
            }
            else {
                var =v;
            }
			var.isinitialized =true;
			defVar.find(left.GetLexeme())->second = var;


		}
		else if(t.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else {
			ParseError(line, "Missing Assignment Operator =");
			return false;
		}
	}
	else {
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	return status;
}

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line,queue<Value> *valq) {
	bool status = false;
	//cout << "in ExprList" << endl;
    Value v;
	status = Expr(in, line,v);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
    valq->push(v);
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == COMA) {
		status = ExprList(in, line,valq);
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}

//Expr:= Term {(+|-) Term}
bool Expr(istream& in, int& line, Value &retVal) {
	Value val1,val2;
	//cout << "in Expr" << endl;
	bool t1 = Term(in, line,val1);
	LexItem tok;

	if( !t1 ) {
		return false;
	}
    //cout << "Expression value " << val1;
	retVal =val1;

	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == PLUS || tok == MINUS)
	{
		t1 = Term(in, line,val2);
		if( !t1 )
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		//evaluate the expression for addition or subtraction
		//and update the retVal object
		//check if the operation of PLUS/MINUS is legal for the
		//type of operands

		if(retVal.GetType()==VCHAR || val2.GetType()==VCHAR){
            ParseError(line,"Run-Time Error-Illegal Mixed Type Operands");
            return false;
		}
		else{
            if(tok == PLUS){
                retVal = retVal+val2;
            }
            else if (tok==MINUS){
                retVal=retVal-val2;
            }
		}

		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}

		//Evaluate: evaluate the expression for addition or subtraction
	}
	Parser::PushBackToken(tok);
	return true;
}

//Term:= SFactor {(*|/) SFactor}
bool Term(istream& in, int& line, Value &retVal) {
	//cout << "in Term" << endl;
	Value op1,op2;
	bool t1 = SFactor(in, line, op1);
	LexItem tok;
	//cout << "status of factor1: " << t1<< endl;
	if( !t1 ) {
		return false;
	}

	tok	= Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	while ( tok == MULT || tok == DIV  )
	{
		t1 = SFactor(in, line,op2);
		//cout << "status of factor2: " << t1<< endl;
		if( !t1 ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}

		if(retVal.GetType()==VCHAR || op2.GetType()==VCHAR){
            ParseError(line,"Run-Time Error-Illegal Mixed Type Operands");
            return false;
		}

		else{

        if(tok == MULT){
            op1=op1*op2;
		}
		if(tok == DIV){
            if((op2==0).GetBool()||(op2==0).GetBool()){
                ParseError(line,"Run-Time Error-Illegal Division by Zero");
                return false;
            }
            op1 = op1/op2;
		}
		}

		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
		//Evaluate: evaluate the expression for multiplication or division
	}
	Parser::PushBackToken(tok);
	retVal = op1;
	return true;
}

//SFactor = Sign Factor | Factor
bool SFactor(istream& in, int& line, Value &retVal)
{
	LexItem t = Parser::GetNextToken(in, line);
	bool status;
	int sign = 1;
	if(t == MINUS )
	{
		sign = -1;
	}
	else if(t == PLUS)
	{
		sign = 1;
	}
	else
		Parser::PushBackToken(t);

	status = Factor(in, line, sign, retVal);
	return status;
}
//LogicExpr = Expr (== | <) Expr
bool LogicExpr(istream& in, int& line, Value &retVal)
{
    Value left,right;
	//cout << "in Logic Expr" << endl;
	bool t1 = Expr(in, line,left);
	LexItem tok;

	if( !t1 ) {
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == LTHAN  || tok == EQUAL)
	{
		t1 = Expr(in, line,right);
		if( !t1 )
		{
			ParseError(line, "Missing expression after relational operator");
			return false;
		}
		if((left.IsBool() && right.IsBool())||(left.IsInt() && (right.IsInt())||(left.IsReal() && (right.IsReal())))) {
            if(tok==LTHAN){
            retVal = left < right;
		}
		else if(tok == EQUAL){
            retVal = left ==right;
		}
		return true;

		}
            else{
                ParseError(line,"Run-Time Error-Illegal Mixed Type operation");
                return false;
            }

	}
	Parser::PushBackToken(tok);
	retVal = left;
	return true;
}

//Factor := ident | iconst | rconst | sconst | (Expr)
bool Factor(istream& in, int& line, int sign, Value &retVal) {
	//cout << "in Factor" << endl;
	LexItem tok = Parser::GetNextToken(in, line);


	if( tok == IDENT ) {
		//check if the var is defined
		//int val;
		string lexeme = tok.GetLexeme();
		if ((defVar.find(lexeme))==defVar.end())
		{
			ParseError(line, "Undefined Variable");
			return false;
		}
		retVal = defVar.find(lexeme)->second;
		if(!retVal.isinitialized){
            ParseError(line, "Undefined Variable");
            return false;
		}

		retVal.SetSign(sign);
		return true;
	}
	else if( tok == ICONST ) {

		retVal = new Value();
		retVal.SetType(VINT);
		retVal.SetInt(stoi(tok.GetLexeme()));
		retVal.SetSign(sign);
		return true;
	}
	else if( tok == SCONST ) {

		retVal = new Value(tok.GetLexeme());
		retVal.SetType(VCHAR);
		retVal.SetChar(tok.GetLexeme());
		return true;
	}
	else if( tok == RCONST ) {

        retVal = new Value(stof(tok.GetLexeme()));
        retVal.SetType(VREAL);
        retVal.SetReal(stof(tok.GetLexeme()));
        retVal.SetSign(sign);
		return true;
	}
	else if( tok == LPAREN ) {
		bool ex = Expr(in, line,retVal);
		if( !ex ) {
			ParseError(line, "Missing expression after (");
			return false;
		}
		if( Parser::GetNextToken(in, line) == RPAREN ){
			retVal.SetSign(sign);
			return ex;
		}

		ParseError(line, "Missing ) after expression");
		return false;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	ParseError(line, "Unrecognized input");
	return false;
}