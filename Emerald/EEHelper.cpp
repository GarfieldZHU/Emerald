#include "EEHelper.h"

#include <Windows.h>

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//----------------------------------------------------------------------------------------------------
	bool EEIsBigEndian()
	{
		unsigned short test = 0x1234;

		if (*((unsigned char*)&test) == 0x12)
			return true;
		else
			return false;
	}

	//----------------------------------------------------------------------------------------------------
	void ResumeSpeed()
	{
		WORD* byte_pos = (WORD*)::timeGetTime;
		if (*byte_pos == 0x3D83) // cmp [83,3D] 指令  
			return;

		HANDLE hMyHandle = ::OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, ::GetCurrentProcessId());
		if (hMyHandle)
		{
			BYTE cmp_[7] = { 0x83, 0x3D, 0x14, 0x00, 0xB3, 0x76, 0x00 };
			::WriteProcessMemory(hMyHandle, (BYTE*)byte_pos, cmp_, 7, 0);
			::CloseHandle(hMyHandle);
		}
	}

	//----------------------------------------------------------------------------------------------------
	std::string EEOpenFile()
	{
		std::string filePos;
		filePos.resize(100);
		OPENFILENAMEA ofn = { 0 };
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = "文本文件(*.txt)\0*.txt\0CPP(*.cpp)\0*.cpp\0所有文件(*.*)\0*.*\0";
		ofn.lpstrInitialDir = NULL;
		ofn.lpstrFile = &filePos[0];
		ofn.nMaxFile = 100;
		ofn.nFilterIndex = 0;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

		int result = GetOpenFileNameA(&ofn);
		if (result) 
		{
			//return filePos;
		}

		return filePos;
	}
}