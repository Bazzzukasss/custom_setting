QT += widgets

TEMPLATE = lib
CONFIG += staticlib c++17
DESTDIR = ../bin

SOURCES += \
    src/custom_setting.cpp \
    src/custom_setting_data.cpp \
    src/custom_setting_item.cpp \
    src/custom_setting_item_delegate.cpp \
    src/custom_setting_item_tree_model.cpp \
    src/custom_setting_manager.cpp \
    src/custom_setting_serializer.cpp \
    src/custom_setting_tree_widget.cpp \
    src/custom_setting_widget.cpp \
    src/custom_widgets.cpp

HEADERS += \
    inc/custom_setting.h \
    inc/custom_setting_data.h \
    inc/custom_setting_item.h \
    inc/custom_setting_item_delegate.h \
    inc/custom_setting_item_tree_model.h \
    inc/custom_setting_manager.h \
    inc/custom_setting_serializer.h \
    inc/custom_setting_tree_widget.h \
    inc/custom_setting_widget.h \
    inc/custom_widgets.h

INCLUDEPATH += inc

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
