//author: Mark M.
//using EASendMail

#include <iostream>
#include <tchar.h>
#include <Windows.h>
#include "easendmailobj.tlh"
	using namespace EASendMailObjLib;

	const int ConnectNormal = 0;
	const int ConnectSSLAuto = 1;
	const int ConnectSTARTTLS = 2;
	const int ConnectDirectSSL = 3;
	const int ConnectTryTLS = 4;

	int _tmain(int argc, _TCHAR * argv[])
	{
		::CoInitialize(NULL);

		IMailPtr oSmtp = NULL;
		oSmtp.CreateInstance(__uuidof(EASendMailObjLib::Mail));

		oSmtp->LicenseCode = _T("TryIt");

		oSmtp->FromAddr = _T("watchdogsnoreply@gmail.com");

		oSmtp->AddRecipientEx(_T("remarkablymire@gmail.com"), 0);

        oSmtp->Subject = _T("Watchdogs Test");

		oSmtp->BodyText = _T("Watchdogs Test");

		oSmtp->ServerAddr = _T("smtp.gmail.com");

		oSmtp->UserName = _T("watchdogsnoreply@gmail.com");

		oSmtp->Password = _T("watchdogs123");

		oSmtp->ConnectType = ConnectTryTLS;
		oSmtp->ServerPort = 587; // 25 or 587 or 465
		oSmtp->ConnectType = ConnectSSLAuto;

		_tprintf(_T("Attemping to send email ...\r\n"));

		if (oSmtp->SendMail() == 0)
		{
			_tprintf(_T("Email was sent successfully!\r\n"));
		}
		else
		{
			_tprintf(_T("Failed to send email with the following error: %s\r\n"),
				(const TCHAR*)oSmtp->GetLastErrDescription());
		}

		return 0;
	}
