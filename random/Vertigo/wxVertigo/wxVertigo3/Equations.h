#ifndef _EQUATIONS_
#define _EQUATIONS_

#include "enum.h"
#include "IFS.h"

//class C_IFS();

class C_Equations
{
public:
	C_Equations()
	{
		equationName = ID_MISC;
		equationType = ID_Title;

		// Attempting to pass the parent to the child IFS class, to access equationName
		IFS = new C_IFS();
	};
	~C_Equations(){};

	C_IFS *IFS;
	int equationName, equationType;

private:
	//Nothing
};

#endif