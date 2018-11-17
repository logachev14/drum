#-------------------------------------------------
#
# Project created by QtCreator 2018-06-24T17:51:43
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = drum
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    current_proto/sender.cpp \
    common_functions/common_functions.cpp \
    slonyk/slonyk/slonyk_master.cpp \
    slonyk/slonyk/slonyk_segment.cpp \
    slonyk/callbacks_cpp11_test.cpp \
    reg_proto/reg_proto.cpp

HEADERS  += mainwindow.h \
    printer_resisters.h \
    current_proto/sender.h \
    motion/motion.h \
    project_config.h \
    common_functions/common_functions.h \
    global_macros/qt_global_macros.h \
    circular_buffer/circular_buffer.h \
    slonyk/slonyk/sl_session.h \
    slonyk/slonyk/slonyk_data_link_provider.h \
    slonyk/slonyk/slonyk_master.h \
    slonyk/slonyk/slonyk_network_provider.h \
    slonyk/slonyk/slonyk_settings.h \
    slonyk/slonyk/slonyk_slave.h \
    slonyk/slonyk/slonyk_table.h \
    slonyk/slonyk/slonyk_transport_provider.h \
    slonyk/eng_settings.h \
    slonyk/i_data_link_provider.h \
    slonyk/i_descriptor.h \
    slonyk/i_network_provider.h \
    slonyk/i_provider.h \
    slonyk/i_register.h \
    slonyk/i_table.h \
    slonyk/i_transport_provider.h \
    slonyk/lwip_transport_provider.h \
    slonyk/project_config.h \
    slonyk/standart_registers.h \
    slonyk/types.h \
    slonyk/callbacks.h \
    reg_proto/reg_proto.h \
    slonyk/slonyk/slonyk_types.h

FORMS    += mainwindow.ui
INCLUDEPATH += $$PWD/slonyk


