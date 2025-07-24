#include <windows.h>
#include <stdio.h>

int wmain(int argc, wchar_t *argv[])
{
	int iRet = 0;

	wprintf(L"winver9x PE Patcher\n");
	wprintf(L"Because MSVC sucks\n\n");
	
	if (argc != 2)
	{
		wprintf(L"Usage: pepatcher <file>\n");
		return 1;
	}

	HANDLE hFile = CreateFileW(argv[1], GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (!hFile || hFile == INVALID_HANDLE_VALUE)
	{
		wprintf(L"Failed to open file '%s'\n", argv[1]);
		return GetLastError();
	}

	DWORD cbFile = GetFileSize(hFile, NULL);
	LPBYTE lpFile = malloc(cbFile);
	if (!lpFile)
	{
		wprintf(L"Failed to allocate buffer for file '%s'\n", argv[1]);
		iRet = 1;
		goto cleanup;
	}

	if (!ReadFile(hFile, lpFile, cbFile, NULL, NULL))
	{
		wprintf(L"Failed to read file '%s'", argv[1]);
		iRet = GetLastError();
		goto cleanup;
	}

	PIMAGE_DOS_HEADER pDosHead = (PIMAGE_DOS_HEADER)lpFile;
	if (pDosHead->e_magic != 0x5A4D)
	{
		wprintf(L"Invalid DOS header\n");
		iRet = 1;
		goto cleanup;
	}

	PIMAGE_NT_HEADERS32 pNtHead = (PIMAGE_NT_HEADERS32)(lpFile + pDosHead->e_lfanew);
	if (pNtHead->Signature != 0x00004550)
	{
		wprintf(L"Invalid NT header\n");
		iRet = 1;
		goto cleanup;
	}

	wprintf(L"Patching NT header for file '%s'\n", argv[1]);
	
	wprintf(
		L"   Current OS version: %u.%u\n",
		pNtHead->OptionalHeader.MajorOperatingSystemVersion,
		pNtHead->OptionalHeader.MinorOperatingSystemVersion
	);

	wprintf(
		L"   Current subsystem version: %u.%u\n",
		pNtHead->OptionalHeader.MajorSubsystemVersion,
		pNtHead->OptionalHeader.MinorSubsystemVersion
	);

	pNtHead->OptionalHeader.MajorOperatingSystemVersion =  3;
	pNtHead->OptionalHeader.MinorOperatingSystemVersion = 10;
	pNtHead->OptionalHeader.MajorSubsystemVersion       =  3;
	pNtHead->OptionalHeader.MinorSubsystemVersion       = 10;

	CloseHandle(hFile);
	hFile = CreateFileW(argv[1], GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, NULL, NULL);

	if (!WriteFile(hFile, lpFile, cbFile, NULL, NULL))
	{
		wprintf(L"Failed to write to file '%s'\n", argv[1]);
		iRet = GetLastError();
		goto cleanup;
	}
	else
	{
		wprintf(L"Successfully patched\n");
		iRet = GetLastError();
		goto cleanup;
	}


cleanup:
	if (hFile)
		CloseHandle(hFile);
	if (lpFile)
		free(lpFile);

	return iRet;
}