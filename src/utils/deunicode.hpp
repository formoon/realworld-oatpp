#pragma once
#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>
#include <iomanip>
#include <type_traits>
#include <codecvt>
#include <cstdint>
#include <unordered_map>
#include <regex>

using namespace std;

string slugify(string sin);
void slugify_init();
void unicode_demo();

