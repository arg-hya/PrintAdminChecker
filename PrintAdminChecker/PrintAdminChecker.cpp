// PrintAdminChecker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <windows.h>
#include <winspool.h>

static const char* REGKEY_ArghyaData = "ArghyaData";

BOOL IsPrintAdmin(LPTSTR pPrinterName)
{
    HANDLE hPrinter = NULL;
    PRINTER_DEFAULTS pd;
    BOOL bFlag;
	BOOL bRet = false;

    ZeroMemory(&pd, sizeof(pd));
    pd.DesiredAccess = PRINTER_ACCESS_ADMINISTER;
    bFlag = OpenPrinter(pPrinterName, &hPrinter, &pd);
    if (!bFlag || (hPrinter == NULL))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL WriteValue(LPTSTR pPrinterName)
{
	HANDLE hPrinter = NULL;
	PRINTER_DEFAULTS pd;
	BOOL bFlag;
	BOOL bRet = false;

	ZeroMemory(&pd, sizeof(pd));
	pd.DesiredAccess = PRINTER_ACCESS_ADMINISTER;
	OpenPrinter(pPrinterName, &hPrinter, &pd);

	BYTE pData[1];
	pData[0] = 1;

	bRet = SetPrinterData(hPrinter, (LPTSTR)REGKEY_ArghyaData, REG_BINARY, pData, sizeof(pData));
	if (bRet)
	{
		std::cout << "SetPrinterData Failed";
		return FALSE;
	}

	return TRUE;
}

BOOL GetDC(LPTSTR pPrinterName)
{
	int nNumMEDIATYPES = DeviceCapabilities(pPrinterName, NULL, DC_MEDIATYPES, NULL, NULL);

	DWORD* MEDIATYPES = new DWORD[nNumMEDIATYPES ];

	ZeroMemory(MEDIATYPES, nNumMEDIATYPES);

	DeviceCapabilities(pPrinterName, NULL, DC_MEDIATYPES, (LPSTR)MEDIATYPES, NULL);


	int nNumMEDIATYPES_names = DeviceCapabilities(pPrinterName, NULL, DC_MEDIATYPENAMES, NULL, NULL);

	//DWORD* MEDIATYPES_names = new DWORD[nNumMEDIATYPES_names];
	char* MEDIATYPES_names = new char[64 * nNumMEDIATYPES_names];

	ZeroMemory(MEDIATYPES_names, 64 * nNumMEDIATYPES_names);

	DeviceCapabilities(pPrinterName, NULL, DC_MEDIATYPENAMES, MEDIATYPES_names, NULL);

	for (int i = 0; i < nNumMEDIATYPES; i++)
	{
		std::cout << std::string(MEDIATYPES_names).c_str() << " = " << MEDIATYPES[i] << std::endl;
		MEDIATYPES_names += 64 ;
	}

	return TRUE;
}

BOOL GetValue(LPTSTR pPrinterName)
{
	HANDLE hPrinter = NULL;
	PRINTER_DEFAULTS pd;
	BOOL bFlag;
	BOOL bRet = false;


	DWORD dwDataType = REG_BINARY;
	WORD* pOutput = 0;
	DWORD Size = 0;
	int a;

	ZeroMemory(&pd, sizeof(pd));
	pd.DesiredAccess = PRINTER_ACCESS_ADMINISTER;
	OpenPrinter(pPrinterName, &hPrinter, &pd);

	//DeviceCapabilities(pPrinterName, NULL, DC_MEDIATYPES, (LPWSTR)pMediaTypes, pDevmode);

	DWORD dwReturn = GetPrinterData(hPrinter, _T("DC_BinIDs"), &dwDataType, NULL, Size,
		&Size);
	if (ERROR_MORE_DATA == dwReturn)
	{
		int num = Size / sizeof(WORD);
		pOutput = new WORD[num];
		ZeroMemory(pOutput, num);

		if (ERROR_SUCCESS == GetPrinterData(hPrinter, _T("DC_BinIDs"), &dwDataType, (PBYTE)pOutput, Size, &Size))
		{
			for (size_t i = 0; i < num; i++)
			{
				std::cout << pOutput[i] << std::endl;
				//pOutput++;
			}


			return TRUE;
		}
	}

	return FALSE;
}

BOOL GetMediaTypeIDs(LPTSTR pPrinterName)
{
	HANDLE hPrinter = NULL;
	PRINTER_DEFAULTS pd;
	BOOL bFlag;
	BOOL bRet = false;


	DWORD dwDataType = 4;// REG_BINARY;
	DWORD* pOutput = 0;
	DWORD Size = 0;
	int a;

	ZeroMemory(&pd, sizeof(pd));
	pd.DesiredAccess = PRINTER_ACCESS_ADMINISTER;
	OpenPrinter(pPrinterName, &hPrinter, &pd);

	//DWORD *dwResult;
	//dwResult = new DWORD();
	//*dwResult = 1;
	//if (ERROR_SUCCESS == GetPrinterData(hPrinter, _T("DC_MediaTypeIDs"), (LPDWORD)&dwDataType, (PBYTE)dwResult, sizeof(DWORD), &Size))
	//{
	//	if (1)
	//	{
	//		GetPrinterData(hPrinter, _T("DC_MediaTypeIDs"), (LPDWORD)&dwDataType, (PBYTE)pOutput, (*dwResult) * sizeof(DWORD), &Size);
	//	}
	//	//hrResult = S_DEVCAP_OUTPUT_FULL_REPLACEMENT;
	//}


	//DeviceCapabilities(pPrinterName, NULL, DC_MEDIATYPES, (LPWSTR)pMediaTypes, pDevmode);

	DWORD dwReturn = GetPrinterData(hPrinter, _T("DC_MediaTypeIDs"), &dwDataType, NULL, Size,
		&Size);
	if (ERROR_MORE_DATA == dwReturn)
	{
		int num = Size / sizeof(DWORD);
		pOutput = new DWORD[num];
		ZeroMemory(pOutput, num);

		if (ERROR_SUCCESS == GetPrinterData(hPrinter, _T("DC_MediaTypeIDs"), &dwDataType, (PBYTE)pOutput, Size, &Size))
		{
			for (size_t i = 0; i < num; i++)
			{
				std::cout << (pOutput[i]) << std::endl;
				//pOutput++;
			}


			return TRUE;
		}
	}

	return FALSE;
}

int main()
{
    DWORD numprinters;
    DWORD defprinter = 0;
    DWORD               dwSizeNeeded = 0;
    DWORD               dwItem;
    LPPRINTER_INFO_2    printerinfo = NULL;




    DWORD size = 0;
    TCHAR* buffer = NULL;

    // Get the size of the default printer name.
    GetDefaultPrinter(NULL, &size);
    if (size)
    {
        // Allocate a buffer large enough to hold the printer name.
        buffer = new TCHAR[size];

        // Get the printer name.
        GetDefaultPrinter(buffer, &size);

        //Display Printer Name
        std::cout << "Default Printer Name : " << buffer << std::endl;

        if (FALSE == IsPrintAdmin(buffer))
        {
            std::cout << "is NOT PrintAdmin" << std::endl;
        }
        else
        {
            std::cout << "is PrintAdmin" << std::endl;
			//GetDC(buffer);
			GetValue(buffer);
			std::cout << "is PrintAdmin" << std::endl;
			GetMediaTypeIDs(buffer);
			//WriteValue(buffer);
        }
    }
    return 0;
}