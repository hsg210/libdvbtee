#-------------------------------------------------
#
# Project created by QtCreator 2013-03-06T00:21:30
#
#-------------------------------------------------

QT       -= gui
QT       -= core

TARGET = dvbtee
TEMPLATE = lib

DEFINES += DVBTEE_LIBRARY

include ( libdvbtee.pri )

LIBS += -L$$PWD/value/ -lvalueobj

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE1EA1848
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = dvbtee.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
	target.path = /opt/usr/lib
    } else {
	target.path = /usr/lib
    }
    INSTALLS += target
}

unix|win32: LIBS += -lcurl

LIBS += -L../usr/include/

LIBS += -ldvbpsi

symbian: LIBS += -ldvbpsi
else:unix|win32: LIBS += -L$$PWD/../usr/lib/ -ldvbpsi

INCLUDEPATH += $$PWD/../usr/include
DEPENDPATH += $$PWD/../usr/include

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../usr/lib/libdvbpsi.a

macx: LIBS += -L/opt/local/lib -liconv

QMAKE_CXXFLAGS += -Wno-unused-parameter -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE -D__USE_LARGEFILE64 -D__STDC_FORMAT_MACROS
QMAKE_CXXFLAGS += -DHAVE_LIBCURL
QMAKE_CXXFLAGS += -DHAVE_ARPA_INET_H
QMAKE_CXXFLAGS += -DHAVE_NETDB_H
QMAKE_CXXFLAGS += -DHAVE_STRTOK_R
QMAKE_CXXFLAGS += -DHAVE_SYS_SOCKET_H
QMAKE_CXXFLAGS += -DHAVE_IFADDRS_H

disablehdhr {
} else {
    # some distros use a different location for libhdhomerun headers :-/
    INCLUDEPATH += /opt/local/include
    INCLUDEPATH += /usr/include/libhdhomerun
    INCLUDEPATH += /usr/lib/libhdhomerun
    DEPENDPATH += /usr/lib/libhdhomerun

    symbian: LIBS += -lhdhomerun
    macx: LIBS += -L/opt/local/lib/ -lhdhomerun
    else:unix|win32: LIBS += -L/usr/lib/ -lhdhomerun

    QMAKE_CXXFLAGS += -DUSE_HDHOMERUN
}

unix:!macx:!symbian {
    QMAKE_CXXFLAGS += -DUSE_LINUXTV
}

OTHER_FILES += Makefile.am
