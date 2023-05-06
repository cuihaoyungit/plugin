#include "dynlib.h"

/*
bool DynLib::Load()
{
	std::string strlibPath = "./";
	strlibPath += libName;
	mInst = (DYNLIB_HANDLE)DYNLIB_LOAD(strlibPath.c_str());

	return mInst != nullptr;
}

bool DynLib::UnLoad()
{
	DYNLIB_UNLOAD(mInst);
	return true;
}

void* DynLib::GetSymbol(const char* szProcName)
{
	return (DYNLIB_HANDLE)DYNLIB_GETSYM(mInst, szProcName);
}

*/