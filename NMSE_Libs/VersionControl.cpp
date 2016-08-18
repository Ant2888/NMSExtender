#include "VersionControl.h"

static bool GetVersionStruc(std::string exe, tagVS_FIXEDFILEINFO* versStruc){
	std::cout << "USING " << exe << std::endl;
	DWORD size = GetFileVersionInfoSize(exe.c_str(), 0);
	if (size){
		UCHAR* buffer = new UCHAR[size];
		if (GetFileVersionInfo(exe.c_str(), NULL, size, buffer)){
			tagVS_FIXEDFILEINFO* preVersStruc = NULL;
			UINT strucSize = sizeof(tagVS_FIXEDFILEINFO);
			if (VerQueryValue(buffer, "\\", (void **)&preVersStruc, (PUINT)&strucSize)){
				if (preVersStruc){
					*versStruc = *preVersStruc;
					return true;
				}
				else{
					std::cout << "Failed to load the version struc (ERRCODE): " << GetLastError() << std::endl;
					delete[] buffer;
					return false;
				}
			}
			else{
				std::cout << "Failed to query value (ERRCODE): " << GetLastError() << std::endl;
				delete[] buffer;
				return false;
			}
		}
		else{
			std::cout << "Failed to retrieve version info (ERRCODE): " << GetLastError() << std::endl;
			delete[] buffer;
			return false;
		}
	}
	else{
		std::cout << "Failed to retrieve version size (ERRCODE): " << GetLastError() << std::endl;
		return false;
	}
	return false;
}


bool ValidateVersion(std::string exe){
	tagVS_FIXEDFILEINFO versStruc;
	DWORD64* versNum;

	if (!GetVersionStruc(exe, &versStruc)){
		return false;
	}

	UINT64 vers = versStruc.dwFileDateMS;
	std::cout << "VERSION: " << vers << std::endl;
	return true;
};

