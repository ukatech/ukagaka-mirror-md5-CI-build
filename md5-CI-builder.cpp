#include "my-gists/ukagaka/updates.hpp"
using namespace std;
int wmain(int argc,wchar_t** argv){
	wstring mode=argv[1];
	wstring path=argv[2];
	if(mode==L"ghost_dir"){
		{
			updatefile_n::update_file file;
			file.readrules(path+L"/md5buildignore.txt");
			file.read(path+L"/updates.txt");
			file.update(path+L"/");
			file.write(path+L"/updates.txt");
			file.write(path+L"/ghost/master/updates.txt");
		}
		{
			path=path+L"/shell";
			updatefile_n::update_file file;
			file.readrules(path+L"/md5buildignoreforeach.txt");
			for(auto& enty : filesystem::directory_iterator(path)){
				if(is_directory(enty)){
					file.read(enty.path()/L"updates.txt");
					file.update(enty.path()/L"");
					file.write(enty.path()/L"updates.txt");
				}
			}
		}
	}
	else if(mode==L"other"){
		updatefile_n::update_file file;
		file.readrules(path+L"/md5buildignore.txt");
		file.read(path+L"/updates.txt");
		file.update(path);
		file.write(path+L"/updates.txt");
	}
}
