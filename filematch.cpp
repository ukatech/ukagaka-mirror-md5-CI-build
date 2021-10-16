bool IsMatchRule(wstring s, wstring p) {
	auto allStars = [](const wstring& str, int left, int right) {
		for (int i = left; i < right; ++i) {
			if (str[i] != '*') {
				return false;
			}
		}
		return true;
	};
	auto charMatch = [](char u, char v) {
		return u == v || v == '?';
	};

	while (s.size() && p.size() && p.back() != '*') {
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
		if (p[pIndex] == '*') {
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
			sIndex = sRecord;
			pIndex = pRecord;
		}
		else {
			return false;
		}
	}
	return allStars(p, pIndex, p.size());
}
bool IsMatchRules(const wstring&s, const vector<wstring>&rules){
	for(auto&rule:rules){
		if(IsMatchRule(s,rule))
			return 1;
	}
	return 0;
}

class filepathMatcher{
	vector<wstring> MatchingRules//*.ai
	vector<wstring> NotMatchingRules//!*.inf

public:
	void AddMatchingRule(wstring rule){
		MatchingRules.push_back(rule);
	}
	void AddNotMatchingRule(wstring rule){
		NotMatchingRules.push_back(rule);
	}
	void AddRule(wstring rule){
		if(rule[0]=='!')
			AddNotMatchingRule(rule.substr(1));
		else
			AddMatchingRule(rule);
	}
	bool IsMatch(wstring s){
		if(IsMatchRules(s,NotMatchingRules))
			return false;
		if(IsMatchRules(s,MatchingRules))
			return true;
		return false;
	}
	bool operator()(wstring s){
		return IsMatch(s);
	}
};
