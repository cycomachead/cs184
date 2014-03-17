// Michael Ball CS184 Ray Tracer
// This file has all my includes.

#include "common.h"

#include "simple.h"

#include "camera.h"
#include "objects.h"
#include "scene.h"
#include "film.h"
#include "sampler.h"
#include "tracer.h"

// The main thing controlling the work.
Scene scene;
int SCENE = 0;
string COMMANDS = "";
int LOGGING = 0;

int main(int argc, const char** argv) {
    argParse(argc, argv);
    if (COMMANDS == "") {
        cout << "Loading Predefined Scene: " << SCENE << endl;
        scene.loadScene(SCENE);
    } else {
        cout << "Loading Scene: " << COMMANDS << endl;
        scene.loadScene(COMMANDS);
    }

    if (LOGGING > 1) {
        cout << "RENDERING..." << endl;
    }

    scene.render();
    return 0;
}

void argParse(int argc, const char** argv) {
    // Basically, all I care about is getting the filename to parse
    // Then the parser will take care of the dirty work.
    int arg = 1;
    try {
        while (arg < argc) {
            if (!strncmp(argv[arg], "-file", 5) and arg < argc - 1) {
                COMMANDS = (string) argv[arg + 1];
            } else if (!strncmp(argv[arg], "-log", 4) and arg < argc - 1) {
                LOGGING = atoi(argv[arg + 1]);
            } else if (!strncmp(argv[arg], "-scene", 4) and arg < argc - 1) {
                SCENE = atoi(argv[arg + 1]);
            }
            if (arg == argc - 1) {
                cerr << "Invalid Arguments Found" << endl;
                cerr << "LOGGING LEVEL: " << LOGGING << endl;
                cerr << "COMMAND`S FILE: " << COMMANDS << endl;
            }
            arg += 2;
        }
    } catch (...) {
        cerr << "Exception Occurred while trying to process arguments" << endl;
        cerr << "Exiting";
        exit (EXIT_FAILURE);
    }
    if (LOGGING > 0) {
        cout << "Parameters:\tLOGGING: " << LOGGING << "\tCOMMANDS: " << COMMANDS;
        cout << "\tSCENE: " << SCENE << endl;
    }
}
