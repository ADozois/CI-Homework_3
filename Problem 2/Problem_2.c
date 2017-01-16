
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define NBR_WEIGHTS 2
#define LEARNING_RATE 0.1
#define EPOCH_MAX 100
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
  int Output [10][200];
};

void parseFile(char *file, Data *train, Data *test);

void parseLine(char *line, Image *img);

void printImage(Image *img);

void training(Hopfeild *network, Data *data);

void printWeights(Hopfeild *network);

void recovery(Hopfeild *network, Data *data);

void printOutput(Hopfeild *network, Data *data);

int converge(int *output, Data *data);

int main (void){
  char *path = "/home/gemini/TUM/CI/CI-Homework_3/Problem 2/testInput22B.txt";
  Data train, test;
  int flag = 0;
  int j = 0, i = 0;
  char buff[100];
  Hopfeild network;

  for (int i = 0; i < 10; ++i) {
    train.Images[i].Index = 0;
    test.Images[i].Index = 0;
  }

  /*while(scanf("%s",buff) == 1) {
    if (strcmp(buff, "-") == 0){
      if (flag == 0) {
        j++;
      }
      else
        i++;
    } else if (strcmp(buff, "---") == 0){
      flag = 1;
    } else{
      if (flag == 0)
        parseLine(buff, &(train.Images[j]));
      else
        parseLine(buff, &(test.Images[i]));
    }
  }
  train.NbrImage = j +1;
  test.NbrImage = i + 1;*/

  parseFile(path,&train,&test);

  training(&network,&train);

  recovery(&network,&test);

  //printImage(&train.Images[0]);

  //printf("\n");

  printOutput(&network, &test);

  //printImage(&network.Output[3]);

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
        if (flag == 0)
          j++;
        else
          i++;
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
    test->NbrImage = i + 1;
  } else {
    printf("Can't open file");
  }
}

void parseLine(char *line, Image *img) {

  for (int j = 0; j < 20; ++j) {
    if (line[j] == '.'){
      img->Img[j+img->Index] = -1;
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
      if (img->Img[i+index] == 1)
        printf("%c",'*');
      else
        printf("%c",'.');
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
          network->Weights[i][j] += data->Images[pt].Img[i] * data->Images[pt].Img[j];
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

void recovery(Hopfeild *network, Data *data){
  for (int iter = 0; iter < 1; ++iter) {
    for (int pt = 0; pt < data->NbrImage; ++pt) {
      for (int n1 = 0; n1 < 200; ++n1) {
        network->Output[pt][n1] = 0;
        for (int n2 = 0; n2 < 200; ++n2) {
          network->Output[pt][n1] += network->Weights[n1][n2] * data->Images[pt].Img[n2];
        }
        if (network->Output[pt][n1] >= 0){
          network->Output[pt][n1] = 1;
        } else{
          network->Output[pt][n1] = -1;
        }
      }
    }
  }
}

void printOutput(Hopfeild *network, Data *data) {
  int index;
  for (int pt = 0; pt < data->NbrImage; ++pt) {
    index = 0;
    for (int j = 0; j < 10; ++j) {
      for (int i = 0; i < 20; ++i) {
        if (network->Output[pt][i+index] == 1)
          printf("%c",'*');
        else
          printf("%c",'.');
      }
      printf("\n");
      index += 20;
    }
    if (data->NbrImage > 1 && pt != data->NbrImage-1)
      printf("-\n");
  }
}
