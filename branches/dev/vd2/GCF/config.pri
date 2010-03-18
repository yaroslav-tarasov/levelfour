######################################################################
# Generic Component Framework Library from VCreate Logic
######################################################################

# Provide the complete path of the directory into which you have
# checked out GCF. Under this directory it must be posible to
# fetch include and source directories. Provided from the 
# command line
# GCF_BASE = D:/GenericComponentFramework/GCFSVN/

isEmpty(GCF_BASE) {

    win32-msvc* {
        error("GCF_BASE variable not specified. Use qmake -tp vc -recursive \"GCF_BASE=/path/to/GCF\"");
    }
    
    unix {
        error("GCF_BASE variable not specified. Use qmake -recursive \"GCF_BASE=/path/to/GCF\"");
    }

}

INCLUDEPATH    += $$GCF_BASE/include
CONFIG         += ordered thread qt 
isEmpty(GCF_DEBUG_BUILD) {
    CONFIG += release
} else {
    CONFIG += debug
}

isEmpty(PROJECT_BASE) {

    win32-msvc* {
     DLLDESTDIR  = $$GCF_BASE/bin
     DESTDIR     = $$GCF_BASE/lib
     GCF_LIBS   += $$GCF_BASE/lib/GCF2.lib
    } 
    
    win32-g++ {
     DLLDESTDIR  = $$GCF_BASE/bin
     DESTDIR     = $$GCF_BASE/lib
     GCF_LIBS   += $$GCF_BASE/lib/libGCF2.a
    }
    
    unix {
     DESTDIR     = $$GCF_BASE/bin
     GCF_LIBS   += -L$$GCF_BASE/bin -L$$GCF_BASE/lib -lGCF     
    }

    exists ( $$GCF_BASE/ProjectOptions.pri )
        include( $$GCF_BASE/ProjectOptions.pri )

} else {

    win32-msvc* {
     DLLDESTDIR  = $$PROJECT_BASE/bin
     DESTDIR     = $$PROJECT_BASE/lib
     GCF_LIBS   += $$PROJECT_BASE/lib/GCF2.lib
    } 
    
    win32-g++ {
     DLLDESTDIR  = $$PROJECT_BASE/bin
     DESTDIR     = $$PROJECT_BASE/lib
     GCF_LIBS   += $$PROJECT_BASE/lib/libGCF2.a
    }
    
    unix {
     DESTDIR     = $$PROJECT_BASE/bin
     GCF_LIBS   += -L$$PROJECT_BASE/bin -L$$PROJECT_BASE/lib -lGCF 
    }

    exists ( $$PROJECT_BASE/ProjectOptions.pri )
        include( $$PROJECT_BASE/ProjectOptions.pri )
}

win32-msvc* {
 DEFINES    += -D_CRT_SECURE_NO_DEPRECATE
 CONFIG     += embed_manifest_exe warn_on dll
} else {
 DEFINES    += _CRT_SECURE_NO_DEPRECATE
 CONFIG     += warn_off
}

isEmpty(PROJECT_BASE) {

GCF_TEMP_DIR    = $$GCF_BASE/tmp
OBJECTS_DIR     = $$GCF_BASE/tmp
MOC_DIR         = $$GCF_BASE/tmp
RCC_DIR         = $$GCF_BASE/tmp
UI_DIR          = $$GCF_BASE/tmp
UI_HEADERS_DIR  = $$GCF_BASE/tmp

} else {

GCF_TEMP_DIR    = $$PROJECT_BASE/tmp
OBJECTS_DIR     = $$PROJECT_BASE/tmp
MOC_DIR         = $$PROJECT_BASE/tmp
RCC_DIR         = $$PROJECT_BASE/tmp
UI_DIR          = $$PROJECT_BASE/tmp
UI_HEADERS_DIR  = $$PROJECT_BASE/tmp

}

QT             += xml network

