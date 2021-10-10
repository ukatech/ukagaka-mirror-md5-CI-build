
bool match(const char* pattern, const char *str)
{
	if (*pattern == '\0' && *str == '\0')
		return true;
	if (*srcstr == '\0')
	{
		while (*pattern == '*')
			pattern++;
		return (*pattern == '\0');
	}

	if (*pattern == '?')
	{
		return match(pattern + 1, str + 1);
	}
	else if (*pattern == '*')
	{
		return match(pattern + 1, str) || match(pattern + 1, str + 1) || match(pattern, str + 1);
	}
	else if (*pattern == *str)
	{
		return match(pattern + 1, str + 1);
	}

	return false;
}

bool isMatch(string s, string p) {
	auto allStars = [](const string& str, int left, int right) {
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

	int sIndex = 0, pIndex = 0;
	int sRecord = -1, pRecord = -1;
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

