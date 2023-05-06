// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once
#ifndef __Plugin_H__
#define __Plugin_H__

#include "pobject.h"
#include <iostream>

namespace Plugin
{

	class Plugin : public PObject
	{
	public:
		Plugin() = default;
		virtual ~Plugin() {}

		virtual int Init() override
		{
			return 0;
		}

		virtual int Exit() override
		{
			return 0;
		}

		virtual int Start() override
		{
			return 0;
		}

		virtual int Stop() override
		{
			return 0;
		}

		virtual int Cancel() override
		{
			return 0;
		}

		virtual std::string Dump() override
		{
			return "Plugin Version 1.0.0";
		}

		virtual std::string ModuleName() override {
			return "Plugin.dll";
		}

	protected:
	private:
	};
}

//extern "C" {
//	PLUGIN_EXPORT Plugin::PObject* PluginInstance();
//}

//extern "C" {
//	PLUGIN_EXPORT Plugin::PObject* PluginInstance();
//}


// ²å¼þÊ¹ÓÃ
/*
	typedef Object* Instance();
 	void* handle = LoadLibraryA("./pluginObj.dll");
	if (!handle)
	{
		std::cout << GetLastError() << std::endl;
		return 1;
	}

	Instance* GetInstance = (Instance*)GetProcAddress((HMODULE)handle, "Instance");
	if (!GetInstance)
	{
		std::cout << "Error Instance function" << std::endl;
		return 1;
	}

	Object* obj = GetInstance();
	if (nullptr != obj)
	{
		obj->Dump();
	}

	delete obj;
	FreeLibrary((HMODULE)handle);
*/



#endif // __Plugin_H__