#include <QDebug>
#include <QMimeData>
#include "custom_setting_item_tree_model.h"

using namespace custom_setting;

ItemTreeModel::ItemTreeModel(QObject* parent)
    : ItemTreeModel(nullptr, parent)
{
}

ItemTreeModel::ItemTreeModel(Item* rootItem, QObject* parent)
    : QAbstractItemModel(parent)
{
    mRootItem = rootItem;

    if (mRootItem)
    {
        mRootItem->setParent(this);
    }
}

QVariant ItemTreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
    {
        return QVariant();
    }

    auto item = getItem(index);

    if (!item)
    {
        return QVariant();
    }

    auto column = index.column();

    return column == 0 ? QVariant().fromValue(item->getCaption())
                       : QVariant().fromValue(item->getSetting(column));
}

bool ItemTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
    {
        return false;
    }

    setSettingValue(index, value);

    return true;
}

QVariant ItemTreeModel::headerData(int section,
                                   Qt::Orientation orientation,
                                   int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
        if (section >= mHeaders.size())
        {
            section = mHeaders.size() - 1;
        }

		return mHeaders[section];
	}

    return QVariant();
}

bool ItemTreeModel::setHeaderData(int section,
                                  Qt::Orientation orientation,
                                  const QVariant &value, int role)
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(value)
    Q_UNUSED(role)

    return false;
}

int ItemTreeModel::columnCount(const QModelIndex& ) const
{
    return mHeaders.size();
}

Qt::ItemFlags ItemTreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return {};
    }

    Qt::ItemFlags flags = QAbstractItemModel::flags(index) |
                          Qt::ItemIsDragEnabled |
                          Qt::ItemIsDropEnabled |
                          mFlags;

    auto setting = getSetting(index);
    if (setting && !setting->isReadOnly())
    {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}

Item* ItemTreeModel::getItem(const QModelIndex& index) const
{
    if (!mRootItem || !index.isValid())
    {
        return mRootItem;
    }

    return static_cast<Item*>(index.internalPointer());
}

Setting *ItemTreeModel::getSetting(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return nullptr;
    }

    auto item = getItem(index);
    return item ? item->getSetting(index.column())
                : nullptr;
}

Item* ItemTreeModel::getRootItem() const
{
	return mRootItem;
}

QModelIndex ItemTreeModel::index(int row, int column, const QModelIndex& parent) const
{

    if (parent.isValid() && parent.column() != 0)
    {
        return QModelIndex();
    }

    auto mParent = getItem(parent);

    if (!mParent)
    {
        return QModelIndex();
    }

    auto childItem = mParent->getItem(row);
    return childItem ? createIndex(row, column, childItem)
                     : QModelIndex();
}

QModelIndex ItemTreeModel::parent(const QModelIndex& index) const
{

    if (!index.isValid())
    {
        return QModelIndex();
    }

    auto childItem = getItem(index);

    if(childItem == 0)
    {
        return QModelIndex();
    }

    auto mParent = dynamic_cast<Item*>(childItem->parent());

    if (mParent == mRootItem)
    {
        return QModelIndex();
    }

    return createIndex(mParent->getNumber(), 0, mParent);
}

int ItemTreeModel::rowCount(const QModelIndex& parent) const
{
    auto mParent = getItem(parent);
    return mParent ? mParent->getItems().count() : 0;
}

void ItemTreeModel::setRootItem(Item* item)
{
    beginResetModel();

    if (mRootItem != nullptr )
    {
        delete mRootItem;
    }

    mRootItem = item;
    mRootItem->setModel(this);
    mRootItem->setParent(this);

    connect(mRootItem, &Item::signalDataChanged, this, &ItemTreeModel::refresh);

    endResetModel();
}

void ItemTreeModel::addItems(Item* parentItem, const Item::List& items)
{
    beginResetModel();

    parentItem->addItemsPrivate(items);

    endResetModel();
}

void ItemTreeModel::setItems(Item* parentItem, const Item::List& items)
{
    beginResetModel();

    parentItem->setItemsPrivate(items);

    endResetModel();
}

void ItemTreeModel::removeItem(Item* parentItem, Item* item)
{
    beginResetModel();

    parentItem->removeItemPrivate(item);

    endResetModel();
}

void ItemTreeModel::clearItems(Item* parentItem)
{
    beginResetModel();

    parentItem->clearItems();

    endResetModel();
}

void ItemTreeModel::setSettingValue(const QModelIndex& index, const QVariant& value)
{
    if (index.isValid() && value.isValid())
    {
        auto item = getItem(index);

        if (item)
        {
            auto itemSetting = item->getSetting(index.column());
            if (itemSetting)
            {
                if (mDataChangeModel == DataChangeMode::eExternal)
                {
                    itemSetting->notifyValue(value);
                }
                else
                {
                    itemSetting->setValue(value);
                }
            }
        }
    }
}

void ItemTreeModel::setHeaders(const QStringList& headers)
{
	
	if (mHeaders.size() != headers.size())
	{
		beginResetModel();
		mHeaders = headers;
		endResetModel();
	}
    else
    {
        mHeaders = headers;
    }

    refresh();
}

void ItemTreeModel::itemEditionStarted()
{
    mIsItemEditing = true;
}

void ItemTreeModel::itemEditionFinished()
{
    mIsItemEditing = false;
}

const QStringList& ItemTreeModel::getHeaders()
{
    return mHeaders;
}

void ItemTreeModel::refresh()
{
    if (!mIsItemEditing)
    {
        emit dataChanged(QModelIndex(), QModelIndex());
    }
}

void ItemTreeModel::setDataChangeModel(DataChangeMode dataChangeModel)
{
    mDataChangeModel = dataChangeModel;
}

void ItemTreeModel::setFlags(Qt::ItemFlags aFlags)
{
    mFlags = aFlags;
}
