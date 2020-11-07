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
    QWidget(parent), ui(new Ui::CustomSettingTreeWidget)
{
    ui->setupUi(this);

    connect(ui->mTreeWidget, &QTreeView::clicked, [this]() {
        if (mIsOneClickMode && ui->mTreeWidget->currentItem() && ui->mTreeWidget->currentItem()->childCount() > 0)
        {
            if (ui->mTreeWidget->isExpanded(ui->mTreeWidget->currentIndex()))
            {
                ui->mTreeWidget->collapse(ui->mTreeWidget->currentIndex());
            }
            else
            {
                ui->mTreeWidget->expand(ui->mTreeWidget->currentIndex());
            }
        }
    });

    if(setting != nullptr)
    {
        add(setting);
        setToolTip(setting->getDescription());
    }
}

CustomSettingTreeWidget::~CustomSettingTreeWidget()
{
    delete ui;
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
        ui->mTreeWidget->addTopLevelItem(sub_item);
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

void CustomSettingTreeWidget::setSizeHint(QWidget* widget)
{
    if (mItemHeight > 0)
    {
        widget->setMinimumHeight(mItemHeight);
        widget->setMaximumHeight(mItemHeight * mRowsPerItem);
    }

    if (mItemWidth > 0)
    {
        widget->setMinimumWidth(mItemWidth);
        widget->setMaximumWidth(mItemWidth);
    }
}

QTreeWidgetItem* CustomSettingTreeWidget::add(Setting* setting, const QStringList& styles, const QIcon &icon)
{
    return add(setting, nullptr, icon, styles);
}

QTreeView* CustomSettingTreeWidget::view()
{
    return ui->mTreeWidget;
}

QWidget* CustomSettingTreeWidget::createCustomWidget(Setting* setting)
{
    auto widget = new CustomSettingWidget(setting, this);
    setSizeHint(widget);

    return widget;
}

QWidget* CustomSettingTreeWidget::createCustomTreeWidget(Setting* setting)
{
    auto widget = new CustomSettingTreeWidget(setting, this);

    widget->setItemHeight(mItemHeight);
    widget->setItemWidth(mItemWidth);
    widget->setRowsPerItem(mRowsPerItem);
    widget->setOneClickMode(mIsOneClickMode);
    widget->view()->setAlternatingRowColors(view()->alternatingRowColors());
    setSizeHint(widget);

    return widget;
}

void CustomSettingTreeWidget::createCaptionWidget(Setting *setting, QTreeWidgetItem *sub_item, const QString &style)
{
    auto captionWidget = new QLabel(setting->getCaption());
    captionWidget->setMaximumHeight(mItemHeight);
    captionWidget->setMinimumHeight(mItemHeight);
    captionWidget->setStyleSheet(style);
    captionWidget->setToolTip(mShowTooltips ? setting->getDescription() : "");
    ui->mTreeWidget->setItemWidget(sub_item, 0, captionWidget);
}

void CustomSettingTreeWidget::createValuesWidget(Setting *setting, QTreeWidgetItem *sub_item, const QString& style)
{
    auto valueWidget = createCustomWidget(setting);
    if (valueWidget)
    {
        valueWidget->setStyleSheet(style);
        ui->mTreeWidget->setItemWidget(sub_item, 1, valueWidget);
    }
}

void CustomSettingTreeWidget::createValuesWidget(Item* setting_item, QTreeWidgetItem* sub_item, const QString& style)
{
    int column{1};
    auto settings = setting_item->getSettings();
    if(ui->mTreeWidget->columnCount() < settings.size())
    {
        ui->mTreeWidget->setColumnCount(settings.size() + 1);
    }

    for (auto setting : settings)
    {
        auto valueWidget = setting->getSettings().isEmpty() ? createCustomWidget(setting)
                                                            : createCustomTreeWidget(setting);
        if (valueWidget)
        {
            valueWidget->setStyleSheet(style);
            ui->mTreeWidget->setItemWidget(sub_item, column++, valueWidget);
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
