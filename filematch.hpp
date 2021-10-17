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
	void AddRule(wstring rule);
	bool IsMatch(wstring s);
	bool operator()(wstring s){
		return IsMatch(s);
	}
	//ForDir
private:
	static filesystem::path base_path;
	self_t&& GetCopySelfFor(wstring dirlevel);
	void ForDir_mapper(filesystem::path path, function<void(filesystem::path)>do_what);
public:
	void ForDir(filesystem::path Dir, function<void(filesystem::path)>do_what);
};
typedef filepathMatcher_t<true> DefaultAllMatchFilepathMatcher;
typedef filepathMatcher_t<false> DefaultNoneMatchFilepathMatcher;
