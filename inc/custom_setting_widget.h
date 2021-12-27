#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include "custom_setting.h"
#include "custom_widgets.h"

class QVBoxLayout;

namespace custom_setting{
class Setting;
}

class CustomSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomSettingWidget(QWidget* parent = nullptr);
    ~CustomSettingWidget();

    void bindToSetting(custom_setting::Setting* setting);
    void setSetting(custom_setting::Setting* setting);
    void setReadOnly(bool isReadOnly);
    void setSizeHint(int itemWidth, int itemHeight, int itemRowsCount);

    QVariant getSettingValue() const;

signals:
    void signalEditingFinished();
    void signalWidgetDeleted(QWidget* editor);

private:
    bool mIsReadOnly{false};
    QWidget* mWidget{nullptr};
    QVBoxLayout* mLayout{nullptr};
    custom_setting::Setting* mSetting{nullptr};
    int mItemHeight{-1};
    int mItemWidth{-1};
    int mItemsRowsCount{1};

private:
    void clear();
    void applySizeHint();

    template <typename SettingType, typename WidgetType>
    bool bindSetting_if(custom_setting::Setting* setting)
    {
        if (auto customSetting = dynamic_cast<SettingType*>(setting))
        {
            auto customWidget = new WidgetType();
            mWidget = customWidget;
            customWidget->bindToSetting(customSetting);
            connect(customWidget, &WidgetType::signalEditingFinished,
                    this, &CustomSettingWidget::signalEditingFinished);
            mLayout->insertWidget(0, mWidget);
            applySizeHint();

            return true;
        }

        return false;
    }

    template<typename SettingType, typename WidgetType>
    bool setSetting_if(custom_setting::Setting* setting)
    {
        if (auto customSetting = dynamic_cast<SettingType*>(setting))
        {
            auto tmpSetting = new SettingType(*customSetting, this);
            mSetting = tmpSetting;
            if (setting->isReadOnly() || mIsReadOnly)
            {
                auto customWidget = new CustomLabel;
                customWidget->bindToSetting(tmpSetting);
                mWidget = customWidget;
            }
            else
            {
                auto customWidget = new WidgetType();
                customWidget->bindToSetting(tmpSetting);
                connect(customWidget, &WidgetType::signalEditingFinished,
                        this, &CustomSettingWidget::signalEditingFinished);
                mWidget = customWidget;
            }
            mLayout->insertWidget(0, mWidget);
            applySizeHint();

            return true;
        }

        return false;
    }
};
