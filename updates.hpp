#include <string>
#include <unordered_map>
#include <ctime>
#include <filesystem>
#include <cstdio>
#include "my-gists/codepage.hpp"
#include "my-gists/file/fgetstring.h"
#include "filematch.hpp"
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
	size_t size;
	wstring time;
	explicit update_file_info(wstring str) {
		name=str.substr(0,str.find(L"\1"));
		str=str.substr(str.find(L"\1")+1);
		md5 = str.substr(0, str.find(L"\1"));
		str = str.substr(str.find(L"\1") + 1);
		//size=1732date=2021-03-21T14:33:27
		str=str.substr(5,str.size()-1);
		//1732date=2021-03-21T14:33:27
		size = (size_t)wcstoll(str.substr(0,str.find(L"\1")).c_str(), nullptr, 10);
		str = str.substr(str.find(L"\1") + 6);
		//2021-03-21T14:33:27
		time = str;
	}
	explicit operator wstring(){
		return name+L"\1"+md5+L"\1"+L"size="+to_wstring(size)+L"\1"+L"date="+time+L"\1";
	}
};
class update_file{
	CODEPAGE_n::CODEPAGE charset;
	unordered_map<filesystem::path,update_file_info> path_map;
	DefaultAllMatchFilepathMatcher matcher;
	//map<update_file_info,wstring>md5_map;
	void readrules(filesystem::path file_path) {
		wstring str;
		auto fp=_wfopen(file_path.c_str(), L"r");
		while(fgetstring(str,fp)){
			if(str.size())
				matcher.AddRule(str);
		}
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
				filesystem::path k(v.name);
				//if(filesystem::exists(k))
				path_map.insert_or_assign(k,v);
			}
		}
	};
	void update(filesystem::path file_path){
		for(auto it = path_map.begin(); it != path_map.end(); ) {
			if(filesystem::exists(it->first)){
				path_map.erase(it);
			}
		}
		matcher.ForDir(file_path,
			lambda(filesystem::path file_path){
				update_file_info v(file_path);
				filesystem::path k(v.name);
				path_map.insert_or_assign(k,v);
			}
		);
	};
	void write(FILE* fp){
		fputws((L"charset,"+ CODEPAGE_n::CodePagetoString(charset)+L"\r\n").c_str(),fp);
		for(auto&pair:path_map){
			fputws((L"file,"+(wstring)pair.second+L"\r\n").c_str(),fp);
		}
	};
};
