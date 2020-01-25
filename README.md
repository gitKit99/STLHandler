# STLHandler

STLHandler is console application for generating and managing stl files.

STL is file format for storing 3d models. Usually, it's used for 3d printing.

User just need to call the command that will be executed and its parameters. For example, you can generate cube, sphere, cone or cylinder 
using same appropriate command name and parameters (size, location in Cartesian space and path, where file will be written). Then app
will generate model in triangulated mesh form.

Also program has ability to split stl model by plane in two pieces. For this, you should input pathes of source file, two files for
saving pieces and plane definition.
