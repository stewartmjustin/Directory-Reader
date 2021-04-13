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
  char userPWD[255];
  char groupPWD[255];
  int inodeNum;
  char permissions[255];
  int biteSize;
  char name[255];
  char lastAccessDate[255];
  char lastModDate[255];
  char directory[255];
  int type;
  int empty;
};

struct levelTreeFiles {
  int level;
  int currentPosition;
  struct treeFileInfo fileArray[255];
};

struct inodeFileInfo {
  int inodeNum;
  int size;
  int blocksAlloced;
  int sizeDiv; /*HOW DO I DO THIS*/
  char name[255];
  char directory[255];
  int empty;
};

struct levelInodeFiles {
  int level;
  int currentPosition;
  struct inodeFileInfo fileArray[255];
};

int directoryLevelCount(char *path, char *extension, int count);
int max(int a, int b);
void treeFileInfo(struct dirent dirp, char *path, int level, struct levelTreeFiles *levelsArray, int fileOrDir, char *dirName);
int treeDirectoryTravel(char *path, char *extension, int level, struct levelTreeFiles *levelsArray);
int strcmpFunc(const void *a, const void *b);
int inodeDirectoryTravel(char *path, char *extension, int level, struct levelInodeFiles *levelsArray);
void inodeFileInfo(struct dirent dirp, char *path, int level, struct levelInodeFiles *levelsArray, char *directoryName);
int numCmpFunc(const void *a, const void *b);
int inodeDirSort(const void *a, const void *b);