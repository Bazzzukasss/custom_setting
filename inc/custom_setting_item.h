#pragma once

#include <QObject>
#include "custom_setting.h"

namespace custom_setting
{

class ItemTreeModel;

class Item : public Setting
{
    Q_OBJECT

public:
    using List = QList<Item*>;
    using Map = std::map<Item*, QVector<Setting*>>;

    explicit Item(const QString& key,
                  const QString& caption,
                  const QString& description,
                  QObject* parent = nullptr);

    explicit Item(const QString& caption, QObject* parent = nullptr);

    void setModel(ItemTreeModel* model);
    ItemTreeModel* getModel() const;

    void setItems(const List& items);
    const List& getItems() const;

    void addItems(const List& items);
    void addSettingItems(const Map& items);
    void removeItem(Item* item);

    Item* getItem(int index);
    Setting* getSetting(int index);
    int getNumber() const;

    void clearItems();

protected:
    List mItems;

private:
    ItemTreeModel* m_model{nullptr};

private:
    void addItemsPrivate(const List& items);
    void setItemsPrivate(const List& items);
    void removeItemPrivate(Item* item);
    void clearPrivat();

    friend ItemTreeModel;
};

} // namespace custom_setting
