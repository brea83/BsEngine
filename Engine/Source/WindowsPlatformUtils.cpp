#include "BsPrecompileHeader.h"
#include "PlatformUtils.h"
#include "EngineContext.h"

#include <windows.h>
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Pixie
{
	// reminder the filter is which files are valid in the file dialog
	std::string FileDialogs::OpenFile(const char* filter)
	{
		// more detail on windows file dialogs here
		// https://learn.microsoft.com/en-us/windows/win32/dlgbox/using-common-dialog-boxes#opening-a-file
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };

		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = glfwGetWin32Window(EngineContext::GetEngine()->GetGlfwWindow());
		ofn.lpstrFile = szFile;
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) )
		{
			return ofn.lpstrFile;
		}

		return std::string();
	}

	// reminder the filter is which files are valid in the file dialog
	std::string FileDialogs::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };

		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = glfwGetWin32Window(EngineContext::GetEngine()->GetGlfwWindow());
		ofn.lpstrFile = szFile;
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		//ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrDefExt = "bin\0";
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA(&ofn))
		{
			return ofn.lpstrFile;
		}
		return std::string();
	}
}