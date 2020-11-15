#include <QPainter>
#include <QSortFilterProxyModel>
#include "custom_setting_item_delegate.h"
#include "custom_setting_item.h"
#include "custom_setting_widget.h"

using namespace custom_setting;

Delegate::Delegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    mWidget = new CustomSettingWidget();
}

Delegate::~Delegate()
{
    delete mWidget;
}

void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!index.isValid())
        return;

    QVariant var = index.model()->data(index);

    if(var.isValid())
    {
        Item* item = var.value<Item*>();

        if(item)
        {
            const auto column = index.column();
            const auto itemSettings = item->getSettings();
            const auto setting = column < itemSettings.size() ? itemSettings.at(column) : nullptr;

            mWidget->bindToSetting(setting);
            mWidget->setFixedSize(option.rect.width(),option.rect.height());
            QPixmap pixmap(mWidget->size());
            mWidget->render(&pixmap);
            painter->drawPixmap(option.rect,pixmap);
        }
        else
            QStyledItemDelegate::paint(painter,option,index);
    }
}
/*
QSize Delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if ((mItemSize.isEmpty()) && index.isValid())
	{
        Item *item;
		auto proxyModel = dynamic_cast<const QSortFilterProxyModel*>(index.model());
		if (proxyModel)
            item = static_cast<Item*>(proxyModel->mapToSource(index).internalPointer());
		else
            item = static_cast<Item*>(index.internalPointer());

		if (item)
		{
			auto size = item->getSize(index.column());
			if (!size.isEmpty())
				return size;
		}
    }
	else
        return mItemSize;
    return QStyledItemDelegate::sizeHint(option, index);
}
*/
QWidget *Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    CustomSettingWidget* pEditor = new CustomSettingWidget(parent);
    connect(pEditor, SIGNAL(signalCommit()), this, SLOT(slotCommit()));
    return pEditor;
}

void Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    //CustomSettingWidget* pEditor=qobject_cast<CustomSettingWidget*>(editor);
    //CustomItemData data = index.model()->data(index).value<CustomItemData>();
    //pEditor->setCurrentPropertyIndex( index.column() );
    //pEditor->setData(data);
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
