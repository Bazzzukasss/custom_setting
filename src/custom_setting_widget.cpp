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
#define BIND_SETTING(custom_widget, custom_setting) \
    ui->custom_widget->setVisible(custom_setting);\
    if (custom_setting)\
    {\
        ui->custom_widget->bindToSetting(custom_setting);\
        return;\
    }

    hideAll();

    if(setting->isReadOnly())
    {
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

        BIND_SETTING(mCheckBox, boolSetting)
        BIND_SETTING(mSpinBox, intSetting)
        BIND_SETTING(mDoubleSpinBox, doubleSetting)
        BIND_SETTING(mLineEdit, stringSetting)
        BIND_SETTING(mComboBox, listSetting)
        BIND_SETTING(mFontButton, fontSetting)
        BIND_SETTING(mColorButton, colorSetting)
        BIND_SETTING(mSourceButton, sourceSetting)
        BIND_SETTING(mListBox, changeableListSetting)
        BIND_SETTING(mTextEdit, editableListSetting)
    }
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
