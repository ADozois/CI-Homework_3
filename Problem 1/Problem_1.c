
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define NBR_INPUT_NEURONS 2
#define WEIGHT_MAX 0.7
#define LEARNING_RATE 0.01


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

void createVQ(VQ *network, int neuronsInput, int neuronsOutput, Data *data);

void createInputLayer(VQ *network, int numberNeurons, Data *data);

void createOutputLayer(VQ *network, int numberNeurons, Data *data);

void createLayer(Layer *layer, int numberNeurons, int numberWeights, Data *data);

void initialiseNeuron(Neuron *neuron, int nbrWeights, Data *data);

void computeInput(VQ *network, double input1, double input2);

void computeActivation(Neuron *neuron, double input1, double input2);

void computeOutput(VQ *network);

void computeVQ(VQ *network, double input1, double input2);

void updateWinning(Neuron *neuron, double input1, double input2);

void pickWinner(VQ *network, double input1, double input2);

void train(VQ *network, Data *data);

void printClusterCenters(VQ *network);

void printDebug(VQ *network);


int main(void) {
  char *path = "/home/gemini/TUM/CI/CI-Homework_3/Problem 1/testInput21A.txt";
  char buff[100];
  int i = 0, flag = 0;
  Data data;
  VQ network;

  srand((unsigned) time(NULL)); //Seed initialisation

  while(scanf("%s",buff) == 1) {
    if (flag == 0) {
      data.NbrCluster = atoi(buff);
      flag = 1;
    } else{
      parseLine(buff, &(data.Values[i]));
      ++i;
    }
  }
  data.size = i;

  createVQ(&network, NBR_INPUT_NEURONS, data.NbrCluster, &data);

  train(&network,&data);

  printClusterCenters(&network);

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

void createVQ(VQ *network, int neuronsInput, int neuronsOutput, Data *data) {
  createInputLayer(network, neuronsInput, data);
  createOutputLayer(network, neuronsOutput, data);
}

void createInputLayer(VQ *network, int numberNeurons, Data *data) {
  createLayer(&network->Input, numberNeurons, 0, data);
}

void createOutputLayer(VQ *network, int numberNeurons, Data *data) {
  createLayer(&network->Output, numberNeurons, 2, data);
}

void createLayer(Layer *layer, int numberNeurons, int numberWeights, Data *data) {
  layer->Neurons = (Neuron *) malloc(sizeof(Neuron) * numberNeurons);
  layer->size = numberNeurons;
  layer->Previous = NULL;
  layer->Next = NULL;
  for (int i = 0; i < numberNeurons; ++i) {
    initialiseNeuron(&(layer->Neurons[i]), numberWeights, data);
  }
}

void initialiseNeuron(Neuron *neuron, int nbrWeights, Data *data) {
  int index;
  neuron->Output = 0;
  neuron->Delta = 0;
  neuron->Weights = (double *) malloc(sizeof(double) * nbrWeights);
  neuron->Update = (double *) calloc(nbrWeights, sizeof(double));
  index =  (int)floor((data->size+1)*(double)rand()/RAND_MAX);
  neuron->Weights[0] = data->Values[index].Input1;
  neuron->Weights[1] = data->Values[index].Input2;
}

void computeInput(VQ *network, double input1, double input2){
  network->Input.Neurons[0].Output = input1;
  network->Input.Neurons[1].Output = input2;
}

void computeActivation(Neuron *neuron, double input1, double input2){
  neuron->Output = input1 * neuron->Weights[0] + input2 * neuron->Weights[1];
}

void computeOutput(VQ *network){
  for (int i = 0; i < network->Output.size; ++i) {
    computeActivation(&(network->Output.Neurons[i]),network->Input.Neurons[0].Output,network->Input.Neurons[1].Output);
  }
}

void computeVQ(VQ *network, double input1, double input2){
  computeInput(network, input1, input2);
  computeOutput(network);
}

void updateWinning(Neuron *neuron, double input1, double input2){
  neuron->Update[0] = LEARNING_RATE * (input1 - neuron->Weights[0]);
  neuron->Update[1] = LEARNING_RATE * (input2 - neuron->Weights[1]);
  neuron->Weights[0] = neuron->Weights[0] + neuron->Update[0];
  neuron->Weights[1] = neuron->Weights[1] + neuron->Update[1];
}

void pickWinner(VQ *network, double input1, double input2){
  double x, y, dist, min = 1000000;
  int index = 0;

  for (int i = 0; i < network->Output.size; ++i) {
    x = network->Output.Neurons[i].Weights[0] - input1;
    y = network->Output.Neurons[i].Weights[1] - input2;
    dist = sqrt(pow(x,2.0) + pow(y,2.0));
    if (dist < min){
      min = dist;
      index = i;
    }
  }

  updateWinning(&(network->Output.Neurons[index]), input1, input2);
}

void train(VQ *network, Data *data){
  for (int j = 0; j < 10000; ++j) {
    for (int i = 0; i < data->size; ++i) {
      pickWinner(network,data->Values[i].Input1,data->Values[i].Input2);
    }
  }

}

void printClusterCenters(VQ *network){
  for (int i = 0; i < network->Output.size; ++i) {
    printf("%f,%f\n",network->Output.Neurons[i].Weights[0],network->Output.Neurons[i].Weights[1]);
  }
}

void printDebug(VQ *network){
  for (int i = 0; i < network->Output.size; ++i) {
    printf("Cluster %d: %f, %f\n",i+1,network->Output.Neurons[i].Weights[0],network->Output.Neurons[i].Weights[1]);
  }
  printf("[");
  for (int i = 0; i < network->Output.size; ++i) {
    printf("[%f, %f]",network->Output.Neurons[i].Weights[0],network->Output.Neurons[i].Weights[1]);
    if (i < network->Output.size -1 )
      printf(", ");
  }
  printf("]");
}