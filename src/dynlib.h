#pragma once
#ifndef __DYNLIB_H__
#define __DYNLIB_H__

#include <stdio.h>
#include <iostream>
#include "platform.h"

class DynLib
{
public:
	DynLib(const std::string& name)
	{
		libName = name;
#ifdef DEBUG_MODE
		//libName.append("");
#endif

#if PLATFORM == PLATFORM_WIN
		libName.append(".dll");
#elif PLATFORM == PLATFORM_LINUX || PLATFORM == PLATFORM_ANDROID
		libName.append(".so");
#elif PLATFORM == PLATFORM_APPLE || PLATFORM == PLATFORM_APPLE_IOS
		libName.append(".so");
#endif
		printf("LoadPlugin:%s\n", libName.c_str());
	}

	~DynLib()
	{

	}

	bool Load()
	{
		std::string strlibPath = "./";
		strlibPath += libName;
		mInst = (DYNLIB_HANDLE)DYNLIB_LOAD(strlibPath.c_str());

		return mInst != nullptr;
	}

	bool UnLoad()
	{
		DYNLIB_UNLOAD(mInst);
		return true;
	}

	const std::string& GetName(void) const
	{
		return libName;
	}

	const bool GetMain(void) const
	{
		return false;
	}

	void* GetSymbol(const char* szProcName)
	{
		return (DYNLIB_HANDLE)DYNLIB_GETSYM(mInst, szProcName);
	}

protected:

	std::string libName;

	DYNLIB_HANDLE mInst = {nullptr};
};


#endif // __DYNLIB_H__