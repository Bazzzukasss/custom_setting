#include <QPainter>
#include <QLabel>
#include <QSortFilterProxyModel>
#include "custom_setting_item_delegate.h"
#include "custom_setting_item.h"
#include "custom_setting_widget.h"
#include "custom_setting_item_tree_model.h"

using namespace custom_setting;

Delegate::Delegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool isNeedRender{false};
    CustomSettingWidget widget;

    if(!index.isValid())
        return;

    QStyledItemDelegate::paint(painter, option, index);

    if(index.column() == 0)
    {
        auto item = getItem(index);
        if(item)
        {
            widget.setText(item->getCaption());
            isNeedRender = true;
        }
    }
    else
    {
        auto setting = getSetting(index);
        if(setting)
        {
            widget.setReadOnly(true);
            widget.bindToSetting(setting);
            isNeedRender = true;
        }
    }

    if(isNeedRender)
    {
        widget.setSizeHint(mItemWidth, mItemHeight, mItemsRowsCount);
        widget.setStyleSheet("background-color: transparent;");
        widget.setFixedSize(option.rect.size());

        QPixmap pixmap(widget.size());
        pixmap.fill(Qt::transparent);
        widget.render(&pixmap);
        painter->drawPixmap(option.rect, pixmap);
    }
}

QSize Delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto sHint = QStyledItemDelegate::sizeHint(option, index);
    if(index.isValid())
    {
        auto setting = getSetting(index);

        if(mItemHeight > 0)
        {
            int heightHint;
            heightHint = dynamic_cast<SettingEditableList*>(setting) ? mItemHeight * mItemsRowsCount : mItemHeight;
            sHint.setHeight(heightHint);
        }

        if(mItemWidth > 0)
        {
            sHint.setWidth(mItemWidth);
        }
    }

    return sHint;
}

QWidget *Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    auto setting = getSetting(index);
    CustomSettingWidget* pEditor = new CustomSettingWidget(setting, parent);
    pEditor->setSizeHint(mItemWidth, mItemHeight, mItemsRowsCount);
    connect(pEditor, &CustomSettingWidget::signalStateChanged, this, &Delegate::slotCommit);
    return pEditor;
}

void Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto pEditor = qobject_cast<CustomSettingWidget*>(editor);
    pEditor->setReadOnly(false);
}

void Delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    //CustomItemWidget* pEditor=qobject_cast<CustomItemWidget*>(editor);
    //model->setData(index,QVariant().fromValue(pEditor->getData()));
}

void Delegate::slotCommit()
{
    QWidget* pEditor=qobject_cast<QWidget*>(sender());
    emit commitData(pEditor);
    emit closeEditor(pEditor);
}

Setting *Delegate::getSetting(const QModelIndex &index) const
{
    return dynamic_cast<const ItemTreeModel*>(index.model())->getSetting(index);
}

Item *Delegate::getItem(const QModelIndex &index) const
{
    return dynamic_cast<const ItemTreeModel*>(index.model())->getItem(index);
}

void Delegate::setItemsHeight(int height)
{
    mItemHeight = height;
}

void Delegate::setItemsWidth(int width)
{
    mItemWidth = width;
}

void Delegate::setItemsRowsCount(int count)
{
    mItemsRowsCount = count;
}

void Delegate::setItemsSizeHint(int item_width, int item_height, int item_rows_count)
{
    mItemHeight     = item_width;
    mItemWidth      = item_height;
    mItemsRowsCount = item_rows_count;
}
