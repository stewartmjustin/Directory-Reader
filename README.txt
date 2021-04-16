Files: makefile, header.h, FSreport.c, README.txt
Created by: Justin Stewart
Student ID: 1052722
Email: jstewa28@uoguelph.ca

Hello,

You can compile my program with by typing the command 'make' into the command line.

This will create the executable 'FSreport'.

You can run the program by typing ./Fsreport 'directory type' 'path'.

Where directory type is either '-tree' or '-inode'.

And path is the path to the directory you want to be explored by the program.

My program has the limitation of a max of 125 files per directory, 125 directories per directory and 256 files/directories per level.

My program has worked for everything I have tested so far with the exception of some odd files, like the .git directory.