#include <string>
#include <vector>
#include <filesystem>
#include <functional>
#include "my-gists/STL/replace_all.hpp"
using namespace std;

inline bool IsMatchRule(wstring s, wstring p) {
	auto allStars = [](const wstring& str, int left, int right) {
		for (int i = left; i < right; ++i) {
			if (str[i] != L'*') {
				return false;
			}
		}
		return true;
	};
	auto charMatch = [](wchar_t u, wchar_t v) {
		return u == v || v == L'?';
	};

	while (s.size() && p.size() && p.back() != L'*') {
		if (charMatch(s.back(), p.back())) {
			s.pop_back();
			p.pop_back();
		}
		else {
			return false;
		}
	}
	if (p.empty()) {
		return s.empty();
	}

	size_t sIndex = 0, pIndex = 0;
	long long int sRecord = -1, pRecord = -1;
	while (sIndex < s.size() && pIndex < p.size()) {
		if (p[pIndex] == L'*') {
			++pIndex;
			sRecord = sIndex;
			pRecord = pIndex;
		}
		else if (charMatch(s[sIndex], p[pIndex])) {
			++sIndex;
			++pIndex;
		}
		else if (sRecord != -1 && sRecord + 1 < s.size()) {
			++sRecord;
			sIndex = (size_t)sRecord;
			pIndex = (size_t)pRecord;
		}
		else {
			return false;
		}
	}
	return allStars(p, pIndex, p.size());
}
inline bool IsMatchRules(const wstring&s, const vector<wstring>&rules){
	for(auto&rule:rules){
		if(IsMatchRule(s,rule))
			return 1;
	}
	return 0;
}

template<bool default_as_all_match=true>
class filepathMatcher_t{
	typedef filepathMatcher_t<default_as_all_match> self_t;
	vector<wstring> MatchingRules;//*.ai
	vector<wstring> NotMatchingRules;//!*.inf

	wstring PreprocessRule(wstring rule){
		replace_all(rule,L"\\",L"/");
		if(rule[0]!=L'/')
			rule=L"*/"+rule;
		else
			rule=rule.substr(1);
		if(rule[rule.size()-1]==L'/')
			rule=rule.substr(0,rule.size()-1);
		return rule;
	}
	void AddMatchingRule(wstring rule){
		MatchingRules.push_back(rule);
	}
	void AddNotMatchingRule(wstring rule){
		NotMatchingRules.push_back(rule);
	}
public:
	void AddRule(wstring rule){
		if(rule[0]=='!')
			AddNotMatchingRule(PreprocessRule(rule.substr(1)));
		else
			AddMatchingRule(PreprocessRule(rule));
	}
	bool IsMatch(wstring s){
		if constexpr(default_as_all_match){
			if(IsMatchRules(s,MatchingRules))
				return true;
			if(IsMatchRules(s,NotMatchingRules))
				return false;
		}
		else{
			if(IsMatchRules(s,NotMatchingRules))
				return false;
			if(IsMatchRules(s,MatchingRules))
				return true;
		}
		return default_as_all_match;
	}
	bool operator()(wstring s){
		return IsMatch(s);
	}
	//ForDir
private:
	static inline filesystem::path base_path{};
	static inline wstring path_base_on_base_path{};
	self_t GetCopySelfFor(wstring dirlevel){
		self_t aret;
		for(auto&rule:MatchingRules){
			if(IsMatchRule(dirlevel,rule.substr(0,rule.find(L"/"))))
				aret.AddMatchingRule(rule.substr(rule.find(L"/")+1));
		}
		for(auto&rule:NotMatchingRules){
			if(IsMatchRule(dirlevel,rule.substr(0,rule.find(L"/"))))
				aret.AddNotMatchingRule(rule.substr(rule.find(L"/")+1));
		}
		return aret;
	}
	void ForDir_mapper(filesystem::path path,function<void(filesystem::path)>do_what){
		if(IsMatch((base_path/path.filename()).wstring()))
			if(is_directory(path))
				for(auto& enty : filesystem::directory_iterator(path)){
					base_path = base_path/filesystem::path(enty).filename();
					GetCopySelfFor(filesystem::path(enty).filename()).ForDir_mapper(enty,do_what);
					base_path = base_path.parent_path();
				}
			else{
				do_what(path);
			}
	}
	void ForDir_mapper(filesystem::path path,function<void(filesystem::path,wstring)>do_what){
		if(IsMatch((base_path/path.filename()).wstring()))
			if(is_directory(path)){
				auto path_base_on_base_path_bak= path_base_on_base_path;
				path_base_on_base_path += L"/" + path.filename().wstring();
				for(auto& enty : filesystem::directory_iterator(path)){
					base_path = base_path/filesystem::path(enty).filename();
					GetCopySelfFor(filesystem::path(enty).filename()).ForDir_mapper(enty,do_what);
					base_path = base_path.parent_path();
				}
				path_base_on_base_path = path_base_on_base_path_bak;
			}
			else{
				do_what(path, path_base_on_base_path + L"/" + path.filename().wstring());
			}
	}
public:
	void ForDir(filesystem::path Dir,function<void(filesystem::path)>do_what){
		base_path = Dir.parent_path();
		ForDir_mapper(Dir,do_what);
	}
	void ForDir(filesystem::path Dir, function<void(filesystem::path,wstring)>do_what) {
		path_base_on_base_path = L"";
		base_path = Dir.parent_path();
		ForDir_mapper(Dir, do_what);
	}
};
template class filepathMatcher_t<true>;
template class filepathMatcher_t<false>;
