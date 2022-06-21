#pragma once

#include <string>
#include <vector>

using namespace std;

vector<string> splitString(const string &s);
bool checkPrintable(const char c);
bool checkContainStrings(const vector<string> &s, const vector<string> &t);
bool checkContainStringsAsSubsequence(const vector<string>& s, const vector<string>& t);