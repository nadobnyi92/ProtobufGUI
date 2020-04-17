#ifndef PROTOTREEERROR_H
#define PROTOTREEERROR_H

#include <exception>
#include <string>

#include <google/protobuf/dynamic_message.h>

namespace proto = google::protobuf;

class ProtoTreeError : public std::exception
{
public:
    ProtoTreeError(const std::string& message, const std::string& detail = nullptr);
    ProtoTreeError(proto::Message *);

    const char *what() const noexcept override;
    const char *details() const noexcept;

private:
    std::string mMessage;
    std::string mDetails;
};

#endif // PROTOTREEERROR_H
