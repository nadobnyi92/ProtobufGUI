#ifndef PROTODATA_H
#define PROTODATA_H

#include <string>

#include <QtCore>

#include <google/protobuf/dynamic_message.h>

namespace proto = google::protobuf;

class ProtoData
{
public:
    ProtoData(const google::protobuf::Descriptor *protoclass);
    ProtoData(const google::protobuf::FieldDescriptor * field);
    ProtoData(const std::string& n) : name(n) {}

    std::string getName() const;
    std::string getLabel() const;
    std::string getType() const;
    std::string getValue() const;
    Qt::GlobalColor getRowColor() const;

    void setValue(const std::string& s);

private:
    std::string name;
    proto::FieldDescriptor::Type type;
    proto::FieldDescriptor::Label label;
    std::string value;
};

#endif // PROTODATA_H
