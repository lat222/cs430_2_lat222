Project 2 - Raycasting
------------------------
Code to raycast mathematical primitives based on a scene input file
into a pixel buffer, which can then write the pixel buffer to a PPM formatted file

Usage
------------------------
To use this program, JSON input file must be written with the camera object first, then any sphere or plane objects thereafter. 
All object properties need to follow the exact order as given in the example from the Project description document.
Then, go to the directory in command line and then call make.
Then execute the raycast exectuable with arguments (int in pixels) height, (int in pixels) width, (string) input_file_name, (string) output_file_name.

Known Issues
------------------------
1) Takes a long time to run large pixel height and widths
2) Displays spheres only if the radius is large enough (i.e. greater than 13ish)


Date and Author
-----------------------
10/14/17
Leia Trice - lat222@nau.edu