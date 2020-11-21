#include <QDebug>
#include <QMimeData>
#include "custom_setting_item_tree_model.h"

using namespace custom_setting;
//static const char* CustomItemMimeType = "CustomItemPointer";

ItemTreeModel::ItemTreeModel(QObject *parent)
    : ItemTreeModel(nullptr, parent)
{
}

ItemTreeModel::ItemTreeModel(Item *rootItem, QObject *parent)
    : QAbstractItemModel(parent)
{
    mRootItem = rootItem;
    if(mRootItem)
        mRootItem->setParent(this);
}

QVariant ItemTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    Item *item = getItem(index);

    if(!item)
        return QVariant();

    return QVariant().fromValue(item);
}

bool ItemTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    Item *item = getItem(index);
    if(item == 0)
        return false;

    item = value.value<Item*>();
    emit dataChanged(index, index);

    return true;
}

QVariant ItemTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		if (section >= mHeaders.size())
			section = mHeaders.size() - 1;
		return mHeaders[section];
	}

    return QVariant();
}

bool ItemTreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(value)
    Q_UNUSED(role)
    return false;
}

int ItemTreeModel::columnCount(const QModelIndex& parent) const
{
    return mHeaders.size();
}

Qt::ItemFlags ItemTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return {};

    Qt::ItemFlags flags = QAbstractItemModel::flags(index) | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | mFlags;

    auto setting = getSetting(index);
    if(setting && !setting->isReadOnly())
        flags |= Qt::ItemIsEditable;

    return flags;
}

Item* ItemTreeModel::getItem(const QModelIndex &index) const
{
    if (!mRootItem || !index.isValid())
        return mRootItem;

    return static_cast<Item*>(index.internalPointer());
}

Setting *ItemTreeModel::getSetting(const QModelIndex &index) const
{
    if(!index.isValid())
        return nullptr;

    auto item = getItem(index);
    return item ? item->getSetting(index.column()) : nullptr;
}

Item* ItemTreeModel::getRootItem() const
{
	return mRootItem;
}

QModelIndex ItemTreeModel::index(int row, int column, const QModelIndex &parent) const
{

    if(parent.isValid() && parent.column() != 0)
        return QModelIndex();

    auto mParent = getItem(parent);

    if(!mParent)
        return QModelIndex();

    auto childItem = mParent->getItem(row);
    return childItem ? createIndex(row, column, childItem) : QModelIndex();
}

QModelIndex ItemTreeModel::parent(const QModelIndex &index) const
{

    if (!index.isValid())
        return QModelIndex();

    auto childItem = getItem(index);

    if(childItem == 0)
        return QModelIndex();

    auto mParent = dynamic_cast<Item*>(childItem->parent());

    if (mParent == mRootItem)
        return QModelIndex();

    return createIndex(mParent->getNumber(), 0, mParent);
}

int ItemTreeModel::rowCount(const QModelIndex &parent) const
{
    auto mParent = getItem(parent);
    return mParent ? mParent->getItems().count() : 0;
}
void ItemTreeModel::updateItems(Item* items)
{
    Q_UNUSED(items)
    refresh();
}

void ItemTreeModel::setItems(Item* items)
{
    beginResetModel();

    if(mRootItem != nullptr )
        delete mRootItem;
    mRootItem = items;

    endResetModel();
}

void ItemTreeModel::setHeaders(const QStringList &headers)
{
	
	if (mHeaders.size() != headers.size())
	{
		beginResetModel();
		mHeaders = headers;
		endResetModel();
	}
	else
		mHeaders = headers;
    
	emit dataChanged(QModelIndex(), QModelIndex());
}

const QStringList& ItemTreeModel::getHeaders()
{
    return mHeaders;
}

void ItemTreeModel::refresh()
{
    emit dataChanged(QModelIndex(),QModelIndex());
}

void ItemTreeModel::setFlags(Qt::ItemFlags aFlags)
{
    mFlags = aFlags;
}

/*
Qt::DropActions ItemTreeModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList ItemTreeModel::mimeTypes() const
{
    QStringList types;
    types << CustomItemMimeType;
    return types;
}

QMimeData *ItemTreeModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData* mimeData = new QMimeData();
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    QSet<CustomItem*> items;

    foreach(QModelIndex index, indexes)
        items.insert( getItem(index) );

    stream << items;
    qDebug() << "mimeData:"<< items;
    mimeData->setData(CustomItemMimeType, encodedData);
    return mimeData;
}

bool ItemTreeModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    QSet<CustomItem*> items;
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);
    if(action==Qt::IgnoreAction) return true;
    if(!data->hasFormat(CustomItemMimeType)) return false;
    if(action!=Qt::MoveAction) return true;
    //
    QByteArray encodedData = data->data(CustomItemMimeType);
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    stream >> items;
    qDebug() << "dropData:" << items <<" to: " << row << column << parent.row() << parent.column();
    return true;
}
*/
