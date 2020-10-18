#include "custom_setting_item.h"

using namespace custom_setting;

Item::Item(const QString &key, const QString &caption, const QString &description, QObject *parent) :
    Setting(key, caption, description, parent)
{
}

void Item::addItems(const Item::Vec &items)
{
    mItems.append(items);
    for (auto& item : items)
    {
        item->setParent(this);
        connect(item, &Item::signalDataChanged, this, &Item::signalDataChanged);
    }
}
