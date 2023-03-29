#include <struct_exception.h>

struct_exception::struct_exception() {}

struct_exception::struct_exception(std::string message) : message(message) {}

const char* struct_exception::what() const noexcept
{
    return message.c_str();
}