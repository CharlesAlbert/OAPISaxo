#pragma once

#include <string>
//Convert System::String^ to std::string
std::string SysStringToCppString(System::String^ s);

//convert std::string to System::String^
System::String^ CppStringToSysString(const std::string& s);
