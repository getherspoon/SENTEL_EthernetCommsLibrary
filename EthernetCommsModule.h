// EthernetCommsModule.h : main header file for the EthernetCommsModule DLL
//

#pragma once

#ifndef NO_MFC

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CEthernetCommsModuleApp
// See EthernetCommsModule.cpp for the implementation of this class
//

class CEthernetCommsModuleApp : public CWinApp
{
public:
	CEthernetCommsModuleApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

#endif