#pragma once
#ifndef __PluginManager_H__
#define __PluginManager_H__
#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <intsafe.h>
#include "ipluginmanager.h"
#include "dynlib.h"

class IPlugin;
class IModule;
class DynLib;
class PluginManager : public IPluginManager
{
public:
	typedef std::map<std::string, IPlugin*> PluginInstanceMap;
	typedef std::map<std::string, IModule*> ModuleInstanceMap;
	typedef std::map<std::string, DynLib*>  PluginLibMap;
	typedef std::map<std::string, bool>     PluginNameMap;
	typedef std::vector<std::pair<std::string, IModule*>> NeedExecuteModuleVec;

	typedef void(*DLL_START_PLUGIN_FUNC)(IPluginManager* pm);
	typedef void(*DLL_STOP_PLUGIN_FUNC) (IPluginManager* pm);

	PluginManager();
	virtual ~PluginManager();

	virtual bool LoadPluginConfig() override;

	virtual bool LoadPlugin() override;

	virtual bool Awake() override;

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool CheckConfig() override;

	virtual bool ReadyExecute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	virtual bool Finalize() override;

	virtual void Registered(IPlugin* plugin) override;

	virtual void UnRegistered(IPlugin* plugin) override;

	virtual bool ReLoadPlugin(const std::string& pluginDLLName) override;
	virtual IPlugin* FindPlugin(const std::string& pluginName) override;

	virtual void     AddModule(const std::string& moduleName, IModule* pModule) override;
	virtual void     RemoveModule(const std::string& moduleName) override;
	virtual IModule* FindModule(const std::string& moduleName) override;

	virtual std::list<IModule*> Modules() override;

	virtual bool Execute() override;

	virtual int GetAppID() const override
	{
		return appID;
	}
	virtual void SetAppID(const int appID) override
	{
		this->appID = appID;
	}

	virtual int GetAppType() const override
	{
		return appType;
	}
	virtual void SetAppType(const int type) override
	{
		this->appType = type;
	}

	virtual INT64 GetInitTime() const override
	{
		return this->mnInitTime;
	}
	virtual INT64 GetNowTime() const override
	{
		return this->mnNowTime;
	}

	virtual const std::string& GetConfigPath() const override
	{
		return this->configPath;
	}
	virtual void SetConfigPath(const std::string& strPath) override
	{
		this->configPath = strPath;
	}

	virtual void SetConfigName(const std::string& fileName) override
	{
		this->configName = fileName;
	}
	virtual const std::string& GetConfigName() const override
	{
		return this->configName;
	}

	virtual const std::string& GetAppName() const override
	{
		return this->appName;
	}
	virtual void SetAppName(const std::string& appName) override
	{
		this->appName = appName;
	}

	virtual const std::string& GetLogConfigName() const override
	{
		return this->logConfigName;
	}
	virtual void SetLogConfigName(const std::string& name) override
	{
		this->logConfigName = name;
	}

	virtual IPlugin* GetCurrentPlugin() override
	{
		return currentPlugin;
	}
	virtual IModule* GetCurrentModule() override
	{
		return currentModule;
	}

	virtual void SetCurrentPlugin(IPlugin* pPlugin) override
	{
		this->currentPlugin = pPlugin;
	}
	virtual void SetCurrentModule(IModule* pModule) override
	{
		this->currentModule = pModule;
	}

	virtual int GetAppCPUCount() const override
	{
		return mnCPUCount;
	}
	virtual void SetAppCPUCount(const int count) override
	{
		this->mnCPUCount = count;
	}

protected:
	bool LoadPluginLibrary(const std::string& pluginDLLName);
	bool UnLoadPluginLibrary(const std::string& pluginDLLName);

private:
	INT64 mnInitTime = { 0 };
	INT64 mnNowTime  = { 0 };
	INT   mnCPUCount = { 8 };

	int appID   = 0;
	int appType = 0;
	std::string configPath;
	std::string configName;
	std::string appName;
	std::string logConfigName;

	IPlugin* currentPlugin = { nullptr };
	IModule* currentModule = { nullptr };

	PluginInstanceMap    pluginInstanceMap;    // 插件实例化对象
	ModuleInstanceMap    moduleInstanceMap;    // Module实例对象
	PluginLibMap         pluginLibMap;         // 动态库
	PluginNameMap        pluginNameMap;        // 插件配置文件信息
	NeedExecuteModuleVec mNeedExecuteModuleVec;// 循环执行的Module
};





#endif // __PluginManager_H__