#include "protobufmodel.h"
#include "tree_items/prototreeitem.h"
#include "tree_items/repeatedprotoitem.h"
#include "tree_items/bytestprotoitem.h"
#include "prototreeerror.h"
#include "prototypedialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

google::protobuf::Message *ProtobufModel::getMessage()
{
    return mRootItem != nullptr ? mRootItem->getMessage() : nullptr;
}

std::string ProtobufModel::getSerializedMessage() const
{
    return mRootItem != nullptr ? mRootItem.get()->getStringMessage() : "";
}

void ProtobufModel::loadProtoData()
{
    if(!mRootItem)
        throw ProtoTreeError("Ошибка", "Не определен proto-класс");
    mDataFilePath = QFileDialog::getOpenFileName().toStdString();
    mRootItem->initMessage(mDataFilePath);
}

void ProtobufModel::setProtoClass(const google::protobuf::Descriptor *protoclass)
{
    if(protoclass != nullptr) {
        emit beginResetModel();
        mRootItem = std::unique_ptr<RootProtoItem>(new RootProtoItem(*this, protoclass));
        mRootItem->expand();
        emit endResetModel();
    }
}

void ProtobufModel::onExpand(const QModelIndex &index)
{
    if (!mRootItem || !index.isValid())
        return;

    ProtoTreeItem *item = toItem(index);

    item->expand(); //FIXME after signal
    beginInsertRows(index.sibling(index.row(),0), 0, item->children().size());
    endInsertRows();
    emit expandNode(index);
}

void ProtobufModel::onClearData()
{
    beginResetModel();
    bool needClear = true;
    if(!mDataFilePath.empty()) {
        try {
            mRootItem->initMessage(mDataFilePath);
            needClear = false;
        } catch (const ProtoTreeError& e) {
            emit processProtoError(e);
        }
    }

    if(needClear) {
        mRootItem->clearValue();
    }
    endResetModel();
}

void ProtobufModel::onClearField(const QModelIndex &index)
{
    QModelIndex chngIndex = index.sibling(index.row(), ProtobufModel::COL_VALUE);
    setData( chngIndex, QVariant(), Qt::EditRole );
    emit dataChanged(chngIndex, chngIndex);
}

QModelIndex ProtobufModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!mRootItem || !hasIndex(row, column, parent))
        return QModelIndex();

    ProtoTreeItem *parentItem = parent.isValid() ? toItem(parent) : mRootItem.get();
    return createIndex(row, column, parentItem->children().at(row));
}

QModelIndex ProtobufModel::parent(const QModelIndex &index) const
{       
    if (!mRootItem || !index.isValid())
        return QModelIndex();

    ProtoTreeItem *childItem = toItem(index);
    ProtoTreeItem *parentItem = childItem->parentItem();
    return parentItem == mRootItem.get() ?
        QModelIndex() : createIndex(itemIndex(parentItem), 0, parentItem);
}

int ProtobufModel::rowCount(const QModelIndex &parent) const
{
    if (!mRootItem)
        return 0;

    ProtoTreeItem *parentItem = parent.isValid() ? toItem(parent) : mRootItem.get();

    return parentItem->children().size();
}

int ProtobufModel::columnCount(const QModelIndex &) const
{
    return COL_COUNT;
}

QVariant ProtobufModel::data(const QModelIndex &index, int role) const
{
    if (!mRootItem || !index.isValid())
        return QVariant();

    ProtoTreeItem *item = toItem(index);

    switch (role)
    {
    case Qt::DisplayRole:
        switch(index.column())
        {
        case COL_NAME: return item->name();
        case COL_TYPE: return item->typeName();
        case COL_VALUE: return item->value();
        default: return QVariant();
        }
    case Qt::DecorationRole:
        return index.column() == COL_NAME ? icon(item) : QVariant();
    }
    return QVariant();
}

QIcon ProtobufModel::icon(ProtoTreeItem* item) const
{
    switch (item->state())
    {
        case ProtoTreeItem::STATE_FILL:     return QIcon(":/icons/optional.png");
        case ProtoTreeItem::STATE_OPTIONAL: return QIcon(":/icons/repeated.png");
        case ProtoTreeItem::STATE_EMPTY:    return QIcon(":/icons/required.png");
    }
    return QIcon();
}

QVariant ProtobufModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section)
        {
        case COL_NAME:  return "Поле";
        case COL_TYPE:  return "Тип данных";
        case COL_VALUE: return "Значение";
        }
    }
    return QVariant();
}

bool ProtobufModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole && index.column() == COL_VALUE) {
        toItem(index)->setValue(value);
        return true;
    }
    return false;
}

Qt::ItemFlags ProtobufModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    if(index.column() == COL_VALUE &&
        toItem(index)->getDelegate() != nullptr)
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEnabled;
}

QList<QAction *> ProtobufModel::actions(const QModelIndex& index)
{
    ProtoTreeItem *item = toItem(index);

    QList<QAction*> actions;
    for(ProtoTreeItem::Action act: item->itemActions())
    {
        switch (act)
        {
        case ProtoTreeItem::ACT_INSERT:
        {
            QAction * act = new QAction(this);
            act->setText("Добавить");
            connect(act, &QAction::triggered, this, [item, index, this]() {
                item->createNode(item->field());
                beginInsertRows(index, item->children().size(), item->children().size());
                endInsertRows();
                onExpand(index);
            });
            actions.append(act);
            break;
        }
        case ProtoTreeItem::ACT_REMOVE:
        {
            QAction *act = new QAction(this);
            act->setText("Удалить");
            connect(act, &QAction::triggered, [item, index, this](){
                if(!index.parent().isValid()) return;
                beginRemoveRows(index.parent(), index.row(), index.row());
                item->parentItem()->removeItem(item);
                endRemoveRows();
            });
            actions.append(act);
            break;
        }
        case ProtoTreeItem::ACT_TRANSFORM:
        {
            QAction * act = new QAction(this);
            act->setText("Преобразовать тип");
            connect(act, &QAction::triggered, [item, index, this](){
                ProtoTypeDialog dlg(mProtoManager);
                if(dlg.exec() == QDialog::Accepted) {
                    beginRemoveRows(index, 0, item->children().size());
                    try {
                        item->setDesc(mProtoManager.getClassDescriptor(dlg.pPackage(), dlg.pClass()));
                    } catch (ProtoTreeError& e) {
                        emit processProtoError(e);
                    }
                    endRemoveRows();
                    beginInsertRows(index, 0, item->children().size());
                    endInsertRows();
                    onExpand(index);
                }
            });
            actions.append(act);
            break;
        }
        default: break;
        }
    }
    return actions;
}
int ProtobufModel::itemIndex(ProtoTreeItem *item) const
{
    if(item->parentItem() == nullptr)
        return 0;
    const std::vector<ProtoTreeItem*> & children = item->parentItem()->children();
    auto it = std::find_if(children.begin(), children.end(),
        [&](const ProtoTreeItem* val) { return val == item; });
    return static_cast<int>(std::distance(children.begin(), it));
}

ProtoTreeItem *ProtobufModel::toItem(const QModelIndex &index) const
{
    return index.isValid() ? static_cast<ProtoTreeItem*>(index.internalPointer()) : nullptr; //check nullptr
}

QModelIndex ProtobufModel::fromItem(ProtoTreeItem *item, const QModelIndex &parent) const
{
    QModelIndex idx;
    for(int i = 0; i < rowCount(parent); ++i)
    {
        QModelIndex child = index(i, 0, parent);
        if(toItem(child) == item)
            idx = child;
        else
            idx = fromItem(item, child);
        if(idx.isValid()) break;
    }
    return idx;
}
