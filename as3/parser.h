//

#ifndef PARSER_H
#define PARSER_H
#include "as3.h"
#include "models.h"

void loadPatches(string file);
void loadobj(string file);

void writeObj(string, Model*, string);
#endif