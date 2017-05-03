
#include "stdafx.h"
#include "Utility.h"
#include "msclr\marshal_cppstd.h"
#include <msclr\marshal.h>

using namespace System;
using namespace std;
using namespace System::Runtime::InteropServices;


//Convert System::String^ to std::string
std::string SysStringToCppString(String^ s)
{
	auto intPtrString = Marshal::StringToHGlobalAnsi(s);
	auto rawStringMem = (char*)intPtrString.ToPointer();
	std::string r(rawStringMem);
	Marshal::FreeHGlobal(intPtrString);
	return r;
}

//convert std::string to System::String^
String^ CppStringToSysString(const std::string& s)
{
	return Marshal::PtrToStringAnsi((IntPtr)(char*)s.c_str());
}