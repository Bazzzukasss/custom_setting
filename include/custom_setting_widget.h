#pragma once

#include <QWidget>

class QVBoxLayout;

namespace custom_setting{
class Setting;
}

class CustomSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomSettingWidget(QWidget *parent = nullptr);
    ~CustomSettingWidget();

    void bindToSetting(custom_setting::Setting* setting);
    void setSetting(custom_setting::Setting* setting);
    void setReadOnly(bool is_read_only);
    void setSizeHint(int item_width, int item_height, int item_rows_count);

    const custom_setting::Setting* getSetting() const;

signals:
    void signalEditingFinished();

private:
    bool mIsReadOnly{false};
    QWidget* mWidget{nullptr};
    QVBoxLayout* mLayout{nullptr};
    custom_setting::Setting* mSetting{nullptr};
    int mItemHeight{-1};
    int mItemWidth{-1};
    int mItemsRowsCount{1};

    void clear();
    void applySizeHint();
};
