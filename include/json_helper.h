#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include "../rapidjson/include/rapidjson/rapidjson.h"
#include "../rapidjson/include/rapidjson/document.h"
#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <tuple>

int readJSON(std::string filename);
void printJSON();

#endif // JSON_HELPER_H
