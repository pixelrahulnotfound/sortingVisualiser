#include <limits.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 900
#define HEIGHT 600
#define COUNT 300

int arr[COUNT];
int tempArr[COUNT];
int MAX = INT_MIN;

int i = 0;
int j = 0;
int key = 0;
int minIndex = 0;

int sorted = 0;
int algorithm = 1;

int qs_low[COUNT];
int qs_high[COUNT];
int qs_top = -1;

int ms_size = 1;
int ms_left = 0;

int max(int a, int b) { return (a > b ? a : b); }

void init() {

  MAX = INT_MIN;

  for (int i = 0; i < COUNT; i++) {

    arr[i] = rand() % 100 + 1;

    MAX = max(MAX, arr[i]);
  }
}

void resetSort() {

  i = 0;
  j = 0;
  key = 0;
  minIndex = 0;

  sorted = 0;

  qs_top = -1;

  ms_size = 1;
  ms_left = 0;

  if (algorithm == 4) {

    qs_top++;
    qs_low[qs_top] = 0;
    qs_high[qs_top] = COUNT - 1;
  }
}

void draw_arr() {

  float w = (float)(WIDTH - 20) / COUNT;

  for (int k = 0; k < COUNT; k++) {

    float h = arr[k] * ((float)HEIGHT / MAX);

    Color color = LIGHTGRAY;

    if (k == j || k == j + 1 || k == minIndex)
      color = RED;

    DrawRectangle((int)(w * k + 10), (int)(HEIGHT - h), (int)(w - 1), (int)h,
                  color);
  }
}

void bubbleStep() {

  if (i < COUNT - 1) {

    if (j < COUNT - i - 1) {

      if (arr[j] > arr[j + 1]) {

        int temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }

      j++;
    } else {

      j = 0;
      i++;
    }
  } else {

    sorted = 1;
  }
}

void insertionStep() {

  if (i == 0)
    i = 1;

  if (i < COUNT) {

    key = arr[i];
    j = i - 1;

    while (j >= 0 && arr[j] > key) {

      arr[j + 1] = arr[j];
      j--;
    }

    arr[j + 1] = key;

    i++;
  } else {

    sorted = 1;
  }
}

void selectionStep() {

  if (i < COUNT - 1) {

    minIndex = i;

    for (j = i + 1; j < COUNT; j++) {

      if (arr[j] < arr[minIndex]) {

        minIndex = j;
      }
    }

    int temp = arr[i];
    arr[i] = arr[minIndex];
    arr[minIndex] = temp;

    i++;
  } else {

    sorted = 1;
  }
}

int partition(int low, int high) {

  int pivot = arr[high];
  int p = low;

  for (int k = low; k < high; k++) {

    if (arr[k] < pivot) {

      int temp = arr[k];
      arr[k] = arr[p];
      arr[p] = temp;

      p++;
    }
  }

  int temp = arr[p];
  arr[p] = arr[high];
  arr[high] = temp;

  return p;
}

void quickStep() {

  if (qs_top < 0) {

    sorted = 1;
    return;
  }

  int high = qs_high[qs_top];
  int low = qs_low[qs_top];

  qs_top--;

  int p = partition(low, high);

  if (p - 1 > low) {

    qs_top++;
    qs_low[qs_top] = low;
    qs_high[qs_top] = p - 1;
  }

  if (p + 1 < high) {

    qs_top++;
    qs_low[qs_top] = p + 1;
    qs_high[qs_top] = high;
  }
}

void merge(int left, int mid, int right) {

  int a = left;
  int b = mid;
  int c = left;

  while (a < mid && b < right) {

    if (arr[a] <= arr[b]) {

      tempArr[c++] = arr[a++];
    } else {

      tempArr[c++] = arr[b++];
    }
  }

  while (a < mid)
    tempArr[c++] = arr[a++];

  while (b < right)
    tempArr[c++] = arr[b++];

  for (int k = left; k < right; k++)
    arr[k] = tempArr[k];
}

void mergeStep() {

  if (ms_size >= COUNT) {

    sorted = 1;
    return;
  }

  int mid = ms_left + ms_size;
  int right = ms_left + 2 * ms_size;

  if (mid > COUNT)
    mid = COUNT;

  if (right > COUNT)
    right = COUNT;

  merge(ms_left, mid, right);

  ms_left += 2 * ms_size;

  if (ms_left >= COUNT) {

    ms_left = 0;
    ms_size *= 2;
  }
}

void sortStep() {

  if (sorted)
    return;

  switch (algorithm) {

  case 1:
    bubbleStep();
    break;

  case 2:
    insertionStep();
    break;

  case 3:
    selectionStep();
    break;

  case 4:
    quickStep();
    break;

  case 5:
    mergeStep();
    break;
  }
}

const char *getAlgoName() {

  switch (algorithm) {

  case 1:
    return "BUBBLE SORT";

  case 2:
    return "INSERTION SORT";

  case 3:
    return "SELECTION SORT";

  case 4:
    return "QUICK SORT";

  case 5:
    return "MERGE SORT";
  }

  return "";
}

int main() {

  srand(time(NULL));

  InitWindow(WIDTH, HEIGHT, "SORTING VISUALIZER");

  SetTargetFPS(200);

  init();
  resetSort();

  while (!WindowShouldClose()) {

    if (IsKeyPressed(KEY_R)) {

      init();
      resetSort();
    }

    if (IsKeyPressed(KEY_ONE)) {

      algorithm = 1;
      init();
      resetSort();
    }

    if (IsKeyPressed(KEY_TWO)) {

      algorithm = 2;
      init();
      resetSort();
    }

    if (IsKeyPressed(KEY_THREE)) {

      algorithm = 3;
      init();
      resetSort();
    }

    if (IsKeyPressed(KEY_FOUR)) {

      algorithm = 4;
      init();
      resetSort();
    }

    if (IsKeyPressed(KEY_FIVE)) {

      algorithm = 5;
      init();
      resetSort();
    }

    sortStep();

    BeginDrawing();

    ClearBackground(BLACK);

    draw_arr();

    DrawText(getAlgoName(), 20, 20, 30, GREEN);

    DrawText("1 : BUBBLE", 20, 70, 20, WHITE);
    DrawText("2 : INSERTION", 20, 100, 20, WHITE);
    DrawText("3 : SELECTION", 20, 130, 20, WHITE);
    DrawText("4 : QUICK", 20, 160, 20, WHITE);
    DrawText("5 : MERGE", 20, 190, 20, WHITE);
    DrawText("R : RANDOMIZE", 20, 220, 20, WHITE);

    if (sorted)
      DrawText("SORTED", WIDTH - 170, 20, 30, GREEN);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
