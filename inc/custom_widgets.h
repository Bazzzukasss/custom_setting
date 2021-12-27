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
#include <QListWidget>
#include <QDateTimeEdit>
#include "custom_setting.h"

#define DECLARE_CUSTOM_WIDGET(type, parent, setting) \
    class type : public parent \
    { \
        Q_OBJECT \
        using SettingType = custom_setting::setting; \
\
     public: \
        explicit type(QWidget* parent = nullptr); \
        void bindToSetting(SettingType* setting); \
\
        Q_SIGNAL void signalEditingFinished(); \
\
     protected: \
        void wheelEvent(QWheelEvent*) override { return; } \
\
     private: \
        void onSettingDataChanged(); \
        void onEditingFinished(); \
        void setSetting(SettingType* setting); \
        SettingType* mSetting; \
    };


DECLARE_CUSTOM_WIDGET(CustomSpinBox, QSpinBox, SettingInt)
DECLARE_CUSTOM_WIDGET(CustomSlider, QSlider, SettingInt)
DECLARE_CUSTOM_WIDGET(CustomDoubleSpinBox, QDoubleSpinBox, SettingDouble)
DECLARE_CUSTOM_WIDGET(CustomCheckBox, QCheckBox, SettingBool)
DECLARE_CUSTOM_WIDGET(CustomLineEdit, QLineEdit, SettingString)
DECLARE_CUSTOM_WIDGET(CustomComboBox, QComboBox, SettingStringList)
DECLARE_CUSTOM_WIDGET(CustomFontButton, QPushButton, SettingFont)
DECLARE_CUSTOM_WIDGET(CustomColorButton, QPushButton, SettingColor)
DECLARE_CUSTOM_WIDGET(CustomSourceButton, QPushButton, SettingSource)
DECLARE_CUSTOM_WIDGET(CustomListBox, QComboBox, SettingChangeableStringList)
DECLARE_CUSTOM_WIDGET(CustomDateTimeEdit, QDateTimeEdit, SettingDateTime)

class CustomLabel : public QLabel
{
    Q_OBJECT
    using SettingType = custom_setting::Setting;

public:
    explicit CustomLabel(QWidget* parent = nullptr);

    void bindToSetting(SettingType* setting);

private:
    SettingType* mSetting;

private:
    void onSettingDataChanged();
    void setSetting(SettingType* setting);
};


class CustomEditableListWidget : public QFrame
{
    Q_OBJECT
    using SettingType = custom_setting::SettingEditableStringList;

public:
    explicit CustomEditableListWidget(QWidget* parent = nullptr);

    void bindToSetting(SettingType* setting);

signals:
    void signalEditingFinished();

protected:
    void wheelEvent(QWheelEvent*) override { return; }

private:
    SettingType* mSetting;
    QListWidget* mListWidget;
    QPushButton* mApplyButton;
    QPushButton* mAddButton;
    QPushButton* mRemoveButton;

private:
    void onSettingDataChanged();
    void onEditingFinished();
    void setSetting(SettingType* setting);
    void onAddItem();
    void onRemoveItem();
};


class CustomCheckableListWidget : public QFrame
{
    Q_OBJECT
    using SettingType = custom_setting::SettingCheckableStringList;

public:
    explicit CustomCheckableListWidget(QWidget* parent = nullptr);

    void bindToSetting(SettingType* setting);

signals:
    void signalEditingFinished();

protected:
    void wheelEvent(QWheelEvent* event) override;

private:
    SettingType* mSetting;
    QListWidget* mListWidget;
    QPushButton* mApplyButton;

private:
    void onSettingDataChanged();
    void onEditingFinished();
    void setSetting(SettingType* setting);
};
