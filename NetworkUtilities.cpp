#include "stdafx.h"
#include "NetworkUtilities.h"

#include <algorithm>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/lexical_cast.hpp>

using namespace NetworkUtilities;

bool NetworkUtilities::IsValidIPAddress( const std::string& p_strIPAddress )
{		
	std::string strIPAddress = p_strIPAddress;

	boost::system::error_code bstError;
	boost::asio::ip::address::from_string( p_strIPAddress, bstError );
	
	return (bstError.value() == 0);
}

bool NetworkUtilities::IsValidURL( const std::string& p_strURL )
{
	bool bResult = false;

	bool bFoundProt		= false;
	bool bProtIsValid	= false;

	// check to see if they entered a protocol and make sure it's valid
	const std::string strURLSep = "://";
	
	std::string strUrl( p_strURL );
	std::string::const_iterator itBeg = strUrl.begin();
	std::string::const_iterator itEnd = strUrl.end();
	std::string::const_iterator itSep = itBeg;	
   
	while( !bProtIsValid && (itSep = search(itSep, itEnd, strURLSep.begin(), strURLSep.end())) != itEnd  ) 
	{
		bFoundProt = true;
		// Make sure that, if there is the seperator text, it is not at the begining of the line
		if( itSep != itBeg &&
			itSep + strURLSep.size() != itEnd )
		{
			std::string::const_iterator itBegProt = itSep;
			while(	itBegProt != itBeg &&
					isalpha( itBegProt[-1] ) )
			{
			   --itBegProt;
			}

			if( itBegProt != itSep )
			{
			   char c = itSep[ strURLSep.size() ];
			   if( !NetworkUtilities::IsNonURLCharacter( c )  )
			   {
				   // success	
				   bProtIsValid = true;
			   }
			}
		}
		itSep += strURLSep.size();
	}      

	if( ( bFoundProt && bProtIsValid ) ||
		!bFoundProt )
	{				
		std::string::const_iterator itPath = itBeg;
		if( itSep != itBeg && itSep != itEnd )
		{
			itPath = itSep;
		}
		if( itPath != itEnd && 
			isalnum( itPath[0] ) )
		{
			std::string::const_iterator itFound = std::find_if( itPath, itEnd, NetworkUtilities::IsNonURLCharacter );
			bResult = itFound == itEnd;
		}
	}

	return bResult;
}

bool NetworkUtilities::IsValidMulticastIP( const std::string& p_strIPAddress )
{
	bool bValid = false;
	if( NetworkUtilities::IsValidIPAddress( p_strIPAddress ) )
	{
		std::string strFirstOctet = p_strIPAddress.substr( 0, 3 );
		unsigned short usFirstOctet = 0;
		try
		{
			unsigned short usFirstOctet = boost::lexical_cast<unsigned short>( strFirstOctet );
			bValid = ( (usFirstOctet >= 224) && (usFirstOctet <= 239) );			
		}
		catch( boost::bad_lexical_cast& )
		{
		}		
	}
	return bValid;
}

bool NetworkUtilities::IsNonURLCharacter( const char c )
{
	const std::string strURLch = "~;/?:@=&$-_.+!*'(),%";

	return !( isalnum( c ) ||
			  std::find( strURLch.begin(), strURLch.end(), c ) != strURLch.end() );
}

bool NetworkUtilities::IsValidNetmask(const std::string& p_strNetmask)
{
     bool bResult;     
     // Check to see if the Netmask is Valid
     boost::system::error_code bstError;
     boost::asio::ip::address_v4 bstNetmask = boost::asio::ip::address_v4::from_string( p_strNetmask, bstError );
     if( !bstError )
     {
          bResult = p_strNetmask == "255.255.255.0"    ||
                    p_strNetmask == "255.255.0.0"      ||
                    p_strNetmask == "255.0.0.0";
     }
     return bResult;
}