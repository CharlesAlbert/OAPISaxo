
#include "stdafx.h"
#include "ConfigReader.h"

using namespace std;

ConfigReader::ConfigReader()
{
}

ConfigReader::ConfigReader(std::string strFileName)
{
	m_strFileName = strFileName;
}

//Read AppKey
std::string ConfigReader::AppKey()
{
	char str[MAX_PATH];
	GetPrivateProfileString("OAPIConfiguration", "AppKey", "", str, MAX_PATH, m_strFileName.c_str());
	if (strcmp(str, "") == 0)
		MessageBox(NULL, "AppUrl can't be blank. Check Configuration.ini file.", "Invalid AppURL", MB_ICONERROR);

	return std::string(str);
}

//Read AppSecret
std::string ConfigReader::AppSecret()
{
	char str[MAX_PATH];
	GetPrivateProfileString("OAPIConfiguration", "AppSecret", "", str, MAX_PATH, m_strFileName.c_str());
	if (strcmp(str, "") == 0)
		MessageBox(NULL, "AppUrl can't be blank. Check Configuration.ini file.", "Invalid AppURL", MB_ICONERROR);

	return std::string(str);
}

//Read AppURL
std::string ConfigReader::AppUrl()
{
	char str[MAX_PATH];
	GetPrivateProfileString("OAPIConfiguration", "AppUrl", "", str, MAX_PATH, m_strFileName.c_str());
	if (strcmp(str, "") == 0)
		MessageBox(NULL, "AppUrl can't be blank. Check Configuration.ini file.", "Invalid AppURL", MB_ICONERROR);

	return std::string(str);
}

//Read Authentication URL
std::string ConfigReader::AuthenticationUrl()
{
	char str[MAX_PATH];
	GetPrivateProfileString("OAPIConfiguration", "AuthenticationUrl", "", str, MAX_PATH, m_strFileName.c_str());
	if (strcmp(str, "") == 0)
		MessageBox(NULL, "AuthenticationUrl can't be blank. Check Configuration.ini file.", "Invalid AuthenticationUrl", MB_ICONERROR);

	return std::string(str);
}

//Read OpenAPI Base URL
std::string ConfigReader::OpenApiBaseUrl()
{
	char str[MAX_PATH];
	GetPrivateProfileString("OAPIConfiguration", "OpenApiBaseUrl", "", str, MAX_PATH, m_strFileName.c_str());
	if (strcmp(str, "") == 0)
		MessageBox(NULL, "OpenApiBaseUrl can't be blank. Check Configuration.ini file.", "Invalid OpenApiBaseUrl", MB_ICONERROR);

	return std::string(str);
}