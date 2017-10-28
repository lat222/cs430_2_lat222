Project 2 - Raycasting
------------------------
Code to raycast mathematical primitives based on a scene input file
into a pixel buffer, which can then write the pixel buffer to a PPM3 formatted file

Usage
------------------------
To use this program, JSON input file must be written with the camera object first, then any sphere or plane objects thereafter. 
Then, go to the directory in command line and then call make.
Then execute the raycast exectuable with arguments (int in pixels) height, (int in pixels) width, (string) input_file_name, (string) output_file_name.

Known Issues
------------------------
1) Does not show multiple spheres at the same time. They override each other for some reason.


Date and Author
-----------------------
10/27/17
Leia Trice - lat222@nau.edu