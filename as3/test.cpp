// TEST FILE FOR PATCHES

#include <algorithm>
#include "as3.h"
#include "parser.h"
#include "patch.h"

vector< vector <vector<glm::vec4> > > patches;

int main(int argc, char** argv) {
    string file1 = "test.bez";

    loadPatches(file1);
    cout << "LoadPatches Successful" << endl;
    cout << "loading Patches" << endl;
    Patch p = Patch(patches);
    cout << "Loaded Patches Succd"
}