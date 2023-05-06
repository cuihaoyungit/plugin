#pragma once
#ifndef __IPluginManager_H__
#define __IPluginManager_H__
#include <list>
#include <iostream>
#include <intsafe.h>

enum CLASS_OBJECT_EVENT
{
	COE_CREATE_NODATA,
	COE_CREATE_BEFORE_ATTACHDATA,
	COE_CREATE_LOADDATA,
	COE_CREATE_AFTER_ATTACHDATA,
	COE_CREATE_BEFORE_EFFECT,
	COE_CREATE_EFFECTDATA,
	COE_CREATE_AFTER_EFFECT,
	COE_CREATE_READY,
	COE_CREATE_HASDATA,
	COE_CREATE_FINISH,
	COE_CREATE_CLIENT_FINISH,
	COE_BEFOREDESTROY,
	COE_DESTROY,
};

template<typename DerivedType, typename BaseType>
class TIsDerived
{
public:
	static int AnyFunction(BaseType*)
	{
		return 1;
	}

	static  char AnyFunction(void*)
	{
		return 0;
	}

	enum
	{
		Result = (sizeof(int) == sizeof(AnyFunction((DerivedType*)nullptr))),
	};
};

class IPlugin;
class IModule;

class IPluginManager
{
public:
	IPluginManager()
	{

	}
	
	virtual ~IPluginManager(){}

	virtual bool LoadPluginConfig()
	{
		return true;
	}

	virtual bool LoadPlugin()
	{
		return true;
	}

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

	template <typename T>
	T* FindModule()
	{
		IModule* pLogicModule = FindModule(typeid(T).name());
		if (pLogicModule)
		{
			if (!TIsDerived<T, IModule>::Result)
			{
				return nullptr;
			}
			//TODO OSX…œdynamic_cast∑µªÿ¡ÀNULL
#if PLATFORM == PLATFORM_APPLE
			T* pT = (T*)pLogicModule;
#else
			T* pT = dynamic_cast<T*>(pLogicModule);
#endif
			assert(nullptr != pT);
			return pT;
		}
		assert(nullptr);
		return nullptr;
	}

	template <typename T>
	void ReplaceModule(IModule* pModule)
	{
		IModule* pLogicModule = FindModule(typeid(T).name());
		if (pLogicModule)
		{
			RemoveModule(typeid(T).name());
		}

		AddModule(typeid(T).name(), pModule);
	};

	virtual bool ReLoadPlugin(const std::string& pluginDLLName) = 0;

	virtual void Registered(IPlugin* plugin) = 0;
	virtual void UnRegistered(IPlugin* plugin) = 0;

	virtual IPlugin* FindPlugin(const std::string& pluginName) = 0;
	virtual void AddModule(const std::string& moduleName, IModule* pModule) = 0;
	virtual void RemoveModule(const std::string& moduleName) = 0;
	virtual IModule* FindModule(const std::string& moduleName) = 0;

	virtual std::list<IModule*> Modules() = 0;

	virtual int GetAppID() const = 0;
	virtual void SetAppID(const int appID) = 0;

	virtual int GetAppType() const = 0;
	virtual void SetAppType(const int type) = 0;

	virtual INT64 GetInitTime() const = 0;
	virtual INT64 GetNowTime() const = 0;

	virtual const std::string& GetConfigPath() const = 0;
	virtual void SetConfigPath(const std::string& strPath) = 0;

	virtual void SetConfigName(const std::string& fileName) = 0;
	virtual const std::string& GetConfigName() const = 0;

	virtual const std::string& GetAppName() const = 0;
	virtual void SetAppName(const std::string& appName) = 0;

	virtual const std::string& GetLogConfigName() const = 0;
	virtual void SetLogConfigName(const std::string& name) = 0;

	virtual IPlugin* GetCurrentPlugin() = 0;
	virtual IModule* GetCurrentModule() = 0;

	virtual void SetCurrentPlugin(IPlugin* pPlugin) = 0;
	virtual void SetCurrentModule(IModule* pModule) = 0;

	virtual int GetAppCPUCount() const = 0;
	virtual void SetAppCPUCount(const int count) = 0;

};





#endif // __IPluginManager_H__
