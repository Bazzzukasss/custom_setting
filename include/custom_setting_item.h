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

    Item* getItem(int index) { return 0 <= index && index < mItems.size() ? mItems.at(index) : nullptr; }
    Setting* getSetting(int index) { return 1 <= index && index <= mSettings.size() ? mSettings.at(index - 1) : nullptr; }
    int getNumber() const;


protected:
    List mItems;
};

} // namespace custom_setting
