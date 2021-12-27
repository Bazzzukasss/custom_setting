#include "custom_setting_item.h"
#include "custom_setting_item_tree_model.h"

using namespace custom_setting;

Item::Item(const QString& key,
           const QString& caption,
           const QString& description,
           QObject* parent) :
    Setting(key, caption, description, parent)
{
}

Item::Item(const QString& caption, QObject* parent)
    : Item("", caption, "", parent)
{
}

void Item::addItems(const Item::List& items)
{
    if (m_model)
    {
        m_model->addItems(this, items);
    }
    else
    {
        addItemsPrivate(items);
    }
}

void Item::setItems(const List& items)
{
    if (m_model)
    {
        m_model->setItems(this, items);
    }
    else
    {
        setItemsPrivate(items);
    }
}

void Item::addSettingItems(const Map& items)
{
    List itemList;

    for (auto [item, settingList] : items)
    {
        item->addSettings(settingList);
        itemList.append(item);
    }

    addItems(itemList);
}

void Item::removeItem(Item* item)
{
    if (m_model)
    {
        m_model->removeItem(this, item);
    }
    else
    {
        removeItemPrivate(item);
    }
}

const Item::List& Item::getItems() const
{
    return mItems;
}

Item* Item::getItem(int index)
{
    return 0 <= index && index < mItems.size()
               ? mItems.at(index)
               : nullptr;
}

Setting* Item::getSetting(int index)
{
    return 1 <= index && index <= mSettings.size()
               ? mSettings.at(index - 1)
               : nullptr;
}

int Item::getNumber() const
{
    auto parentItem = dynamic_cast<Item*>(parent());

    if (parentItem)
    {
        return parentItem->mItems.indexOf(const_cast<Item*>(this));
    }

    return 0;
}

ItemTreeModel* Item::getModel() const
{
    return m_model;
}

void Item::setModel(ItemTreeModel* model)
{
    m_model = model;
    for (auto& item : mItems)
    {
        item->setModel(m_model);
    }
}

void Item::clearItems()
{
    if (m_model)
    {
        m_model->clearItems(this);
    }
    else
    {
        clearPrivat();
    }
}

void Item::addItemsPrivate(const List& items)
{
    mItems.append(items);

    for (auto& item : items)
    {
        item->setParent(this);
        item->setModel(m_model);

        connect(item, &Item::signalDataChanged, this, &Item::signalDataChanged);
    }
}

void Item::setItemsPrivate(const List& items)
{
    mItems.clear();
    addItemsPrivate(items);
}

void Item::removeItemPrivate(Item* item)
{
    mItems.removeOne(item);
}

void Item::clearPrivat()
{
    mItems.clear();
}
