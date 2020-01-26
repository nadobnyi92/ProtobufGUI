#include "prototreeerror.h"

ProtoTreeError::ProtoTreeError(const char *message, const char *detail)
    : mMessage(message) , mDetails(detail) {}

ProtoTreeError::ProtoTreeError(google::protobuf::Message * message)
{
    if(!message->IsInitialized())
    {
        mDetails = "Пропущены обязательные поля: " + message->InitializationErrorString();
        mMessage = "Ошибка разбора сообщения";
    }
}

const char *ProtoTreeError::what() const noexcept
{
    return mMessage.data();
}

const char *ProtoTreeError::details() const noexcept
{
    return mDetails.c_str();
}
