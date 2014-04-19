// Parser for CS184 AS3.
// Parses .bez and .obj files

#include "parser.h"

// int LOGLEVEL;

/** A function that splits STRING, by a DELIMiter, and stores it
    in ELEM. Returns elem, which is a vector containing the split
    string. Disclaimer found this method on stack overflow. **/
vector<string> &split(const string &s, char delim,
    vector<string> &elems) {
    stringstream sstream(s);
    string item;
    while (getline(sstream, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

/** Same as the following but a function that returns the a new Vector.
    **/
vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

// Split a string using C++ strings, and a multichar delimiter.
// This also had help from stack overflow...
vector<string> splitMulti(string in, string delim) {
    vector<string> result;
    size_t pos = 0;
    string token;
    while ((pos = in.find(delim)) != string::npos) {
        token = in.substr(0, pos);
        result.push_back(token);
        in.erase(0, pos + delim.length());
    }
    result.push_back(token);
    return result;
}

void parseInputLine(vector<string> line) {
    if (!line.at(0).compare("vertex") || !line.at(0).compare("v")) {
        float x = atof(line.at(1).c_str());
        float y = atof(line.at(2).c_str());
        float z = atof(line.at(3).c_str());
        glm::vec3 vertex(x, y, z);
        vertices.push_back(vertex);
    } else if (!line.at(0).compare("vn") || !line.at(0).compare("n")) {
        float x = atof(line.at(1).c_str());
        float y = atof(line.at(2).c_str());
        float z = atof(line.at(3).c_str());
        glm::vec3 normal(x, y, z);
        normals.push_back(normal);
    } else if (!line.at(0).compare("f")) {
        noNormal = false;
        if (line.at(1).find("//") != string::npos) {
            vector<glm::vec3> tri;
            vector<glm::vec3> normal;
            for (int i = 1; i < line.size(); i += 1) {
                vector<string> sv1 = splitMulti(line.at(i), "//");
                glm::vec3 v1 = vertices.at(atoi(sv1.at(0).c_str()) - 1);
                glm::vec3 n1 = normals.at(atoi(sv1.at(1).c_str()) - 1);
                normal.push_back(n1);
                tri.push_back(v1);
            }
            shapes.push_back(tri);
            shapeNormals.push_back(normal);
        } else if (line.at(1).find("/") != string::npos) {
            vector<string> sv1 = split(line.at(1), '/');
            vector<string> sv2 = split(line.at(2), '/');
            vector<string> sv3 = split(line.at(3), '/');
            glm::vec3 v1 = vertices.at(atoi(sv1.at(0).c_str()) - 1);
            glm::vec3 n1 = normals.at(atoi(sv1.at(2).c_str()) - 1);
            glm::vec3 v2 = vertices.at(atoi(sv2.at(0).c_str()) - 1);
            glm::vec3 n2 = normals.at(atoi(sv2.at(2).c_str()) - 1);
            glm::vec3 v3 = vertices.at(atoi(sv3.at(0).c_str()) - 1);
            glm::vec3 n3 = normals.at(atoi(sv3.at(2).c_str()) - 1);
            vector<glm::vec3> tri;
            tri.push_back(v1);
            tri.push_back(v2);
            tri.push_back(v3);
            vector<glm::vec3> normal;
            normal.push_back(n1);
            normal.push_back(n2);
            normal.push_back(n3);

            shapes.push_back(tri);
            shapeNormals.push_back(normal);
        } else {
            noNormal = true;
            int len = line.size();
            vector<glm::vec3> poly;
            glm::vec3 v1 = vertices.at(atoi(line.at(1).c_str())-1);
            glm::vec3 v2 = vertices.at(atoi(line.at(2).c_str())-1);
            glm::vec3 v3 = vertices.at(atoi(line.at(3).c_str())-1);

            poly.push_back(v1);
            poly.push_back(v2);
            poly.push_back(v3);

            if (len == 5) {
                glm::vec3 v4 = vertices.at(atoi(line.at(4).c_str())-1);
                poly.push_back(v4);
            }

            shapes.push_back(poly);
        }
    }
}

void loadobj(string file) {
    ifstream obj(file.c_str());
    if (!obj.is_open()) {
        cout << "Unable to open file " << file << endl;
        cerr << "EXITING" << endl;
        exit(1);
    }

    string line;
    vector<string> parsedline;
    while (getline(obj, line)) {
        if (line.compare("")) {
            parsedline = split(line, ' ');
            if (parsedline.at(0).compare("#")) {
               parseInputLine(parsedline);
            }
        }
    }
}

vector<string> loadScene(string file) {
    ifstream obj(file.c_str());
    if (!obj.is_open()) {
        cout << "Unable to open file " << file << endl;
        cerr << "EXITING" << endl;
        exit(1);
    }

    string line;
    vector<string> result;
    while (getline(obj, line)) {
        if (line.compare("")) {
            result.push_back(line);
        }
    }
    return result;
}

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

void writeObj(string name, Model* m, string inFile) {

    // Get vertices from model
    vector <vector<glm::vec3>* >* shapes = m->getShapes();
    vector <vector<glm::vec3>* >* normals = m->getNormals();
    // Triangles or quads?
    int vertices = shapes->at(0)->size();
    ofstream output;
    output.open(name.c_str(), fstream::out);
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
            output << "vn " << normal->at(j)[0] << " " << normal->at(j)[1] << " ";
            output << normal->at(j)[2] << "\n";
            count += 1;
        }
        // Write the line to create a "face" syntax is V#//N#
        // when defining only vertices and normals
        int num = count - vertices;
        output << "f " << num << "//" << num << " " << num + 1 << "//" << num + 1;
        output << " " << num + 2 << "//" << num + 2;
        if (vertices == 4) { // If we have a quad.
            output << " " << num + 3 << "//" << num + 3;
        }
        output << "\n\n"; // Add line separator after each face.
    }
    output.close();
}
