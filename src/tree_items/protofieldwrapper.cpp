#include "protofieldwrapper.h"
namespace
{
template <typename T>
struct FieldWrapperAccess
{
    T(proto::Reflection::*get)(const proto::Message&, const proto::FieldDescriptor*) const;
    T(proto::Reflection::*get_repeated)(const proto::Message&, const proto::FieldDescriptor*, int) const;
    void(proto::Reflection::*set)(proto::Message*, const proto::FieldDescriptor*, T) const;
    void(proto::Reflection::*set_repeated)(proto::Message*, const proto::FieldDescriptor*, T) const;
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
                                          &proto::Reflection::Add##N2 }; \
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
    };

createWrapper(proto::int32, Int32, INT32)
createWrapper(proto::int64, Int64, INT64)
createWrapper(proto::uint32, UInt32, UINT32)
createWrapper(proto::uint64, UInt64, UINT64)

#define REG_WRAPPER(N) regWrapper(protofd::CPPTYPE_##N, new FieldWrapper##N)
}

FieldWrapperRegister::FieldWrapperRegister()
{
    REG_WRAPPER(INT32);
    REG_WRAPPER(INT64);
    REG_WRAPPER(UINT32);
    REG_WRAPPER(UINT64);
}

FieldWrapperRegister& FieldWrapperRegister::get() {
    static FieldWrapperRegister rg;
    return rg;
}

bool FieldWrapperRegister::regWrapper(protofd::CppType type, FieldWrapper* wrapper) {
    bool isReg = false;
    auto it = mWrappers.find(type);
    if(it == mWrappers.end()) {
        mWrappers[type] = wrapper;
        isReg = true;
    }
    return isReg;
}
FieldWrapper* FieldWrapperRegister::wrapper(protofd::CppType type) {
    FieldWrapper * wrapper = nullptr;
    auto it = mWrappers.find(type);
    if(it != mWrappers.end()) {
        wrapper = it->second;
    }
    return wrapper;
}
