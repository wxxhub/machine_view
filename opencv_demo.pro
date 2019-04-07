#-------------------------------------------------
#
# Project created by QtCreator 2019-03-27T12:52:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opencv_demo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /home/wxx/lib/Opencv3.4.0/include \
               /home/wxx/lib/Opencv3.4.0/include/opencv \
               /home/wxx/lib/Opencv3.4.0/include/opencv2 \

LIBS += /home/wxx/lib/Opencv3.4.0/lib/libopencv_highgui.so \
        /home/wxx/lib/Opencv3.4.0/lib/libopencv_core.so \
        /home/wxx/lib/Opencv3.4.0/lib/libopencv_imgcodecs.so \
        /home/wxx/lib/Opencv3.4.0/lib/libopencv_objdetect.so \
        /home/wxx/lib/Opencv3.4.0/lib/libopencv_imgproc.so \


SOURCES += \
        main.cpp \
    src/mainwindow.cpp \
    src/image_process.cpp \
    src/countricewindow.cpp \
    src/numberdetector.cpp \
    src/numdetwindow.cpp

HEADERS += \
        include/mainwindow.h \
    include/image_process.h \
    include/countricewindow.h \
    include/numberdetector.h \
    include/numdetwindow.h

FORMS += \
    ui/countricewindow.ui \
    ui/mainwindow.ui \
    ui/numdetwindow.ui
