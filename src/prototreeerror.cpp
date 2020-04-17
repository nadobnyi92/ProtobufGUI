#include "prototreeerror.h"

ProtoTreeError::ProtoTreeError(const std::string& message, const std::string& detail)
    : mMessage(message) , mDetails(detail) {}

ProtoTreeError::ProtoTreeError(google::protobuf::Message * message)
{
    if(!message->IsInitialized()) {
        mDetails = "skiped required fuelds: " + message->InitializationErrorString();
        mMessage = "Failed parsing message";
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
