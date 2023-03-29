#pragma once
#include <exception>
#include <string>

class struct_exception : std::exception
{
public:
    const std::string message;
    struct_exception();
    struct_exception(std::string message);

    const char* what() const noexcept override;
};