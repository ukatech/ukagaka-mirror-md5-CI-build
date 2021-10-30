#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <map>
#include <ctime>
#include <filesystem>
#include <cstdio>
#include "filematch.hpp"
#include "my-gists/windows/MD5maker.hpp"
#include "my-gists/codepage.hpp"
#include "my-gists/STL/to_time_t.hpp"
#include "my-gists/file/fgetstring.h"
using namespace std;
wstring time2str(time_t time){
	wchar_t buf[512];
	if(wcsftime(buf,512,L"%FT%T",gmtime(&time)))
		return buf;
	else
		return L"2012-12-21T00:00:00";
};
struct update_file_info{
	//ghost/master/data/Game/UNO/bReverse.png4dbfeee82e8f6e476b5687c18154a571size=1732date=2021-03-21T14:33:27
	wstring name;
	wstring md5;
	uintmax_t size;
	wstring time;
	explicit update_file_info(wstring str) {
		name=str.substr(0,str.find(L"\1"));
		str=str.substr(str.find(L"\1")+1);
		md5 = str.substr(0, str.find(L"\1"));
		str = str.substr(str.find(L"\1") + 1);
		//size=1732date=2021-03-21T14:33:27
		str=str.substr(5);
		//1732date=2021-03-21T14:33:27
		size = (uintmax_t)wcstoll(str.substr(0,str.find(L"\1")).c_str(), nullptr, 10);
		str = str.substr(str.find(L"\1") + 6);
		//2021-03-21T14:33:27
		time = str.substr(0, str.find(L"\1"));
	}
	explicit update_file_info(filesystem::path file,wstring filename) {
		name = filename;
		md5 = CODEPAGE_n::MultiByteToUnicode(MD5maker.get_file_md5(file.string()),CODEPAGE_n::CP_ACP);
		size = file_size(file);
		time = time2str(to_time_t(last_write_time(file)));
	}
	explicit operator wstring(){
		return name+L"\1"+md5+L"\1"+L"size="+to_wstring(size)+L"\1"+L"date="+time+L"\1";
	}
};
class update_file{
	CODEPAGE_n::CODEPAGE charset;
	map<wstring,update_file_info> path_map;
	DefaultAllMatchFilepathMatcher matcher;
	//map<update_file_info,wstring>md5_map;
public:
	void readrules(filesystem::path file_path) {
		matcher.clear();
		wstring str;
		auto fp=_wfopen(file_path.c_str(), L"r");
		while(fgetstring(str,fp)){
			if(str.size())
				matcher.AddRule(str);
		}
		matcher.reverse();//match->ignore
	}
	void read(filesystem::path file_path){
		path_map.clear();
		charset=CODEPAGE_n::CP_ACP;
		wstring str;
		string nstr;
		auto fp=_wfopen(file_path.c_str(), L"r");
		while(fgetstring(nstr,fp)){
			str=CODEPAGE_n::MultiByteToUnicode(nstr,charset);
			auto key = str.substr(0,str.find(L","));
			str= str.substr(str.find(L",")+1);
			if (key == L"charset") {
				charset = CODEPAGE_n::StringtoCodePage(str.c_str());
			}
			if(key==L"file"){
				update_file_info v(str);
				auto k(v.name);
				//if(filesystem::exists(k))
				path_map.insert_or_assign(k,v);
			}
		}
	};
	void update(filesystem::path file_path){
		for(auto it = path_map.begin(); it != path_map.end();) {
			if(!filesystem::exists(file_path.parent_path()/it->first)){
				it=path_map.erase(it);
			}
			else
				it++;
		}
		matcher.ForDir(file_path,
			[this](filesystem::path file_path,wstring filename){
				filename= filename.substr(1);//"/ghost"->"ghost"
				update_file_info v(file_path, filename);
				filesystem::path k(v.name);
				path_map.insert_or_assign(k,v);
			}
		);
	};
	void write(filesystem::path filepath){
		auto fp = _wfopen(filepath.wstring().c_str(), L"wb");
		if (fp) {
			write(fp);
			fclose(fp);
		}
	};
	void write(FILE* fp){
		fputs(("charset,"+ CODEPAGE_n::UnicodeToMultiByte(CODEPAGE_n::CodePagetoString(charset), charset)+"\r\n").c_str(),fp);
		for(auto&pair:path_map){
			fputs(("file,"+ CODEPAGE_n::UnicodeToMultiByte((wstring)pair.second, charset)+"\r\n").c_str(),fp);
		}
	};
};
