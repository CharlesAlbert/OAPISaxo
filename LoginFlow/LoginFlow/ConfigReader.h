#pragma once

#include <string>

//Class to read confoguration items from Configuration.ini
class ConfigReader
{
public:
	ConfigReader();
	ConfigReader(std::string strFileName);

	std::string AppKey();
	std::string AppSecret();
	std::string AppUrl();
	std::string AuthenticationUrl();
	std::string OpenApiBaseUrl();

private:
	std::string m_strFileName;
};