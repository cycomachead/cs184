//

#ifndef PARSER_H
#define PARSER_H
#include "as3.h"
#include "models.h"
#include <string.h>

void loadPatches(string file);
void loadobj(string file);

vector<string> loadScene(string file);
void writeObj(string, Model*, string);
#endif