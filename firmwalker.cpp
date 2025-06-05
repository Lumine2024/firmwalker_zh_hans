#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <locale>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif
	if(argc > 3 || argc < 2) {
		cout << "使用方法\n" << argv[0] << " + 输入文件 + [可选：输出文件，默认firmwalker.txt]" << endl;
		return 1;
	}
	string firmdir = argv[1];
	string output_file = (argc == 3) ? argv[2] : "firmwalker.txt";
	if(!fs::exists(firmdir)) {
		cout << "输入的文件夹不存在" << endl;
		return 1;
	}
	ofstream out(output_file);
	auto msg = [&](const string &s) {
		cout << s << endl;
		out << s << endl;
	};
	auto getArray = [&](const string &path) -> vector<string> {
		vector<string> ret;
		ifstream in(path);
		if(!in.is_open()) {
			msg("无法打开文件: " + path);
			return ret;
		}
		string line;
		while(getline(in, line)) {
			ret.emplace_back(line);
		}
		in.close();
		return ret;
	};
	msg("------------firmwalker-------------");
	msg("");
	msg("***固件地址***");
	msg(firmdir);
	msg("****正在搜索密码文件***");
	auto passfiles = getArray("data/passfiles");
	for(auto &passfile : passfiles) {
		msg("############################################# " + passfile);
		for(auto &entry : fs::recursive_directory_iterator(firmdir)) {
			if(entry.path().filename() == passfile) {
				msg(entry.path().string());
			}
		}
		msg("");
	}
	msg("");
	msg("***正在搜索Unix-MD5哈希***");
	regex md5_regex(R"(\$1\$\w{8}\S{23})");
	set<string> md5_set;
	for(auto &entry : fs::recursive_directory_iterator(firmdir)) {
		if(!fs::is_regular_file(entry.path())) continue;
		ifstream in(entry.path());
		if(!in.is_open()) continue;
		string line;
		while(getline(in, line)) {
			sregex_iterator it(line.begin(), line.end(), md5_regex);
			sregex_iterator end;
			for(; it != end; ++it) {
				md5_set.insert(it->str());
			}
		}
		in.close();
	}
	for(const auto &md5 : md5_set) {
		msg(md5);
	}
	msg("");
	// 跳过SSL相关文件
	msg("***正在搜索SSH相关文件***");
	auto sshfiles = getArray("data/sshfiles");
	for(auto &sshfile : sshfiles) {
		msg("############################################# " + sshfile);
		for(auto &entry : fs::recursive_directory_iterator(firmdir)) {
			if(entry.path().filename() == sshfile) {
				msg(entry.path().string());
			}
		}
		msg("");
	}
	msg("");
	msg("***正在搜索files文件夹***");
	auto files = getArray("data/files");
	for(auto &file : files) {
		msg("############################################# " + file);
		for(auto &entry : fs::recursive_directory_iterator(firmdir)) {
			if(entry.path().filename() == file) {
				msg(entry.path().string());
			}
		}
		msg("");
	}
	msg("");

	msg("***正在搜索数据库文件***");
	auto dbfiles = getArray("data/dbfiles");
	for(auto &dbfile : dbfiles) {
		msg("############################################# " + dbfile);
		for(auto &entry : fs::recursive_directory_iterator(firmdir)) {
			if(entry.path().filename() == dbfile) {
				msg(entry.path().string());
			}
		}
		msg("");
	}
	msg("");
	msg("***正在搜索shell脚本***");
	msg("############################################# " "shell脚本");
	for(auto &entry : fs::recursive_directory_iterator(firmdir)) {
		if(entry.path().extension() == ".sh") {
			msg(entry.path().string());
		}
	}
	msg("");
	msg("***正在搜索其他.bin文件***");
	msg("############################################# " "其他.bin文件");
	for(auto &entry : fs::recursive_directory_iterator(firmdir)) {
		if(entry.path().extension() == ".bin") {
			msg(entry.path().string());
		}
	}
	msg("");
	msg("***正在搜索文件样式***");
	auto patterns = getArray("data/patterns");
	for(auto &pattern : patterns) {
		msg("-----------------" + pattern + "-----------------");
		for(auto &entry : fs::recursive_directory_iterator(firmdir)) {
			if(!fs::is_regular_file(entry.path())) continue;
			ifstream in(entry.path());
			if(!in.is_open()) continue;
			string line;
			while(getline(in, line)) {
				if(line.find(pattern) != string::npos) {
					msg(entry.path().string());
					break;
				}
			}
			in.close();
		}
		msg("");
	}
	msg("");
	msg("***正在搜索网络服务器***");
	msg("############################################# " "网络服务器");
	auto webservers = getArray("data/webservers");
	for(auto &webserver : webservers) {
		msg("############################################# " + webserver);
		for(auto &entry : fs::recursive_directory_iterator(firmdir)) {
			if(entry.path().filename() == webserver) {
				msg(entry.path().string());
			}
		}
		msg("");
	}
	msg("");
	msg("***正在搜索重要二进制文件***");
	msg("############################################# " "重要二进制文件");
	auto binaries = getArray("data/binaries");
	for(auto &binary : binaries) {
		msg("############################################# " + binary);
		for(auto &entry : fs::recursive_directory_iterator(firmdir)) {
			if(entry.path().filename() == binary) {
				msg(entry.path().string());
			}
		}
		msg("");
	}
	msg("");
	msg("***正在搜索IP地址***");
	msg("############################################# IP地址");
	regex ip_regex(R"(\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b)");
	set<string> ip_set;
	for(auto &entry : fs::recursive_directory_iterator(firmdir)) {
		if(!fs::is_regular_file(entry.path())) continue;
		ifstream in(entry.path());
		if(!in.is_open()) continue;
		string line;
		while(getline(in, line)) {
			sregex_iterator it(line.begin(), line.end(), ip_regex);
			sregex_iterator end;
			for(; it != end; ++it) {
				ip_set.insert(it->str());
			}
		}
		in.close();
	}
	for(const auto &ip : ip_set) {
		msg(ip);
	}
	msg("");
	msg("***正在搜索URL***");
	msg("############################################# " "URL");
	regex url_regex(R"((http|https)://[^/"]+)");
	set<string> url_set;
	for(auto &entry : fs::recursive_directory_iterator(firmdir)) {
		if(!fs::is_regular_file(entry.path())) continue;
		ifstream in(entry.path());
		if(!in.is_open()) continue;
		string line;
		while(getline(in, line)) {
			sregex_iterator it(line.begin(), line.end(), url_regex);
			sregex_iterator end;
			for(; it != end; ++it) {
				url_set.insert(it->str());
			}
		}
		in.close();
	}
	for(const auto &url : url_set) {
		msg(url);
	}
	msg("");
	msg("***正在搜索邮箱***");
	msg("############################################# " "邮箱");
	regex email_regex(R"(([[:alnum:]_.-]+@[[:alnum:]_.-]+?\.[[:alpha:].]{2,6}))");
	set<string> email_set;
	for(auto &entry : fs::recursive_directory_iterator(firmdir)) {
		if(!fs::is_regular_file(entry.path())) continue;
		ifstream in(entry.path());
		if(!in.is_open()) continue;
		string line;
		while(getline(in, line)) {
			sregex_iterator it(line.begin(), line.end(), email_regex);
			sregex_iterator end;
			for(; it != end; ++it) {
				email_set.insert(it->str());
			}
		}
		in.close();
	}
	for(const auto &email : email_set) {
		msg(email);
	}
	msg("");
	msg("搜索完毕");
	return 0;
}