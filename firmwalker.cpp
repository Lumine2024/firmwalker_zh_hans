#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
	if(argc > 3 || argc < 2) {
		cout << "ʹ�÷���\n" << argv[0] << " + �����ļ� + [��ѡ������ļ���Ĭ��firmwalker.txt]" << endl;
		return 1;
	}
	string firmdir = argv[1];
	string output_file = (argc == 3) ? argv[2] : "firmwalker.txt";
	if(!fs::exists(firmdir)) {
		cout << "������ļ��в�����" << endl;
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
			msg("�޷����ļ�: " + path);
			return ret;
		}
		string line;
		while(getline(in, line)) {
			ret.emplace_back(line);
		}
		in.close();
		return ret;
	};
	msg("***�̼���ַ***");
	msg(firmdir);
	msg("****�������������ļ�***");
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
	// ����SSL����ļ�
	msg("***��������SSH����ļ�***");
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
	msg("***��������files�ļ���***");
	auto files = getArray("data/files");
	for(auto &file : sshfiles) {
		msg("############################################# " + file);
		for(auto &entry : fs::recursive_directory_iterator(firmdir)) {
			if(entry.path().filename() == file) {
				msg(entry.path().string());
			}
		}
		msg("");
	}
	msg("");

	msg("***�����������ݿ��ļ�***");
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
	msg("***��������shell�ű�***");
	msg("############################################# " "shell�ű�");
	for(auto &entry : fs::recursive_directory_iterator(firmdir)) {
		if(entry.path().extension() == ".sh") {
			msg(entry.path().string());
		}
	}
	msg("");
	msg("***������������.bin�ļ�***");
	msg("############################################# " "����.bin�ļ�");
	for(auto &entry : fs::recursive_directory_iterator(firmdir)) {
		if(entry.path().extension() == ".bin") {
			msg(entry.path().string());
		}
	}
	msg("");
	msg("***���������ļ���ʽ***");
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
	msg("***�����������������***");
	msg("############################################# " "���������");
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
	msg("***����������Ҫ�������ļ�***");
	msg("############################################# " "��Ҫ�������ļ�");
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
	msg("***��������IP��ַ***");
	msg("############################################# IP��ַ");
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
	msg("***��������URL***");
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
	msg("***������������***");
	msg("############################################# " "����");
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
	msg("�������");
	return 0;
}