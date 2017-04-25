#pragma once

#include <string>


class ConfigReader
{
public:
	ConfigReader();
	ConfigReader(std::string strFileName);

	std::string AccessToken();
	std::string OpenApiBaseUrl();

private:
	std::string m_strFileName;
};