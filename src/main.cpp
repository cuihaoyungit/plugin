#include "pobject.h"
#include "plugin.h"

#include <windows.h>
#include "pluginmanager.h"
#include <memory>

//# 针对插件的几点改动和优化
//#1 插件.h文件中的导出函数声明 不是必须的 注销了
//#2 在.cpp中直接声明导出函数和实现体
//#3 在工程的导出模式 宏定义 dll 模式 exports

int main(int argc, char *argv[])
{
	// test1
	{
		typedef Plugin::PObject* Instance();
		void* handle = LoadLibraryA("./hello_library.dll");
		if (!handle)
		{
			std::cout << GetLastError() << std::endl;
			return 1;
		}

		Instance* GetInstance = (Instance*)GetProcAddress((HMODULE)handle, "PluginInstance");
		if (!GetInstance)
		{
			std::cout << "Error Instance function" << std::endl;
			return 1;
		}

		Plugin::PObject* obj = GetInstance();
		if (nullptr != obj)
		{
			obj->Dump();
		}

		delete obj;
		obj = nullptr;
		FreeLibrary((HMODULE)handle);
	}


	// test2
	{
		std::shared_ptr<IPluginManager> pPluginManager(new PluginManager());

		pPluginManager->LoadPluginConfig();
		pPluginManager->LoadPlugin();

		pPluginManager->Awake();
		pPluginManager->Init();
		pPluginManager->AfterInit();
		pPluginManager->CheckConfig();
		pPluginManager->ReadyExecute();
		pPluginManager->Execute();
		pPluginManager->BeforeShut();
		pPluginManager->Shut();
		pPluginManager->Finalize();
		pPluginManager = nullptr;
	}
    return 0;
}
