#include "custom_setting_widget.h"

using namespace custom_setting;

CustomSettingWidget::CustomSettingWidget(QWidget* parent) :
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
    emit signalWidgetDeleted(this);
}

void CustomSettingWidget::bindToSetting(Setting* setting)
{
    clear();

    if (!setting)
    {
        return;
    }

    setToolTip(setting->getDescription());

    if (setting->isReadOnly() || mIsReadOnly)
    {
        auto customWidget = new CustomLabel();
        customWidget->bindToSetting(setting);
        mWidget = customWidget;
        mLayout->insertWidget(0, mWidget);
        applySizeHint();
    }
    else
    {
        std::vector<std::function<bool()>> bindMethods = {
            {[&]{ return bindSetting_if<SettingBool, CustomCheckBox>(setting);}},
            {[&]{ return bindSetting_if<SettingInt, CustomSpinBox>(setting);}},
            {[&]{ return bindSetting_if<SettingDouble, CustomDoubleSpinBox>(setting);}},
            {[&]{ return bindSetting_if<SettingString, CustomLineEdit>(setting);}},
            {[&]{ return bindSetting_if<SettingStringList, CustomComboBox>(setting);}},
            {[&]{ return bindSetting_if<SettingFont, CustomFontButton>(setting);}},
            {[&]{ return bindSetting_if<SettingColor, CustomColorButton>(setting);}},
            {[&]{ return bindSetting_if<SettingSource, CustomSourceButton>(setting);}},
            {[&]{ return bindSetting_if<SettingDateTime, CustomDateTimeEdit>(setting);}},
            {[&]{ return bindSetting_if<SettingEditableStringList, CustomEditableListWidget>(setting);}},
            {[&]{ return bindSetting_if<SettingChangeableStringList, CustomListBox>(setting);}},
            {[&]{ return bindSetting_if<SettingCheckableStringList, CustomCheckableListWidget>(setting);}},
        };

        for (const auto& bindMethod : bindMethods)
        {
            if (bindMethod())
            {
                break;
            }
        }
    }
}

void CustomSettingWidget::setSetting(Setting *setting)
{
    clear();

    if (!setting)
    {
        return;
    }

    setToolTip(setting->getDescription());

    std::vector<std::function<bool()>> setMethods = {
        {[&]{ return setSetting_if<SettingBool, CustomCheckBox>(setting);}},
        {[&]{ return setSetting_if<SettingInt, CustomSpinBox>(setting);}},
        {[&]{ return setSetting_if<SettingDouble, CustomDoubleSpinBox>(setting);}},
        {[&]{ return setSetting_if<SettingString, CustomLineEdit>(setting);}},
        {[&]{ return setSetting_if<SettingStringList, CustomComboBox>(setting);}},
        {[&]{ return setSetting_if<SettingFont, CustomFontButton>(setting);}},
        {[&]{ return setSetting_if<SettingColor, CustomColorButton>(setting);}},
        {[&]{ return setSetting_if<SettingSource, CustomSourceButton>(setting);}},
        {[&]{ return setSetting_if<SettingDateTime, CustomDateTimeEdit>(setting);}},
        {[&]{ return setSetting_if<SettingEditableStringList, CustomEditableListWidget>(setting);}},
        {[&]{ return setSetting_if<SettingChangeableStringList, CustomListBox>(setting);}},
        {[&]{ return setSetting_if<SettingCheckableStringList, CustomCheckableListWidget>(setting);}},
        };

    for (const auto& setMethod : setMethods)
    {
        if (setMethod())
        {
            break;
        }
    }
}

void CustomSettingWidget::setReadOnly(bool isReadOnly)
{
    mIsReadOnly = isReadOnly;
}

void CustomSettingWidget::setSizeHint(int itemWidth,
                                      int itemHeight,
                                      int itemRowsCount)
{
    mItemHeight = itemHeight;
    mItemWidth = itemWidth;
    mItemsRowsCount = itemRowsCount;

    applySizeHint();
}

QVariant CustomSettingWidget::getSettingValue() const
{
    return mSetting ? mSetting->getValue() : QVariant();
}

void CustomSettingWidget::clear()
{
    if (mSetting)
    {
        delete mSetting;
    }

    if (mWidget)
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

    if (mWidget)
    {
        mWidget->setMinimumWidth(minimumWidth());
        mWidget->setMaximumWidth(maximumWidth());
        mWidget->setMinimumHeight(minimumHeight());
        mWidget->setMaximumHeight(maximumHeight());
    }
}
