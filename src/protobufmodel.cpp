#include "protobufmodel.h"
#include "tree_items/prototreeitem.h"
#include "tree_items/repeatedprotoitem.h"
#include "tree_items/bytestprotoitem.h"
#include "prototreeerror.h"

#include <QFileDialog>
#include <QMessageBox>

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
        mRootItem = std::unique_ptr<RootProtoItem>(new RootProtoItem(protoclass));
        mRootItem->expand();
        emit endResetModel();
    }
}

void ProtobufModel::onExpand(const QModelIndex &index)
{
    if (!mRootItem || !index.isValid())
        return;

    ProtoTreeItem *item = toItem(index);

    item->expand();
    beginInsertRows(index.sibling(index.row(),0), 0, item->children().size());
    endInsertRows();
}

void ProtobufModel::onAddItem(const QModelIndex &index)
{
    ProtoTreeItem *item = toItem(index);
    RepeatedProtoItem *rItem = dynamic_cast<RepeatedProtoItem*>(item);
    if(rItem != nullptr) {
        beginInsertRows(index.sibling(index.row(), 0), item->children().size(), item->children().size());
        rItem->addItem();
        endInsertRows();
    }
}

void ProtobufModel::onRemoveItem(const QModelIndex &index)
{
    if(index.parent().isValid()) {
        int row = itemIndex(toItem(index));
        ProtoTreeItem *pItem = toItem(index.parent());
        beginRemoveRows(index.parent(), row, row);
        pItem->removeRow(row);
        endRemoveRows();
    }
}

void ProtobufModel::onReplaceType(const QModelIndex &index, const proto::Descriptor* desc)
{
    BytesProtoItem *pItem = static_cast<BytesProtoItem*>(index.internalPointer()); //fixit
    beginRemoveRows(index, 0, pItem->children().size());
    try {
        pItem->setDesc(desc);
    } catch (ProtoTreeError& e) {
        emit processProtoError(e);
    }
    endRemoveRows();
    beginInsertRows(index, 0, pItem->children().size());
    endInsertRows();
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

QModelIndex ProtobufModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!mRootItem || !hasIndex(row, column, parent))
        return QModelIndex();

    ProtoTreeItem *parentItem = parent.isValid() ? toItem(parent) : mRootItem.get();
    return createIndex(row, column, parentItem->children().at(row).get());
}

QModelIndex ProtobufModel::parent(const QModelIndex &index) const
{       
    if (!mRootItem || !index.isValid())
        return QModelIndex();

    ProtoTreeItem *childItem = toItem(index);
    ProtoTreeItem *parentItem = childItem->parentItem();
    return parentItem == mRootItem.get() ?
        QModelIndex() : createIndex(itemIndex(parentItem), index.column(), parentItem);
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
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal) {
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

int ProtobufModel::itemIndex(ProtoTreeItem *item) const
{
    if(item->parentItem() == nullptr)
        return 0;
    const std::vector<std::unique_ptr<ProtoTreeItem> > & children = item->parentItem()->children();
    auto it = std::find_if(children.begin(), children.end(),
        [&](const std::unique_ptr<ProtoTreeItem>& val) { return val.get() == item; });
    return static_cast<int>(std::distance(children.begin(), it));
}

ProtoTreeItem *ProtobufModel::toItem(const QModelIndex &index) const
{
    return index.isValid() ? static_cast<ProtoTreeItem*>(index.internalPointer()) : nullptr;
}
