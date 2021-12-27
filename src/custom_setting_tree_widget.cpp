#include "custom_setting_tree_widget.h"
#include "custom_widgets.h"
#include "custom_setting_widget.h"

using namespace custom_setting;

CustomSettingTreeWidget::CustomSettingTreeWidget(QWidget* parent) :
    CustomSettingTreeWidget(nullptr, parent)
{    
}

CustomSettingTreeWidget::CustomSettingTreeWidget(Setting* setting, QWidget* parent) :
    QTreeWidget(parent)
{
    setHeaderHidden(true);
    connect(this, &QTreeWidget::clicked, [this]() {
        if (mIsOneClickMode && currentItem() && currentItem()->childCount() > 0)
        {
            if (isExpanded(currentIndex()))
            {
                collapse(currentIndex());
            }
            else
            {
                expand(currentIndex());
            }
        }
    });

    if (setting != nullptr)
    {
        add(setting);
        setToolTip(setting->getDescription());
    }
}

QTreeWidgetItem* CustomSettingTreeWidget::add(Setting* setting,
                                              QTreeWidgetItem* item,
                                              const QIcon& icon,
                                              const QStringList& styles)
{
    if (setting->getCaption().isEmpty())
    {
        return nullptr;
    }

    QString style;
    auto sub_item = new QTreeWidgetItem();

    if (!styles.empty())
    {
        style = mTreeLevel < styles.size() ? styles[mTreeLevel] : styles.back();
    }

    if (item == nullptr)
    {
        addTopLevelItem(sub_item);
    }
    else
    {
        item->addChild(sub_item);
        if (!icon.isNull())
        {
            item->setIcon(0, icon);
        }
    }    

    createCaptionWidget(setting, sub_item, style);

    auto customItem = dynamic_cast<Item*>(setting);
    if (customItem)
    {
        createValuesWidget(customItem, sub_item, style);
        createChildsWidget(customItem, sub_item, styles);
    }
    else
    {
        createValuesWidget(setting, sub_item, style);
        createChildsWidget(setting, sub_item, styles);
    }

    return item;
}

QTreeWidgetItem* CustomSettingTreeWidget::add(Setting* setting,
                                              const QStringList& styles,
                                              const QIcon& icon)
{
    return add(setting, nullptr, icon, styles);
}

void CustomSettingTreeWidget::showItemTooltip(bool isVisible)
{
    mShowTooltips = isVisible;
}

void CustomSettingTreeWidget::setOneClickMode(bool isEnable)
{
    mIsOneClickMode = isEnable;
}

void CustomSettingTreeWidget::setItemsHeight(int height)
{
    mItemHeight = height;
}

void CustomSettingTreeWidget::setItemsWidth(int width)
{
    mItemWidth = width;
}

void CustomSettingTreeWidget::setItemsRowsCount(int count)
{
    mItemsRowsCount = count;
}

void CustomSettingTreeWidget::setItemsSizeHint(int itemWidth,
                                               int itemHeight,
                                               int itemRowsCount)
{
    mItemHeight     = itemWidth;
    mItemWidth      = itemHeight;
    mItemsRowsCount = itemRowsCount;
}

void CustomSettingTreeWidget::applySizeHint(int itemWidth,
                                            int itemHeight,
                                            int itemRowsCount)
{
    setItemsSizeHint(itemWidth, itemHeight, itemRowsCount);
    if (itemHeight > 0)
    {
        setMinimumHeight(itemHeight);
        setMaximumHeight(itemHeight * itemRowsCount);
    }

    if (itemWidth > 0)
    {
        setFixedWidth(itemWidth);
    }
}
QWidget* CustomSettingTreeWidget::createCustomWidget(Setting* setting)
{
    auto widget = new CustomSettingWidget(this);

    widget->bindToSetting(setting);
    widget->setSizeHint(mItemWidth, mItemHeight, mItemsRowsCount);

    return widget;
}

QWidget* CustomSettingTreeWidget::createCustomTreeWidget(Setting* setting)
{
    auto widget = new CustomSettingTreeWidget(setting, this);

    widget->setOneClickMode(mIsOneClickMode);
    widget->setAlternatingRowColors(alternatingRowColors());
    widget->applySizeHint(mItemWidth, mItemHeight, mItemsRowsCount);

    return widget;
}

void CustomSettingTreeWidget::createCaptionWidget(Setting* setting,
                                                  QTreeWidgetItem* subItem,
                                                  const QString& style)
{
    auto captionWidget = new QLabel(setting->getCaption());

    captionWidget->setMaximumHeight(mItemHeight);
    captionWidget->setMinimumHeight(mItemHeight);
    captionWidget->setStyleSheet(style);
    captionWidget->setToolTip(mShowTooltips ? setting->getDescription() : "");
    setItemWidget(subItem, 0, captionWidget);
}

void CustomSettingTreeWidget::createValuesWidget(Setting* setting,
                                                 QTreeWidgetItem* subItem,
                                                 const QString& style)
{
    auto valueWidget = createCustomWidget(setting);
    if (valueWidget)
    {
        valueWidget->setStyleSheet(style);
        setItemWidget(subItem, 1, valueWidget);
    }
}

void CustomSettingTreeWidget::createValuesWidget(Item* settingItem,
                                                 QTreeWidgetItem* subItem,
                                                 const QString& style)
{
    int column{1};
    auto settings = settingItem->getSettings();
    if(columnCount() < settings.size())
    {
        setColumnCount(settings.size() + 1);
    }

    for (auto setting : settings)
    {
        auto valueWidget =
            setting->getSettings().isEmpty() ? createCustomWidget(setting)
                                             : createCustomTreeWidget(setting);
        if (valueWidget)
        {
            valueWidget->setStyleSheet(style);
            setItemWidget(subItem, column++, valueWidget);
        }
    }
}

void CustomSettingTreeWidget::createChildsWidget(Setting* setting,
                                                 QTreeWidgetItem* subItem,
                                                 const QStringList& styles)
{
    mTreeLevel++;
    for (auto child_setting : setting->getSettings())
    {
        add(child_setting, subItem, {}, styles);
    }
    mTreeLevel--;
}

void CustomSettingTreeWidget::createChildsWidget(Item* settingItem,
                                                 QTreeWidgetItem* subItem,
                                                 const QStringList& styles)
{
    mTreeLevel++;
    for (auto child_item : settingItem->getItems())
    {
        add(child_item, subItem, {}, styles);
    }
    mTreeLevel--;
}
