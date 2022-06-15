#pragma once
#include <unordered_map>
#include <vector>
#include "MemberProp.h"

using std::string;
using std::unordered_map;
using std::vector;

// SMBIOS type macros which is according to SMBIOS 2.7 specification.
#pragma region SMBIOS Type Macros
#define SMBIOS_TYPE_BIOS_INFORMATION                     0
#define SMBIOS_TYPE_SYSTEM_INFORMATION                   1
#define SMBIOS_TYPE_BASEBOARD_INFORMATION                2
#define SMBIOS_TYPE_SYSTEM_ENCLOSURE                     3
#define SMBIOS_TYPE_PROCESSOR_INFORMATION                4
#define SMBIOS_TYPE_MEMORY_CONTROLLER_INFORMATION        5
#define SMBIOS_TYPE_MEMORY_MODULE_INFORMATON             6
#define SMBIOS_TYPE_CACHE_INFORMATION                    7
#define SMBIOS_TYPE_PORT_CONNECTOR_INFORMATION           8
#define SMBIOS_TYPE_SYSTEM_SLOTS                         9
#define SMBIOS_TYPE_ONBOARD_DEVICE_INFORMATION           10
#define SMBIOS_TYPE_OEM_STRINGS                          11
#define SMBIOS_TYPE_SYSTEM_CONFIGURATION_OPTIONS         12
#define SMBIOS_TYPE_BIOS_LANGUAGE_INFORMATION            13
#define SMBIOS_TYPE_GROUP_ASSOCIATIONS                   14
#define SMBIOS_TYPE_SYSTEM_EVENT_LOG                     15
#define SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY                16
#define SMBIOS_TYPE_MEMORY_DEVICE                        17
#define SMBIOS_TYPE_32BIT_MEMORY_ERROR_INFORMATION       18
#define SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS          19
#define SMBIOS_TYPE_MEMORY_DEVICE_MAPPED_ADDRESS         20
#define SMBIOS_TYPE_BUILT_IN_POINTING_DEVICE             21
#define SMBIOS_TYPE_PORTABLE_BATTERY                     22
#define SMBIOS_TYPE_SYSTEM_RESET                         23
#define SMBIOS_TYPE_HARDWARE_SECURITY                    24
#define SMBIOS_TYPE_SYSTEM_POWER_CONTROLS                25
#define SMBIOS_TYPE_VOLTAGE_PROBE                        26
#define SMBIOS_TYPE_COOLING_DEVICE                       27
#define SMBIOS_TYPE_TEMPERATURE_PROBE                    28
#define SMBIOS_TYPE_ELECTRICAL_CURRENT_PROBE             29
#define SMBIOS_TYPE_OUT_OF_BAND_REMOTE_ACCESS            30
#define SMBIOS_TYPE_BOOT_INTEGRITY_SERVICE               31
#define SMBIOS_TYPE_SYSTEM_BOOT_INFORMATION              32
#define SMBIOS_TYPE_64BIT_MEMORY_ERROR_INFORMATION       33
#define SMBIOS_TYPE_MANAGEMENT_DEVICE                    34
#define SMBIOS_TYPE_MANAGEMENT_DEVICE_COMPONENT          35
#define SMBIOS_TYPE_MANAGEMENT_DEVICE_THRESHOLD_DATA     36
#define SMBIOS_TYPE_MEMORY_CHANNEL                       37
#define SMBIOS_TYPE_IPMI_DEVICE_INFORMATION              38
#define SMBIOS_TYPE_SYSTEM_POWER_SUPPLY                  39
#define SMBIOS_TYPE_ADDITIONAL_INFORMATION               40
#define SMBIOS_TYPE_ONBOARD_DEVICES_EXTENDED_INFORMATION 41
#define SMBIOS_TYPE_MANAGEMENT_CONTROLLER_HOST_INTERFACE 42
#define SMBIOS_TYPE_TPM_DEVICE                           43
#pragma endregion

typedef string MemberName;
typedef unordered_map<MemberName, MemberProp> SmbiosDictionary;

class SmbiosMember
{
public:
	vector<SmbiosDictionary> smbios_tables;
	SmbiosMember();

private:
	SmbiosDictionary type0_table, type1_table;
	void InitialType0Table();
	void InitialType1Table();
};

