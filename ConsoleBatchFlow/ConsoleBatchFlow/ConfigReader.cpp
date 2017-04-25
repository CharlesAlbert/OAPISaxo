
#include "stdafx.h"
#include <windows.h>
#include "ConfigReader.h"

using namespace std;

ConfigReader::ConfigReader()
{

}

ConfigReader::ConfigReader(std::string strFileName)
{
	m_strFileName = strFileName;
}

std::string ConfigReader::AccessToken()
{
	char str[1024];
	GetPrivateProfileString("OAPIConfiguration", "AccessToken", "", str, 1024, m_strFileName.c_str());
	if (strcmp(str, "") == 0)
		cout << "AccessToken can't be blank. Check Configuration.ini file.";

	return std::string(str);
}

std::string ConfigReader::OpenApiBaseUrl()
{
	char str[MAX_PATH];
	GetPrivateProfileString("OAPIConfiguration", "OpenApiBaseUrl", "", str, MAX_PATH, m_strFileName.c_str());
	if (strcmp(str, "") == 0)
		cout << "OpenApiBaseUrl can't be blank. Check Configuration.ini file.";

	return std::string(str);
}