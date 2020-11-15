#pragma once

#include <QObject>
#include "custom_setting.h"

namespace custom_setting
{

class Item : public Setting
{
    Q_OBJECT
public:
    using List = QList<Item*>;

    explicit Item(const QString& key, const QString& caption, const QString& description, QObject* parent = nullptr);

    void addItems(const List &items);
    inline const List& getItems() const { return mItems; }

    Item* getItem(int index){ return mItems.at(index); }
    int getNumber() const;


protected:
    List mItems;
};

} // namespace custom_setting
