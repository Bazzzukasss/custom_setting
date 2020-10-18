#pragma once

#include <QWidget>
#include "custom_setting.h"

namespace Ui {
class CustomSettingWidget;
}

class CustomSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomSettingWidget(QWidget *parent = nullptr);
    explicit CustomSettingWidget(custom_setting::Setting* setting, QWidget *parent = nullptr);
    ~CustomSettingWidget();

    void bindToSetting(custom_setting::Setting* setting);

private:
    Ui::CustomSettingWidget *ui;
};
