
#include "val.h"




// add this with op 
    Value Value::operator+(const Value& op) const {
        if( GetType() == op.GetType() )
        {
        	if( IsInt() ) return Value( Itemp + op.GetInt() );
        	if( IsReal() ) return Value( Rtemp + op.GetReal() );
		}
		else if(IsInt() && op.IsReal())
		{
			
			return Value( (float) GetInt() + op.GetReal());
		}
		else if(IsReal() && op.IsInt())
		{
			return Value(GetReal() + (float) op.GetInt());
		}
		
		return Value();
		
    }
    
    // subtract op from this
    Value Value::operator-(const Value& op) const {
        if( GetType() == op.GetType() )
        {
        	if( IsInt() ) return Value( Itemp - op.GetInt() );
        	if( IsReal() ) return Value( Rtemp - op.GetReal() );
		}
		else if(IsInt() && op.IsReal())
		{
			
			return Value( (float) GetInt() - op.GetReal());
		}
		else if(IsReal() && op.IsInt())
		{
			return Value(GetReal() - (float) op.GetInt());
		}
		
		return Value();
		
    }
    
    // multiply this with op
    Value Value::operator*(const Value& op) const {
        if( GetType() == op.GetType() )
        {
        	if( IsInt() ) return Value( Itemp * op.GetInt() );
        	if( IsReal() ) return Value( Rtemp * op.GetReal() );
		}
		else if(IsInt() && op.IsReal())
		{
			
			return Value( (float) GetInt() * op.GetReal());
		}
		else if(IsReal() && op.IsInt())
		{
			return Value(GetReal() * (float) op.GetInt());
		}
		
		return Value();
		
    }
    
    // divide this by op
    Value Value::operator/(const Value& op) const {
        if( GetType() == op.GetType() )
        {
        	if( IsInt() ) return Value( Itemp / op.GetInt() );
        	if( IsReal() ) return Value( Rtemp / op.GetReal() );
		}
		else if(IsInt() && op.IsReal())
		{
			
			return Value( (float) GetInt() / op.GetReal());
		}
		else if(IsReal() && op.IsInt())
		{
			return Value(GetReal() / ((float) op.GetInt()));
		}
		
		return Value();
		
    }
    
    Value Value::operator==(const Value& op) const {
        if( GetType() == op.GetType() )
        {
        	if( IsInt() ) return Value( (bool)(Itemp == op.GetInt() ));
        	if( IsReal() ) return Value( (bool) (Rtemp == op.GetReal() ));
        	if( IsChar()) return Value( (bool) (Stemp < op.GetChar()));
		}
		else if(IsInt() && op.IsReal())
		{
			
			return Value( (bool) (((float) GetInt())== op.GetReal()));
		}
		else if(IsReal() && op.IsInt())
		{
			return Value( (bool) (GetReal() == ((float) op.GetInt())));
		}
		
		return Value();
		
    }
    
    Value Value::operator<(const Value& op) const {
        if( GetType() == op.GetType() )
        {
        	if( IsInt() ) return Value( Itemp < op.GetInt() );
        	if( IsReal() ) return Value( Rtemp < op.GetReal() );
        	if( IsChar()) return Value( Stemp < op.GetChar());
		}
		else if(IsInt() && op.IsReal())
		{
			
			return Value( ((float) GetInt()) < op.GetReal());
		}
		else if(IsReal() && op.IsInt())
		{
			return Value(GetReal() < ((float) op.GetInt()));
		}
		
		return Value();
		
    }
