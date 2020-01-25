#include "protobufmodel.h"
#include "tree_items/prototreeitem.h"
#include "tree_items/repeatedprotoitem.h"
#include "tree_items/bytestprotoitem.h"

#include <QFileDialog>

QString ProtobufModel::getMessage() const
{
    return mRootItem != nullptr ?
                QString::fromStdString(mRootItem.get()->getStringMessage()) : QString();
}

void ProtobufModel::loadProtoData()
{
    if(!mRootItem)
        throw ProtoInitException("Не определен proto-класс");
    mDataFilePath = QFileDialog::getOpenFileName().toStdString();
    if(!mRootItem->initMessage(mDataFilePath))
        throw ProtoInitException("Ошибка разбора proto-класса");
}

void ProtobufModel::setProtoClass(const google::protobuf::Descriptor *protoclass)
{
    if(protoclass != nullptr)
    {
        emit beginResetModel();
        mRootItem = std::make_unique<RootProtoItem>(protoclass);
        mRootItem->expand();
        emit endResetModel();
    }
}

void ProtobufModel::onExpand(const QModelIndex &index)
{
    if (!mRootItem || !index.isValid())
        return;

    ProtoTreeItem *item = static_cast<ProtoTreeItem*>(index.internalPointer());

    item->expand();
    beginInsertRows(index.siblingAtColumn(0), 0, item->rowCount());
    endInsertRows();
}

void ProtobufModel::onAddItem(const QModelIndex &index)
{
    ProtoTreeItem *item = static_cast<ProtoTreeItem*>(index.internalPointer());
    RepeatedProtoItem *rItem = dynamic_cast<RepeatedProtoItem*>(item);
    if(rItem != nullptr)
    {
        beginInsertRows(index.siblingAtColumn(0), item->rowCount(), item->rowCount());
        rItem->addItem();
        endInsertRows();
    }
}

void ProtobufModel::onRemoveItem(const QModelIndex &index)
{
    if(index.parent().isValid())
    {
        int row = static_cast<ProtoTreeItem*>(index.internalPointer())->row();
        ProtoTreeItem *pItem = static_cast<ProtoTreeItem*>(index.parent().internalPointer());
        beginRemoveRows(index.parent(), row, row);
        pItem->removeRow(row);
        endRemoveRows();
    }
}

void ProtobufModel::onReplaceType(const QModelIndex &index, const proto::Descriptor* desc)
{
    BytesProtoItem *pItem = static_cast<BytesProtoItem*>(index.internalPointer());
    beginRemoveRows(index, 0, pItem->children().size());
    pItem->setDesc(desc);
    endRemoveRows();
    beginInsertRows(index, 0, pItem->childItems().size());
    endInsertRows();
}

void ProtobufModel::onClearData()
{
    beginResetModel();
    if(mDataFilePath.empty() || !mRootItem->initMessage(mDataFilePath))
        mRootItem->clearValue();
    endResetModel();
}

QModelIndex ProtobufModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!mRootItem || !hasIndex(row, column, parent))
        return QModelIndex();

    ProtoTreeItem *parentItem = parent.isValid() ?
        static_cast<ProtoTreeItem*>(parent.internalPointer()) : mRootItem.get();
    return createIndex(row, column, parentItem->child(row));
}

QModelIndex ProtobufModel::parent(const QModelIndex &index) const
{       
    if (!mRootItem || !index.isValid())
        return QModelIndex();

    ProtoTreeItem *childItem = static_cast<ProtoTreeItem*>(index.internalPointer());
    ProtoTreeItem *parentItem = childItem->parentItem();

    return parentItem == mRootItem.get() ?
        QModelIndex() : createIndex(parentItem->row(), index.column(), parentItem);
}

int ProtobufModel::rowCount(const QModelIndex &parent) const
{
    if (!mRootItem)
        return 0;

    ProtoTreeItem *parentItem = parent.isValid() ?
        static_cast<ProtoTreeItem*>(parent.internalPointer()) : mRootItem.get();

    return parentItem->rowCount();
}

int ProtobufModel::columnCount(const QModelIndex &parent) const
{
    return COL_COUNT;
}

QVariant ProtobufModel::data(const QModelIndex &index, int role) const
{
    if (!mRootItem || !index.isValid())
        return QVariant();

    ProtoTreeItem *item = static_cast<ProtoTreeItem*>(index.internalPointer());

    switch (role)
    {
        case Qt::DisplayRole:
        {
            switch(index.column())
            {
                case COL_NAME:
                    return item->name();
                case COL_TYPE:
                    return item->typeName();
                case COL_VALUE:
                    return item->value();
                default:
                    return QVariant();
            }
        }
        case Qt::BackgroundRole:
            return item->color();
        case Qt::DecorationRole:
            return index.column() == COL_VALUE ? icon(item) : QVariant();
    }
    return QVariant();
}

QIcon ProtobufModel::icon(ProtoTreeItem* item) const
{
    switch (item->label())
    {
        case proto::FieldDescriptor::LABEL_OPTIONAL:
            return QIcon(":/icons/optional.png");
        case proto::FieldDescriptor::LABEL_REPEATED:
            return QIcon(":/icons/repeated.png");
        case proto::FieldDescriptor::LABEL_REQUIRED:
            return QIcon(":/icons/required.png");
    }
    return QIcon();
}

QVariant ProtobufModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case COL_NAME:
                return "Поле";
            case COL_TYPE:
                return "Тип данных";
            case COL_VALUE:
                return "Значение";
        }
    }
    return QVariant();
}


bool ProtobufModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole && index.column() == COL_VALUE)
    {
        ProtoTreeItem *item = static_cast<ProtoTreeItem*>(index.internalPointer());
        item->setValue(value);
        return true;
    }
    return false;
}


Qt::ItemFlags ProtobufModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    if(index.column() == COL_VALUE &&
        static_cast<ProtoTreeItem*>(index.internalPointer())->getDelegate() != nullptr)
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEnabled;
}


ProtobufModel::ProtoInitException::ProtoInitException(const char * msg) noexcept : msg(msg) {}
const char *ProtobufModel::ProtoInitException::what() const noexcept { return msg.c_str(); }
