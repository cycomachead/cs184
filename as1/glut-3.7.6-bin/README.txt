README -- AS1
Michael Ball
cs184-cl

(No partners.)

Complies on: fiesta.cs

Also, any other Mac in the Mac Lab. Compilation was also done on two OS X 10.9.2
systems with no issues.

FEATURES:

* All basic assignment features have been implemented
* Options for multiple circles (1 - 9):
    * Use 'u' or 'l' to increase the circle count by 1
    * Use 'd' or 'j' to decrease the circle count by 1
    * Think Up or Down, or jkl navigation from editing media.
    * You anything < 1 becomes 1, and anything > 9 becomes 9.
* Command line switch -circles N
    * Start the program with a specified number of circles
    * N is an int in [1, 9]
* Circle Patterns:
    * 1: BIG circle
    * 2: Colors
    * 3: MICKEY MOUSE
    * 4: Corners
    * 5: Dice style
    * 6: Pyramid
    * 7: H-shape
    * 8: playing deck style 8
    * 9: Grid
* Writing to a file using -file N
    * Create 'as1.png' of N by N pixels
    * N currently has problems when the image enters gigapixel territory.
    * 23,000x23,000 seems to work. though you need a good GPU and some ram. :D
        * (I didn't buy a Mac Pro for nothing, ok?! :))
    * There is a current weird thing with the I picked PNG library which
        causes images to be rotated by -90 deg, but the image is the same
        otherwise.
