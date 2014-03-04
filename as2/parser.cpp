// Parser for CS184 OBJ file format.
// Follows Specs from here:
// http://inst.eecs.berkeley.edu/~cs184/fa12/assignments/hw5.pdf
// inital design ideas from here:
// http://cs.dvc.edu/HowTo_Cparse.html

// Basic Includes
#include <iostream>
#include <fstream>
//#include <cstring>
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
// #include <cstdio>


// Libraries
// #undef Success
// #include "lib/Eigen/Eigen"
// #include "raytracer.cpp"

using namespace std;

class Parser {
public:
    string data;

    string comment = "#";

    vector<tuple<string, int>> commands = {
        {"size", 2},
        {"maxdepth", 1},
        {"output", 1},
        {"camera", 3},
        {"sphere", 4},
        {"maxverts", 1},
        {"maxvertnorms", 1},
        {"vertex", 3},
        {"vertexnormal", 6},
        {"tri", 3},
        {"trinormal", 3},
        {"translate", 3},
        {"rotate", 4},
        {"scale", 3},
        {"pushTransform", 0},
        {"popTransform", 0},
        {"direction", 6},
        {"point", 6},
        {"attenuation", 3},
        {"ambient", 3},
        {"diffuse", 3},
        {"specular", 3},
        {"shininess", 1},
        {"emission", 3}
    };


    void parse() {
        cout << comment << endl;

        ifstream file;

        file.open(data);

        string line;
        while(getline(file, line)) {
            cout << line << endl;
        }

        file.clear();
        file.close();
    }

    Parser(string s);
    Parser();
};

Parser::Parser(string s) {
    data = s;
}

Parser::Parser() { }

int main(int argc, char** argv) {
    if (argc > 1) {
        Parser p = Parser((string) argv[1]);
        p.parse();
    }
}