#pragma once

#include <QAbstractItemModel>
#include "custom_setting_item.h"

namespace custom_setting {

class ItemTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    ItemTreeModel(QObject *parent = 0);
    ItemTreeModel(Item *rootItem,QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

    void setItems(Item* items);
    void setSettingValue(const QModelIndex &index, const QVariant& setting);
    void setFlags(Qt::ItemFlags aFlags);
    void setHeaders(const QStringList& headers);

    void refresh();
    //Qt::DropActions supportedDropActions() const;
    //QStringList mimeTypes() const;
    //QMimeData *mimeData(const QModelIndexList &indexes) const;
    //bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

private:
    Item* mRootItem{nullptr};
    QStringList mHeaders;
    Qt::ItemFlags mFlags;

    Item* getItem(const QModelIndex &index) const;
    Setting* getSetting(const QModelIndex &index) const;
    Item* getRootItem()const;
    const QStringList& getHeaders();
};
}
