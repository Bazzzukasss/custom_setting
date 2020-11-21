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
    void setReadOnly(bool is_read_only);
    void setSizeHint(int item_width, int item_height, int item_rows_count);
    void setText(const QString& text);

signals:
    void signalStateChanged();

private:
    void hideAll();

    Ui::CustomSettingWidget *ui;
    bool mIsReadOnly{false};
};
