#pragma once

#include <QtWidgets>
#include "custom_setting.h"
#include "custom_setting_item.h"

namespace Ui
{
class CustomSettingTreeWidget;
}

class CustomSettingTreeWidget : public QWidget
{
    Q_OBJECT

    const int kDefaultItemHeight{26};
    const int kDefaultItemWidth{100};
    const int kDefaultRowsPerItem{8};

 public:
    explicit CustomSettingTreeWidget(QWidget* parent = nullptr);
    explicit CustomSettingTreeWidget(custom_setting::Setting* setting, QWidget* parent);
    ~CustomSettingTreeWidget();

    QTreeWidgetItem* add(custom_setting::Setting* setting, const QStringList& styles = {},
                                const QIcon& icon = {});

    QTreeView* view();

    inline void setItemHeight(int height) { mItemHeight = height; }
    inline void setItemWidth(int width) { mItemWidth = width; }
    inline void setRowsPerItem(int count) { mRowsPerItem = count; }
    inline void showItemTooltip(bool is_visible) { mShowTooltips = is_visible; }
    inline void setOneClickMode(bool is_enable) { mIsOneClickMode = is_enable; }

 private:
    void createCaptionWidget(custom_setting::Setting* setting, QTreeWidgetItem *sub_item, const QString& style);

    QWidget* createCustomWidget(custom_setting::Setting* setting);
    QWidget* createCustomTreeWidget(custom_setting::Setting* setting);

    void createValuesWidget(custom_setting::Setting* setting, QTreeWidgetItem* sub_item, const QString& style);
    void createValuesWidget(custom_setting::Item* setting_item, QTreeWidgetItem* sub_item, const QString& style);

    void createChildsWidget(custom_setting::Setting* setting, QTreeWidgetItem* sub_item, const QStringList& styles);
    void createChildsWidget(custom_setting::Item* setting_item, QTreeWidgetItem* sub_item, const QStringList& styles);

    QTreeWidgetItem* add(custom_setting::Setting* setting, QTreeWidgetItem* item,
                                const QIcon &icon, const QStringList& styles);

    void setSizeHint(QWidget *widget);

    Ui::CustomSettingTreeWidget* ui;
    int mTreeLevel{0};
    int mItemHeight{kDefaultItemHeight};
    int mItemWidth{kDefaultItemWidth};
    int mRowsPerItem{kDefaultRowsPerItem};
    bool mShowTooltips{true};
    bool mIsOneClickMode{false};
};
