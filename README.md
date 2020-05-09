# GraphMazeSolver
A project on Graph Modeling and Graph Algorithms

This project uses the Boost library to implement an algorithm to find
a path through a bidirectional maze(implemented as a graph).

## Instructions:

After putting the 'input.txt' file into the same folder,
use the MAKEFILE in the included folder by running a command.

make -f .\Makefile BOOST_HOME="{Directory of your boost libraries}"

For example, my Boost Binaries were installed in my local folder, so I ran:
make -f .\Makefile BOOST_HOME="C:/local/boost_1_71_0/"

This will produce a 'SpiderMaze.exe' (or '.out') file for this project. When this .exe is run, 
an output with the required information will be produced. That output file will be called 
'output.txt' and will contain the directions traveled for given spider mazes.


Tested on Windows 10 Powershell - Curtis Davis
