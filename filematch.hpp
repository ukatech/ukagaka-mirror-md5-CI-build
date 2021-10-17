#include <string>
#include <vector>
#include <filesystem>
#include <functional>

template<bool default_as_all_match=true>
class filepathMatcher_t{
	using namespace std;
	typedef filepathMatcher_t<default_as_all_match> self_t;
	vector<wstring> MatchingRules;//*.ai
	vector<wstring> NotMatchingRules;//!*.inf

	wstring& PreprocessRule(wstring& rule);
	void AddMatchingRule(wstring rule);
	void AddNotMatchingRule(wstring rule);
public:
	void clear(){
		MatchingRules.clear();
		NotMatchingRules.clear();
	}
	void AddRule(wstring rule);
	bool IsMatch(wstring s);
	bool operator()(wstring s){
		return IsMatch(s);
	}
	void reverse() {
		swap(MatchingRules, NotMatchingRules);
	}
	//ForDir
private:
	static filesystem::path base_path;
	static wstring path_base_on_base_path;
	self_t&& GetCopySelfFor(wstring dirlevel);
	void ForDir_mapper(filesystem::path path, function<void(filesystem::path)>do_what);
	void ForDir_mapper(filesystem::path path, function<void(filesystem::path,wstring)>do_what);
public:
	void ForDir(filesystem::path Dir, function<void(filesystem::path)>do_what);
	void ForDir(filesystem::path Dir, function<void(filesystem::path,wstring)>do_what);
};
typedef filepathMatcher_t<true> DefaultAllMatchFilepathMatcher;
typedef filepathMatcher_t<false> DefaultNoneMatchFilepathMatcher;
