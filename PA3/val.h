#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>
#include <queue>
#include <map>

using namespace std;

enum ValType { VINT, VREAL, VCHAR, VBOOL, VERR };

class Value {
    ValType	T;
     bool    Btemp;
	int		Itemp;
	float   Rtemp;
    string	Stemp;

public:
    Value() : T(VERR), Itemp(0), Rtemp(0.0){}
    Value(bool vb) : T(VBOOL), Btemp(vb), Itemp(0), Rtemp(0.0) {}
    Value(int vi) : T(VINT), Itemp(vi) {}
	Value(float vr) : T(VREAL), Itemp(0), Rtemp(vr) {}
    Value(string vs) : T(VCHAR), Itemp(0), Rtemp(0.0), Stemp(vs) {}


    ValType GetType() const { return T; }
    bool IsErr() const { return T == VERR; }
    bool IsInt() const { return T == VINT; }
    bool IsChar() const { return T == VCHAR; }
    bool IsReal() const {return T == VREAL;}
    bool IsBool() const {return T == VBOOL;}

    int GetInt() const { if( IsInt() ) return Itemp; throw "RUNTIME ERROR: Value not an integer"; }

    string GetChar() const { if( IsChar() ) return Stemp; throw "RUNTIME ERROR: Value not a string"; }

    float GetReal() const { if( IsReal() ) return Rtemp; throw "RUNTIME ERROR: Value not an integer"; }

    bool GetBool() const {if(IsBool()) return Btemp; throw "RUNTIME ERROR: Value not a boolean";}

    bool isinitialized = false;
    void SetType(ValType type)
    {
    	T = type;
	}

    void SetInt(int val)
    {
    	Itemp = val;
	}

	void SetReal(float val)
    {
    	Rtemp = val;
	}

	void SetChar(string val)
    {
    	Stemp = val;
	}

	void SetBool(bool val)
    {
    	Btemp = val;
	}

	void SetSign(int sign)
	{
	    if (IsInt()){
            SetInt(sign*GetInt());
	    }
	    else if (IsReal()){
            SetReal(sign*GetReal());
	    }
	}


    // add op to this
    Value operator+(const Value& op) const;

    // subtract op from this
    Value operator-(const Value& op) const;

    // multiply this by op
    Value operator*(const Value& op) const;

    // divide this by op
    Value operator/(const Value& op) const;

    Value operator==(const Value& op) const;

	Value operator<(const Value& op) const;

    friend ostream& operator<<(ostream& out, const Value& op) {
        if( op.IsInt() ) out << op.Itemp;
        else if( op.IsChar() ) out << op.Stemp;
        else if( op.IsReal()) out << op.Rtemp;
        else out << "ERROR";
        return out;
    }
};


#endif
