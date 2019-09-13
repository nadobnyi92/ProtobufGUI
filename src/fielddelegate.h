#ifndef STRINGFIELDDELEGATE_H
#define STRINGFIELDDELEGATE_H

#include <QItemDelegate>

class FieldDelegate : public QItemDelegate
{
public:
    FieldDelegate(QObject * parent = nullptr);

    // QAbstractItemDelegate interface
public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};

#endif // STRINGFIELDDELEGATE_H
