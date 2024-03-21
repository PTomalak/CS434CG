#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <iostream>
#include <vector>
#include <array>
#include <fstream>

int readJSON(std::string filename);
void printJSON();

#endif // JSON_HELPER_H
