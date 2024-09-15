#pragma once
#include <string>
#include <vector>
#include <Arduino.h>

std::vector<std::string> split(const std::string &s, char delim);

template <typename T>
void parseInt(const std::string& s, T& target)
{
    String temp = String(s.c_str());
    target = temp.toInt();
}
