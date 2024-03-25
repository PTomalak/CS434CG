#ifndef JSON_HELPER_H
#define JSON_HELPER_H



#ifdef __APPLE__
#include "/opt/homebrew/Cellar/rapidjson/1.1.0/include/rapidjson/rapidjson.h"
#include "/opt/homebrew/Cellar/rapidjson/1.1.0/include/rapidjson/document.h"
#else
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#endif

#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <tuple>

int readJSON(std::string filename);
void printJSON();

#endif // JSON_HELPER_H
