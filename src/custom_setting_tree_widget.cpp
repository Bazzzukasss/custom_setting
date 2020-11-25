#include "ui_custom_setting_tree_widget.h"
#include "custom_setting_tree_widget.h"
#include "custom_widgets.h"
#include "custom_setting_widget.h"

using namespace custom_setting;

CustomSettingTreeWidget::CustomSettingTreeWidget(QWidget* parent) :
    CustomSettingTreeWidget(nullptr, parent)
{    
}

CustomSettingTreeWidget::CustomSettingTreeWidget(Setting *setting, QWidget *parent) :
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

    if(setting != nullptr)
    {
        add(setting);
        setToolTip(setting->getDescription());
    }
}

QTreeWidgetItem* CustomSettingTreeWidget::add(Setting* setting, QTreeWidgetItem* item,
                                            const QIcon& icon, const QStringList& styles)
{
    if (setting->getCaption().isEmpty())
        return nullptr;

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
    if(customItem)
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

QTreeWidgetItem* CustomSettingTreeWidget::add(Setting* setting, const QStringList& styles, const QIcon &icon)
{
    return add(setting, nullptr, icon, styles);
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

void CustomSettingTreeWidget::setItemsSizeHint(int item_width, int item_height, int item_rows_count)
{
    mItemHeight     = item_width;
    mItemWidth      = item_height;
    mItemsRowsCount = item_rows_count;
}

void CustomSettingTreeWidget::applySizeHint(int item_width, int item_height, int item_rows_count)
{
    setItemsSizeHint(item_width, item_height, item_rows_count);
    if (item_height > 0)
    {
        setMinimumHeight(item_height);
        setMaximumHeight(item_height * item_rows_count);
    }

    if (item_width > 0)
    {
        setFixedWidth(item_width);
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

void CustomSettingTreeWidget::createCaptionWidget(Setting *setting, QTreeWidgetItem *sub_item, const QString &style)
{
    auto captionWidget = new QLabel(setting->getCaption());
    captionWidget->setMaximumHeight(mItemHeight);
    captionWidget->setMinimumHeight(mItemHeight);
    captionWidget->setStyleSheet(style);
    captionWidget->setToolTip(mShowTooltips ? setting->getDescription() : "");
    setItemWidget(sub_item, 0, captionWidget);
}

void CustomSettingTreeWidget::createValuesWidget(Setting *setting, QTreeWidgetItem *sub_item, const QString& style)
{
    auto valueWidget = createCustomWidget(setting);
    if (valueWidget)
    {
        valueWidget->setStyleSheet(style);
        setItemWidget(sub_item, 1, valueWidget);
    }
}

void CustomSettingTreeWidget::createValuesWidget(Item* setting_item, QTreeWidgetItem* sub_item, const QString& style)
{
    int column{1};
    auto settings = setting_item->getSettings();
    if(columnCount() < settings.size())
    {
        setColumnCount(settings.size() + 1);
    }

    for (auto setting : settings)
    {
        auto valueWidget = setting->getSettings().isEmpty() ? createCustomWidget(setting)
                                                            : createCustomTreeWidget(setting);
        if (valueWidget)
        {
            valueWidget->setStyleSheet(style);
            setItemWidget(sub_item, column++, valueWidget);
        }
    }
}

void CustomSettingTreeWidget::createChildsWidget(Setting *setting, QTreeWidgetItem *sub_item, const QStringList &styles)
{
    mTreeLevel++;
    for (auto child_setting : setting->getSettings())
    {
        add(child_setting, sub_item, {}, styles);
    }
    mTreeLevel--;
}

void CustomSettingTreeWidget::createChildsWidget(Item* setting_item, QTreeWidgetItem* sub_item, const QStringList& styles)
{
    mTreeLevel++;
    for (auto child_item : setting_item->getItems())
    {
        add(child_item, sub_item, {}, styles);
    }
    mTreeLevel--;
}
