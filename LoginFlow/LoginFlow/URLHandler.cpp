#include "stdafx.h"
#include "URLHandler.h"
#include "Mshtml.h"
#include "msclr\marshal_cppstd.h"
#include <msclr\marshal.h>
#include <strsafe.h>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;
using namespace System;
using namespace System::Text;
using namespace System::Web;
using namespace System::Net;
using namespace System::Net::Http;
using namespace System::Net::Http::Headers;
using namespace System::Runtime::Serialization::Json;
using namespace System::Runtime::InteropServices;
using namespace msclr::interop;
using namespace System::Xml;

CURLHandler::CURLHandler()
{
	m_configReader = std::make_unique<ConfigReader>("../LoginFlow/Configuration.ini");
}


//Execute GetCliensMe request
std::string CURLHandler::GetClientsMe()
{
	try
	{
		HttpClient^ client = gcnew HttpClient();
		client->DefaultRequestHeaders->Add("Authorization", CppStringToSysString(m_accessToken));

		auto response = client->GetAsync(String::Format("{0}/port/v1/clients/me", CppStringToSysString(m_configReader->OpenApiBaseUrl())));

		return SysStringToCppString(response->Result->Content->ReadAsStringAsync()->Result);
	}
	catch (System::Exception^ ex)
	{
		throw ex->Message;
	}
}

//Execute GetPollChart data request
std::string CURLHandler::GetPollChartData(std::string& mode, std::string& time, std::string& uic, std::string& assetType, 
										std::string& horizon, std::string& count)
{
	try
	{
		HttpClient^ client = gcnew HttpClient();
		client->DefaultRequestHeaders->Add("Authorization", CppStringToSysString(m_accessToken));
		auto url = String::Format("{0}/chart/v1/charts/?Mode={1}&Time={2}&Uic={3}&AssetType={4}&Horizon={5}&Count={6}&TimeZoneId=0",
								CppStringToSysString(m_configReader->OpenApiBaseUrl()),
								CppStringToSysString(mode),
								CppStringToSysString(time),
								CppStringToSysString(uic),
								CppStringToSysString(assetType),
								CppStringToSysString(horizon),
								CppStringToSysString(count));

		auto response = client->GetAsync(url);
		return SysStringToCppString(response->Result->Content->ReadAsStringAsync()->Result);
	}
	catch (System::Exception^ ex)
	{
		throw ex->Message;
	}
}

//Navigate to OAPI and get login page response
void CURLHandler::Navigate(CExplorer& explorer)
{
	try
	{
		auto base64Saml = Convert::ToBase64String(Encoding::UTF8->GetBytes(BuildSamlRequest()));
		auto saml = Encoding::ASCII->GetBytes("SAMLRequest=" + HttpUtility::UrlEncode(base64Saml));

		VARIANT varTag;
		VariantInit(&varTag);
		IntPtr intPtr = (IntPtr)&varTag;
		Marshal::GetNativeVariantForObject(saml, intPtr);

		System::String^ str("Content-Type: application/x-www-form-urlencoded");
		VARIANT varTag1;
		VariantInit(&varTag1);
		IntPtr intPtr1 = (IntPtr)&varTag1;
		Marshal::GetNativeVariantForObject(str, intPtr1);

		std::string strAuthURL(m_configReader->AuthenticationUrl());
		explorer.Navigate(strAuthURL.append("/AuthnRequest").c_str(), NULL, NULL, &varTag, &varTag1);
	}
	catch (System::Exception^ e)
	{
		throw e->Message;
	}
}


//Get access token from documet response received from OAPI
bool CURLHandler::StoreAccessToken(CExplorer& explorer)
{
	bool bRet = false;
	HRESULT hr = 0;

	LPDISPATCH pDisp = explorer.get_Document();
	if (pDisp != NULL)
	{
		CComPtr<IHTMLDocument3> pIDoc3;
		if (SUCCEEDED(hr = pDisp->QueryInterface(IID_IHTMLDocument3, (void**)&pIDoc3)))
		{
			// First, look for the application-state meta element
			BSTR appStateBStr = L"Application-State";
			CComPtr<IHTMLElementCollection> pIColl;
			if (SUCCEEDED(hr = pIDoc3->getElementsByName(appStateBStr, &pIColl)))
			{
				long len = 0;
				if (SUCCEEDED(hr = pIColl->get_length(&len)))
				{
					// If there is no application-state, it is not the right page
					if (len >= 1)
					{
						IDispatch *pdispMetaElement;
						IHTMLMetaElement *pMetaElement;
						_variant_t index = 0;
						BSTR pszOptText = 0;

						if (SUCCEEDED(hr = pIColl->item(index, index, &pdispMetaElement)))
						{
							if (SUCCEEDED(hr = pdispMetaElement->QueryInterface(IID_IHTMLMetaElement, (void **)&pMetaElement)))
							{
								pdispMetaElement->Release();
								// Application state looks like this: <meta name="Application-State" content="service=IDP;federated=False;env=Test;state=Ok;authenticated=True;">
								if (SUCCEEDED(hr = pMetaElement->get_content(&pszOptText)))
								{
									pMetaElement->Release();
									CW2A bstrConverted(pszOptText);
									std::string content(bstrConverted);

									// Split at ";" sepearator
									std::stringstream ss;
									std::vector<std::string> tokens;
									ss.str(content);
									std::string item;
									while (std::getline(ss, item, ';'))
									{
										tokens.push_back(item);
									}

									// Look for the "state=idp" - this means that the process is finished
									// Without checking this, we don't know whether the authentication failed or just haven't gotten there yet
									if (FindString(tokens, "service=idp"))
									{
										// This is the IDP response page. Check if the list of values contains authenticated=true
										if (FindString(tokens, "authenticated=true") && FindString(tokens, "state=token"))
										{
											bRet = true;
										}
									}
								}
							}
						}
					}
				}
			}

			if (!bRet)
				return false;

			CComPtr<IHTMLDocument2> pIDoc2;
			if (SUCCEEDED(hr = pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pIDoc2)))
			{
				CComPtr<IHTMLElement> pBody;
				// On the final page, the SAML-Response is on an attribute on the body element, named SSO_SAML2_TOKEN
				if (SUCCEEDED(hr = pIDoc2->get_body(&pBody)))
				{
					//For some dumb reason 'pBody' can still be NULL here
					if (pBody)
					{
						BSTR attrBStr = L"SSO_SAML2_TOKEN";
						VARIANT varAttr;
						if (SUCCEEDED(hr = pBody->getAttribute(attrBStr, 0, &varAttr)))
						{
							//_bstr_t bs(varAttr)
							BSTR bstr = varAttr.bstrVal;
							if (varAttr.vt != VT_NULL && bstr)
							{
								GetAccessToken(Encoding::UTF8->GetString(Convert::FromBase64String(marshal_as<System::String^>(bstr))));
								bRet = true;
							}
						}
					}
				}
			}
		}
		pDisp->Release();
	}
	return bRet;
}

//Find case-insensitive string from vector
bool CURLHandler::FindString(std::vector<std::string> compareFrom, std::string compateThis)
{
	std::string myString;
	auto ret = std::find_if(compareFrom.begin(), compareFrom.end(), [&compateThis](std::string& item) {
		//Transform the each vector item to lower temporally 
		std::transform(item.begin(), item.end(), item.begin(), ::tolower);
		return std::equal(item.cbegin(), item.cend(), compateThis.cbegin(), compateThis.cend(),
			[](auto c1, auto c2) { return std::toupper(c1) == std::toupper(c2); });
	});
	return ret != compareFrom.end();
}

//Convert System::String^ to std::string
std::string CURLHandler::SysStringToCppString(String^ s)
{
	auto intPtrString = Marshal::StringToHGlobalAnsi(s);
	auto rawStringMem = (char*)intPtrString.ToPointer();
	std::string r(rawStringMem);
	Marshal::FreeHGlobal(intPtrString);
	return r;
}

//convert std::string to System::String^
String^ CURLHandler::CppStringToSysString(const std::string& s)
{
	return Marshal::PtrToStringAnsi((IntPtr)(char*)s.c_str());
}

//building saml request
System::String^ CURLHandler::BuildSamlRequest()
{
	SYSTEMTIME systime;
	GetSystemTime(&systime);

	CString date;
	date.Format("%d-%02d-%02dT%02d:%02d:%02dZ",
		systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);
	String^ strDate = gcnew System::String(date);

	UUID newId;
	UuidCreate(&newId);
	CString guid;

	RPC_CSTR szUuid = NULL;
	if (UuidToString(&newId, &szUuid) == RPC_S_OK)
	{
		guid = szUuid;
	}

	String^ guidString = gcnew String(guid);

	System::String^ str(String::Format("<samlp:AuthnRequest ID=\"_{0}\" Version=\"2.0\" ForceAuthn=\"false\" IsPassive=\"false\""
		" ProtocolBinding=\"urn:oasis:names:tc:SAML:2.0:bindings:HTTP-POST\" xmlns:samlp=\"urn:oasis:names:tc:SAML:2.0:protocol\""
		" IssueInstant=\"{1}\" Destination=\"{2}/login/AuthnRequest\" AssertionConsumerServiceURL=\"{3}\">"
		" <samlp:NameIDPolicy AllowCreate=\"false\" />"
		" <saml:Issuer xmlns:saml=\"urn:oasis:names:tc:SAML:2.0:assertion\">{4}</saml:Issuer>"
		"</samlp:AuthnRequest>", 
		guidString, 
		strDate, 
		CppStringToSysString(m_configReader->AuthenticationUrl()),
		CppStringToSysString(m_configReader->AppUrl()), 
		CppStringToSysString(m_configReader->AppUrl())));
	return str;
}

//store access token, this can be re-used after receiving once.
//OPenAPI request can be executed on this access token.
void CURLHandler::GetAccessToken(System::String^ samlToken)
{
	System::String^ authorizationCode = ParseAndGetAuthorizationCode(samlToken);
	auto requestPayload = "grant_type=authorization_code&code=" + authorizationCode;
	CppDemo::CppTokenResponse^ tokenResponse = SendAuthorizationRequest(requestPayload);
	m_accessToken = SysStringToCppString(tokenResponse->TokenType) + " " + SysStringToCppString(tokenResponse->AccessToken);
}

System::String^ CURLHandler::ParseAndGetAuthorizationCode(System::String^ saml)
{
	XmlDocument^ xml = gcnew XmlDocument();
	xml->LoadXml(saml); // if the SAML is malformed, this will throw an exception
	
	auto nsmgr = gcnew System::Xml::XmlNamespaceManager(xml->NameTable);
	nsmgr->AddNamespace("samlp", "urn:oasis:names:tc:SAML:2.0:protocol");
	nsmgr->AddNamespace("saml", "urn:oasis:names:tc:SAML:2.0:assertion");
	auto attribute = xml->SelectSingleNode("/samlp:Response/saml:Assertion/saml:AttributeStatement/saml:Attribute[@Name='AuthorizationCode']/saml:AttributeValue", nsmgr);
	return attribute->InnerText;
}

CppDemo::CppTokenResponse^ CURLHandler::SendAuthorizationRequest(System::String^ requestPayload)
{
	try
	{
		System::String^ credentials = String::Format("{0}:{1}",
													CppStringToSysString(m_configReader->AppKey()),
													CppStringToSysString(m_configReader->AppSecret()));
		auto auth = Convert::ToBase64String(Encoding::UTF8->GetBytes(credentials));

		auto client = gcnew HttpClient();
		client->DefaultRequestHeaders->Add("Authorization", String::Format("Basic {0}", auth));

		auto content = gcnew StringContent(requestPayload);
		{
			content->Headers->ContentType = gcnew MediaTypeHeaderValue("application/x-www-form-urlencoded");

			auto response = client->PostAsync(CppStringToSysString(m_configReader->AuthenticationUrl() + "/token"), content);
			auto stream = response->Result->Content->ReadAsStreamAsync();

			CppDemo::CppTokenResponse cppResponse;
			auto serializer = gcnew DataContractJsonSerializer(cppResponse.GetType());
			auto tokenResponse = static_cast<CppDemo::CppTokenResponse^>(serializer->ReadObject(stream->Result));
			return tokenResponse;
		}
	}
	catch (System::Exception^ ex)
	{
		throw ex->Message;
	}
}