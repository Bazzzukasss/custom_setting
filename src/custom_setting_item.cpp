#include "custom_setting_item.h"

using namespace custom_setting;

Item::Item(const QString &key, const QString &caption, const QString &description, QObject *parent) :
    Setting(key, caption, description, parent)
{
}

void Item::addItems(const Item::List &items)
{
    mItems.append(items);
    for (auto& item : items)
    {
        item->setParent(this);
        connect(item, &Item::signalDataChanged, this, &Item::signalDataChanged);
    }
}

int Item::getNumber() const
{
    auto parentItem = dynamic_cast<Item*>(parent());
    if (parentItem)
        return parentItem->mItems.indexOf(const_cast<Item*>(this));

    return 0;
}
