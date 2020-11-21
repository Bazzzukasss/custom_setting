#include "custom_setting_widget.h"
#include "ui_custom_setting_widget.h"

using namespace custom_setting;

CustomSettingWidget::CustomSettingWidget(QWidget *parent) :
    CustomSettingWidget(nullptr, parent)
{    
}

CustomSettingWidget::CustomSettingWidget(Setting* setting, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomSettingWidget)
{
    ui->setupUi(this);

    bindToSetting(setting);
}

CustomSettingWidget::~CustomSettingWidget()
{
    delete ui;
}

void CustomSettingWidget::bindToSetting(custom_setting::Setting *setting)
{
#define BIND_SETTING(widget_type, custom_widget, custom_setting) \
    ui->custom_widget->setVisible(custom_setting);\
    if (custom_setting)\
    {\
        ui->custom_widget->bindToSetting(custom_setting);\
        connect(ui->custom_widget, &widget_type::signalStateChanged, this, &CustomSettingWidget::signalStateChanged);\
        return;\
    }

    hideAll();

    if(!setting)
    {
        ui->mLabel->clear();
        ui->mLabel->setStyleSheet("");
        ui->mLabel->setVisible(true);
        return;
    }

    setToolTip(setting->getDescription());

    if(setting->isReadOnly() || mIsReadOnly)
    {
        ui->mLabel->clear();
        ui->mLabel->setStyleSheet("");
        ui->mLabel->bindToSetting(setting);
        ui->mLabel->setVisible(true);
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

        BIND_SETTING(CustomCheckBox, mCheckBox, boolSetting)
        BIND_SETTING(CustomSpinBox, mSpinBox, intSetting)
        BIND_SETTING(CustomDoubleSpinBox, mDoubleSpinBox, doubleSetting)
        BIND_SETTING(CustomLineEdit, mLineEdit, stringSetting)
        BIND_SETTING(CustomComboBox, mComboBox, listSetting)
        BIND_SETTING(CustomFontButton, mFontButton, fontSetting)
        BIND_SETTING(CustomColorButton, mColorButton, colorSetting)
        BIND_SETTING(CustomSourceButton, mSourceButton, sourceSetting)
        BIND_SETTING(CustomListBox, mListBox, changeableListSetting)
        BIND_SETTING(CustomTextEdit, mTextEdit, editableListSetting)
    }
}

void CustomSettingWidget::setReadOnly(bool is_read_only)
{
    mIsReadOnly = is_read_only;
}

void CustomSettingWidget::setSizeHint(int item_width, int item_height, int item_rows_count)
{
    if (item_height > 0)
    {
        setMinimumHeight(item_height);
        setMaximumHeight(item_height * item_rows_count);
    }

    if (item_width > 0)
    {
        setFixedWidth(item_width);
    }

    for(auto child : children())
    {
        auto widget = qobject_cast<QWidget*>(child);
        if(widget)
        {
            widget->setMinimumWidth(minimumWidth());
            widget->setMaximumWidth(maximumWidth());
            widget->setMinimumHeight(minimumHeight());
            widget->setMaximumHeight(maximumHeight());
        }
    }
}

void CustomSettingWidget::setText(const QString &text)
{
    ui->mLabel->setText(text);
}

void CustomSettingWidget::hideAll()
{
    for(auto w : children())
    {
        if(w != ui->verticalLayout)
        {
            qobject_cast<QWidget*>(w)->setVisible(false);
        }
    }
}
