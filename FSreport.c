#include "header.h"

int main(int argc, char const *argv[]) {
  char path[256];
  int numLevels, i;
  struct levelTreeFiles *treeArray;
  struct levelInodeFiles *inodeArray;

  if (argc != 3) {
  	printf("Incorrect number of command line arguments: %d\n", argc);
  	return 0;
  }

  if (strcmp("-inode", argv[1]) != 0 && strcmp("-tree", argv[1]) != 0) {
  	printf("Incorect output type: %s\n", argv[1]);
  	return 0;
  }

  strcpy(path, argv[2]);

  numLevels = directoryLevelCount(path, NULL, 0);
  printf("levels: %d\n", numLevels);

  if (strcmp("-tree", argv[1]) == 0) {
  	treeArray = malloc(sizeof(struct levelTreeFiles) * numLevels);

  	for (i = 0; i < numLevels; i++) {
  	  treeArray[i].level = i + 1;
  	  treeArray[i].currentPosition = 0;
  	}

  	treeDirectoryTravel(path, NULL, 0, treeArray);

  	free(treeArray);
  }

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

  closedir(dir);

  return max(newCount, count);
}

int max(int a, int b) {
  if (a >= b)
  	return a;
  return b;
}

void treeFileInfo(struct dirent dirp, char *path, int level, struct levelTreeFiles *levelsArray, int fileOrDir, char *dirName) {
  struct stat sb;
  struct passwd *pwd;
  struct group *grp;
  char newName[256], aTime[256], mTime[256], permission[256];

  strcpy(newName, path);
  strcat(newName, "/");
  strcat(newName, dirp.d_name);

  if (stat(newName, &sb) == -1) {
  	printf("stat Failed\n");
  	return;
  }

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

  if (sb.st_mode && S_IRUSR)
  	strcat(permission, "r");
  else
  	strcat(permission, "-");

  if (sb.st_mode && S_IWUSR)
  	strcat(permission, "w");
  else
  	strcat(permission, "-");

  if (sb.st_mode && S_IXUSR)
  	strcat(permission, "x");
  else
  	strcat(permission, "-");

  /*if (sb.st_mode && S_IRGRP)
  	strcat(permission, "r");
  else
  	strcat(permission, "-");

  if (sb.st_mode && S_IWGRP)
  	strcat(permission, "w");
  else
  	strcat(permission, "-");

  if (sb.st_mode && S_IXGRP)
  	strcat(permission, "x");
  else
  	strcat(permission, "-");

  if (sb.st_mode && S_IROTH)
  	strcat(permission, "r");
  else
  	strcat(permission, "-");

  if (sb.st_mode && S_IWOTH)
  	strcat(permission, "w");
  else
  	strcat(permission, "-");

  if (sb.st_mode && S_IXOTH)
  	strcat(permission, "x");
  else
  	strcat(permission, "-");*/

  strcat(permission, "\0");

  printf("%s\n", permission);

  /*printf("File Permissions: \t");
    printf( (S_ISDIR(sb.st_mode)) ? "d" : "-");
    printf( (sb.st_mode & S_IRUSR) ? "r" : "-");
    printf( (sb.st_mode & S_IWUSR) ? "w" : "-");
    printf( (sb.st_mode & S_IXUSR) ? "x" : "-");
    printf( (sb.st_mode & S_IRGRP) ? "r" : "-");
    printf( (sb.st_mode & S_IWGRP) ? "w" : "-");
    printf( (sb.st_mode & S_IXGRP) ? "x" : "-");
    printf( (sb.st_mode & S_IROTH) ? "r" : "-");
    printf( (sb.st_mode & S_IWOTH) ? "w" : "-");
    printf( (sb.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n\n");*/

  /*printf("%s(%s) %ld %s %ld %s\n", pwd->pw_name, grp->gr_name, dirp.d_ino, permission, sb.st_size, dirp.d_name);
  printf("%s %s\n", aTime, mTime);*/

  /*levelsArray[level].fileArray[levelsArray.currentPosition]*/
}

int treeDirectoryTravel(char *path, char *extension, int level, struct levelTreeFiles *levelsArray) {
  DIR *dir;
  struct dirent *dp;
  char newPath[256];
  struct dirent files[256], directories[256];
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
  while (dp != NULL) {
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

int strcmpFunc(const void *a, const void *b) {
  struct dirent *A, *B;

  A = (struct dirent *)a;
  B = (struct dirent *)b;

  return strcmp(A->d_name, B->d_name);
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