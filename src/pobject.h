#pragma once
#ifndef __POBJECT_H__
#define __POBJECT_H__

#include <iostream>

#if defined(PLUGIN_STATICLIB)
#define PLUGIN_EXPORT
#elif defined(_MSC_VER)
#if defined(PLUGIN_DYNAMICLIB) || defined(PLUGIN_EXPORTS)
#define PLUGIN_EXPORT  __declspec(dllexport)
#else
#define PLUGIN_EXPORT  __declspec(dllimport)
#endif
#elif defined(__GNUC__)
#define PLUGIN_EXPORT  __attribute__((visibility("default")))
#else
#define PLUGIN_EXPORT
#endif

namespace Plugin{

	class CNotify;
	class PObject
	{
	public:
		PObject() {}
		virtual ~PObject() {}

		// 插件详细
		virtual std::string Dump()		 = 0;

		// 插件名称
		virtual std::string ModuleName() = 0;
	
		// 插件初始化
		virtual int Init()				 = 0;
	
		// 插件退出
		virtual int Exit()				 = 0;

		// 启动
		virtual int Start()				 = 0;
	
		// 暂停
		virtual int Stop()				 = 0;
	
		// 取消
		virtual int Cancel()			 = 0;

		// 消息回调通知
		virtual int SetNotity(CNotify* pNotify) {
			this->m_pNotify = pNotify;
			return 0;
		}

	private:
		CNotify* m_pNotify = {nullptr};
	};


	class CNotify
	{
	public:
		CNotify(){}
		virtual ~CNotify(){}

		virtual void Progress(int now, int max) = 0;

		virtual void WeakPassWord(std::string &user, std::string &password) = 0;

		virtual void MsgResult(const int &code)			= 0;

		virtual void MsgResult(const std::string& code) = 0;

	protected:
	private:
	};
}

#endif // __POBJECT_H__