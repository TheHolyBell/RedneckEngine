#include "OpenFileDialog.h"
#include <Windows.h>
#include <thread>

void OpenFileDialog::ShowDialogAsync(std::function<void(const std::string&)> _callback)
{
	std::thread([=] {
			OPENFILENAME ofn = {};       // common dialog box structure
			TCHAR szFile[260] = {};       // if using TCHAR macros

			// Initialize OPENFILENAME
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = nullptr;
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "All Files (*.*)";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = nullptr;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = nullptr;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileNameA(&ofn) == TRUE)
			{
			_callback(ofn.lpstrFile);
		}}).detach();
}

std::optional<std::string> OpenFileDialog::ShowDialog()
{
	OPENFILENAME ofn = {};       // common dialog box structure
	TCHAR szFile[260] = {};       // if using TCHAR macros

	// Initialize OPENFILENAME
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All Files (*.*)";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = nullptr;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	if (GetOpenFileNameA(&ofn) == TRUE)
		return ofn.lpstrFile;
	return {};
}
