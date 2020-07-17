#-------------------------------------------------
#
# Project created by QtCreator 2019-11-15T10:23:15
#
#-------------------------------------------------

QT       -= core
QT       -= gui

TARGET = Super
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11
CONFIG += release

#gcc release version add -o3 Optimize
unix{

   # QMAKE_CXXFLAGS += -march=native
    CONFIG(release,debug|release)
    {
       QMAKE_CXXFLAGS_RELEASE+=-O3
       DEFINES+=NDEBUG
    }
}



DEFINES+=__USE_FILE_OFFSET64  #to be confirmed



#check is arm platform?
unix
{
   if(contains(QT_ARCH,arm.*)){
          DEFINES+=OS_ARM_LINUX
   }else{

   }
}





#
#QMAKE_CXXFLAGS +=


MOC_DIR += ./GeneratedFiles/Release
OBJECTS_DIR += ./GeneratedFiles/obj
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

#MOC_DIR += temp/moc
#RCC_DIR += temp/rcc
#UI_DIR += temp/ui
#OBJECTS_DIR += temp/obj


win32:DESTDIR=../lib/win32
#unix:DESTDIR=../lib/linux




unix
{
    QMAKE_CXXFLAGS += -fpermissive
   if(contains(DEFINES,OS_ARM_LINUX)){
        #QMAKE_CXXFLAGS+=-mno-unaligned-access
        #DEFINES+=__ARM_FEATURE_UNALIGNED
         DESTDIR=../lib/arm_linux
   }else{
        DESTDIR=../lib/linux
   }
}


HEADERS += \
    Super/Basic/AudioCodec.h \
    Super/Basic/AudioHelper.h \
    Super/Basic/BasicOperation.h \
    Super/Basic/BigInt.h \
    Super/Basic/BitIO.h \
    Super/Basic/BsaeType.h \
    Super/Basic/ByteArray.h \
    Super/Basic/ConfigFile.h \
    Super/Basic/Console.h \
    Super/Basic/CRC.h \
    Super/Basic/define.h \
    Super/Basic/Directory.h \
    Super/Basic/FIFO.h \
    Super/Basic/File.h \
    Super/Basic/FileCopy.h \
    Super/Basic/global.h \
    Super/Basic/Int24.h \
    Super/Basic/Int128.h \
    Super/Basic/IPAddress.h \
    Super/Basic/Log.h \
    Super/Basic/Math.h \
    Super/Basic/MD5.h \
    Super/Basic/MediaTimer.h \
    Super/Basic/Mem.h \
    Super/Basic/MemoryPool.h \
    Super/Basic/MemPoolFast.h \
    Super/Basic/MessageQueue.h \
    Super/Basic/MyAutoPtr.h \
    Super/Basic/NetStream.h \
    Super/Basic/Noise.h \
    Super/Basic/ObjectPool.h \
    Super/Basic/PerformanceTest.h \
    Super/Basic/PID.h \
    Super/Basic/Process.h \
    Super/Basic/ProcessGuard.h \
    Super/Basic/Rand.h \
    Super/Basic/RefCount.h \
    Super/Basic/RingBuffer.h \
    Super/Basic/RingBuffer_Test.h \
    Super/Basic/RingBufferProcess.h \
    Super/Basic/RingBufferSimple.h \
    Super/Basic/SharedData.h \
    Super/Basic/ShareMemory.h \
    Super/Basic/ShareMemory.hpp \
    Super/Basic/Singleton.h \
    Super/Basic/SP_uuid.h \
    Super/Basic/SString.h \
    Super/Basic/StringConvert.h \
    Super/Basic/SuperSocket.h \
    Super/Basic/SuperTime.h \
    Super/Basic/SystemCtrl.h \
    Super/Basic/Temp.h \
    Super/Basic/Test_Basic.h \
    Super/Basic/Thread.h \
    Super/Basic/ThreadSync.h \
    Super/Basic/ThreadSync.hpp \
    Super/Basic/Timer.h \
    Super/Basic/TinyStr.h \
    Super/Basic/TinyXML.h \
    Super/Basic/Tstring.h \
    Super/Basic/Tools.h \
    Super/Tool/CopyFileByThread.h \
    Super/Net/NetBase.h \
    Super/Net/NetServerBase.h \
    Super/Net/TcpClientBase.h \
    Super/Net/TcpServerBase.h \
    Super/Net/TcpServerByEPOLL.h \
    Super/Net/TcpServerByIOCP.h \
    Super/Net/TcpServerByMultiThread.h \
    Super/Net/TcpServerBySelect.h \
    Super/Net/ThreadClientSocket.h \
    Super/Net/zSocket.h



SOURCES += \
    Super/Basic/AudioCodec.cpp \
    Super/Basic/AudioHelper.cpp \
    Super/Basic/BasicOperation.cpp \
    Super/Basic/BigInt.cpp \
    Super/Basic/BitIO.cpp \
    Super/Basic/BsaeType.cpp \
    Super/Basic/ByteArray.cpp \
    Super/Basic/ConfigFile.cpp \
    Super/Basic/CRC.cpp \
    Super/Basic/FIFO.cpp \
    Super/Basic/FileCopy.cpp \
    Super/Basic/global.cpp \
    Super/Basic/Int24.cpp \
    Super/Basic/Int128.cpp \
    Super/Basic/IPAddress.cpp \
    Super/Basic/Log.cpp \
    Super/Basic/Math.cpp \
    Super/Basic/MD5.cpp \
    Super/Basic/MediaTimer.cpp \
    Super/Basic/Mem.cpp \
    Super/Basic/MemoryPool.cpp \
    Super/Basic/MemPoolFast.cpp \
    Super/Basic/MessageQueue.cpp \
    Super/Basic/NetStream.cpp \
    Super/Basic/Noise.cpp \
    Super/Basic/ObjectPool.cpp \
    Super/Basic/PerformanceTest.cpp \
    Super/Basic/PID.cpp \
    Super/Basic/Process.cpp \
    Super/Basic/ProcessGuard.cpp \
    Super/Basic/Rand.cpp \
    Super/Basic/RefCount.cpp \
    Super/Basic/RingBuffer_Test.cpp \
    Super/Basic/RingBufferSimple.cpp \
    Super/Basic/SharedData.cpp \
    Super/Basic/ShareMemory.cpp \
    Super/Basic/Singleton.cpp \
    Super/Basic/SP_uuid.cpp \
    Super/Basic/SString.cpp \
    Super/Basic/SuperSocket.cpp \
    Super/Basic/SuperTime.cpp \
    Super/Basic/SystemCtrl.cpp \
    Super/Basic/Temp.cpp \
    Super/Basic/Test_Basic.cpp \
    Super/Basic/Thread.cpp \
    Super/Basic/ThreadSync.cpp \
    Super/Basic/Timer.cpp \
    Super/Basic/TinyStr.cpp \
    Super/Basic/TinyXML.cpp \
    Super/Basic/Tstring.cpp \
    Super/Basic/Tools.cpp \
    Super/Tool/CopyFileByThread.cpp \
    Super/Net/NetBase.cpp \
    Super/Net/NetServerBase.cpp \
    Super/Net/TcpClientBase.cpp \
    Super/Net/TcpServerBase.cpp \
    Super/Net/TcpServerByEPOLL.cpp \
    Super/Net/TcpServerByIOCP.cpp \
    Super/Net/TcpServerByMultiThread.cpp \
    Super/Net/TcpServerBySelect.cpp \
    Super/Net/ThreadClientSocket.cpp \
    Super/Net/zSocket.cpp



unix {
    target.path = /usr/lib
    INSTALLS += target
}
