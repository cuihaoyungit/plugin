#pragma once
#ifndef __IMODULE_H__
#define __IMODULE_H__
#include <iostream>
#include <assert.h>

class IPluginManager;
class IModule
{

public:
	IModule():
		pPluginManager(nullptr)
	{
	}

	virtual ~IModule() {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Init()
	{
		return true;
	}

	virtual bool AfterInit()
	{
		return true;
	}

	virtual bool CheckConfig()
	{
		return true;
	}

	virtual bool ReadyExecute()
	{
		return true;
	}

	virtual bool Execute()
	{
		return true;
	}

	virtual bool BeforeShut()
	{
		return true;
	}

	virtual bool Shut()
	{
		return true;
	}

	virtual bool Finalize()
	{
		return true;
	}

	virtual bool OnReloadPlugin()
	{
		return true;
	}

	virtual IPluginManager* GetPluginManager() const
	{
		return pPluginManager;
	}
	std::string name;
protected:
	IPluginManager* pPluginManager;
};


#endif // __IMODULE_H__