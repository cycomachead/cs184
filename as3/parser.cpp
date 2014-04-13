// Parser for CS184 AS3.
// Parses .bez and .obj files

#include "parser.h"

// int LOGLEVEL;

void loadPatches(string file) {

    ifstream inpfile(file.c_str());
    if(!inpfile.is_open()) {
        cout << "Unable to open file" << endl;
        cerr << "EXITING" << endl;
        exit(1);
    }

    string line;
    int numPatches;
    int linecount = 0;
    vector< vector<glm::vec4> > patchset;
    vector<glm::vec4> currentline;
    while(inpfile.good()) {
        linecount += 1;
        vector<string> splitline;
        string buf;

        getline(inpfile, line);
        stringstream ss(line);

        while (ss >> buf) {
            splitline.push_back(buf);
        }

        // blank lines separate patchsets
        if (splitline.size() == 0 and patchset.size() != 0) {
            if (LOGLEVEL > 3) {
                cout << "APPENDING GROUP OF SIZE: " << patchset.size() << endl;
            }
            patches.push_back(patchset);
            patchset.clear();
            continue;
        }

        if (linecount == 1) {
            numPatches = atoi(splitline[0].c_str());
            if (LOGLEVEL > 3) {
                cout << "NUM PATCHES: " << numPatches << endl;
            }
            continue;
        }

        currentline.clear();
        if (LOGLEVEL > 4) {
            cout << "LINE HAS " << splitline.size() << " ITEMS" << endl;
        }
        for(int i = 0; i < splitline.size(); i += 3) {
            glm::vec4 point = glm::vec4(
                atof(splitline[i].c_str()),
                atof(splitline[i + 1].c_str()),
                atof(splitline[i + 2].c_str()),
                1);
            currentline.push_back(point);
        }
        if (currentline.size() > 0) {
            patchset.push_back(currentline);
        }
    }

    inpfile.close();
}

void loadObj(string file) {

}
