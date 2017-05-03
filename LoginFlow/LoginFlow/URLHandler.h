#pragma once

#include "CppTokenResponse.h"
#include "ConfigReader.h"
#include "Explorer.h"
#include <string>
#include <memory>
#include <vector>

//Class generate and parse url
class CURLHandler
{
public:
	CURLHandler();

	std::string CURLHandler::GetPollChartData(std::string& mode, 
											  std::string& time, 
										      std::string& uic, 
											  std::string& assetType,
											  std::string& horizon, 
											  std::string& count);
	std::string GetClientsMe();
	void Navigate(CExplorer& explorer);
	bool StoreAccessToken(CExplorer& explorer);
	std::unique_ptr<ConfigReader> m_configReader;

private:
	std::string m_accessToken;
private:
	bool FindString(std::vector<std::string> compareFrom, std::string compateThis);

	System::String^ BuildSamlRequest();
	void GetAccessToken(System::String^ samlToken);
	System::String^ ParseAndGetAuthorizationCode(System::String^ saml);
	CppDemo::CppTokenResponse^ SendAuthorizationRequest(System::String^ requestPayload);
};
