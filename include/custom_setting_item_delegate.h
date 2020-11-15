#pragma once

#include <QStyledItemDelegate>

class CustomSettingWidget;

namespace custom_setting {

class Delegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    Delegate(QObject* parent =0);
    ~Delegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    //QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

public slots:
    void slotCommit();

private:
    CustomSettingWidget* mWidget;

};
}
