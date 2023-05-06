
#include "plugin.h"
namespace Plugin {

}

//extern "C"
//{
//	Plugin::PObject* PluginInstance()
//	{
//		return new Plugin::Plugin();
//	}
//}

// 不需要显示在.h中声明导出函数 因为这个是作为插件动态使用的
extern "C"
{
	PLUGIN_EXPORT Plugin::PObject* PluginInstance()
	{
		return new Plugin::Plugin();
	}
}

