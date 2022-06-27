#pragma once

#include <string>
#include <vector>
#include <sstream>

using namespace std;

int getRandomNumberFromList(const vector<int> &numbers);
vector<string> splitString(const string &s);
vector<string> splitString(const string &text, const string &seperate);
vector<string> splitString(const string& text, const string& seperate, int maximum);
string replaceString(const string& text, const string& oldSubstring, const string &newSubstring);
string replaceString(const string& text, const string& oldSubstring, const string& newSubstring, int maximum);
bool checkPrintable(const char c);
bool checkContainStrings(const vector<string> &s, const vector<string> &t);
bool checkContainStringsAsSubsequence(const vector<string>& s, const vector<string>& t);
bool checkContainStringsAsSubarray(const vector<string> &s, const vector<string> &t);
string toLowerString(string s);