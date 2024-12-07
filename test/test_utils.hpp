#pragma once

#include <iostream>
#include <string>

inline void PRINT_TEST_FAILED(const std::string& test_name, const std::string&& message)
{
    std::cout << "TEST FAILED : " << test_name << " : " << message << '\n';
}