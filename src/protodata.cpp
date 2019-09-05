#include "protodata.h"

ProtoData::ProtoData(const google::protobuf::Descriptor *protoclass)
{
    name = protoclass->name();
    label = proto::FieldDescriptor::LABEL_REQUIRED;
    type = proto::FieldDescriptor::TYPE_MESSAGE;
}

ProtoData::ProtoData(const google::protobuf::FieldDescriptor * field)
{
    name = field->name();
    label = field->label();
    type = field->type();
}

std::string ProtoData::getName() const
{
    return name;
}

std::string ProtoData::getLabel() const
{
    switch (label)
    {
        case proto::FieldDescriptor::LABEL_OPTIONAL:
            return "optional";
        case proto::FieldDescriptor::LABEL_REPEATED:
            return "repeated";
        case proto::FieldDescriptor::LABEL_REQUIRED:
            return "required";
    }
    return "";
}

std::string ProtoData::getType() const
{
    switch(type)
    {
        case proto::FieldDescriptor::TYPE_DOUBLE:
            return "Double";
        case proto::FieldDescriptor::TYPE_FLOAT:
            return "Float";
        case proto::FieldDescriptor::TYPE_INT64:
            return "Int64";
        case proto::FieldDescriptor::TYPE_UINT64:
            return "Uint64";
        case proto::FieldDescriptor::TYPE_INT32:
            return "Int32";
        case proto::FieldDescriptor::TYPE_FIXED64:
            return "Fixed64";
        case proto::FieldDescriptor::TYPE_FIXED32:
            return "Fixed32";
        case proto::FieldDescriptor::TYPE_BOOL:
            return "Boolean";
        case proto::FieldDescriptor::TYPE_STRING:
            return "String";
        case proto::FieldDescriptor::TYPE_GROUP:
            return "Group";
        case proto::FieldDescriptor::TYPE_MESSAGE:
            return "Message";
        case proto::FieldDescriptor::TYPE_BYTES:
            return "Bytes";
        case proto::FieldDescriptor::TYPE_UINT32:
            return "Uint32";
        case proto::FieldDescriptor::TYPE_ENUM:
            return "Enum";
        case proto::FieldDescriptor::TYPE_SFIXED32:
            return "SFixed32";
        case proto::FieldDescriptor::TYPE_SFIXED64:
            return "SFixed64";
        case proto::FieldDescriptor::TYPE_SINT32:
            return "SInt32";
        case proto::FieldDescriptor::TYPE_SINT64:
            return "SInt64";
    }
    return "";
}

std::string ProtoData::getValue() const
{
    return value;
}

Qt::GlobalColor ProtoData::getRowColor() const
{
    switch(type)
    {
        case proto::FieldDescriptor::TYPE_DOUBLE:
        case proto::FieldDescriptor::TYPE_FLOAT:
            return Qt::cyan;
        case proto::FieldDescriptor::TYPE_INT64:
        case proto::FieldDescriptor::TYPE_UINT64:
        case proto::FieldDescriptor::TYPE_INT32:
        case proto::FieldDescriptor::TYPE_FIXED64:
        case proto::FieldDescriptor::TYPE_FIXED32:
        case proto::FieldDescriptor::TYPE_SFIXED32:
        case proto::FieldDescriptor::TYPE_SFIXED64:
        case proto::FieldDescriptor::TYPE_SINT32:
        case proto::FieldDescriptor::TYPE_SINT64:
        case proto::FieldDescriptor::TYPE_UINT32:
            return Qt::green;
        case proto::FieldDescriptor::TYPE_BOOL:
            return Qt::blue;
        case proto::FieldDescriptor::TYPE_STRING:
            return Qt::yellow;
        case proto::FieldDescriptor::TYPE_GROUP:
        case proto::FieldDescriptor::TYPE_MESSAGE:
        case proto::FieldDescriptor::TYPE_BYTES:
            return Qt::red;
        case proto::FieldDescriptor::TYPE_ENUM:
            return Qt::magenta;
    }
    return Qt::white;
}
