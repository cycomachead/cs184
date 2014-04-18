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
    vector< vector<glm::vec3> > patchset;
    vector<glm::vec3> currentline;
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
            glm::vec3 point = glm::vec3(
                atof(splitline[i].c_str()),
                atof(splitline[i + 1].c_str()),
                atof(splitline[i + 2].c_str()));
            currentline.push_back(point);
        }
        if (currentline.size() > 0) {
            patchset.push_back(currentline);
        }
    }

    if (LOGLEVEL > 6) {
        cout << "PARSING COMPLETE" << endl;
    }
    inpfile.close();
}

void loadObj(string file) {

}

void writeObj(string name, Model* m, string inFile) {
    
    // Get vertices from model 
    vector <vector<glm::vec3>* >* shapes = m->getShapes();
    vector <vector<glm::vec3>* >* normals = m->getNormals();
    // Triangles or quads?
    int vertices = shapes->at(0)->size();
    ofstream output;
    output.open(name, fstream::out);
    output << "# CS184 Assignment 3 Object Output\n";
    output << "# Based on Input: " << inFile << "\n";
    output << "# File: " << name << "\n";
    output << "# Contains " << shapes->size() << " polygons of " << vertices;
    output << " vertices\n\n";
    // OBJ vertices are all numbered sequentially from beginning of the file
    int count = 1;
    for (int i = 0; i < shapes->size(); i += 1) {
        // assumes shapes and normals are always same length
        vector<glm::vec3>* shape = shapes->at(i);
        vector<glm::vec3>* normal = normals->at(i);
        for (int j = 0; j < shape->size(); j += 1) {
            output << "v " << shape->at(j)[0] << " " << shape->at(j)[1] << " ";
            output << shape->at(j)[2] << "\n";
            output << "n " << normal->at(j)[0] << " " << normal->at(j)[1] << " ";
            output << normal->at(j)[2] << "\n";
            count += 1;
        }
        // Write the line to create a "face" syntax is V#//N#
        // when defining only vertices and normals
        int num = count - shapes->size();
        output << "f " << num << "//" << num << " " << num + 1 << "//" << num + 1;
        output << " " << num + 2 << "//" << num + 2;
        if (vertices == 4) { // If we have a quad.
            output << " " << num + 3 << "//" << num + 3;
        }
        output << "\n\n"; // Add line separator after each face.
    }
    output.close();
}
