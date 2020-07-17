#ifndef PlatformDef_H
#define PlatformDef_H


#ifdef _WIN32
#define OS_WIN32
#endif

#ifdef _WIN64
#define OS_WIN64
#endif

#if defined(OS_WIN32)||defined(OS_WIN64)
#  define OS_WINDOWS
#  define OS_WIN
#endif

#if defined(__ANDROID__)|| defined(ANDROID)
#  define OS_ANDROID
#  define OS_LINUX
#endif

#if defined(__APPLE__)
#  define OS_MACOS
#endif


#if defined(__linux__)||defined(__linux)
#define OS_LINUX
#endif

//arm platform TODO:
#if defined(__arm__)||defined(__arm64__)||defined(_ARM)||defined(_M_ARM)
     ||defined(__ARM_ARCH_7__)||defined(__ARM_ARCH_7A__)||defined(__ARM_ARCH_7R__)
     ||defined(__ARM_ARCH_7M__) ||defined(__ARM_ARCH_7S__)

#define OS_ARM
#define OS_ARM_LINUX
#endif


#if defined(OS_WIN)
#  undef OS_UNIX
#elif !defined(OS_UNIX)
#  define OS_UNIX
#endif





//from qsystemdetection.h

//
//#ifndef QGLOBAL_H
//# include <QtCore/qglobal.h>
//#endif


/*
   The operating system, must be one of: (Q_OS_x)

     DARWIN   - Any Darwin system (macOS, iOS, watchOS, tvOS)
     MACOS    - macOS
     IOS      - iOS
     WATCHOS  - watchOS
     TVOS     - tvOS
     WIN32    - Win32 (Windows 2000/XP/Vista/7 and Windows Server 2003/2008)
     WINRT    - WinRT (Windows Runtime)
     CYGWIN   - Cygwin
     SOLARIS  - Sun Solaris
     HPUX     - HP-UX
     LINUX    - Linux [has variants]
     FREEBSD  - FreeBSD [has variants]
     NETBSD   - NetBSD
     OPENBSD  - OpenBSD
     INTERIX  - Interix
     AIX      - AIX
     HURD     - GNU Hurd
     QNX      - QNX [has variants]
     QNX6     - QNX RTP 6.1
     LYNX     - LynxOS
     BSD4     - Any BSD 4.4 system
     UNIX     - Any UNIX BSD/SYSV system
     ANDROID  - Android platform
     HAIKU    - Haiku

   The following operating systems have variants:
     LINUX    - both Q_OS_LINUX and Q_OS_ANDROID are defined when building for Android
              - only Q_OS_LINUX is defined if building for other Linux systems
     FREEBSD  - Q_OS_FREEBSD is defined only when building for FreeBSD with a BSD userland
              - Q_OS_FREEBSD_KERNEL is always defined on FreeBSD, even if the userland is from GNU
*/
//
//#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
//#  include <TargetConditionals.h>
//#  if defined(TARGET_OS_MAC) && TARGET_OS_MAC
//#    define OS_DARWIN
//#    define OS_BSD4
//#    ifdef __LP64__
//#      define OS_DARWIN64
//#    else
//#      define OS_DARWIN32
//#    endif
//#    if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
//#      define QT_PLATFORM_UIKIT
//#      if defined(TARGET_OS_WATCH) && TARGET_OS_WATCH
//#        define Q_OS_WATCHOS
//#      elif defined(TARGET_OS_TV) && TARGET_OS_TV
//#        define Q_OS_TVOS
//#      else
//#        // TARGET_OS_IOS is only available in newer SDKs,
//#        // so assume any other iOS-based platform is iOS for now
//#        define Q_OS_IOS
//#      endif
//#    else
//#      // TARGET_OS_OSX is only available in newer SDKs,
//#      // so assume any non iOS-based platform is macOS for now
//#      define Q_OS_MACOS
//#    endif
//#  else
//#    error "Qt has not been ported to this Apple platform - see http://www.qt.io/developers"
//#  endif
//#elif defined(__ANDROID__) || defined(ANDROID)
//#  define Q_OS_ANDROID
//#  define Q_OS_LINUX
//#elif defined(__CYGWIN__)
//#  define Q_OS_CYGWIN
//#elif !defined(SAG_COM) && (!defined(WINAPI_FAMILY) || WINAPI_FAMILY==WINAPI_FAMILY_DESKTOP_APP) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
//#  define Q_OS_WIN32
//#  define Q_OS_WIN64
//#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
//#  if defined(WINAPI_FAMILY)
//#    ifndef WINAPI_FAMILY_PC_APP
//#      define WINAPI_FAMILY_PC_APP WINAPI_FAMILY_APP
//#    endif
//#    if defined(WINAPI_FAMILY_PHONE_APP) && WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
//#      define Q_OS_WINRT
//#    elif WINAPI_FAMILY==WINAPI_FAMILY_PC_APP
//#      define Q_OS_WINRT
//#    else
//#      define Q_OS_WIN32
//#    endif
//#  else
//#    define Q_OS_WIN32
//#  endif
//#elif defined(__sun) || defined(sun)
//#  define Q_OS_SOLARIS
//#elif defined(hpux) || defined(__hpux)
//#  define Q_OS_HPUX
//#elif defined(__native_client__)
//#  define Q_OS_NACL
//#elif defined(__EMSCRIPTEN__)
//#  define Q_OS_WASM
//#elif defined(__linux__) || defined(__linux)
//#  define Q_OS_LINUX
//#elif defined(__FreeBSD__) || defined(__DragonFly__) || defined(__FreeBSD_kernel__)
//#  ifndef __FreeBSD_kernel__
//#    define Q_OS_FREEBSD
//#  endif
//#  define Q_OS_FREEBSD_KERNEL
//#  define Q_OS_BSD4
//#elif defined(__NetBSD__)
//#  define Q_OS_NETBSD
//#  define Q_OS_BSD4
//#elif defined(__OpenBSD__)
//#  define Q_OS_OPENBSD
//#  define Q_OS_BSD4
//#elif defined(__INTERIX)
//#  define Q_OS_INTERIX
//#  define Q_OS_BSD4
//#elif defined(_AIX)
//#  define Q_OS_AIX
//#elif defined(__Lynx__)
//#  define Q_OS_LYNX
//#elif defined(__GNU__)
//#  define Q_OS_HURD
//#elif defined(__QNXNTO__)
//#  define Q_OS_QNX
//#elif defined(__INTEGRITY)
//#  define Q_OS_INTEGRITY
//#elif defined(VXWORKS) /* there is no "real" VxWorks define - this has to be set in the mkspec! */
//#  define Q_OS_VXWORKS
//#elif defined(__HAIKU__)
//#  define Q_OS_HAIKU
//#elif defined(__MAKEDEPEND__)
//#else
//#  error "Qt has not been ported to this OS - see http://www.qt-project.org/"
//#endif
//
//#if defined(OS_WIN32) || defined(OS_WIN64) || defined(OS_WINRT)
//#  define OS_WINDOWS
//#  define OS_WIN
//#endif
//
//
//// Compatibility synonyms
//#ifdef OS_DARWIN
//#define OS_MAC
//#endif
//#ifdef OS_DARWIN32
//#define OS_MAC32
//#endif
//#ifdef OS_DARWIN64
//#define OS_MAC64
//#endif
//#ifdef OS_MACOS
//#define OS_MACX
//#define OS_OSX
//#endif
//
//#ifdef OS_DARWIN
//#  include <Availability.h>
//#  include <AvailabilityMacros.h>
//#
//#  ifdef OS_MACOS
//#    if !defined(__MAC_OS_X_VERSION_MIN_REQUIRED) || __MAC_OS_X_VERSION_MIN_REQUIRED < __MAC_10_6
//#       undef __MAC_OS_X_VERSION_MIN_REQUIRED
//#       define __MAC_OS_X_VERSION_MIN_REQUIRED __MAC_10_6
//#    endif
//#    if !defined(MAC_OS_X_VERSION_MIN_REQUIRED) || MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_6
//#       undef MAC_OS_X_VERSION_MIN_REQUIRED
//#       define MAC_OS_X_VERSION_MIN_REQUIRED MAC_OS_X_VERSION_10_6
//#    endif
//#  endif
//#
//#  // Numerical checks are preferred to named checks, but to be safe
//#  // we define the missing version names in case Qt uses them.
//#
//#  if !defined(__MAC_10_11)
//#       define __MAC_10_11 101100
//#  endif
//#  if !defined(__MAC_10_12)
//#       define __MAC_10_12 101200
//#  endif
//#  if !defined(__MAC_10_13)
//#       define __MAC_10_13 101300
//#  endif
//#  if !defined(__MAC_10_14)
//#       define __MAC_10_14 101400
//#  endif
//#  if !defined(__MAC_10_15)
//#       define __MAC_10_15 101500
//#  endif
//#  if !defined(MAC_OS_X_VERSION_10_11)
//#       define MAC_OS_X_VERSION_10_11 __MAC_10_11
//#  endif
//#  if !defined(MAC_OS_X_VERSION_10_12)
//#       define MAC_OS_X_VERSION_10_12 __MAC_10_12
//#  endif
//#  if !defined(MAC_OS_X_VERSION_10_13)
//#       define MAC_OS_X_VERSION_10_13 __MAC_10_13
//#  endif
//#  if !defined(MAC_OS_X_VERSION_10_14)
//#       define MAC_OS_X_VERSION_10_14 __MAC_10_14
//#  endif
//#  if !defined(MAC_OS_X_VERSION_10_15)
//#       define MAC_OS_X_VERSION_10_15 __MAC_10_15
//#  endif
//#
//#  if !defined(__IPHONE_10_0)
//#       define __IPHONE_10_0 100000
//#  endif
//#  if !defined(__IPHONE_10_1)
//#       define __IPHONE_10_1 100100
//#  endif
//#  if !defined(__IPHONE_10_2)
//#       define __IPHONE_10_2 100200
//#  endif
//#  if !defined(__IPHONE_10_3)
//#       define __IPHONE_10_3 100300
//#  endif
//#  if !defined(__IPHONE_11_0)
//#       define __IPHONE_11_0 110000
//#  endif
//#  if !defined(__IPHONE_12_0)
//#       define __IPHONE_12_0 120000
//#  endif
//#endif
//
//#ifdef __LSB_VERSION__
//#  if __LSB_VERSION__ < 40
//#    error "This version of the Linux Standard Base is unsupported"
//#  endif
//#ifndef QT_LINUXBASE
//#  define QT_LINUXBASE
//#endif
//#endif
//
//
//
//
//
//
//
//




//#if 0
//
//C++ ��ο�ƽ̨�жϲ���ϵͳ��32λ����64λ��
//1���жϲ���ϵͳλ���������жϱ�������Ҳ�����ж�cpu;
//2�������ò���ϵͳAPI,Ҫ���ƽ̨�жϡ�
//3���������˳�������ԭ���Լ�Ϊʲô;
//
//�Լ�ͨ��sizeof�жϣ���׼ȷ�������Ǹ��������йء�
//
//�����ṩ��һ��˼·��������cmake��
//    cmake�ڼ���������ʱ������һ�ֺܱ����ķ����������ڲ�����ʵ�ʴ���������ֱ��֪��Ŀ��ƽ̨����Ϣ��
//    �����������ġ�
//    ��������һ��.cpp�ļ�������һЩƽ̨����#ifdef
//
//    /* Identify known platforms by name.  */
//#if defined(__linux) || defined(__linux__) || defined(linux)
//# define PLATFORM_ID "Linux"
//
//#elif defined(__CYGWIN__)
//# define PLATFORM_ID "Cygwin"
//
//#elif defined(__MINGW32__)
//# define PLATFORM_ID "MinGW"
//
//#elif defined(__APPLE__)
//# define PLATFORM_ID "Darwin"
//
//#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
//# define PLATFORM_ID "Windows"
//
//#elif defined(__FreeBSD__) || defined(__FreeBSD)
//# define PLATFORM_ID "FreeBSD"
//
//    ...
//    �Լ��ܹ�����#ifdef
//
//#if defined(_WIN32) && defined(_MSC_VER)
//# if defined(_M_IA64)
//#  define ARCHITECTURE_ID "IA64"
//
//# elif defined(_M_X64) || defined(_M_AMD64)
//#  define ARCHITECTURE_ID "x64"
//
//# elif defined(_M_IX86)
//#  define ARCHITECTURE_ID "X86"
//
//# elif defined(_M_ARM64)
//#  define ARCHITECTURE_ID "ARM64"
//
//# elif defined(_M_ARM)
//#  if _M_ARM == 4
//#   define ARCHITECTURE_ID "ARMV4I"
//#  elif _M_ARM == 5
//#   define ARCHITECTURE_ID "ARMV5I"
//#  else
//#   define ARCHITECTURE_ID "ARMV" STRINGIFY(_M_ARM)
//#  endif
//
//# elif defined(_M_MIPS)
//#  define ARCHITECTURE_ID "MIPS"
//
//# elif defined(_M_SH)
//#  define ARCHITECTURE_ID "SHx"
//
//# else /* unknown architecture */
//#  define ARCHITECTURE_ID ""
//# endif
//
//#elif defined(__WATCOMC__)
//# if defined(_M_I86)
//#  define ARCHITECTURE_ID "I86"
//
//# elif defined(_M_IX86)
//#  define ARCHITECTURE_ID "X86"
//
//# else /* unknown architecture */
//#  define ARCHITECTURE_ID ""
//# endif
//
//#else
//#  define ARCHITECTURE_ID
//#endif
//    ֮�󣬶�������char const*������
//
//    char const* info_platform = "INFO" ":" "platform[" PLATFORM_ID "]";
//char const* info_arch = "INFO" ":" "arch[" ARCHITECTURE_ID "]";
//Ȼ������cpp����ɶ����ơ�����ΪҪ�����������ǵġ����ڽ������������˵��Ŀ��ƽ̨�Ķ�������û���ڱ���ƽ̨�����еġ�����cmake����һ�и��ݵģ�ֱ�Ӵ򿪶������ļ��������ַ�����
//
//    ������Windows x64ƽ̨�ϣ��ղ������������ͻᱻ�����INFO:platform[Windows]��INFO:arch[x64]�����������ַ�����һ���Ǵ����ڶ������ļ���ġ�����ֻҪ����INFO:platform������[]��ľ���ƽ̨������INFO:arch������[]��ľ��Ǽܹ���
//
//    ͬ��cmake�ﻹ�жԱ������ͱ������汾����Ϣ�ļ�⣬������ô���ġ�
//
//
//#endif



#endif // QSYSTEMDETECTION_H
