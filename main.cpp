#include <set>
#include <map>
#include <string>
#include <cctype>
#include <utility>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iterator>

using namespace std;

class FindWordInText
{
public:
    FindWordInText() : count(1){};
    explicit FindWordInText(string str) : file_road(std::move(str)), count(1)
    {
        ReadTxt(file_road);
    }

    //按照路径读取文本
    bool ReadTxt(string);
    //将一个单词处理成标准小写单词同时去除连接的标点符号如, . ;等
    static string WordTransform(string);
    //查询单词是否出现过，如果出现过则按格式打印行号和原文
    bool FindWord(const string &str);
    bool FindWordCaseSensitive(const string &str);

private:
    //清空缓存
    void clear();
    //按格式打印
    void Print(const int &size, const string &word, map<string, set<int>>::iterator &it);
    //文本的总行数行数
    int count;
    //文件路径
    string file_road;
    ifstream file_stream;
    //按行保留原文
    vector<string> raw_txt;
    //记录每个单词出现的行号
    map<string, set<int>> word_map;

};

void FindWordInText::clear()
{
    count = 1;
    file_road.clear();
    raw_txt.clear();
    word_map.clear();
    file_stream.close();
}

bool FindWordInText::ReadTxt(string str)
{
    clear();
    file_road = std::move(str);
    file_stream.open(file_road);
    if (!file_stream.is_open())
    {
        cout << "Failed to open file!" << endl;
        return false;
    }

    string raw_str;
    while (getline(file_stream, raw_str))
    {
        raw_txt.push_back(raw_str);
        istringstream str_stream(raw_str);
        string word;
        while (str_stream >> word)
        {
            //处理标准的word
            word = WordTransform(word);
            word_map[word].insert(count);
        }
        count++;
    }
    return true;
}

string FindWordInText::WordTransform(string word)
{
    for (auto it = word.begin(); it != word.end(); it++)
    {
        if (!isalpha(*it))
        {
            it = word.erase(it);
            if (it == word.end())
                return word;
            continue;
        }
        else
            *it = tolower(*it); // 大小写转换
    }
    return word;
}

bool FindWordInText::FindWord(const string &str)
{
    string word = WordTransform(str);
    auto it = word_map.find(word);
    if (it == word_map.end())
    {
        cout << "Can't find the word " + word << endl;
        cout << "------------------------------------------------------------------------------" << endl;
        return false;
    }
    //统计次数
    int size = it->second.size();
    Print(size, word, it);

    return true;
}

bool FindWordInText::FindWordCaseSensitive(const string &str)
{

}

void FindWordInText::Print(const int &size, const string &word, map<string, set<int>>::iterator &it_map)
{
    cout << word << " occurs " << size << " times " << endl;
    for (int it : it_map->second)
    {
        cout << "\t(line " << it << ")" << raw_txt[it - 1] << endl;
    }
//    for (auto it = it_map->second.begin(); it != it_map->second.end(); it++)
//    {
//        cout << "\t(line " << *it << ")" << raw_txt[(*it) - 1] << endl;
//    }
    cout << "------------------------------------------------------------------------------" << endl;
}

int main()
{
    FindWordInText t1("//home//r1turn0//WorkSpace//C++//FindWordInText//EnglishText.txt");
    t1.FindWord("only");
    // "a `to` b

    return 0;
}
