#pragma once
#ifndef __Platform_H__
#define __Platform_H__

#define PLATFORM_WIN       1
#define PLATFORM_LINUX     2
#define PLATFORM_APPLE     3
#define PLATFORM_SYMBIAN   4
#define PLATFORM_APPLE_IOS 5
#define PLATFORM_ANDROID   6

/* Platform defined */
/* Finds the current platform */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined( __WIN32__ ) || defined( _WIN32 ) || defined(_WINDOWS) || defined(WIN) || defined(_WIN64) || defined( __WIN64__ )
#   define PLATFORM PLATFORM_WIN
//////////////////////////////////////////////////////////////////////////
#elif defined( __APPLE_CC__) || defined(__APPLE__) || defined(__OSX__)
// Device                                                     Simulator
// Both requiring OS version 4.0 or greater
#   if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 40000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 40000
#       define PLATFORM PLATFORM_APPLE_IOS
#   else
#       define PLATFORM PLATFORM_APPLE
#   endif
//////////////////////////////////////////////////////////////////////////
#elif defined(linux) && defined(__arm__)
// TODO: This is NOT the correct way to detect the Tegra 2 platform but it works for now.
// It doesn't appear that GCC defines any platform specific macros.
#   define PLATFORM PLATFORM_TEGRA2
#elif defined(__ANDROID__)
#   define PLATFORM PLATFORM_ANDROID
//////////////////////////////////////////////////////////////////////////
#elif defined( __native_client__ )
#   define PLATFORM PLATFORM_NACL
#   ifndef STATIC_LIB
#       error must be built as static for NaCl (NF_STATIC=true in cmake)
#   endif
#   ifdef BUILD_RENDERSYSTEM_D3D9
#       error d3d9 is nor supported on NaCl (ONF_BUILD_RENDERSYSTEM_D3D9 false in cmake)
#   endif
#   ifdef BUILD_RENDERSYSTEM_GL
#       error gl is nor supported on NaCl (ONF_BUILD_RENDERSYSTEM_GL=false in cmake)
#   endif
#   ifndef BUILD_RENDERSYSTEM_GLES2
#       error GLES2 render system is needed for NaCl (ONF_BUILD_RENDERSYSTEM_GLES2=false in cmake)
#   endif
#else
#   define PLATFORM PLATFORM_LINUX
#endif

#if PLATFORM == PLATFORM_WIN
#include <Windows.h>
#    define DYNLIB_HANDLE hInstance
#    define DYNLIB_LOAD( a ) LoadLibraryExA( a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH )
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) FreeLibrary( a )

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

#elif PLATFORM == PLATFORM_LINUX || PLATFORM == PLATFORM_ANDROID
#include <dlfcn.h>
#define DYNLIB_HANDLE void*
#define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY | RTLD_GLOBAL)
#define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#define DYNLIB_UNLOAD( a ) dlclose( a )

#elif PLATFORM == PLATFORM_APPLE || PLATFORM == PLATFORM_APPLE_IOS
#include <dlfcn.h>
#define DYNLIB_HANDLE void*
#define DYNLIB_LOAD( a ) dlopen( a, RTLD_LOCAL|RTLD_LAZY)
#define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#define DYNLIB_UNLOAD( a ) dlclose( a )

#endif


//----------------------------------------------------------------------------
// Windows Settings
#if PLATFORM == PLATFORM_WIN

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

#pragma warning(disable:4091) 
#include <Windows.h>
#define EXPORT extern "C"  __declspec(dllexport)

#include <Dbghelp.h>
// If we're not including this from a client build, specify that the stuff
// should get exported. Otherwise, import it.
#if defined( STATIC_LIB )
// Linux compilers don't have symbol import/export directives.
#       define Export
#       define Private
#else
#       if defined( NONCLIENT_BUILD )
#           define Export __declspec( dllexport )
#       else
#           if defined( __MINGW32__ )
#               define Export
#           else
#               define Export __declspec( dllimport )
#           endif
#       endif
#       define Private
#   endif
// Win32 compilers use _DEBUG for specifying debug builds.
// for MinGW, we set DEBUG
#   if defined(_DEBUG) || defined(DEBUG)
#       define DEBUG_MODE 1
#   endif

// Disable unicode support on MingW for GCC 3, poorly supported in stdlibc++
// STLPORT fixes this though so allow if found
// MinGW C++ Toolkit supports unicode and sets the define __MINGW32_TOOLBOX_UNICODE__ in _mingw.h
// GCC 4 is also fine
#if defined(__MINGW32__)
# if NF_COMP_VER < 400
#  if !defined(_STLPORT_VERSION)
#   include<_mingw.h>
#   if defined(__MINGW32_TOOLBOX_UNICODE__) || COMP_VER > 345
#    define UNICODE_SUPPORT 1
#   else
#    define UNICODE_SUPPORT 0
#   endif
#  else
#   define UNICODE_SUPPORT 1
#  endif
# else
#  define UNICODE_SUPPORT 1
# endif
#else
#  define UNICODE_SUPPORT 1
#endif

#endif // PLATFORM == PLATFORM_WIN
//----------------------------------------------------------------------------
// Android Settings
/*
#if PLATFORM == PLATFORM_ANDROID
#   define Export
#   define UNICODE_SUPPORT 1
#   define DEBUG_MODE 0
#   define Private
#   define CLOCKS_PER_SEC  1000
//  pragma def were found here: http://www.inf.pucrs.br/~eduardob/disciplinas/SistEmbarcados/Mobile/Nokia/Tools/Carbide_vs/WINSCW/Help/PDF/C_Compilers_Reference_3.2.pdf
#   pragma warn_unusedarg off
#   pragma warn_emptydecl off
#   pragma warn_possunwant off
// A quick define to overcome different names for the same function
#   define stricmp strcasecmp
#   ifdef DEBUG
#       define NF_DEBUG_MODE 1
#   else
#       define NF_DEBUG_MODE 0
#   endif
#endif
*/
//----------------------------------------------------------------------------
// Linux/Apple/iOs/Android/Symbian/Tegra2/NaCl Settings
#if PLATFORM == PLATFORM_LINUX || PLATFORM == PLATFORM_APPLE || PLATFORM == PLATFORM_APPLE_IOS || PLATFORM == PLATFORM_ANDROID || PLATFORM == PLATFORM_TEGRA2 || PLATFORM == PLATFORM_NACL

//#include <syswait.h>

// Enable GCC symbol visibility
#   if defined( GCC_VISIBILITY )
#       define Export  __attribute__ ((visibility("default")))
#       define Private __attribute__ ((visibility("hidden")))
#   else
#       define Export
#       define Private
#   endif

// A quick define to overcome different names for the same function
#   define stricmp strcasecmp

// Unlike the Win32 compilers, Linux compilers seem to use DEBUG for when
// specifying a debug build.
// (??? this is wrong, on Linux debug builds aren't marked in any way unless
// you mark it yourself any way you like it -- zap ???)
#   if defined(_DEBUG) || defined(DEBUG)
#       define DEBUG_MODE 1
#   endif

// Always enable unicode support for the moment
// Perhaps disable in old versions of gcc if necessary
#define UNICODE_SUPPORT 1
#define MAX_PATH 255

#define EXPORT extern "C" __attribute ((visibility("default")))

#endif

//----------------------------------------------------------------------------


#define GET_CLASS_NAME(className) (#className)
#define CLASS_NAME(className) (#className)

#include <memory>
#define NF_SHARE_PTR std::shared_ptr
#define NF_NEW new



#endif // __Platform_H__