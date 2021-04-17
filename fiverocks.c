#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "helper.h"

struct FieldSize {
  int x;
  int y;
} size;

struct SystemProperties {
  int isGameEnd;
  int countOfFreeFields;
  int maxLengthOfCircle;
  int maxLengthOfCross;
} properties;

typedef struct FieldSize sField;
typedef struct SystemProperties sProperties;
int coordX, coordY, turnCross, turnCircle;
char property1[20], property2[20], property3[20], property4[20];
int convertProperty1, convertProperty2, convertProperty3, convertProperty4;
/*
property1 - countOfFreeFields
property2 - maxLengthOfCross
property3 - maxLengthOfCircle
property4 - isGameEnd
*/

FILE *fp;

void Setup(int arr[][size.y]){
  if (fgets(property1, 20, fp) == NULL){
    printf("Error with first property");
    exit(0);
  }

  if (fgets(property2, 20, fp) == NULL){
    printf("Error with second property");
    exit(0);
  }

  if (fgets(property3, 20, fp) == NULL){
    printf("Error with third property");
    exit(0);
  }

  if (fgets(property4, 20, fp) == NULL){
    printf("Error with fourth property");
    exit(0);
  }

  convertProperty1 = atoi(property1);
  convertProperty2 = atoi(property2);
  convertProperty3 = atoi(property3);
  convertProperty4 = atoi(property4);

  properties.countOfFreeFields = convertProperty1;
  properties.maxLengthOfCross = convertProperty2;
  properties.maxLengthOfCircle = convertProperty3;
  properties.isGameEnd = convertProperty4;
  turnCross = 1;
  turnCircle = 0;

  for (int i = 0; i < size.y; i++) {
    for (int j = 0; j < size.x; j++) {
      arr[i][j] = 0;
    }
  }
}

void SaveGame(int arr[][size.x]){
  FILE *gs;

  if ((gs = fopen("gamesave", "w"))  == NULL){
    printf("%s\n", strerror(errno));
    exit(0);
  }

  for (int i = 0; i < size.y; i++) {
    for(int j = 0; j < size.x; j++){
        //fprintf(gs, "%d ", arr[i][j]);
        putc(arr[i][j], gs);
    }
    fprintf(gs, "\n");
  }

  fclose(gs);
}

void PrintSaveGame(int arr[][size.y]){
    FILE *rgs;

    int mass[size.x][size.y];

  if ((rgs = fopen("gamesave", "r"))  == NULL){
    printf("%s\n", strerror(errno));
    exit(0);
  }
  int m = 0, n = 0;
  while (!feof(rgs)){
    int buffer = getc(rgs);
    mass[m][n] = buffer;
    n++;
    if (buffer == '\n'){
        m++;
        n = 0;
    }
  }

  for (int i = 0; i < size.y; i++) {
    for (int j = 0; j < size.x; j++){
        printf("%d ", mass[i][j]);
    }
    printf("\n");
  }

  fclose(rgs);
}

void InputData(int arr[][size.y]){

  int coordX, coordY;
  int fieldArray[size.y][size.x];
  printf("\nEnter coordinate(for exit enter '0 0'): ");

  scanf("%d %d", &coordX, &coordY);
  if (coordX == 0 && coordY == 0) {
    for (int i = 0; i < size.x; i++){
        for (int j = 0; j < size.y; j++){
            fieldArray[i][j] = arr[i][j];
        }
    }
    SaveGame(fieldArray);

    exit(0);
  }
  if (coordX > size.x || coordX < 0 || coordY > size.y || coordY < 0){
    printf("Incorrect input data. Please retry. Enter numbers between 0 and 6\n");
    return;
  }
  coordX--;
  coordY--;

  if (turnCross && !turnCircle && arr[coordY][coordX] == 0){
    arr[coordY][coordX] = 1;
    turnCross = 0;
    turnCircle = 1;
    properties.countOfFreeFields--;
  } else if (!turnCross && turnCircle && arr[coordY][coordX] == 0){
    arr[coordY][coordX] = 2;
    turnCross = 1;
    turnCircle = 0;
    properties.countOfFreeFields--;
  }

  if (properties.countOfFreeFields == 0) properties.isGameEnd = 1;
}

void CheckWinner(int arr[][size.y]){

  // Horizontal
  for (int i = 0; i < size.y; i++) {
    for (int j = 0; j < size.x; j++) {
      int k = j;
      int currX = 0;
      int currO = 0;

      while (arr[i][k] == 1 && k < size.x){
        currX++;
        k++;
        if (k == 5) break;
      }

      k = j;

      while (arr[i][k] == 2 & k < size.x){
        currO++;
        k++;
        if (k == 5) break;
      }

      j = k;

      if (properties.maxLengthOfCross < currX) properties.maxLengthOfCross = currX;
      if (properties.maxLengthOfCircle < currO) properties.maxLengthOfCircle = currO;
    }
  }

  // Vertical
  for (int i = 0; i < size.y; i++) {
    for (int j = 0; j < size.x; j++) {
      int k = j;
      int currX = 0;
      int currO = 0;

      while (arr[k][i] == 1 && k < size.y){
        currX++;
        k++;
        if (k == 5) break;
      }

      k = j;

      while (arr[k][i] == 2 && k < size.y){
        currO++;
        k++;
        if (k == 5) break;
      }

      j = k;

      if (properties.maxLengthOfCross < currX) properties.maxLengthOfCross = currX;
      if (properties.maxLengthOfCircle < currO) properties.maxLengthOfCircle = currO;
    }
  }

  // Main diagonal
  for (int i = 0; i < size.y; i++) {
    for (int j = 0; j < size.x; j++) {
      int k = j, l = i;
      int currX = 0;
      int currO = 0;

      while (arr[l][k] == 1 && k < size.x && l < size.y){
        currX++;
        k++;
        l++;
        if (k == 5 && l == 5) break;
      }

      k = j;
      l = i;

      while (arr[l][k] == 2 && k < size.x && l < size.y){
        currO++;
        k++;
        l++;
        if (k == 5 && l == 5) break;
      }

      j = k;
      i = l;

      if (properties.maxLengthOfCross < currX) properties.maxLengthOfCross = currX;
      if (properties.maxLengthOfCircle < currO) properties.maxLengthOfCircle = currO;
    }
  }

  // ----------Diagonals above main diagonal----------

  for (int i = 0; i < size.y-1; i++) {
    for (int j = i+1; j < size.x; j++) {
      int k = j, l = i;
      int currX = 0;
      int currO = 0;

      while (arr[l][k] == 1 && k < size.x && l < size.y){
        currX++;
        k++;
        l++;
        if (k == 5 && l == 5) break;
      }

      k = j;
      l = i;

      while (arr[l][k] == 2 && k < size.x && l < size.y){
        currO++;
        k++;
        l++;
        if (k == 5 && l == 5) break;
      }

      j = k;
      i = l;

      if (properties.maxLengthOfCross < currX) properties.maxLengthOfCross = currX;
      if (properties.maxLengthOfCircle < currO) properties.maxLengthOfCircle = currO;
    }
  }

  for (int i = 0; i < size.y-2; i++) {
    for (int j = i+2; j < size.x; j++) {
      int k = j, l = i;
      int currX = 0;
      int currO = 0;

      while (arr[l][k] == 1 && k < size.x && l < size.y){
        currX++;
        k++;
        l++;
        if (k == 5 && l == 5) break;
      }

      k = j;
      l = i;

      while (arr[l][k] == 2 && k < size.x && l < size.y){
        currO++;
        k++;
        l++;
        if (k == 5 && l == 5) break;
      }

      j = k;
      i = l;

      if (properties.maxLengthOfCross < currX) properties.maxLengthOfCross = currX;
      if (properties.maxLengthOfCircle < currO) properties.maxLengthOfCircle = currO;
    }
  }

  for (int i = 0; i < size.y-3; i++) {
    for (int j = i+3; j < size.x; j++) {
      int k = j, l = i;
      int currX = 0;
      int currO = 0;

      while (arr[l][k] == 1 && k < size.x && l < size.y){
        currX++;
        k++;
        l++;
        if (k == 5 && l == 5) break;
      }

      k = j;
      l = i;

      while (arr[l][k] == 2 && k < size.x && l < size.y){
        currO++;
        k++;
        l++;
        if (k == 5 && l == 5) break;
      }

      j = k;
      i = l;

      if (properties.maxLengthOfCross < currX) properties.maxLengthOfCross = currX;
      if (properties.maxLengthOfCircle < currO) properties.maxLengthOfCircle = currO;
    }
  }

  //--------------------------------------------------

  // ----------Diagonals under main diagonal----------

  for (int i = 1; i < size.y; i++) {
    for (int j = 0; j < size.x-1; j++) {
      int k = j, l = i;
      int currX = 0;
      int currO = 0;

      while (arr[l][k] == 1 && k < size.x && l < size.y){
        currX++;
        k++;
        l++;
        if (k == 5 && l == 5) break;
      }

      k = j;
      l = i;

      while (arr[l][k] == 2 && k < size.x && l < size.y){
        currO++;
        k++;
        l++;
        if (k == 5 && l == 5) break;
      }

      j = k;
      i = l;

      if (properties.maxLengthOfCross < currX) properties.maxLengthOfCross = currX;
      if (properties.maxLengthOfCircle < currO) properties.maxLengthOfCircle = currO;
    }
  }

  for (int i = 2; i < size.y; i++) {
    for (int j = 0; j < size.x-2; j++) {
      int k = j, l = i;
      int currX = 0;
      int currO = 0;

      while (arr[l][k] == 1 && k < size.x && l < size.y){
        currX++;
        k++;
        l++;
        if (k == 5 && l == 5) break;
      }

      k = j;
      l = i;

      while (arr[l][k] == 2 && k < size.x && l < size.y){
        currO++;
        k++;
        l++;
        if (k == 5 && l == 5) break;
      }

      j = k;
      i = l;

      if (properties.maxLengthOfCross < currX) properties.maxLengthOfCross = currX;
      if (properties.maxLengthOfCircle < currO) properties.maxLengthOfCircle = currO;
    }
  }

  for (int i = 3; i < size.y; i++) {
    for (int j = 0; j < size.x-3; j++) {
      int k = j, l = i;
      int currX = 0;
      int currO = 0;

      while (arr[l][k] == 1 && k < size.x && l < size.y){
        currX++;
        k++;
        l++;
        if (k == 5 && l == 5) break;
      }

      k = j;
      l = i;

      while (arr[l][k] == 2 && k < size.x && l < size.y){
        currO++;
        k++;
        l++;
        if (k == 5 && l == 5) break;
      }

      j = k;
      i = l;

      if (properties.maxLengthOfCross < currX) properties.maxLengthOfCross = currX;
      if (properties.maxLengthOfCircle < currO) properties.maxLengthOfCircle = currO;
    }
  }

  //--------------------------------------------------

  // Secondary diagonal

  for (int i = 0, j = 4; i < size.x, j > -1; i++, j--){
        int k = j, l = i;
        int currX = 0;
        int currO = 0;

        while (arr[l][k] == 1 && l < size.x && k > -1){
                currX++;
                k--;
                l++;
                if (k == -1 && l == 5) break;
        }

        k = j;
        l = i;

        while (arr[l][k] == 2 && l < size.x && k > -1){
                currO++;
                k--;
                l++;
                if (k == -1 && l == 5) break;
        }

        j = k;
        i = l;

        if (properties.maxLengthOfCross < currX) properties.maxLengthOfCross = currX;
        if (properties.maxLengthOfCircle < currO) properties.maxLengthOfCircle = currO;
  }

  // ----------Diagonals above secondary diagonal----------

  for (int i = 3; i > -1; i--) {
    for (int m = 0, n = i; m < size.x, n > -1; m++, n--) {
      int k = n, l = m;
      int currX = 0;
      int currO = 0;

      while (arr[l][k] == 1 && l < size.x && k > -1){
              currX++;
              k--;
              l++;

              if (k == -1 && l == 5) break;
      }

      k = n;
      l = m;

      while (arr[l][k] == 2 && l < size.x && k > -1){
              currO++;
              k--;
              l++;

              if (k == -1 && l == 5) break;
      }

      n = k;
      m = l;

      if (properties.maxLengthOfCross < currX) properties.maxLengthOfCross = currX;
      if (properties.maxLengthOfCircle < currO) properties.maxLengthOfCircle = currO;
    }
  }

  //--------------------------------------------------

  // ----------Diagonals under secondary diagonal----------

  for (int i = 1; i < size.x; i++) {
    for (int m = i, n = 4; m < size.x, n > -1; m++, n--) {
      int k = n, l = m;
      int currX = 0;
      int currO = 0;

      while (arr[l][k] == 1 && l < size.x && k > -1){
              currX++;
              k--;
              l++;

              if (k == -1 && l == 5) break;
      }

      k = n;
      l = m;

      while (arr[l][k] == 2 && l < size.x && k > -1){
              currO++;
              k--;
              l++;

              if (k == -1 && l == 5) break;
      }

      n = k;
      m = l;

      if (properties.maxLengthOfCross < currX) properties.maxLengthOfCross = currX;
      if (properties.maxLengthOfCircle < currO) properties.maxLengthOfCircle = currO;
    }
  }

  //--------------------------------------------------
}

int main(int argc, char const *argv[]) {
 if ((fp = fopen("properties.txt", "r"))  == NULL){
    printf("%s\n", strerror(errno));
    exit(0);
  }

  size.x = 5;
  size.y = 5;

  int field[size.x][size.y];

  int gameSave[size.x][size.y];


  printf("\nFiveRocks v1.3\n\n");

  Setup(field);
  while (!properties.isGameEnd){
    DrawField(size.x, field);
    InputData(field);
  }

  if (properties.isGameEnd){
    DrawField(size.x, field);
    CheckWinner(field);
    SaveGame(field);
  }

  PrintSaveGame(gameSave);

  if (properties.maxLengthOfCross > properties.maxLengthOfCircle) printf("\nWin: X\n");
  else if (properties.maxLengthOfCross < properties.maxLengthOfCircle) printf("\nWin: O\n");
  else if (properties.maxLengthOfCross == properties.maxLengthOfCircle) printf("\nWin: X and O\n");

  printf("maxLengthOfCross = %d;\n", properties.maxLengthOfCross);
  printf("maxLengthOfCircle = %d\n", properties.maxLengthOfCircle);

  fclose(fp);

  return 0;
}
