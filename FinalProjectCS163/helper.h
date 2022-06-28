#pragma once

#include <string>
#include <vector>
#include <sstream>

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

using namespace std;

const vector<int> SOURCE_CHARACTERS = { 192, 193, 194, 195, 200, 201, 202, 204, 
205, 210, 211, 212, 213, 217, 218, 221, 224, 225, 226, 227, 232, 
233, 234, 236, 237, 242, 243, 244, 245, 249, 250, 253, 258, 259, 
272, 273, 296, 297, 360, 361, 416, 417, 431, 432, 7840, 7841, 7842, 
7843, 7844, 7845, 7846, 7847, 7848, 7849, 7850, 7851, 7852, 7853, 7854, 
7855, 7856, 7857, 7858, 7859, 7860, 7861, 7862, 7863, 7864, 7865, 7866, 
7867, 7868, 7869, 7870, 7871, 7872, 7873, 7874, 7875, 7876, 7877, 7878, 
7879, 7880, 7881, 7882, 7883, 7884, 7885, 7886, 7887, 7888, 7889, 7890, 
7891, 7892, 7893, 7894, 7895, 7896, 7897, 7898, 7899, 7900, 7901, 7902, 
7903, 7904, 7905, 7906, 7907, 7908, 7909, 7910, 7911, 7912, 7913, 7914, 
7915, 7916, 7917, 7918, 7919, 7920, 7921 };

const vector<char> DESTINATION_CHARACTERS = {'A', 'A', 'A', 'A', 'E',
        'E', 'E', 'I', 'I', 'O', 'O', 'O', 'O', 'U', 'U', 'Y', 'a', 'a',
        'a', 'a', 'e', 'e', 'e', 'i', 'i', 'o', 'o', 'o', 'o', 'u', 'u',
        'y', 'A', 'a', 'D', 'd', 'I', 'i', 'U', 'u', 'O', 'o', 'U', 'u',
        'A', 'a', 'A', 'a', 'A', 'a', 'A', 'a', 'A', 'a', 'A', 'a', 'A',
        'a', 'A', 'a', 'A', 'a', 'A', 'a', 'A', 'a', 'A', 'a', 'E', 'e',
        'E', 'e', 'E', 'e', 'E', 'e', 'E', 'e', 'E', 'e', 'E', 'e', 'E',
        'e', 'I', 'i', 'I', 'i', 'O', 'o', 'O', 'o', 'O', 'o', 'O', 'o',
        'O', 'o', 'O', 'o', 'O', 'o', 'O', 'o', 'O', 'o', 'O', 'o', 'O',
        'o', 'O', 'o', 'U', 'u', 'U', 'u', 'U', 'u', 'U', 'u', 'U', 'u',
        'U', 'u', 'U', 'u', };


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
bool turnNonUnicodeString(tgui::String& s);