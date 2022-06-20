// SmbiosMemConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <Windows.h>
#include <winioctl.h>
#include < sstream >
#include "IOCTLValue.h"
#include "AaeonSmbiosApi.h"

#define AAEON_DEVICE L"\\\\.\\Aaeon_SmbiosMemoryLink"
DWORD SMBIOSEntryPoint = 0;
DWORD SMBIOSSize = 0;
using namespace std;
void RasiePrivileges(void)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp = { 0 };

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		&hToken)) {
		//printf("Failed OpenProcessToken\r\n");
		return;
	}

	LookupPrivilegeValue(NULL, SE_SYSTEM_ENVIRONMENT_NAME,
		&tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	DWORD len;
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, &len);

	if (GetLastError() != ERROR_SUCCESS) {
		//printf("Failed RasiePrivileges()\r\n");
		return;
	}
}

DWORD getEntryPoint()
{
	typedef struct
	{
		DWORD SMBIOSAdr;
		DWORD DontCare;
	}*PSMBIOS;
	
	unsigned long dwLen = 0;
	wchar_t name[256] = L"SmbiosV3EntryPointTable";
	PVOID p_query_buffer = NULL;
	TCHAR guid[256] = L"{4b3082a3-80c6-4d7e-9cd0-583917265df1}";
	unsigned int query_buffer_size = 8;
	PVOID  pBuffer = malloc(255);
	DWORD dwAttributes = 0x00000001 | 0x00000002 | 0x00000004; /*VARIABLE_ATTRIBUTE_NON_VOLATILE*/
	RasiePrivileges();
	dwLen = GetFirmwareEnvironmentVariable(
		name, guid, pBuffer, query_buffer_size);

	PSMBIOS PEntryPoint = (PSMBIOS)pBuffer;
	if (0 == dwLen)
	{
		printf("ErrCode = %ld.\n", GetLastError());
	}
	else
	{
		printf("Variable size: %d.\n", dwLen);
		printf("0x%lx\n", PEntryPoint->SMBIOSAdr);
		SMBIOSSize = 
		SMBIOSEntryPoint = PEntryPoint->SMBIOSAdr;
	}
	free(pBuffer);
	return SMBIOSEntryPoint;
}

void WriteSMBIOS(int isString, int type, int dataIndex, int DataSize, UCHAR data[])
{
	cout << "Entery WriteSMBIOS" << endl;
	HANDLE hDevice = NULL;
	BOOL result;
	DWORD dwOutput;
	int buff[5] = { 0 };
	AAEON_SMBIOS bAAEON_SMBIOS;
	// Create device handler to driver
	hDevice = CreateFile(AAEON_DEVICE,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		cout << "Open IOCTL Failed." << endl;
		return;
	}
	bAAEON_SMBIOS.bEntryPoint = SMBIOSEntryPoint;
	bAAEON_SMBIOS.bType = (UCHAR)type;
	bAAEON_SMBIOS.bDataIndex = (UCHAR)dataIndex;
	bAAEON_SMBIOS.bDataSize = (UCHAR)DataSize;
	bAAEON_SMBIOS.bIsString = (UCHAR)isString;
	
	for (int i = 0; i < DataSize; ++i)
	{
		bAAEON_SMBIOS.bData[i] = (UCHAR)data[i];
		cout << data[i] << " | ";
	}
	printf("CallSMBIOS Entry Point: 0x%lx", bAAEON_SMBIOS.bEntryPoint);
	// Entry Drive IO Control
	result = DeviceIoControl(hDevice,
		IOCTL_AAEON_WRITE_SMBIOS,
		&bAAEON_SMBIOS,
		sizeof(AAEON_SMBIOS),
		&bAAEON_SMBIOS,
		sizeof(AAEON_SMBIOS),
		&dwOutput,
		NULL);

	if (result == FALSE) {
		cout << "Last Error: " << GetLastError() << endl;
	}
}

int main()
{
	AaeonSmbiosInitial();

	SmbiosMemberInfo* member_info = new SmbiosMemberInfo();
	int type;
	string member_name, str_data;
	UCHAR data[255];
	cout << "Input Smbios int parameter \"Type\"\n";
	cin >> hex >> type;
	cout << "Input Smbios String parameter \"Member Name\"\n";
	cin >> member_name;
	if (AaeonSmbiosGetMemInfo((SmbiosType)type, member_name, member_info))
	{
		printf("SmbiosMember   Type: %s\n", (member_info->type == 1 ? "String" : "Value"));
		printf("SmbiosMember offset: %d\n", member_info->offset);
		printf("SmbiosMember length: %d\n", member_info->length);

		if (member_info->type)
		{
			cout << "Input Var  \"String Data\"\n";
			cin >> str_data;
			for (int i = 0; i < str_data.length(); ++i)
				data[i] = str_data[i];
			data[str_data.length()] = '\0';
			WriteSMBIOS((int)member_info->type, type, (int)member_info->offset, str_data.length() + 1, data);
		}
		else
		{
			cout << "Input Var  \"Data\"\n";
			/*cin >> str_data;
			for (int i = 0; i < member_info->length; i++)
			{
				data[i] = (str_data[i * 2] -'0')* 10 + str_data[i * 2 + 1] - '0';
				printf("%d ", data[i]);
			}*/
			data[0] = 0xAB;
			data[1] = 0xCD;
			WriteSMBIOS((int)member_info->type, type, (int)member_info->offset, (int)member_info->length, data);
		}
	}
	else
	{
		cout << "Failed retrieve Smbios Member Information!\n";
	}

	delete member_info;
	AaeonSmbiosUninitial();
	system("pause");

    return 0;
}