#include "updates.hpp"
int wmain(int argc,wchar_t** argv){
	update_file file;
	file.readrules(L"C:\\Users\\steve02081504\\Documents\\workstation\\Taromati2_workdirs\\ghost-in-the-shell\\md5buildignore.txt");
	file.read(L"C:\\Users\\steve02081504\\Documents\\workstation\\Taromati2_workdirs\\ghost-in-the-shell\\updates.txt");
	file.update(L"C:\\Users\\steve02081504\\Documents\\workstation\\Taromati2_workdirs\\ghost-in-the-shell\\");
	file.write(L"C:\\Users\\steve02081504\\Documents\\workstation\\Taromati2_workdirs\\ghost-in-the-shell\\updates.txt");
}
