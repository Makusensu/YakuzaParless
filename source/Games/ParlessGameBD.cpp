#include "ParlessGameBD.h"
#include "../StringHelpers.h"
#include "Utils/MemoryMgr.h"
#include "Utils/Trampoline.h"
#include "Utils/Patterns.h"
#include <MinHook.h>
#include <iostream>


using namespace hook;
using namespace Memory;

__int64 (*ParlessGameBD::orgBDAddFileEntry)(__int64 a1, char* filepath, __int64 a3, int a4, __int64 a5, __int64 a6, char a7, __int64 a8, char a9, char a10, char a11, char a12, char a13) = NULL;
__int64 (*ParlessGameBD::orgBDCpkEntry)(__int64 a1, __int64 a2, __int64 a3, __int64 a4) = NULL;

std::string ParlessGameBD::get_name()
{
	return "Binary Domain";
}

bool ParlessGameBD::hook_add_file()
{
	void* renameFilePathsFunc;

	Trampoline* trampoline = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));

	//hook_BindCpk = (t_CriBind*)get_pattern("8a 06 46 3c ?? 75", 6);
	//org_BindDir = (t_CriBind)0x140B35ADC;

	/*
	if (MH_CreateHook(hook_BindCpk, &CBaseParlessGame::BindCpk, reinterpret_cast<LPVOID*>(&org_BindCpk)) != MH_OK)
	{
		return false;
	}

	if (MH_EnableHook(hook_BindCpk) != MH_OK)
	{
		cout << "Hook could not be enabled. Aborting.\n";
		return false;
	}
	*/

	// might want to hook the other call of this function as well, but currently not necessary (?)
	renameFilePathsFunc = get_pattern("8a 06 46 3c ?? 75", 6);
	ReadCall(renameFilePathsFunc, orgBDAddFileEntry);

	// this will take care of every file that is read from disk
	InjectHook(renameFilePathsFunc, trampoline->Jump(orgBDAddFileEntry));

	//renameFilePathsFunc = get_pattern("8B 4D D7 4A 8D 74 28 20 48 83 E6 E0 48 8D BE 9F 02 00 00", -13);
	//ReadCall(renameFilePathsFunc, orgBDCpkEntry);
	//InjectHook(renameFilePathsFunc, trampoline->Jump(Y0CpkEntry));

	//cout << "Applied CPK loading hook.\n";

	//org_BindCpk = (t_CriBind)((char*)org_BindCpk + 1);

	return true;
}