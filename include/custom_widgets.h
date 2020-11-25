#pragma once

#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QProxyStyle>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include "custom_setting.h"

/**
 * Macros for generation custom widgets classes
 * @brief Custom widgets classes coud be binded to CustomSetting to edit it by user.
 * @param type define name of generated class
 * @param parent define parent clas of generated class
 * @param setting define type of CustomSetting family class which will be binded to generated widget
 */

#define CUSTOM_WIDGET(type, parent, setting) \
    class type : public parent \
    { \
        Q_OBJECT \
        using SettingType = custom_setting::setting; \
\
     public: \
        explicit type(QWidget* parent = nullptr); \
        void bindToSetting(SettingType* setting); \
\
        Q_SIGNAL void signalStateChanged(); \
        Q_SIGNAL void signalEditingFinished(); \
\
     protected: \
        void wheelEvent(QWheelEvent*) override { return; } \
\
     private: \
        void update(); \
        SettingType* mSetting; \
    };

// Dont forget to add an imlementation of generated classes
CUSTOM_WIDGET(CustomSpinBox, QSpinBox, SettingInt)
CUSTOM_WIDGET(CustomSlider, QSlider, SettingInt)
CUSTOM_WIDGET(CustomDoubleSpinBox, QDoubleSpinBox, SettingDouble)
CUSTOM_WIDGET(CustomCheckBox, QCheckBox, SettingBool)
CUSTOM_WIDGET(CustomLineEdit, QLineEdit, SettingString)
CUSTOM_WIDGET(CustomComboBox, QComboBox, SettingStringList)
CUSTOM_WIDGET(CustomFontButton, QPushButton, SettingFont)
CUSTOM_WIDGET(CustomColorButton, QPushButton, SettingColor)
CUSTOM_WIDGET(CustomSourceButton, QPushButton, SettingSource)
CUSTOM_WIDGET(CustomTextEdit, QTextEdit, SettingEditableList)
CUSTOM_WIDGET(CustomListBox, QComboBox, SettingChangeableList)

class CustomLabel : public QLabel
{
    Q_OBJECT

 public:
    explicit CustomLabel(QWidget* parent = nullptr);
    void bindToSetting(custom_setting::Setting* setting);

 private:
    void update();
    custom_setting::Setting* mSetting;
};
