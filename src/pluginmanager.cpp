#include "pluginmanager.h"
#include <assert.h>
#include <time.h>
#include "dynlib.h"
#include "iplugin.h"

PluginManager::PluginManager():
	IPluginManager(),
	currentPlugin(nullptr),
	currentModule(nullptr)
{
	mnInitTime = time(NULL);
	mnNowTime = mnInitTime;
}

PluginManager::~PluginManager() 
{

}

bool PluginManager::LoadPluginConfig()
{
	pluginNameMap.insert(PluginNameMap::value_type("Tutorial1_library", true));
	return true;
}

bool PluginManager::LoadPlugin()
{
	PluginNameMap::iterator it = pluginNameMap.begin();
	for (; it != pluginNameMap.end(); ++it)
	{
		LoadPluginLibrary(it->first);
	}
	return true;
}

bool PluginManager::Awake()
{
	std::cout << "----Awake----" << std::endl;

	PluginInstanceMap::iterator itAfterInstance = pluginInstanceMap.begin();
	for (; itAfterInstance != pluginInstanceMap.end(); itAfterInstance++)
	{
		SetCurrentPlugin(itAfterInstance->second);
		itAfterInstance->second->Awake();
	}
	return true;
}

bool PluginManager::Init()
{
	std::cout << "----Init----" << std::endl;

	PluginInstanceMap::iterator itInstance = pluginInstanceMap.begin();
	for (; itInstance != pluginInstanceMap.end(); itInstance++)
	{
		SetCurrentPlugin(itInstance->second);
		std::cout << itInstance->first << std::endl;
		itInstance->second->Init();
	}
	return true;
}

bool PluginManager::AfterInit()
{
	std::cout << "----AfterInit----" << std::endl;

	PluginInstanceMap::iterator itAfterInstance = pluginInstanceMap.begin();
	for (; itAfterInstance != pluginInstanceMap.end(); itAfterInstance++)
	{
		SetCurrentPlugin(itAfterInstance->second);
		itAfterInstance->second->AfterInit();
	}
	return true;
}

bool PluginManager::CheckConfig()
{
	std::cout << "----CheckConfig----" << std::endl;

	PluginInstanceMap::iterator itCheckInstance = pluginInstanceMap.begin();
	for (; itCheckInstance != pluginInstanceMap.end(); itCheckInstance++)
	{
		SetCurrentPlugin(itCheckInstance->second);
		itCheckInstance->second->CheckConfig();
	}
	return true;
}

bool PluginManager::ReadyExecute()
{
	std::cout << "----ReadyExecute----" << std::endl;

	PluginInstanceMap::iterator itCheckInstance = pluginInstanceMap.begin();
	for (; itCheckInstance != pluginInstanceMap.end(); itCheckInstance++)
	{
		SetCurrentPlugin(itCheckInstance->second);
		itCheckInstance->second->ReadyExecute();
	}
	return true;
}

bool PluginManager::BeforeShut()
{
	PluginInstanceMap::iterator itBeforeInstance = pluginInstanceMap.begin();
	for (; itBeforeInstance != pluginInstanceMap.end(); itBeforeInstance++)
	{
		SetCurrentPlugin(itBeforeInstance->second);
		itBeforeInstance->second->BeforeShut();
	}
	return true;
}

bool PluginManager::Shut()
{
	PluginInstanceMap::iterator itInstance = pluginInstanceMap.begin();
	for (; itInstance != pluginInstanceMap.end(); ++itInstance)
	{
		SetCurrentPlugin(itInstance->second);
		itInstance->second->Shut();
	}

	return true;
}

bool PluginManager::Finalize()
{
	PluginInstanceMap::iterator itInstance = pluginInstanceMap.begin();
	for (; itInstance != pluginInstanceMap.end(); itInstance++)
	{
		SetCurrentPlugin(itInstance->second);
		itInstance->second->Finalize();
	}

	PluginNameMap::iterator it = pluginNameMap.begin();
	for (; it != pluginNameMap.end(); it++)
	{
		UnLoadPluginLibrary(it->first);
	}

	pluginInstanceMap.clear();
	pluginNameMap.clear();

	return true;
}

void PluginManager::Registered(IPlugin* plugin)
{
	const std::string& pluginName = plugin->GetPluginName();
	if (!FindPlugin(pluginName))
	{
		pluginInstanceMap.insert(PluginInstanceMap::value_type(pluginName, plugin));
		plugin->Install();
	}
	else
	{
		std::cout << pluginName << std::endl;
		assert(0);
	}
}

void PluginManager::UnRegistered(IPlugin* plugin)
{
	PluginInstanceMap::iterator it = pluginInstanceMap.find(plugin->GetPluginName());
	if (it != pluginInstanceMap.end())
	{
		it->second->Uninstall();
		delete it->second;
		it->second = NULL;
		pluginInstanceMap.erase(it);
	}
}

bool PluginManager::ReLoadPlugin(const std::string& pluginDLLName)
{
	//1.shut all module of this plugin
	//2.unload this plugin
	//3.load new plugin
	//4.init new module
	//5.tell others who has been reloaded
	PluginInstanceMap::iterator itInstance = pluginInstanceMap.find(pluginDLLName);
	if (itInstance == pluginInstanceMap.end())
	{
		return false;
	}
	//1
	IPlugin* pPlugin = itInstance->second;
	pPlugin->BeforeShut();
	pPlugin->Shut();
	pPlugin->Finalize();

	//2
	PluginLibMap::iterator it = pluginLibMap.find(pluginDLLName);
	if (it != pluginLibMap.end())
	{
		DynLib* pLib = it->second;
		DLL_STOP_PLUGIN_FUNC pFunc = (DLL_STOP_PLUGIN_FUNC)pLib->GetSymbol("DllStopPlugin");

		if (pFunc)
		{
			pFunc(this);
		}

		pLib->UnLoad();

		delete pLib;
		pLib = NULL;
		pluginLibMap.erase(it);
	}

	//3
	DynLib* pLib = new DynLib(pluginDLLName);
	bool bLoad = pLib->Load();
	if (bLoad)
	{
		pluginLibMap.insert(PluginLibMap::value_type(pluginDLLName, pLib));

		DLL_START_PLUGIN_FUNC pFunc = (DLL_START_PLUGIN_FUNC)pLib->GetSymbol("DllStartPlugin");
		if (!pFunc)
		{
			std::cout << "Reload Find function DllStartPlugin Failed in [" << pLib->GetName() << "]" << std::endl;
			assert(0);
			return false;
		}

		pFunc(this);
	}
	else
	{
#if PLATFORM == PLATFORM_LINUX
		char* error = dlerror();
		if (error)
		{
			std::cout << stderr << " Reload shared lib[" << pLib->GetName() << "] failed, ErrorNo. = [" << error << "]" << std::endl;
			std::cout << "Reload [" << pLib->GetName() << "] failed" << std::endl;
			assert(0);
			return false;
		}
#elif PLATFORM == PLATFORM_WIN
		std::cout << stderr << " Reload DLL[" << pLib->GetName() << "] failed, ErrorNo. = [" << GetLastError() << "]" << std::endl;
		std::cout << "Reload [" << pLib->GetName() << "] failed" << std::endl;
		assert(0);
		return false;
#endif // NF_PLATFORM
	}

	//4
	PluginInstanceMap::iterator itReloadInstance = pluginInstanceMap.begin();
	for (; itReloadInstance != pluginInstanceMap.end(); itReloadInstance++)
	{
		if (pluginDLLName != itReloadInstance->first)
		{
			itReloadInstance->second->OnReloadPlugin();
		}
	}
	return true;
}

IPlugin* PluginManager::FindPlugin(const std::string& pluginName)
{
	PluginInstanceMap::iterator it = pluginInstanceMap.find(pluginName);
	if (it != pluginInstanceMap.end())
	{
		return it->second;
	}
	return nullptr;
}

void PluginManager::AddModule(const std::string& moduleName, IModule* pModule)
{
	if (!FindModule(moduleName))
	{
		moduleInstanceMap.insert(ModuleInstanceMap::value_type(moduleName, pModule));
	}
}

void PluginManager::RemoveModule(const std::string& moduleName)
{
	ModuleInstanceMap::iterator it = moduleInstanceMap.find(moduleName);
	if (it != moduleInstanceMap.end())
	{
		moduleInstanceMap.erase(it);

// 		auto iter = std::find_if(mNeedExecuteModuleVec.begin(),
// 			mNeedExecuteModuleVec.end(),
// 			[&moduleName](const std::pair<std::string, IModule*>& xPair) ->bool {
// 				return xPair.first == moduleName;
// 			});
// 
// 		if (iter != mNeedExecuteModuleVec.end())
// 			mNeedExecuteModuleVec.erase(iter);
	}
}

IModule* PluginManager::FindModule(const std::string& moduleName)
{
	if (moduleName.empty()) {
		return nullptr;
	}

	ModuleInstanceMap::iterator it = moduleInstanceMap.find(moduleName);
	if (it != moduleInstanceMap.end())
	{
		return it->second;
	}

	if (this->GetCurrentModule())
	{
		std::cout << this->GetCurrentModule()->name << " want to find module: " << moduleName << " but got null_ptr!!!" << std::endl;
	}

	return nullptr;
}

std::list<IModule*> PluginManager::Modules()
{
	std::list<IModule*> xModules;

	ModuleInstanceMap::iterator itCheckInstance = moduleInstanceMap.begin();
	for (; itCheckInstance != moduleInstanceMap.end(); itCheckInstance++)
	{
		xModules.push_back(itCheckInstance->second);
	}
	return xModules;
}

bool PluginManager::Execute()
{
	mnNowTime = time(NULL);

	bool bRet = true;

	for (auto& xPair : mNeedExecuteModuleVec)
	{
		bool tembRet = xPair.second->Execute();
		bRet = bRet && tembRet;
	}
	return bRet;
}

bool PluginManager::LoadPluginLibrary(const std::string& pluginDLLName)
{
	PluginLibMap::iterator it = pluginLibMap.find(pluginDLLName);
	if (it == pluginLibMap.end()) 
	{
		DynLib* pLib = new DynLib(pluginDLLName);
		bool bLoad = pLib->Load();

		if (bLoad) {
			pluginLibMap.insert(PluginLibMap::value_type(pluginDLLName, pLib));

			DLL_START_PLUGIN_FUNC pFunc = (DLL_START_PLUGIN_FUNC)pLib->GetSymbol("DllStartPlugin");
			if (!pFunc)
			{
				std::cout << "Find function DllStartPlugin Failed in [" << pLib->GetName() << "]" << std::endl;
				assert(0);
				return false;
			}

			pFunc(this);
			return true;
		}
		else {
#if PLATFORM == PLATFORM_LINUX || PLATFORM == PLATFORM_APPLE
			char* error = dlerror();
			if (error)
			{
				std::cout << stderr << " Load shared lib[" << pLib->GetName() << "] failed, ErrorNo. = [" << error << "]" << std::endl;
				std::cout << "Load [" << pLib->GetName() << "] failed" << std::endl;
				assert(0);
				return false;
			}
#elif PLATFORM == PLATFORM_WIN
			std::cout << stderr << " Load DLL[" << pLib->GetName() << "] failed, ErrorNo. = [" << GetLastError() << "]" << std::endl;
			std::cout << "Load [" << pLib->GetName() << "] failed" << std::endl;
			assert(0);
			return false;
#endif // NF_PLATFORM
		}
	}

	return true;
}

bool PluginManager::UnLoadPluginLibrary(const std::string& pluginDLLName)
{
	PluginLibMap::iterator it = pluginLibMap.find(pluginDLLName);
	if (it != pluginLibMap.end())
	{
		DynLib* pLib = it->second;
		DLL_STOP_PLUGIN_FUNC pFunc = (DLL_STOP_PLUGIN_FUNC)pLib->GetSymbol("DllStopPlugin");
		if (pFunc)
		{
			pFunc(this);
		}

		pLib->UnLoad();

		delete pLib;
		pLib = nullptr;
		pluginLibMap.erase(it);

		return true;
	}
	return false;
}
