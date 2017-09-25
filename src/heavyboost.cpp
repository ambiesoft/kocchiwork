#include "stdafx.h"


#include "heavyboost.h"

tstring doformat(tstring ins)
{
	return ( boost::wformat(NS("%1% is not a file.")) % ins.c_str() ).str();
}

tstring dolex(int i)
{
	return boost::lexical_cast<tstring>(i);
}
