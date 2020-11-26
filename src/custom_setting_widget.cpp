#include <QVBoxLayout>
#include "custom_setting_widget.h"
#include "custom_setting.h"
#include "custom_widgets.h"

using namespace custom_setting;

CustomSettingWidget::CustomSettingWidget(QWidget *parent) :
    QWidget(parent)
{
    mLayout = new QVBoxLayout(this);
    mLayout->setSpacing(0);
    mLayout->setMargin(0);
    mLayout->addStretch();
}

CustomSettingWidget::~CustomSettingWidget()
{
    clear();
}

void CustomSettingWidget::bindToSetting(custom_setting::Setting *setting)
{
#define BIND_SETTING(widget_type, custom_setting) \
    if (custom_setting)\
    {\
        auto customWidget = new widget_type;\
        mWidget = customWidget;\
        customWidget->bindToSetting(custom_setting);\
        connect(customWidget, &widget_type::signalStateChanged, this, &CustomSettingWidget::signalEditingFinished);\
        mLayout->insertWidget(0, mWidget); \
        applySizeHint();\
        return;\
    }

    clear();

    if(!setting)
        return;

    setToolTip(setting->getDescription());

    if(setting->isReadOnly() || mIsReadOnly)
    {
        auto customWidget = new CustomLabel;
        customWidget->bindToSetting(setting);
        mWidget = customWidget;
        mLayout->insertWidget(0, mWidget);
        applySizeHint();
    }
    else
    {
        auto boolSetting            = dynamic_cast<SettingBool*>(setting);
        auto intSetting             = dynamic_cast<SettingInt*>(setting);
        auto doubleSetting          = dynamic_cast<SettingDouble*>(setting);
        auto stringSetting          = dynamic_cast<SettingString*>(setting);
        auto listSetting            = dynamic_cast<SettingStringList*>(setting);
        auto fontSetting            = dynamic_cast<SettingFont*>(setting);
        auto colorSetting           = dynamic_cast<SettingColor*>(setting);
        auto sourceSetting          = dynamic_cast<SettingSource*>(setting);
        auto editableListSetting    = dynamic_cast<SettingEditableList*>(setting);
        auto changeableListSetting  = dynamic_cast<SettingChangeableList*>(setting);

        BIND_SETTING(CustomCheckBox, boolSetting)
        BIND_SETTING(CustomSpinBox, intSetting)
        BIND_SETTING(CustomDoubleSpinBox, doubleSetting)
        BIND_SETTING(CustomLineEdit, stringSetting)
        BIND_SETTING(CustomComboBox, listSetting)
        BIND_SETTING(CustomFontButton, fontSetting)
        BIND_SETTING(CustomColorButton, colorSetting)
        BIND_SETTING(CustomSourceButton, sourceSetting)
        BIND_SETTING(CustomListBox, changeableListSetting)
        BIND_SETTING(CustomTextEdit, editableListSetting)
    }
}

void CustomSettingWidget::setSetting(Setting *setting)
{
#define SET_SETTING(widget_type, setting_type, custom_setting) \
    if(custom_setting) \
    { \
        auto tmpSetting = new setting_type(*custom_setting, this); \
        mSetting = tmpSetting; \
        if(setting->isReadOnly() || mIsReadOnly) \
        { \
            auto customWidget = new CustomLabel; \
            customWidget->bindToSetting(tmpSetting); \
            mWidget = customWidget; \
        } \
        else \
        { \
            auto customWidget = new widget_type; \
            customWidget->bindToSetting(tmpSetting); \
            connect(customWidget, &widget_type::signalStateChanged, this, &CustomSettingWidget::signalEditingFinished); \
            mWidget = customWidget; \
        } \
        mLayout->insertWidget(0, mWidget); \
        applySizeHint(); \
        return; \
    }

    clear();

    if(!setting)
        return;

    setToolTip(setting->getDescription());

    auto boolSetting            = dynamic_cast<SettingBool*>(setting);
    auto intSetting             = dynamic_cast<SettingInt*>(setting);
    auto doubleSetting          = dynamic_cast<SettingDouble*>(setting);
    auto stringSetting          = dynamic_cast<SettingString*>(setting);
    auto listSetting            = dynamic_cast<SettingStringList*>(setting);
    auto fontSetting            = dynamic_cast<SettingFont*>(setting);
    auto colorSetting           = dynamic_cast<SettingColor*>(setting);
    auto sourceSetting          = dynamic_cast<SettingSource*>(setting);
    auto editableListSetting    = dynamic_cast<SettingEditableList*>(setting);
    auto changeableListSetting  = dynamic_cast<SettingChangeableList*>(setting);

    SET_SETTING(CustomCheckBox, SettingBool, boolSetting)
    SET_SETTING(CustomSpinBox, SettingInt, intSetting)
    SET_SETTING(CustomDoubleSpinBox, SettingDouble, doubleSetting)
    SET_SETTING(CustomLineEdit, SettingString, stringSetting)
    SET_SETTING(CustomComboBox, SettingStringList, listSetting)
    SET_SETTING(CustomFontButton, SettingFont, fontSetting)
    SET_SETTING(CustomColorButton, SettingColor, colorSetting)
    SET_SETTING(CustomSourceButton, SettingSource, sourceSetting)
    SET_SETTING(CustomListBox, SettingChangeableList, changeableListSetting)
    SET_SETTING(CustomTextEdit, SettingEditableList, editableListSetting)
}

void CustomSettingWidget::setReadOnly(bool is_read_only)
{
    mIsReadOnly = is_read_only;
}

void CustomSettingWidget::setSizeHint(int item_width, int item_height, int item_rows_count)
{
    mItemHeight = item_height;
    mItemWidth = item_width;
    mItemsRowsCount = item_rows_count;

    applySizeHint();
}

QVariant CustomSettingWidget::getSettingValue() const
{
    return mSetting ? mSetting->getValue() : QVariant();
}

void CustomSettingWidget::clear()
{
    if(mSetting)
        delete mSetting;

    if(mWidget)
    {
        mLayout->removeWidget(mWidget);
        delete mWidget;
    }
}

void CustomSettingWidget::applySizeHint()
{
    if (mItemHeight > 0)
    {
        setMinimumHeight(mItemHeight);
        setMaximumHeight(mItemHeight * mItemsRowsCount);
    }

    if (mItemWidth > 0)
    {
        setFixedWidth(mItemWidth);
    }

    if(mWidget)
    {
        mWidget->setMinimumWidth(minimumWidth());
        mWidget->setMaximumWidth(maximumWidth());
        mWidget->setMinimumHeight(minimumHeight());
        mWidget->setMaximumHeight(maximumHeight());
    }
}
