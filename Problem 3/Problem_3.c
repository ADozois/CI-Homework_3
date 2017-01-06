
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define POUR_TO_ADD 0.1
#define NBR_WEIGHTS 2
#define LEARNING_RATE 0.01
#define EPOCH_MAX 100000
#define LAMBDA 2

typedef struct Data Data;
typedef struct City City;
typedef struct Neuron Neuron;
typedef struct SOM SOM;
typedef struct Layer Layer;

struct Layer {
  Neuron *Neurons;
  int size;
};

struct City {
  int Id;
  int Input1;
  int Input2;
};

struct Data {
  int size;
  int Max_1;
  int Min_1;
  int Max_2;
  int Min_2;
  City cities[1000];
};

struct SOM {
  int size;
  Layer layer;
};

struct Neuron {
  double *Weights;
  double *Update;
  int CityId;
  int Registred;
  Neuron *Next;
  Neuron *Previous;
};

void parseFile(char *file, Data *data);

void parseLine(char *line, City *data);

void findMaxMin(Data *data);

void createSOM(SOM *network, Data *data);

void createLayer(Layer *layer, int numberNeurons, int numberWeights, Data *data);

void initialiseNeuron(Neuron *neuron, Neuron *next, Neuron *prev, int nbrWeights, Data *data);

int main(void) {
  char *path = "/home/gemini/TUM/CI/CI-Homework_3/Problem 3/testInput23A.txt";
  char buff[100];
  int i = 0, flag = 0;
  Data data;
  SOM network;

  parseFile(path, &data);

  createSOM(&network, &data);

  return EXIT_SUCCESS;
}

void parseFile(char *path, Data *data) {
  FILE *file;
  int size = 100, i = 0;
  char buff[size];

  file = fopen(path, "r");

  if (file) {
    while (fgets(buff, size, (FILE *) file) != NULL) {
      parseLine(buff, &(data->cities[i]));
      i++;
    }
    data->size = i;
  } else {
    printf("Can't open file");
  }
  findMaxMin(data);
}

void parseLine(char *line, City *data) {
  char *token[3], *ptr;
  int i = 0;

  ptr = strtok(line, ",\n");

  while (ptr != NULL) {
    token[i] = ptr;
    ptr = strtok(NULL, ",\n");
    ++i;
  }

  data->Id = atoi(token[0]);
  data->Input1 = atoi(token[1]);
  data->Input2 = atoi(token[2]);
}

void findMaxMin(Data *data) {
  int max1 = data->cities[0].Input1;
  int max2 = data->cities[0].Input2;
  int min1 = data->cities[0].Input1;
  int min2 = data->cities[0].Input2;

  for (int i = 1; i < data->size; ++i) {
    if (data->cities[i].Input1 > max1)
      max1 = data->cities[i].Input1;
    if (data->cities[i].Input2 > max2)
      max2 = data->cities[i].Input2;
    if (data->cities[i].Input1 < min1)
      min1 = data->cities[i].Input1;
    if (data->cities[i].Input2 < min2)
      min2 = data->cities[i].Input2;
  }

  data->Max_1 = max1;
  data->Max_2 = max2;
  data->Min_1 = min1;
  data->Min_2 = min2;
}

void createSOM(SOM *network, Data *data) {
  int numberNeurons;
  numberNeurons = data->size + (int) ceil(data->size * POUR_TO_ADD);
  network->size = numberNeurons;
  createLayer(&(network->layer), numberNeurons, NBR_WEIGHTS, data);
}

void createLayer(Layer *layer, int numberNeurons, int numberWeights, Data *data) {
  layer->Neurons = (Neuron *) malloc(sizeof(Neuron) * numberNeurons);
  layer->size = numberNeurons;
  for (int i = 0; i < numberNeurons; ++i) {
    if (i == 0) {
      initialiseNeuron(&(layer->Neurons[i]),
                       &(layer->Neurons[i + 1]),
                       &(layer->Neurons[numberNeurons - 1]),
                       numberWeights,
                       data);
    } else if (i == numberNeurons - 1) {
      initialiseNeuron(&(layer->Neurons[i]), &(layer->Neurons[0]), &(layer->Neurons[i - 1]), numberWeights, data);
    } else {
      initialiseNeuron(&(layer->Neurons[i]), &(layer->Neurons[i + 1]), &(layer->Neurons[i - 1]), numberWeights, data);
    }
  }
}

void initialiseNeuron(Neuron *neuron, Neuron *next, Neuron *prev, int nbrWeights, Data *data) {
  int index;
  neuron->Next = next;
  neuron->Previous = prev;
  neuron->Weights = (double *) malloc(sizeof(double) * nbrWeights);
  neuron->Update = (double *) calloc(nbrWeights, sizeof(double));
  neuron->Weights[0] = rand() % (data->Max_1 + 1 - data->Min_1) + data->Min_1;
  neuron->Weights[1] = rand() % (data->Max_2 + 1 - data->Min_2) + data->Min_2;
  neuron->CityId = 0;
  neuron->Registred = 0;
}