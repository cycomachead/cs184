// TEST FILE FOR PATCHES

#include <algorithm>
#include "as3.h"
#include "model.h"
#include "parser.h"
#include "patch.h"

vector< vector <vector<glm::vec4> > > patches;
int LOGLEVEL = 10;

int main(int argc, char** argv) {
    string file1 = "test.bez";
    loadPatches(file1);
    cout << "LoadPatches Successful" << endl;
    cout << "PATCHES LENGTH: " << patches.size() << endl;
    cout << "loading Patches" << endl;
    vector <vector<glm::vec4> > item = patches.at(0);
    Patch* p = new Patch(item);
    cout << "Loaded Patches Successful" << endl;
    cout << "PATCH CHILDREN... " << p->hasChildren() << endl;
    cout << "PATCHES CORNERS... " << p->getVerticies().size() << endl;
    cout << "SUBDIVISION LEVEL 1 " << endl;
    p->subdivideQuad();
    cout << "Patch now has children: " << p->hasChildren() << endl;
    vector< vector<glm::vec3> > children = p->getPolygons();
    cout << "POLYGONS LEN:" << children.size() << endl;
    Model* m = new Model(patches, 0.1);
    m->buildAdaptive();
    cout << "Model -- Adaptive Subdivision built" << endl;
    m->subdivideAll();
    cout << "Model subdivided successfully" << endl;
    vector <vector<glm::vec3> > all = m->getAllPolygons();
    cout << "TEST.BEZ TRIANGLES " << all.size() << endl;
}

