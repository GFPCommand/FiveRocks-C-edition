#include <stdio.h>
#include "helper.h"

void DrawField(int size, int arr[][size]){
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (arr[i][j] == 0){
        printf("* ");
      } else if (arr[i][j] == 1){
        printf("X ");
      } else if (arr[i][j] == 2){
        printf("O ");
      }
    }
    printf("\n");
  }
}
