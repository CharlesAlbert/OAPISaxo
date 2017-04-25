// ConsoleBatchFlow.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ConfigReader.h"
#include <memory>
#include <afx.h>
#include <Rpcdce.h>

using namespace std;
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Net::Http;

//convert std::string to System::String^
String^ CppStringToSysString(const std::string& s)
{
	return Marshal::PtrToStringAnsi((IntPtr)(char*)s.c_str());
}

//Convert System::String^ to std::string
std::string SysStringToCppString(String^ s)
{
	auto intPtrString = Marshal::StringToHGlobalAnsi(s);
	auto rawStringMem = (char*)intPtrString.ToPointer();
	std::string r(rawStringMem);
	Marshal::FreeHGlobal(intPtrString);
	return r;
}

int main()
{
	//read configuration parameters
	std::unique_ptr<ConfigReader> configReader = std::make_unique<ConfigReader>("../ConsoleBatchFlow/Configuration.ini");
	std::string accessToken = configReader->AccessToken();
	std::string baseURL = configReader->OpenApiBaseUrl();

	std::cout << "Started Processing Batch..." << std::endl;

	auto batchURL = System::String::Format("{0}/ref/batch", CppStringToSysString(baseURL));
	auto getCurrenciesUrl = System::String::Format("{0}/ref/v1/currencies", CppStringToSysString(baseURL));
	auto getTimezonesUrl = System::String::Format("{0}/ref/v1/timezones", CppStringToSysString(baseURL));

	auto client = gcnew HttpClient();
	client->DefaultRequestHeaders->Add("Authorization", String::Format("BEARER {0}", CppStringToSysString(accessToken)));

	//Generate guid
	UUID newId;
	UuidCreate(&newId);
	CString guid;

	RPC_CSTR szUuid = NULL;
	if (UuidToString(&newId, &szUuid) == RPC_S_OK)
	{
		guid = szUuid;
	}
	else
	{
		cout << "Unable to generate guid" << endl;
		return 0;
	}

	String^ guidString = gcnew String(guid);
	auto content = gcnew MultipartContent("mixed", String::Format("batch_{0}", guidString));

	auto batchRequest = gcnew HttpRequestMessage(HttpMethod::Post, gcnew Uri(batchURL));

	// Prepare contents of Urls to be batched
	auto getQuery1Content = gcnew HttpMessageContent(gcnew HttpRequestMessage(HttpMethod::Get, gcnew Uri(getCurrenciesUrl)));
	auto getQuery2Content = gcnew HttpMessageContent(gcnew HttpRequestMessage(HttpMethod::Get, gcnew Uri(getTimezonesUrl)));

	content->Add(getQuery1Content);
	content->Add(getQuery2Content);

	// Add both the Get Request to Batch Request content
	batchRequest->Content = content;

	auto response = client->SendAsync(batchRequest);
	auto responseContents = response->Result->Content->ReadAsStringAsync();

	cout << "Response String of Requests:" << SysStringToCppString(responseContents->Result) << endl;
    return 0;
}

