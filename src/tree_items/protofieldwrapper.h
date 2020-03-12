#ifndef PROTOFIELDWRAPPER_H
#define PROTOFIELDWRAPPER_H

#include <google/protobuf/dynamic_message.h>

#include <QVariant>

namespace proto = google::protobuf;
using protofd = google::protobuf::FieldDescriptor;

template <typename T>
struct FieldWrapperAccess
{
    T(proto::Reflection::*get)(const proto::Message&, const proto::FieldDescriptor*) const;
    T(proto::Reflection::*get_repeated)(const proto::Message&, const proto::FieldDescriptor*, int) const;
    void(proto::Reflection::*set)(proto::Message*, const proto::FieldDescriptor*, T) const;
    void(proto::Reflection::*set_repeated)(proto::Message*, const proto::FieldDescriptor*, T) const;
    T minValue;
    T maxValue;
    QVariant getFieldData(const proto::Message& message, const proto::FieldDescriptor* field){
        return QVariant::fromValue((message.GetReflection()->*get)(message, field));
    }
    QVariant getRepeatedFieldData(const proto::Message& message, const proto::FieldDescriptor* field, int idx) {
        return QVariant::fromValue((message.GetReflection()->*get_repeated)(message, field, idx));
    }
    void setFieldData(proto::Message* message, const proto::FieldDescriptor* field, const QVariant& data) {
        (message->GetReflection()->*set)(message, field, qvariant_cast<T>(data));
    }
    void setRepeatedFieldData(proto::Message* message, const proto::FieldDescriptor* field, const QVariant& data) {
        (message->GetReflection()->*set_repeated)(message, field, qvariant_cast<T>(data));
    }
};

class FieldWrapper
{
public:
    virtual ~FieldWrapper() {}
    virtual QVariant getFieldData(const proto::Message& message, const proto::FieldDescriptor* field) = 0;
    virtual QVariant getRepeatedFieldData(const proto::Message& message, const proto::FieldDescriptor* field, int idx) = 0;
    virtual void setFieldData(proto::Message* message, const proto::FieldDescriptor* field, const QVariant& data) = 0;
    virtual void setRepeatedFieldData(proto::Message* message, const proto::FieldDescriptor* field, const QVariant& data) = 0;
};

class FieldWrapperRegister
{
    FieldWrapperRegister() {}
public:
    FieldWrapperRegister(const FieldWrapperRegister&) = delete;
    FieldWrapperRegister(FieldWrapperRegister&&) = delete;
    FieldWrapperRegister& operator=(const FieldWrapperRegister&) = delete;
    FieldWrapperRegister& operator=(FieldWrapperRegister&&) = delete;

    static FieldWrapperRegister& get() {
        static FieldWrapperRegister rg;
        return rg;
    }

    bool regWrapper(protofd::CppType type, FieldWrapper* wrapper) {
        bool isReg = false;
        auto it = mWrappers.find(type);
        if(it != mWrappers.end()) {
            mWrappers[type] = wrapper;
            isReg = true;
        }
        return isReg;
    }
    FieldWrapper* wrapper(protofd::CppType type) {
        FieldWrapper * wrapper = nullptr;
        auto it = mWrappers.find(type);
        if(it != mWrappers.end()) {
            wrapper = it->second;
        }
        return wrapper;
    }

private:
    std::map<protofd::CppType, FieldWrapper*> mWrappers;
};

#define createWrapper(N1, N2, N3) \
    class FieldWrapper##N3 : public FieldWrapper \
    { \
    private: \
        FieldWrapperAccess<N1> fw; \
    public: \
        FieldWrapper##N3 () { \
            fw = FieldWrapperAccess<N1> { &proto::Reflection::Get##N2, \
                                          &proto::Reflection::GetRepeated##N2, \
                                          &proto::Reflection::Set##N2, \
                                          &proto::Reflection::Add##N2, \
                                          std::numeric_limits<N1>::min(), \
                                          std::numeric_limits<N1>::max() }; \
        } \
        QVariant getFieldData(const proto::Message& message, const proto::FieldDescriptor* field) { \
            return fw.getFieldData(message, field); \
        } \
        QVariant getRepeatedFieldData(const proto::Message& message, const proto::FieldDescriptor* field, int idx) {\
            return fw.getRepeatedFieldData(message, field, idx); \
        }\
        void setFieldData(proto::Message* message, const proto::FieldDescriptor* field, const QVariant& data) {\
            fw.setFieldData(message, field, data); \
        } \
        void setRepeatedFieldData(proto::Message* message, const proto::FieldDescriptor* field, const QVariant& data) {\
            fw.setRepeatedFieldData(message, field, data); \
        } \
    }; \
    static bool IS_REG_CPP_TYPE_##N3 = FieldWrapperRegister::get().regWrapper(protofd::CPPTYPE_##N3, new FieldWrapper##N3);


#endif // PROTOFIELDWRAPPER_H
