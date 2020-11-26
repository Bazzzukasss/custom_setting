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
    if(!index.isValid())
        return;

    if(index.column() == 0)
    {
        auto myOption = option;
        myOption.displayAlignment = Qt::AlignLeft | Qt::AlignTop;
        QStyledItemDelegate::paint(painter, myOption, index);
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
        auto setting = index.data().value<Setting*>();
        if(setting)
        {
            CustomSettingWidget widget;
            widget.setReadOnly(true);
            widget.bindToSetting(setting);
            widget.setSizeHint(mItemWidth, mItemHeight, mItemsRowsCount);
            widget.setStyleSheet("background-color: transparent;");
            widget.setFixedSize(option.rect.size());

            QPixmap pixmap(widget.size());
            pixmap.fill(Qt::transparent);
            widget.render(&pixmap);
            painter->drawPixmap(option.rect, pixmap);
        }
    }
}

QSize Delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto sHint = QStyledItemDelegate::sizeHint(option, index);
    if(index.isValid())
    {
        if(mItemHeight > 0)
        {
            int heightHint;
            auto setting = index.data().value<Setting*>();
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
    CustomSettingWidget* pEditor = new CustomSettingWidget(parent);
    pEditor->setSizeHint(mItemWidth, mItemHeight, mItemsRowsCount);
    pEditor->setReadOnly(false);
    connect(pEditor, &CustomSettingWidget::signalEditingFinished, this, &Delegate::slotCommit);

    return pEditor;
}

void Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto pEditor = qobject_cast<CustomSettingWidget*>(editor);
    auto setting = index.data().value<Setting*>();
    pEditor->setSetting(setting);
}

void Delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto pEditor = qobject_cast<CustomSettingWidget*>(editor);
    model->setData(index, pEditor->getSettingValue());
}

void Delegate::slotCommit()
{
    QWidget* pEditor = qobject_cast<QWidget*>(sender());
    emit commitData(pEditor);
    emit closeEditor(pEditor);
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
