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
    explicit CustomSettingTreeWidget(custom_setting::Setting* setting,
                                     QWidget* parent);

    QTreeWidgetItem* add(custom_setting::Setting* setting,
                         const QStringList& styles = {},
                         const QIcon& icon = {});

    void showItemTooltip(bool isVisible);
    void setOneClickMode(bool isEnable);

    void setItemsHeight(int height);
    void setItemsWidth(int width);
    void setItemsRowsCount(int count);
    void setItemsSizeHint(int itemWidth, int itemHeight, int itemRowsCount);

 private:
    void createCaptionWidget(custom_setting::Setting* setting,
                              QTreeWidgetItem* subItem,
                              const QString& style);

    QWidget* createCustomWidget(custom_setting::Setting* setting);
    QWidget* createCustomTreeWidget(custom_setting::Setting* setting);

    void createValuesWidget(custom_setting::Setting* setting,
                            QTreeWidgetItem* subItem,
                            const QString& style);

    void createValuesWidget(custom_setting::Item* settingItem,
                            QTreeWidgetItem* subItem,
                            const QString& style);

    void createChildsWidget(custom_setting::Setting* setting,
                            QTreeWidgetItem* subItem,
                            const QStringList& styles);

    void createChildsWidget(custom_setting::Item* settingItem,
                            QTreeWidgetItem* subItem,
                            const QStringList& styles);

    QTreeWidgetItem* add(custom_setting::Setting* setting,
                         QTreeWidgetItem* item,
                         const QIcon &icon,
                         const QStringList& styles);

    void applySizeHint(int itemWidth, int itemHeight, int itemRowsCount);

private:
    int mTreeLevel{0};
    int mItemHeight{kDefaultItemHeight};
    int mItemWidth{kDefaultItemWidth};
    int mItemsRowsCount{kDefaultRowsPerItem};
    bool mShowTooltips{true};
    bool mIsOneClickMode{false};
};
