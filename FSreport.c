/*
     Created by: Justin Stewart
     Email: stewartm.justin@outlook.com
*/

#include "header.h"

/*Starting function*/
int main(int argc, char const *argv[]) {
  char path[256];
  int numLevels, i;
  struct levelTreeFiles *treeArray;
  struct levelInodeFiles *inodeArray;

  if (argc != 3) {
  	printf("Incorrect number of command line arguments, 3 needed\n");
  	return 0;
  }

  if (strcmp("-inode", argv[1]) != 0 && strcmp("-tree", argv[1]) != 0) {
  	printf("Incorect output type: %s\nPlease enter \"-tree\" or \"-inode\"\n", argv[1]);
  	return 0;
  }

  strcpy(path, argv[2]);

  numLevels = directoryLevelCount(path, NULL, 0);

  if (strcmp("-tree", argv[1]) == 0) {
  	treeArray = malloc(sizeof(struct levelTreeFiles) * numLevels);

  	for (i = 0; i < numLevels; i++) {
  	  treeArray[i].level = i + 1;
  	  treeArray[i].currentPosition = 0;
    }

  	if (treeDirectoryTravel(path, NULL, 0, treeArray) == -1) {
  	  return 0;
  	}

  	treePrint(treeArray, numLevels);

  	free(treeArray);
  }
  else {
  	inodeArray = malloc(sizeof(struct levelInodeFiles) * numLevels);

  	for (i = 0; i < numLevels; i++) {
  	  inodeArray[i].level = i + 1;
  	  inodeArray[i].currentPosition = 0;
  	}

  	if (inodeDirectoryTravel(path, NULL, 0, inodeArray) == -1) {
  	  return 0;
  	}

  	for (i = 0; i < numLevels; i++) {
  	  qsort(inodeArray[i].fileArray, inodeArray[i].currentPosition, sizeof(struct inodeFileInfo), inodeDirSort);
  	}

  	inodePrint(inodeArray, numLevels);

  	free(inodeArray);
  }

  return 0;
}

/*print the final answer for an inode tree*/
void inodePrint(struct levelInodeFiles *levelsArray, int levelCount) {
  int i = 0, j = 0;

  for (i = 0; i < levelCount; i++) {
  	for (j = 0; j < levelsArray[i].currentPosition; j++) {
  	  if (j == 0 || strcmp(levelsArray[i].fileArray[j].directory, levelsArray[i].fileArray[j - 1].directory) != 0) {
  	  	printf("Level %d: %s\n", i + 1, levelsArray[i].fileArray[j].directory);
  	  }
  	  if (levelsArray[i].fileArray[j].empty == 0) {
  	  	printf("%d:\t%d\t%d\t%d\t%s\n", levelsArray[i].fileArray[j].inodeNum, levelsArray[i].fileArray[j].size, levelsArray[i].fileArray[j].blocksAlloced, levelsArray[i].fileArray[j].sizeDiv, levelsArray[i].fileArray[j].name);
  	  }
  	}
  }
}

/*print the final answer for a tree tree*/
void treePrint(struct levelTreeFiles *levelsArray, int levelCount) {
  int i = 0, j = 0;

  for (i = 0; i < levelCount; i++) {
  	for (j = 0; j < levelsArray[i].currentPosition; j++) {
  	  if (j == 0 || strcmp(levelsArray[i].fileArray[j - 1].directory, levelsArray[i].fileArray[j].directory) != 0) {
  	  	printf("\nLevel %d: %s\n", i + 1, levelsArray[i].fileArray[j].directory);
  	  }
  	  if (j == 0 || levelsArray[i].fileArray[j - 1].type != levelsArray[i].fileArray[j].type) {
  	  	if (levelsArray[i].fileArray[j].type == 0) {
  	  	  printf("Directories\n");
  	  	}
  	  	else if (levelsArray[i].fileArray[j].type == -1) {
  	  	}
  	  	else {
  	  	  printf("\nFiles\n");
  	  	}
  	  }
  	  if (levelsArray[i].fileArray[j].empty != 1) {
  	  	printf("%s (%s)\t%d\t%s\t%d\t%s\n", levelsArray[i].fileArray[j].userPWD, levelsArray[i].fileArray[j].groupPWD, levelsArray[i].fileArray[j].inodeNum, levelsArray[i].fileArray[j].permissions, levelsArray[i].fileArray[j].biteSize, levelsArray[i].fileArray[j].name);
  	    printf("\t%s\t%s\n", levelsArray[i].fileArray[j].lastAccessDate, levelsArray[i].fileArray[j].lastModDate);
  	  }
  	}
  }
}

/*Count the number of levels in the directory*/
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

  closedir(dir);

  return max(newCount, count);
}

/*return the max of 2 numbers*/
int max(int a, int b) {
  if (a >= b)
  	return a;
  return b;
}

/*Fill a struct with info for the -tree file type*/
void treeFileInfo(struct dirent dirp, char *path, int level, struct levelTreeFiles *levelsArray, int fileOrDir, char *dirName) {
  struct stat sb;
  struct passwd *pwd;
  struct group *grp;
  char newName[256], aTime[256], mTime[256], permission[256], dName[256];

  strcpy(newName, path);
  strcat(newName, "/");
  strcat(newName, dirp.d_name);

  if (stat(newName, &sb) == -1) {
  	printf("stat Failed\n");
  	return;
  }

  if (dirName == NULL)
  	strcpy(dName, path);
  else
  	strcpy(dName, dirName);

  pwd = getpwuid(sb.st_uid);
  grp = getgrgid(sb.st_gid);
  strcpy(aTime, ctime(&sb.st_atime));

  if (sb.st_mtime >= sb.st_ctime)
  	strcpy(mTime, ctime(&sb.st_mtime));
  else
  	strcpy(mTime, ctime(&sb.st_ctime));

  aTime[strlen(aTime) - 1] = '\0';
  mTime[strlen(mTime) - 1] = '\0';

  if (S_ISDIR(sb.st_mode))
  	strcpy(permission, "d");
  else
  	strcpy(permission, "-");

  if (sb.st_mode & S_IRUSR)
  	strcat(permission, "r");
  else
  	strcat(permission, "-");

  if (sb.st_mode & S_IWUSR)
  	strcat(permission, "w");
  else
  	strcat(permission, "-");

  if (sb.st_mode & S_IXUSR)
  	strcat(permission, "x");
  else
  	strcat(permission, "-");

  if (sb.st_mode & S_IRGRP)
  	strcat(permission, "r");
  else
  	strcat(permission, "-");

  if (sb.st_mode & S_IWGRP)
  	strcat(permission, "w");
  else
  	strcat(permission, "-");

  if (sb.st_mode & S_IXGRP)
  	strcat(permission, "x");
  else
  	strcat(permission, "-");

  if (sb.st_mode & S_IROTH)
  	strcat(permission, "r");
  else
  	strcat(permission, "-");

  if (sb.st_mode & S_IWOTH)
  	strcat(permission, "w");
  else
  	strcat(permission, "-");

  if (sb.st_mode & S_IXOTH)
  	strcat(permission, "x");
  else
  	strcat(permission, "-");

  strcat(permission, "\0");

  strcpy(levelsArray[level].fileArray[levelsArray[level].currentPosition].userPWD, pwd->pw_name);
  strcpy(levelsArray[level].fileArray[levelsArray[level].currentPosition].groupPWD, grp->gr_name);
  levelsArray[level].fileArray[levelsArray[level].currentPosition].inodeNum = dirp.d_ino;
  strcpy(levelsArray[level].fileArray[levelsArray[level].currentPosition].permissions, permission);
  levelsArray[level].fileArray[levelsArray[level].currentPosition].biteSize = sb.st_size;
  strcpy(levelsArray[level].fileArray[levelsArray[level].currentPosition].name, dirp.d_name);
  strcpy(levelsArray[level].fileArray[levelsArray[level].currentPosition].lastAccessDate, aTime);
  strcpy(levelsArray[level].fileArray[levelsArray[level].currentPosition].lastModDate, mTime);
  strcpy(levelsArray[level].fileArray[levelsArray[level].currentPosition].directory, dName);
  levelsArray[level].fileArray[levelsArray[level].currentPosition].type = fileOrDir;
  levelsArray[level].currentPosition++;
}

/*Go through the levels for -tree and call functions to collect info on files*/
int treeDirectoryTravel(char *path, char *extension, int level, struct levelTreeFiles *levelsArray) {
  DIR *dir;
  struct dirent *dp;
  char newPath[256];
  struct dirent files[125], directories[125];
  int i, numDir = 0, numFiles = 0;

  level++;

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
  while (dp != NULL  && numFiles < 125 && numDir < 125) {
  	if (dp->d_type == DIRTYPE && strcmp(".", dp->d_name) != 0 && strcmp("..", dp->d_name) != 0 && numDir < 256) {
  	  directories[numDir].d_ino = dp->d_ino;
  	  directories[numDir].d_type = dp->d_type;
  	  strcpy(directories[numDir].d_name, dp->d_name);
  	  numDir++;
  	}
  	else if (dp->d_type == FILETYPE && numFiles < 256) {
  	  files[numFiles].d_ino = dp->d_ino;
  	  files[numFiles].d_type = dp->d_type;
  	  strcpy(files[numFiles].d_name, dp->d_name);
  	  numFiles++;
  	}
  	dp = readdir(dir);
  }

  qsort(directories, numDir, sizeof(struct dirent), strcmpFunc);
  qsort(files, numFiles, sizeof(struct dirent), strcmpFunc);

  if (numDir == 0 && numFiles == 0) {
  	levelsArray[level - 1].fileArray[0].empty = 1;
  	if (extension != NULL) {
  	  strcpy(levelsArray[level - 1].fileArray[0].directory, extension);
  	}
  	else {
  	  strcpy(levelsArray[level - 1].fileArray[0].directory, path);
  	}
  	levelsArray[level - 1].fileArray[0].type = -1;
  	levelsArray[level - 1].currentPosition++;
  }
  else {
  	levelsArray[level - 1].fileArray[0].empty = 0;
  }

  for (i = 0; i < numDir; i++) {
  	treeFileInfo(directories[i], newPath, level - 1, levelsArray, 0, extension);
  }

  for (i = 0; i < numFiles; i++) {
  	treeFileInfo(files[i], newPath, level - 1, levelsArray, 1, extension);
  }

  for (i = 0; i < numDir; i++) {
  	treeDirectoryTravel(newPath, directories[i].d_name, level, levelsArray);
  }

  closedir(dir);

  return 0;
}

/*compares the names of the structs to put them in order*/
int strcmpFunc(const void *a, const void *b) {
  struct dirent *A, *B;

  A = (struct dirent *)a;
  B = (struct dirent *)b;

  return strcmp(A->d_name, B->d_name);
}

/*Travel through the levels to collect info for structs for the -inode file*/
int inodeDirectoryTravel(char *path, char *extension, int level, struct levelInodeFiles *levelsArray) {
  DIR *dir;
  struct dirent *dp;
  char newPath[256];
  struct dirent files[125], directories[125];
  int i, numDir = 0, numFiles = 0;

  level++;

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
  while (dp != NULL && numFiles < 125 && numDir < 125) {
  	if (dp->d_type == DIRTYPE && strcmp(".", dp->d_name) != 0 && strcmp("..", dp->d_name) != 0 && numDir < 256) {
  	  directories[numDir].d_ino = dp->d_ino;
  	  directories[numDir].d_type = dp->d_type;
  	  strcpy(directories[numDir].d_name, dp->d_name);
  	  numDir++;
  	}
  	else if (dp->d_type == FILETYPE && numFiles < 256) {
  	  files[numFiles].d_ino = dp->d_ino;
  	  files[numFiles].d_type = dp->d_type;
  	  strcpy(files[numFiles].d_name, dp->d_name);
  	  numFiles++;
  	}
  	dp = readdir(dir);
  }

  qsort(directories, numDir, sizeof(struct dirent), numCmpFunc);
  qsort(files, numFiles, sizeof(struct dirent), numCmpFunc);

  if (numDir == 0 && numFiles == 0) {
  	levelsArray[level - 1].fileArray[levelsArray[level - 1].currentPosition].empty = 1;
  	levelsArray[level - 1].currentPosition++;
  }
  else {
  	levelsArray[level - 1].fileArray[levelsArray[level - 1].currentPosition].empty = 0;
  }

  for (i = 0; i < numDir; i++) {
  	inodeFileInfo(directories[i], newPath, level - 1, levelsArray, extension);
  }

  for (i = 0; i < numFiles; i++) {
  	inodeFileInfo(files[i], newPath, level - 1, levelsArray, extension);
  }

  for (i = 0; i < numDir; i++) {
  	inodeDirectoryTravel(newPath, directories[i].d_name, level, levelsArray);
  }
  
  return 0;
}

/*collect file info and fill a struct for the -inode tree*/
void inodeFileInfo(struct dirent dirp, char *path, int level, struct levelInodeFiles *levelsArray, char *directoryName) {
  int inoNum, biteSize, blocks, sizeDiv;
  char name[256], direct[256], newName[256];
  struct stat sb;

  strcpy(newName, path);
  strcat(newName, "/");
  strcat(newName, dirp.d_name);

  if (stat(newName, &sb) == -1) {
  	printf("stat Failed\n");
  	return;
  }

  if (directoryName == NULL) {
  	strcpy(direct, path);
  }
  else {
  	strcpy(direct, directoryName);
  }

  inoNum = dirp.d_ino;
  biteSize = sb.st_size;
  blocks = sb.st_blocks;
  sizeDiv = ceil((float)sb.st_size/512);
  strcpy(name, dirp.d_name);
  if (directoryName == NULL) {
  	strcpy(direct, path);
  }
  else {
  	strcpy(direct, directoryName);
  }

  levelsArray[level].fileArray[levelsArray[level].currentPosition].inodeNum = inoNum;
  levelsArray[level].fileArray[levelsArray[level].currentPosition].size = biteSize;
  levelsArray[level].fileArray[levelsArray[level].currentPosition].blocksAlloced = blocks;
  levelsArray[level].fileArray[levelsArray[level].currentPosition].sizeDiv = sizeDiv;
  strcpy(levelsArray[level].fileArray[levelsArray[level].currentPosition].name, name);
  strcpy(levelsArray[level].fileArray[levelsArray[level].currentPosition].directory, direct);

  levelsArray[level].currentPosition++;
}

/*sort function for inode number*/
int numCmpFunc(const void *a, const void *b) {
  struct dirent *A, *B;

  A = (struct dirent*)a;
  B = (struct dirent*)b;

  return A->d_ino - B->d_ino;
}

/*sort directory by inode number*/
int inodeDirSort(const void *a, const void *b) {
  struct inodeFileInfo *A, *B;

  A = (struct inodeFileInfo *)a;
  B = (struct inodeFileInfo *)b;

  if (strcmp(A->directory, B->directory) == 0)
  	return A->inodeNum - B->inodeNum;
  return 0;
}