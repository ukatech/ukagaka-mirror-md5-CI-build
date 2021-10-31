#include "my-gists/ukagaka/updates.hpp"
#include "my-gists/file/dir_enum.h"
using namespace std;
int wmain(int argc,wchar_t** argv){
	wstring mode,path;
	if (argc == 3) {
		mode=argv[1];
		path=argv[2];
	}
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
			CDirEnum ef(path);
			CDirEnumEntry entry;
			while(ef.next(entry)){
				if(entry.isdir){
					file.read(path+L"/"+entry.name+L"/updates.txt");
					file.update(path+L"/"+entry.name+L"/");
					file.write(path+L"/"+entry.name+L"/updates.txt");
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
