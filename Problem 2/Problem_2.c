
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define NBR_WEIGHTS 2
#define LEARNING_RATE 0.1
#define EPOCH_MAX 500000
#define LAMBDA 2

typedef struct Image Image;
typedef struct Hopfeild Hopfeild;
typedef struct Data Data;

struct Image{
  int Img [200];
  int Index;
};

struct Data{
  Image Images[10];
  int NbrImage;
};

struct Hopfeild {
  int Input[200];
  int Weights[200][200];
  int Output [200];
};

void parseFile(char *file, Data *train, Data *test);

void parseLine(char *line, Image *img);

void printImage(Image *img);

void training(Hopfeild *network, Data *data);

void printWeights(Hopfeild *network);

int main (void){
  char *path = "/home/gemini/TUM/CI/CI-Homework_3/Problem 2/testInput22A.txt";
  Data train, test;
  Hopfeild network;

  for (int i = 0; i < 10; ++i) {
    train.Images[i].Index = 0;
    test.Images[i].Index = 0;
  }

  parseFile(path,&train,&test);

  training(&network,&train);

  printWeights(&network);

  return EXIT_SUCCESS;
}

void parseFile(char *path, Data *train, Data *test) {
  FILE *file;
  int size = 50, i = 0, j = 0, flag = 0;
  char buff[size];

  file = fopen(path, "r");

  if (file) {
    while (fgets(buff, size, (FILE *) file) != NULL) {
      if (buff[0] == '-' && buff[1] == '\n'){
        j++;
      } else if (buff[0] == '-' && buff[1] == '-' && buff[2] == '-' && buff[3] == '\n'){
        flag = 1;
      } else{
        if (flag == 0)
          parseLine(buff, &(train->Images[j]));
        else
          parseLine(buff, &(test->Images[i]));
      }
    }
    train->NbrImage = j +1;
  } else {
    printf("Can't open file");
  }
}

void parseLine(char *line, Image *img) {

  for (int j = 0; j < 20; ++j) {
    if (line[j] == '.'){
      img->Img[j+img->Index] = 0;
    } else{
      img->Img[j+img->Index] = 1;
    }
  }
  img->Index += 20;
}

void printImage(Image *img){
  int index = 0;
  for (int j = 0; j < 10; ++j) {
    for (int i = 0; i < 20; ++i) {
      printf("%d",img->Img[i+index]);
    }
    printf("\n");
    index += 20;
  }
}

void training(Hopfeild *network, Data *data){
  for (int pt = 0; pt < data->NbrImage; ++pt) {
    for (int i = 0; i < 200; ++i) {
      for (int j = 0; j < 200; ++j) {
        if (i == j)
          network->Weights[i][j] = 0;
        else
          network->Weights[i][j] = data->Images[pt].Img[i] * data->Images[pt].Img[j];
      }
    }
  }
}

void printWeights(Hopfeild *network){
  for (int i = 0; i < 200; ++i) {
    for (int j = 0; j < 200; ++j) {
      printf("%d",network->Weights[i][j]);
    }
    printf("\n");
  }
}