#pragma once

#include <QTreeWidget>
#include "custom_setting.h"
#include "custom_setting_item.h"

class CustomSettingTreeWidget : public QTreeWidget
{
    Q_OBJECT

    static const int kDefaultItemHeight{26};
    static const int kDefaultItemWidth{100};
    static const int kDefaultRowsPerItem{4};

 public:
    explicit CustomSettingTreeWidget(QWidget* parent = nullptr);
    explicit CustomSettingTreeWidget(custom_setting::Setting* setting, QWidget* parent);

    QTreeWidgetItem* add(custom_setting::Setting* setting, const QStringList& styles = {},
                                const QIcon& icon = {});

    inline void showItemTooltip(bool is_visible) { mShowTooltips = is_visible; }
    inline void setOneClickMode(bool is_enable) { mIsOneClickMode = is_enable; }

    void setItemsHeight(int height);
    void setItemsWidth(int width);
    void setItemsRowsCount(int count);
    void setItemsSizeHint(int item_width, int item_height, int item_rows_count);

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

    void applySizeHint(int item_width, int item_height, int item_rows_count);

    int mTreeLevel{0};
    int mItemHeight{kDefaultItemHeight};
    int mItemWidth{kDefaultItemWidth};
    int mItemsRowsCount{kDefaultRowsPerItem};
    bool mShowTooltips{true};
    bool mIsOneClickMode{false};
};
