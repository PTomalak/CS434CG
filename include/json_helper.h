#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <tuple>

int readJSON(std::string filename);
void printJSON();

#endif // JSON_HELPER_H
