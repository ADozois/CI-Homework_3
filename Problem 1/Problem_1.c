
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define NBR_INPUT_NEURONS 2
#define WEIGHT_MAX 0.001

typedef struct Data Data;
typedef struct Value Value;
typedef struct Neuron Neuron;
typedef struct VQ VQ;
typedef struct Layer Layer;

struct Layer {
  Neuron *Neurons;
  int size;
  Layer *Next;
  Layer *Previous;
};

struct Value {
  double Input1;
  double Input2;
};

struct Data {
  int size;
  int NbrCluster;
  double Max;
  double Min;
  Value Values[100];
};

struct VQ {
  Layer Input;
  Layer Output;
};

struct Neuron {
  double *Weights;
  double Output;
  double Delta;
  double *Update;
};

void parseFile(char *file, Data *data);

void parseLine(char *line, Value *data);

void createVQ(VQ *network, int neuronsInput, int neuronsOutput);

void createInputLayer(VQ *network, int numberNeurons);

void createOutputLayer(VQ *network, int numberNeurons);

void createLayer(Layer *layer, int numberNeurons, int numberWeights);

void initialiseNeuron(Neuron *neuron, int nbrWeights);

double linearFunc(double input);

int main(void) {
  char *path = "/home/gemini/TUM/CI/CI-Homework_3/Problem 1/testInput21A.txt";
  Data data;
  VQ network;

  parseFile(path, &data);

  createVQ(&network, NBR_INPUT_NEURONS, data.NbrCluster);

  return 0;
}

void parseFile(char *path, Data *data) {
  FILE *file;
  int size = 100, i = 0;
  char buff[size];

  file = fopen(path, "r");

  if (file) {
    fgets(buff, size, (FILE *) file);
    data->NbrCluster = atoi(buff);
    while (fgets(buff, size, (FILE *) file) != NULL) {
      parseLine(buff, &(data->Values[i]));
      i++;
    }
    data->size = i;
  } else {
    printf("Can't open file");
  }
}

void parseLine(char *line, Value *data) {
  char *token[3], *ptr;
  int i = 0;

  ptr = strtok(line, ",\n");

  while (ptr != NULL) {
    token[i] = ptr;
    ptr = strtok(NULL, ",\n");
    ++i;
  }

  data->Input1 = strtod(token[0], NULL);
  data->Input2 = strtod(token[1], NULL);
}

void createVQ(VQ *network, int neuronsInput, int neuronsOutput) {
  createInputLayer(network, neuronsInput);
  createOutputLayer(network, neuronsOutput);
}

void createInputLayer(VQ *network, int numberNeurons) {
  createLayer(&network->Input, numberNeurons, 0);
}

void createOutputLayer(VQ *network, int numberNeurons) {
  createLayer(&network->Output, numberNeurons, 2);
}

void createLayer(Layer *layer, int numberNeurons, int numberWeights) {
  layer->Neurons = (Neuron *) malloc(sizeof(Neuron) * numberNeurons);
  layer->size = numberNeurons;
  layer->Previous = NULL;
  layer->Next = NULL;
  for (int i = 0; i < numberNeurons; ++i) {
    initialiseNeuron(&(layer->Neurons[i]), numberWeights);
  }
}

void initialiseNeuron(Neuron *neuron, int nbrWeights) {
  int i = 0;
  neuron->Output = 0;
  neuron->Delta = 0;
  neuron->Weights = (double *) malloc(sizeof(double) * nbrWeights);
  neuron->Update = (double *) calloc(nbrWeights, sizeof(double));
  for (i = 0; i < nbrWeights; ++i) {
    neuron->Weights[i] = WEIGHT_MAX * ((double) rand() / (double) RAND_MAX - 0.5);
  }
}

double linearFunc(double input) {
  return input;
}
