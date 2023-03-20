# Directory Reader

This program will lists files and directories inside of a directory with two display options.

## List Files

Files: makefile, header.h, FSreport.c, README.md

Created by: Justin Stewart

Email: stewartm.justin@outlook.com

## Compile

You can compile my program with by typing the command 'make' into the command line.

This will create the executable 'FSreport'.

## Run the Program

You can run the program by typing ./FSreport 'directory type' 'path'.

Where directory type is either '-tree' or '-inode'.

And path is the path to the directory you want to be explored by the program.

## Limitations

My program has the limitation of a max of 125 files per directory, 125 directories per directory and 256 files/directories per level.

My program has worked for everything I have tested so far with the exception of some odd files, like the .git directory.