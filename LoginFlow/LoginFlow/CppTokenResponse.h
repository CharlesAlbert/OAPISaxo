#pragma once

using namespace System::Runtime::Serialization;

namespace CppDemo
{
	[DataContract]
	public ref class CppTokenResponse
	{
	public:
		/// <summary>
		/// The access token; used for accessing Open API services
		/// </summary>
		[DataMember(Name = "access_token")]
		property System::String^ AccessToken;

		/// <summary>
		/// The token type
		/// </summary>
		[DataMember(Name = "token_type")]
		property System::String^ TokenType;

		/// <summary>
		/// The expiry of the access token in seconds
		/// </summary>
		[DataMember(Name = "expires_in")]
		property System::Int32 ExpiresIn;

		/// <summary>
		/// The Refresh token; used for getting a new access token
		/// </summary>
		[DataMember(Name = "refresh_token")]
		property System::String^ RefreshToken;

		/// <summary>
		/// The expiry of the refresh token in seconds
		/// </summary>
		[DataMember(Name = "refresh_token_expires_in")]
		property System::Int32 RefreshTokenExpiresIn;

		/// <summary>
		/// The base URI of the Open API service groups
		/// </summary>
		[DataMember(Name = "base_uri")]
		property System::String^ BaseUri;
	};
}
