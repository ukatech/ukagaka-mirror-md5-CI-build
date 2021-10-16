#include <string>
#include <map>
#include <ctime>
#include <filesystem>
#include "my-gists/codepage.hpp"
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
	map<update_file_info, filesystem::path> path_map;
	//map<update_file_info,wstring>md5_map;
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
			if(key==L"file")
				update_file_info v=update_file_info(str);
				filesystem::path k=v.name;
				if(exists(k))
					path_map.insert_or_assign(k,v);
			}
		}
	};
	void update(filesystem::path file_path){
		path_map.foreach(
			mapper(update_file_info){
				if file_not_exit{
					path_map.remove(update_file_info)
				}
			}
		)
		file_path.foreach_file(
			mapper(file_path){
				if(file_path !_in_ path_map){
					path_map.add(file_path);
				}
				else{
					if(path_map[file_path].time==file_path.last_write_time)
						return;
					else{
						path_map[file_path]=file_path;
					}
				}
			}
		)
	};
	write(Fp){
		write(fp,"charset,"+charset.to_string+"\r\n");
		path_map.for_each(
			mapper(update_file_info){
				write(fp,"file,"+(wstring)update_file_info+"\r\n");
			}
		)
	};
};
