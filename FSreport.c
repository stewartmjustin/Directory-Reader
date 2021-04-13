#include "header.h"

int main(int argc, char const *argv[]) {
  char path[256];
  int numLevels;

  if (argc != 3) {
  	printf("Incorrect number of command line arguments: %d\n", argc);
  	return 0;
  }

  strcpy(path, argv[2]);

  numLevels = directoryLevelCount(path, NULL, 0);
  return 0;
}

int directoryLevelCount(char *path, char *extension, int count) {
  DIR *dir;
  struct dirent *dp;
  char newPath[256];
  char directories[256][256];
  int numDir = 0, i, newCount = 0;

  count++;

  strcpy(newPath, path);
  if (extension != NULL) {
  	strcat(newPath, "/");
  	strcat(newPath, extension);
  }

  dir = opendir(newPath);
  if (dir == NULL) {
  	printf("opendir failed\n");
  	return -1;
  }

  dp = readdir(dir);
  while (dp != NULL) {
  	if (dp->d_type == DIRTYPE && strcmp(".", dp->d_name) != 0 && strcmp("..", dp->d_name) != 0) {
  	  strcpy(directories[numDir], dp->d_name);
  	  numDir++;
  	}
  	dp = readdir(dir);
  }

  for (i = 0; i < numDir; i++) {
  	newCount = max(directoryLevelCount(newPath, directories[i], count), newCount);
  	if (count == -1)
  	  return -1;
  }

  return max(newCount, count);
}

int max(int a, int b) {
  if (a >= b)
  	return a;
  return b;
}

void treeFileInfo(struct dirent dirp, char *path, int level, struct levelTreeFiles *levelsArray, int fileOrDir, char *dirName) {
}

int treeDirectoryTravel(char *path, char *extension, int level, struct levelTreeFiles *levelsArray) {
  return 0;
}

int strcmpFunc(const void *a, const void *b) {
  return 0;
}

int inodeDirectoryTravel(char *path, char *extension, int level, struct levelInodeFiles *levelsArray) {
  return 0;
}

void inodeFileInfo(struct dirent dirp, char *path, int level, struct levelInodeFiles *levelsArray, char *directoryName) {
  
}

int numCmpFunc(const void *a, const void *b) {
  return 0;
}

int inodeDirSort(const void *a, const void *b) {
  return 0;
}