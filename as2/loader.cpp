// INPUT PARSER PROVIDED BY CS184
// Modified by me of course...

#include "loader.h"

void loadX(string file) {

    // store variables and set stuff at the end
    // int width, height;
    // std::string fname = "output.bmp";
    
    stack<Transformation> mtStack;
    vector<Point> verticies;
    Transformation currentT = Transformation::identity();
    BRDF currentB = BRDF();
    

    ifstream inpfile(file.c_str());
    if(!inpfile.is_open()) {
        cout << "Unable to open file" << endl;
        cerr << "EXITING" << endl;
        exit(1);
    } else {
        string line;
        //MatrixStack mst;

        while(inpfile.good()) {
            vector<string> splitline;
            string buf;

            getline(inpfile,line);
            stringstream ss(line);

            while (ss >> buf) {
                splitline.push_back(buf);
            }
            
            // Ignore blank lines
            if (splitline.size() == 0) {
                continue;
            }

            // Ignore comments
            if (splitline[0][0] == '#') {
                continue;
            }

            // Valid commands:
            // size width height
            //  must be first command of file, controls image size
            else if(!splitline[0].compare("size")) {
                scene.width = atoi(splitline[1].c_str());
                scene.height = atoi(splitline[2].c_str());
            }
            // maxdepth depth
            //  max # of bounces for ray (default 5)
            else if(!splitline[0].compare("maxdepth")) {
                scene.maxDepth = atoi(splitline[1].c_str());
            }
            
            // output file to write image to 
            else if(!splitline[0].compare("output")) {
                scene.output = splitline[1];
            }

            // camera lFx lFy lFz lAx lAy lAz upx upy upz fov
            // speciﬁes the camera in the standard way, as in homework 2.
            else if(!splitline[0].compare("camera")) {
                scene.lookFrom << atof(splitline[1].c_str()),
                atof(splitline[2].c_str()),
                atof(splitline[3].c_str());
                scene.lookAt << atof(splitline[4].c_str()),
                atof(splitline[5].c_str()),
                atof(splitline[6].c_str());
                scene.up << atof(splitline[7].c_str()),
                atof(splitline[8].c_str()),
                atof(splitline[9].c_str());
                scene.fov = atof(splitline[10].c_str());
                mtStack.push(Transformation::identity());
            }

            // sphere x y z radius
            //  Deﬁnes a sphere with a given position and radius.
            else if(!splitline[0].compare("sphere")) {
                float x = atof(splitline[1].c_str());
                float y = atof(splitline[1].c_str());
                float z = atof(splitline[1].c_str());
                float r = atof(splitline[4].c_str());
                Sphere* s = new Sphere(x, y, z, r);
                GeometricPrimitive* geo = new GeometricPrimitive();
                geo->thing = s;
                geo->brdf = new BRDF(currentB.ka, currentB.kd, currentB.kr, currentB.ks, currentB.p);
                geo->setTransform(currentT);
                scene.primitives.push_back(geo);
                // Create new sphere:
                //   Store 4 numbers
                //   Store current property values
                //   Store current top of matrix stack
            }
      
            // maxverts number
            // Deﬁnes a maximum number of vertices for later triangle specs. 
            // It must be set before vertices are deﬁned.
            else if(!splitline[0].compare("maxverts")) {
                // Care if you want
                // Here, either declare array size Or you can just use
                // a STL vector, in which case you can ignore this
            }
      
            // maxvertnorms number
            //  Deﬁnes a maximum number of vertices with normals for later speciﬁcations.
            //  It must be set before vertices with normals are deﬁned.
            else if(!splitline[0].compare("maxvertnorms")) {
                // Care if you want
            }
      
            // vertex x y z
            //  Deﬁnes a vertex at the given location.
            //  The vertex is put into a pile, starting to be numbered at 0.
            else if(!splitline[0].compare("vertex")) {
                Point p(atof(splitline[1].c_str()),
                        atof(splitline[2].c_str()),
                        atof(splitline[3].c_str()));
                verticies.push_back(p);
                // Create a new vertex with these 3 values, store in some array
            }
      
            //vertexnormal x y z nx ny nz
            //  Similar to the above, but deﬁne a surface normal with each vertex.
            //  The vertex and vertexnormal set of vertices are completely independent
            else if(!splitline[0].compare("vertexnormal")) {
                // x: atof(splitline[1].c_str()),
                // y: atof(splitline[2].c_str()),
                // z: atof(splitline[3].c_str()));
                // nx: atof(splitline[4].c_str()),
                // ny: atof(splitline[5].c_str()),
                // nz: atof(splitline[6].c_str()));
                // Create a new vertex+normal, store in some array
            }
      
      //tri v1 v2 v3
      //  Create a triangle out of the vertices involved (which have previously been speciﬁed with
      //  the vertex command). The vertices are assumed to be speciﬁed in counter-clockwise order. Your code
      //  should internally compute a face normal for this triangle.
      else if(!splitline[0].compare("tri")) {
          int v1 = atoi(splitline[1].c_str());
          int v2 = atoi(splitline[2].c_str());
          int v3 = atoi(splitline[3].c_str());
          Point a = verticies.at(v1);
          Point b = verticies.at(v2);
          Point c = verticies.at(v3);
          Triangle *t = new Triangle(a, b, c);
          GeometricPrimitive* geo = new GeometricPrimitive();
          // FIXME -- hacky pointer problems?
          geo->thing = t;
          geo->brdf = new BRDF(currentB.ka, currentB.kd, currentB.kr, currentB.ks, currentB.p);
          geo->setTransform(currentT);
          scene.primitives.push_back(geo);
          // Create new triangle:
          //   Store pointer to array of vertices
          //   Store 3 integers to index into array
          //   Store current property values
          //   Store current top of matrix stack
      }
      
      //trinormal v1 v2 v3
      //  Same as above but for vertices speciﬁed with normals.
      //  In this case, each vertex has an associated normal, 
      //  and when doing shading, you should interpolate the normals 
      //  for intermediate points on the triangle.
      else if(!splitline[0].compare("trinormal")) {
        // v1: atof(splitline[1].c_str())
        // v2: atof(splitline[2].c_str())
        // v3: atof(splitline[3].c_str())
        // Create new triangle:
        //   Store pointer to array of vertices (Different array than above)
        //   Store 3 integers to index into array
        //   Store current property values
        //   Store current top of matrix stack
      }

      //translate x y z
      //  A translation 3-vector
      else if(!splitline[0].compare("translate")) {
          // FIXME
        // x: atof(splitline[1].c_str())
        // y: atof(splitline[2].c_str())
        // z: atof(splitline[3].c_str())
        // Update top of matrix stack
      }
      
      // rotate x y z angle
      //  Rotate by angle (in degrees) about the given axis as in OpenGL.
      else if(!splitline[0].compare("rotate")) {
          // FIXME
        // x: atof(splitline[1].c_str())
        // y: atof(splitline[2].c_str())
        // z: atof(splitline[3].c_str())
        // angle: atof(splitline[4].c_str())
        // Update top of matrix stack
      }
      // scale x y z
      //  Scale by the corresponding amount in each axis (a non-uniform scaling).
      else if(!splitline[0].compare("scale")) {
          // FIXME
        // x: atof(splitline[1].c_str())
        // y: atof(splitline[2].c_str())
        // z: atof(splitline[3].c_str())
        // Update top of matrix stack
      }
      
      // pushTransform
      //  Push the current modeling transform on the stack as in OpenGL. 
      //  You might want to do pushTransform immediately after setting 
      //   the camera to preserve the “identity” transformation.
      else if(!splitline[0].compare("pushTransform")) {
          // FIXME 
        //mst.push();
      }
      
      // popTransform
      //  Pop the current transform from the stack as in OpenGL. 
      //  The sequence of popTransform and pushTransform can be used if 
      //  desired before every primitive to reset the transformation 
      //  (assuming the initial camera transformation is on the stack as 
      //  discussed above).
      else if(!splitline[0].compare("popTransform")) {
        //mst.pop();
      }

      // directional x y z r g b
      // The direction to the light source, and the color, as in OpenGL.
      else if(!splitline[0].compare("directional")) {
          Vector3f p(atof(splitline[1].c_str()),
                     atof(splitline[2].c_str()),
                     atof(splitline[3].c_str()));
          Color c(atof(splitline[4].c_str()),
                  atof(splitline[5].c_str()),
                  atof(splitline[6].c_str()));
        // add light to scene...
          Light* l = new dLight(c, p);
          scene.lights.push_back(l);
      }
      //point x y z r g b
      //  The location of a point source and the color, as in OpenGL.
      else if(!splitline[0].compare("point")) {
          Vector3f p(atof(splitline[1].c_str()),
                     atof(splitline[2].c_str()),
                     atof(splitline[3].c_str()));
          Color c(atof(splitline[4].c_str()),
                  atof(splitline[5].c_str()),
                  atof(splitline[6].c_str()));
        // add light to scene...
          Light* l = new pLight(c, p);
          scene.lights.push_back(l);
      }
      // attenuation const linear quadratic
      //  Sets the constant, linear and quadratic attenuations 
      //  (default 1,0,0) as in OpenGL.
      else if(!splitline[0].compare("attenuation")) {
        // const: atof(splitline[1].c_str())
        // linear: atof(splitline[2].c_str())
        // quadratic: atof(splitline[3].c_str())
      }
      
      //ambient r g b
      //  The global ambient color to be added for each object 
      //  (default is .2,.2,.2)
      else if(!splitline[0].compare("ambient")) {
          currentB.ka = Color(atof(splitline[1].c_str()),
                              atof(splitline[2].c_str()),
                              atof(splitline[3].c_str()));
      }

      //diﬀuse r g b
      //  speciﬁes the diﬀuse color of the surface.
      else if(!splitline[0].compare("diffuse")) {
          currentB.kd = Color(atof(splitline[1].c_str()),
                              atof(splitline[2].c_str()),
                              atof(splitline[3].c_str()));
        // Update current properties
      }
      //specular r g b 
      //  speciﬁes the specular color of the surface.
      else if(!splitline[0].compare("specular")) {
          currentB.ks = Color(atof(splitline[1].c_str()),
                              atof(splitline[2].c_str()),
                              atof(splitline[3].c_str()));
        // Update current properties
      }
      //shininess s
      //  speciﬁes the shininess of the surface.
      else if(!splitline[0].compare("shininess")) {
          currentB.p = atof(splitline[1].c_str());
        // Update current properties
      }
      //emission r g b NOTE: EMISSIVE == REFLECTIVE (at least in my world)
      //  gives the emissive color of the surface.
      else if(!splitline[0].compare("emission")) {
          currentB.kr = Color(atof(splitline[1].c_str()),
                              atof(splitline[2].c_str()),
                              atof(splitline[3].c_str()));
        // Update current properties
      } else {
        cerr << "Unknown command: " << splitline[0] << endl;
      }
    }

    inpfile.close();
  }

}
