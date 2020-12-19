#pragma once
#include <string>
#include <vector>
#include <Arduino.h>

using namespace std;

vector<string> split(const string &s, char delim);

template <typename T>
void parseInt(const string& s, T& target)
{
    String temp = String(s.c_str());
    target = temp.toInt();
}
