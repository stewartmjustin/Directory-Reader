#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <libgen.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define FILETYPE 8
#define DIRTYPE 4

struct treeFileInfo {
  char userPWD[256];
  char groupPWD[256];
  int inodeNum;
  char permissions[256];
  int biteSize;
  char name[256];
  char lastAccessDate[256];
  char lastModDate[256];
  char directory[256];
  int type;
  int empty;
};

struct levelTreeFiles {
  int level;
  int currentPosition;
  struct treeFileInfo fileArray[256];
};

struct inodeFileInfo {
  int inodeNum;
  int size;
  int blocksAlloced;
  int sizeDiv; 
  char name[256];
  char directory[256];
  int empty;
};

struct levelInodeFiles {
  int level;
  int currentPosition;
  struct inodeFileInfo fileArray[256];
};

int directoryLevelCount(char *path, char *extension, int count); /*get number of levels in the directory*/
int max(int a, int b); /*find the max of 2 numbers*/
void treeFileInfo(struct dirent dirp, char *path, int level, struct levelTreeFiles *levelsArray, int fileOrDir, char *dirName); /*fill the struct for the file tree*/
int treeDirectoryTravel(char *path, char *extension, int level, struct levelTreeFiles *levelsArray); /*travel through the directory so you can access tree file info*/
int strcmpFunc(const void *a, const void *b); /*qsort for a array of strings*/
int inodeDirectoryTravel(char *path, char *extension, int level, struct levelInodeFiles *levelsArray); /*travel through the inode directorys*/
void inodeFileInfo(struct dirent dirp, char *path, int level, struct levelInodeFiles *levelsArray, char *directoryName); /*fill the struct for inode*/
int numCmpFunc(const void *a, const void *b); /*qsort for somthing*/
int inodeDirSort(const void *a, const void *b); /*qsort for somthing*/