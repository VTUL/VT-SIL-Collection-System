#-------------------------------------------------
#
# Project created by QtCreator 2015-02-26T16:51:46
#
#-------------------------------------------------

#!include( ./examples.pri ) {
# error( "Couldn't find the examples.pri file!" )
#}


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DAQ_Interface
TEMPLATE = app
CONFIG += console


SOURCES += main.cpp\
           CSV_Output.cpp \
           DAQ.cpp \
           DAQ_Interface.cpp \
           Data_Output.cpp \
    library.cpp \
    sensor_data.cpp \
    config_parser.cpp

HEADERS  += \
            CSV_Output.h \
            DAQ.h \
            DAQ_Interface.h \
            Data_Output.h \
            Data_Output_Factory.h \
            Data_Output_Factory_Implementation.h \
            library.h \
    sensor_data.h \
    config_parser.h




win32:CONFIG(release, debug|release): LIBS += -L"$$PWD/../../../../../Program Files (x86)/IVI Foundation/IVI/Lib_x64/msc/" -lVTEXDsa


INCLUDEPATH += "$$PWD/../../../../../Program Files (x86)/IVI Foundation/VISA/WinNT/Include/"
DEPENDPATH += "$$PWD/../../../../../Program Files (x86)/IVI Foundation/VISA/WinNT/Include/"

INCLUDEPATH += "$$PWD/../../../../../Program Files (x86)/IVI Foundation/IVI/Include/"
DEPENDPATH += "$$PWD/../../../../../Program Files (x86)/IVI Foundation/IVI/Include/"



win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += "$$PWD/../../../../../Program Files (x86)/IVI Foundation/IVI/Lib_x64/msc/libVTEXDsa.a"
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += "$$PWD/../../../../../Program Files (x86)/IVI Foundation/IVI/Lib_x64/msc/VTEXDsa.lib"

# CM 4/13/15 - comment out so I get a running system, have this until I can get HDF5 working
#win32:CONFIG(release, debug|release): LIBS += -L"$$PWD/../../../../../Program Files/HDF_Group/HDF5/1.8.14/lib/" -lhdf5
#else:unix: LIBS += -L"$$PWD/../../../../../Program Files/HDF_Group/HDF5/1.8.14/lib/" -lhdf5

#INCLUDEPATH += "$$PWD/../../../../../Program Files/HDF_Group/HDF5/1.8.14/include"
#DEPENDPATH += "$$PWD/../../../../../Program Files/HDF_Group/HDF5/1.8.14/include"

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../Program Files/HDF_Group/HDF5/1.8.14/lib/ -lhdf5_cpp
#else:unix: LIBS += -L$$PWD/../../../../../Program Files/HDF_Group/HDF5/1.8.14/lib/ -lhdf5_cpp

#unix|win32: LIBS += -L"$$PWD/../../../../../Program Files/HDF_Group/HDF5/1.8.14/lib/" -lhdf5_hl

#unix|win32: LIBS += -L"$$PWD/../../../../../Program Files/HDF_Group/HDF5/1.8.14/lib/" -lhdf5_hl_cpp

