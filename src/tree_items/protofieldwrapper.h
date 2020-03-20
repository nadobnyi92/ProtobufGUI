#ifndef PROTOFIELDWRAPPER_H
#define PROTOFIELDWRAPPER_H

#include <google/protobuf/dynamic_message.h>

#include <QVariant>

namespace proto = google::protobuf;
using protofd = google::protobuf::FieldDescriptor;

class FieldWrapper
{
public:
    virtual ~FieldWrapper() {}
    virtual QVariant getFieldData(const proto::Message& message, const proto::FieldDescriptor* field) = 0;
    virtual QVariant getRepeatedFieldData(const proto::Message& message, const proto::FieldDescriptor* field, int idx) = 0;
    virtual void setFieldData(proto::Message* message, const proto::FieldDescriptor* field, const QVariant& data) = 0;
    virtual void setRepeatedFieldData(proto::Message* message, const proto::FieldDescriptor* field, const QVariant& data) = 0;
};

//exists default wrappers for integer and float(?) types
class FieldWrapperRegister
{
    FieldWrapperRegister();
public:
    FieldWrapperRegister(const FieldWrapperRegister&) = delete;
    FieldWrapperRegister(FieldWrapperRegister&&) = delete;
    FieldWrapperRegister& operator=(const FieldWrapperRegister&) = delete;
    FieldWrapperRegister& operator=(FieldWrapperRegister&&) = delete;

    static FieldWrapperRegister& get();
    bool regWrapper(protofd::CppType type, FieldWrapper* wrapper);
    FieldWrapper* wrapper(protofd::CppType type);

private:
    std::map<protofd::CppType, FieldWrapper*> mWrappers;
};

#endif // PROTOFIELDWRAPPER_H
