# CS184 AS2

#### Michael Ball (cs184-cl)

#### Compiles: taylor.cs.berkeley.edu
(Or any Mac running >= 10.8 with clang)

## WEBPAGE:
[http://inst.eecs.berkeley.edu/~cs184-cl/as2/]()

## Background:

## Features and Use:
Compile by running `make`
You can run and get a built in simple scenario by doing `make run`.
__COMMAND LINE FLAGS:__

* `-file [NAME]` will parse a .obj from Prof. Ramamoorthi's format.
   * (Standard .obj files to come later!)
   * The file should specify the output file name.
* `-log [INT]` will print debugging info. >10 generates LARGE amounts of text.
* `-scene [INT]` will run a predefined scene. There are currently 4. Use 0 to 3.
    * The default file of "image_out.png" will be used.
* If both a `FILE` and `SCENE` are provided, the file is parsed.

* With no arguments the following command is given:
    * `./raytracer -log 0 -scene 0`

## IMPLEMENTED:
I've implemented everything EXCEPT transformations... :(

I was at a conference the week before the midterm, and spent the two weeks 
before prepping for our demo and so I didn't have a partner and thus my 
assignment is a bit incomplete.

## NOTES:

* __Image 1:__
    * TIME: ~9s to generate 800x800 png
    * Uses `scene 1` with 1 sphere (with BRDF), and 1 point light 
* __Image 2:__
