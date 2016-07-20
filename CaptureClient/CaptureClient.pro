#-------------------------------------------------
#
# Project created by QtCreator 2016-02-25T13:00:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CaptureClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sniffer/src/Ethernet.cpp \
    sniffer/src/IP.cpp \
    sniffer/src/Layer.cpp \
    sniffer/src/packetDecoder.cpp \
    sniffer/src/sniffer.cpp \
    sniffer/src/TCP.cpp \
    sniffer/src/Udp.cpp \
    LiveServer/src/ADTSAudioLiveServerMediaSubsession.cpp \
    LiveServer/src/ADTSAudioLiveSource.cpp \
    LiveServer/src/CapRTSPLive.cpp \
    LiveServer/src/H264LiveSource.cpp \
    LiveServer/src/H264LiveVideoServerMediaSubssion.cpp \
    LiveServer/src/liveBuffer.cpp \
    util/buffer.cpp \
    util/Queue.cpp \
    CaptureVideoAndAudio/src/CaptureVideoAndAudio.cpp \
    CaptureVideoAndAudio/src/grabdesktop.cpp \
    CaptureVideoAndAudio/src/loopbackRecording.cpp

DEFINES += _XKEYCHECK_H

HEADERS  += \
    mainwindow.h \
    sniffer/include/Ethernet.h \
    sniffer/include/IP.h \
    sniffer/include/Layer.h \
    sniffer/include/packetDecoder.h \
    sniffer/include/Protocol.h \
    sniffer/include/sniffer.h \
    sniffer/include/TCP.h \
    sniffer/include/Udp.h \
    LiveServer/include/ADTSAudioLiveServerMediaSubsession.h \
    LiveServer/include/ADTSAudioLiveSource.h \
    LiveServer/include/buffer.h \
    LiveServer/include/CapRTSPLive.h \
    LiveServer/include/H264LiveSource.h \
    LiveServer/include/H264LiveVideoServerMediaSubssion.h \
    LiveServer/include/liveBuffer.h \
    LiveServer/include/Queue.h \
    util/adts_frame_header.h \
    util/buffer.h \
    util/Queue.h \
    CaptureVideoAndAudio/include/CaptureVideoAndAudio.h \
    CaptureVideoAndAudio/include/ffmpeg.h \
    CaptureVideoAndAudio/include/grabdesktop.h \
    CaptureVideoAndAudio/include/loopbackRecording.h

FORMS    += mainwindow.ui

INCLUDEPATH +=./ffmpeg/include \
            ./WpdPack/include\
            ./Live555/include\
            ./sniffer/include\
            ./LiveServer/include\
            ./CaptureVideoAndAudio/include\
            ./util\

LIBS += -L$$PWD/ffmpeg/lib -lavcodec -lavformat -lswscale -lavdevice -lavutil\
        -L$$PWD/WpdPack/Lib -lPacket -lwpcap\
        -L$$PWD/Live555/lib -lBasicUsageEnvironment -lgroupsock -lliveMedia -lUsageEnvironment


