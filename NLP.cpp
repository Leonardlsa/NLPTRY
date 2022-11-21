#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <valarray>

using namespace std;
typedef pair<int, int> PII;
const string PATH = "D:\\chinese_words.txt";

const int M1 = 1e9 + 7;
const int M2 = 1e9 + 9;
const int B = 0x9FCF;

struct word {
    string words;
    PII w;
    int length;

    inline bool operator<(const word &other) const {
        if (length == other.length)return w < other.w;
        else return length < other.length;
    }

    inline bool operator>(const word &other) const {
        if (length == other.length)return w > other.w;
        else return length > other.length;
    }
};

PII get_hash(const string &s) {
    PII res = {0, 0};
    for (int i = 0; i < s.size(); ++i) {
        res.first = (long long) (res.first * B + s[i]) % M1;
    }
    for (int i = 0; i < s.size(); ++i) {
        res.second = (long long) (res.second * B + s[i]) % M2;
    }
    return res;
}

inline bool cmp(PII a, PII b) {
    return a.first == b.first && a.second == b.second;
}

void initialize(bool foreward, const string &s, int depth, vector<PII> &result) {
    result.clear();
    string t;
    if (foreward) {//-->
        t = s[0];
        result.push_back(get_hash(t));

        for (int i = 1; i < min(depth, (int) s.length()); i++) {
            t += s[i];
            result.push_back(get_hash(t));

        }
    } else {
        depth = min(depth, (int) s.length());
        int start = s.length() - depth;
        for (int i = 1; i <= min(depth, (int) s.length()); i++) {
            t = s.substr(s.length() - i, s.length());
            result.push_back(get_hash(t));

        }
    }
}

int main() {
    set<word> wordDatabase;
    string temp;
    ifstream fin;
    fin.open(PATH);

    if (!fin.is_open())cout << "您他们放什么呢" << endl;
    while (fin >> temp) {
        wordDatabase.insert(word{temp, get_hash(temp), (int) temp.size()});
        fin >> temp >> temp;
    }

    string match, mm;
    cout<<"请输入需分析语句："<<endl;
    cin >> match;
    mm = match;//save
    vector<PII> hashmatch;
    vector<string> ans, re_ans;

    int maxlen = wordDatabase.rbegin()->length;

    loop:
    hashmatch.clear();
    initialize(true, match, maxlen, hashmatch);
    for (auto wo = wordDatabase.rbegin(); wo != wordDatabase.rend(); ++wo) {
        if (wo->length <= hashmatch.size() && cmp(wo->w, hashmatch[wo->length - 1])) {
            ans.push_back(wo->words);
//            cout << wo->words<<' ';
            match.erase(0, wo->length);
            if (match == "")break;
            else goto loop;
        }
    }
//    cout << endl << "---------------------" << endl;


    match = mm;//load
    loop1:
    hashmatch.clear();

    initialize(false, match, maxlen, hashmatch);
    for (auto wo = wordDatabase.rbegin(); wo != wordDatabase.rend(); ++wo) {
        if (wo->length <= hashmatch.size() && cmp(wo->w, hashmatch[wo->length - 1])) {
            re_ans.push_back(wo->words);
//            cout << wo->words << ' ';
            match.erase(match.length() - wo->length, match.length());
            if (match == "")break;
            goto loop1;
        }
    }
//    cout << endl;
    bool ambigious = false;
    if (ans.size() == re_ans.size()) {
        for (int i = 0; i < ans.size(); ++i) {
            if (ans[i] != re_ans[ans.size() - 1 - i])ambigious = true;
        }
    } else ambigious = true;
    if (!ambigious) {
        cout << "原文分析成功！" << endl;
        for (auto i: ans) {
            cout << i << ' ';
        }
    } else {
        cout << "原文有二义性，需进一步分析。" << endl;
        cout << "正序下：";
        for (auto i: ans)cout << i << ' ';
        cout << endl;
        cout << "逆序下：";
        for (auto i = re_ans.rbegin(); i != re_ans.rend(); i++)cout << *i << ' ';
    }
    cout << endl;
    return 0;
}