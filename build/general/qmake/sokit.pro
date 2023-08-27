# ----------------------------------------------------
# sokit.pro
# ----------------------------------------------------

TEMPLATE = app
TARGET = sokit

QT += gui widgets network
#CONFIG += debug_and_release build_all thread
CONFIG += release
DEFINES += QT_NETWORK_LIB
INCLUDEPATH += .
INCLUDEPATH += ./../../../tmp
INCLUDEPATH += ./../../../src/sokit
DEPENDPATH += .
UI_DIR += ./../../../tmp
RCC_DIR += ./../../../tmp

win32 {
#    DEFINES += QT_LARGEFILE_SUPPORT
#    CONFIG += windows qt_static
    CONFIG += windows

#    QMAKE_CFLAGS_MT =-MT
#    QMAKE_CFLAGS_MT_DBG =-MTd
#    QMAKE_CFLAGS_MT_DLL =-MD
#    QMAKE_CFLAGS_MT_DLLDBG =-MDd
}

CONFIG(debug, debug|release) {
    message("Config:  Debug")
    DESTDIR = $$PWD/../../../bin/debug
    MOC_DIR += $$PWD/../../../tmp/debug
    OBJECTS_DIR += $$PWD/../../../tmp/debug
    INCLUDEPATH += $$PWD/../../../tmp/debug

    QMAKE_CFLAGS_DEBUG = $$unique(QMAKE_CFLAGS_DEBUG)
    QMAKE_CXXFLAGS_DEBUG = $$unique(QMAKE_CFLAGS_DEBUG)

    CONFIG(qt_static) {
        QMAKE_CFLAGS_DEBUG -= $$QMAKE_CFLAGS_MT_DLLDBG
        QMAKE_CFLAGS_DEBUG += $$QMAKE_CFLAGS_MT_DBG
        QMAKE_CXXFLAGS_DEBUG -= $$QMAKE_CFLAGS_MT_DLLDBG
        QMAKE_CXXFLAGS_DEBUG += $$QMAKE_CFLAGS_MT_DBG
    } else {
        QMAKE_CFLAGS_DEBUG -= $$QMAKE_CFLAGS_MT_DBG
        QMAKE_CFLAGS_DEBUG += $$QMAKE_CFLAGS_MT_DLLDBG
        QMAKE_CXXFLAGS_DEBUG -= $$QMAKE_CFLAGS_MT_DBG
        QMAKE_CXXFLAGS_DEBUG += $$QMAKE_CFLAGS_MT_DLLDBG
    }
} else {
    message("Config:  Release")
#    DESTDIR = $$PWD/../../../bin/release
#    MOC_DIR += $$PWD/../../../tmp/release
#    OBJECTS_DIR += $$PWD/../../../tmp/release
#    INCLUDEPATH += $$PWD/../../../tmp/release
#    DESTDIR = ./bin/release
#    MOC_DIR += ./tmp/release
#    OBJECTS_DIR += ./tmp/release
#    INCLUDEPATH += ./tmp/release

    QMAKE_CFLAGS_RELEASE = $$unique(QMAKE_CFLAGS_RELEASE)
    QMAKE_CXXFLAGS_RELEASE = $$unique(QMAKE_CXXFLAGS_RELEASE)

    CONFIG(qt_static) {
        QMAKE_CFLAGS_RELEASE -= $$QMAKE_CFLAGS_MT_DLL
        QMAKE_CFLAGS_RELEASE += $$QMAKE_CFLAGS_MT
        QMAKE_CXXFLAGS_RELEASE -= $$QMAKE_CFLAGS_MT_DLL
        QMAKE_CXXFLAGS_RELEASE += $$QMAKE_CFLAGS_MT
    } else {
        QMAKE_CFLAGS_RELEASE -= $$QMAKE_CFLAGS_MT
        QMAKE_CFLAGS_RELEASE += $$QMAKE_CFLAGS_MT_DLL
        QMAKE_CXXFLAGS_RELEASE -= $$QMAKE_CFLAGS_MT
        QMAKE_CXXFLAGS_RELEASE += $$QMAKE_CFLAGS_MT_DLL
    }
}


#message(1 $$PWD --  $$DESTDIR ; $$absolute_path($$RCC_DIR) ; ))

#message(4 INCLUDEPATH: $$INCLUDEPATH)

ROOT_SRC = ../../../src/sokit

#message($$ROOT_SRC)
HEADERS += \
    $$ROOT_SRC/resource.h \
    $$ROOT_SRC/setting.h \
    $$ROOT_SRC/toolkit.h \
    $$ROOT_SRC/baseform.h \
    $$ROOT_SRC/clientform.h \
    $$ROOT_SRC/clientskt.h \
    $$ROOT_SRC/helpform.h \
    $$ROOT_SRC/logger.h \
    $$ROOT_SRC/main.h \
    $$ROOT_SRC/notepadform.h \
    $$ROOT_SRC/transferskt.h \
    $$ROOT_SRC/transferform.h \
    $$ROOT_SRC/serverskt.h \
    $$ROOT_SRC/serverform.h
SOURCES += \
    $$ROOT_SRC/baseform.cpp \
    $$ROOT_SRC/clientform.cpp \
    $$ROOT_SRC/clientskt.cpp \
    $$ROOT_SRC/helpform.cpp \
    $$ROOT_SRC/logger.cpp \
    $$ROOT_SRC/main.cpp \
    $$ROOT_SRC/notepadform.cpp \
    $$ROOT_SRC/serverform.cpp \
    $$ROOT_SRC/serverskt.cpp \
    $$ROOT_SRC/setting.cpp \
    $$ROOT_SRC/toolkit.cpp \
    $$ROOT_SRC/transferform.cpp \
    $$ROOT_SRC/transferskt.cpp
FORMS += \
    $$ROOT_SRC/clientform.ui \
    $$ROOT_SRC/helpform.ui \
    $$ROOT_SRC/serverform.ui \
    $$ROOT_SRC/transferform.ui
TRANSLATIONS += $$ROOT_SRC/sokit.ts
RESOURCES += $$ROOT_SRC/icons.qrc

#QMAKE_PRE_LINK = lupdate $$PWD/sokit.pro
QMAKE_POST_LINK = lrelease $$clean_path($$PWD/../../../src/sokit/sokit.ts) -qm sokit.lan #$$DESTDIR/sokit.lan

win32 {
    RC_FILE = ../../../src/sokit/sokit.rc
    LIBS += -lWs2_32 -lWinmm -lImm32
#    QMAKE_LFLAGS_DEBUG += /PDB:"$$DESTDIR/sokit.pdb"
#    QMAKE_CFLAGS_DEBUG += /Fd"$$OBJECTS_DIR/sokit.pdb"
#    QMAKE_CXXFLAGS_DEBUG += /Fd"$$OBJECTS_DIR/sokit.pdb"

   CONFIG(qt_static) {
        exists( $(QTDIR)/lib/static ) {
	   QMAKE_LIBDIR_QT = $(QTDIR)/lib/static 
       }
    } else {
        exists( $(QTDIR)/lib/shared ) {
	   QMAKE_LIBDIR_QT = $(QTDIR)/lib/shared 
       }
    }
}

message($$clean_path($$PWD/../../../src/sokit/sokit.ts))
