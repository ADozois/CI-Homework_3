
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define NBR_INPUT_NEURONS 2
#define WEIGHT_MAX 0.7
#define LEARNING_RATE 0.01
#define EPOXH_MAX 100
#define LAMBDA 2


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
  int cluster;
};

struct Data {
  int size;
  int NbrCluster;
  double Max;
  double Min;
  Value Values[1000];
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

void updateWinning(Neuron *neuron, double input1, double input2, int epoch);

void pushLoser(Neuron *neuron, double input1, double input2, int epoch);

void pickWinner(VQ *network, double input1, double input2, int epoch);

void train(VQ *network, Data *data);

void printClusterCenters(VQ *network);

void printDebug(VQ *network);

double learningRate(int epoch);

void mean(Data *data, double *mean);

void sd(Data *data, double *sd, double *mean);

void iniatiliseWeights(Data *data, VQ *network);

double getDistance(double weight_1, double weight_2, double pos_1, double pos_2);

void kmean(Data *data, VQ *network);

double RandRange(double Min, double Max);

double max(Data *data, int input);

double min(Data *data, int input);

void kmeansPlusPlus(Data *data, VQ *network);

int maxJointDist(double *values, int size);


int main(void) {
  char *path = "/home/gemini/TUM/CI/CI-Homework_3/Problem 1/testInput21B.txt";
  char buff[100];
  int i = 0, flag = 0;
  Data data;
  VQ network;
  double mean_[2], sd_[2];

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

  //parseFile(path,&data);

  createVQ(&network, NBR_INPUT_NEURONS, data.NbrCluster, &data);

  kmeansPlusPlus(&data,&network);

  //printDebug(&network);

  train(&network,&data);

  printClusterCenters(&network);

  //printDebug(&network);

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
    if (data->NbrCluster == 0){
      return;
    }
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
  neuron->Output = 0;
  neuron->Delta = 0;
  neuron->Weights = (double *) calloc(nbrWeights, sizeof(double));
  neuron->Update = (double *) calloc(nbrWeights, sizeof(double));
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

void updateWinning(Neuron *neuron, double input1, double input2, int epoch) {
  neuron->Update[0] = learningRate(epoch) * (input1 - neuron->Weights[0]);
  neuron->Update[1] = learningRate(epoch) * (input2 - neuron->Weights[1]);
  neuron->Weights[0] = neuron->Weights[0] + neuron->Update[0];
  neuron->Weights[1] = neuron->Weights[1] + neuron->Update[1];
}

void pushLoser(Neuron *neuron, double input1, double input2, int epoch) {
  neuron->Update[0] = 0.00001 * (input1 - neuron->Weights[0]);
  neuron->Update[1] = 0.00001 * (input2 - neuron->Weights[1]);
  neuron->Weights[0] = neuron->Weights[0] - neuron->Update[0];
  neuron->Weights[1] = neuron->Weights[1] - neuron->Update[1];
}

void pickWinner(VQ *network, double input1, double input2, int epoch) {
  double x, y, dist, min = 1000000;
  int index = 0;

  for (int i = 0; i < network->Output.size; ++i) {
    x = network->Output.Neurons[i].Weights[0] - input1;
    y = network->Output.Neurons[i].Weights[1] - input2;
    dist = sqrt(pow(x-y,2.0));
    if (dist < min){
      min = dist;
      index = i;
    }
  }

  updateWinning(&(network->Output.Neurons[index]), input1, input2, epoch);

  /*for (int i = 0; i < network->Output.size; ++i) {
    if (i != index)
      pushLoser(&(network->Output.Neurons[i]), input1, input2, epoch);
  }*/
}

void train(VQ *network, Data *data){
  for (int j = 0; j < EPOXH_MAX; ++j) {
      kmean(data,network);
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
  printf("]\n");
}

double learningRate(int epoch){
  return LEARNING_RATE * (1 - (epoch/EPOXH_MAX));
}

void mean(Data *data, double *mean) {
  double sum[2] = {0,0};

  for (int i = 0; i < data->size; ++i) {
    sum[0] += data->Values[i].Input1;
    sum[1] += data->Values[i].Input2;
  }
  mean[0] = sum[0]/data->size;
  mean[1] = sum[1]/data->size;
}

void sd(Data *data, double *sd, double *mean) {
  double sum[2] = {0,0};
  for (int i = 0; i < data->size; ++i) {
    sum[0] += pow(data->Values[i].Input1 - mean[0],2.0);
    sum[1] += pow(data->Values[i].Input2 - mean[1],2.0);
  }
  sd[0] = sqrt(sum[0]/data->size);
  sd[1] = sqrt(sum[1]/data->size);
}

void iniatiliseWeights(Data *data, VQ *network){
  network->Output.Neurons[0].Weights[0] = data->Values[0].Input1;
  network->Output.Neurons[0].Weights[1] = data->Values[0].Input2;

  for (int i = 0; i < network->Output.size; ++i) {

  }
}

double getDistance(double weight_1, double weight_2, double pos_1, double pos_2) {
  double dist, diff_1, diff_2;

  diff_1 = (weight_1 - pos_1);
  diff_2 = (weight_2 - pos_2);

  dist = sqrt(pow(diff_1, 2.0) + pow(diff_2, 2.0));

  return dist;
}

void kmean(Data *data, VQ *network) {
  double tmp, dist = getDistance(network->Output.Neurons[0].Weights[0],
                                 network->Output.Neurons[0].Weights[1],
                                 data->Values[0].Input1,
                                 data->Values[0].Input2);
  int index = 0;
  double sum[network->Output.size];
  int nbr[network->Output.size];


  for (int i = 0; i < data->size; ++i) {
    for (int j = 0; j < network->Output.size; ++j) {
      tmp = getDistance(network->Output.Neurons[j].Weights[0],
                        network->Output.Neurons[j].Weights[1],
                        data->Values[i].Input1,
                        data->Values[i].Input2);
      if (tmp < dist) {
        dist = tmp;
        index = j;
      }
    }
    dist = 100000000;
    data->Values[i].cluster = index;
  }

  for (int k = 0; k < network->Output.size; ++k) {
    sum[k] = 0;
    nbr[k] = 0;
  }

  for (int l = 0; l < data->size; ++l) {
    sum[data->Values[l].cluster] += data->Values[l].Input1;
    nbr[data->Values[l].cluster] += 1;
  }
  for (int m = 0; m < network->Output.size; ++m) {
    network->Output.Neurons[m].Weights[0] = sum[m]/nbr[m];
  }

  for (int k = 0; k < network->Output.size; ++k) {
    sum[k] = 0;
    nbr[k] = 0;
  }

  for (int l = 0; l < data->size; ++l) {
    sum[data->Values[l].cluster] += data->Values[l].Input2;
    nbr[data->Values[l].cluster] += 1;
  }
  for (int m = 0; m < network->Output.size; ++m) {
    network->Output.Neurons[m].Weights[1] = sum[m]/nbr[m];
  }
}

double RandRange(double Min, double Max)
{
  double diff = Max-Min;
  return  (((diff+1)/RAND_MAX) * rand() + Min);
}

double max(Data *data, int input){
  double tmp, value;

  if (input == 1){
    value = data->Values[0].Input1;
  } else{
    value = data->Values[0].Input2;
  }
  for (int i = 0; i < data->size; ++i) {
    if (input == 1){
      tmp = data->Values[i].Input1;
    } else{
      tmp = data->Values[i].Input2;
    }

    if(tmp > value){
      value = tmp;
    }
  }
  return value;
}

double min(Data *data, int input){
  double tmp, value;

  if (input == 1){
    value = data->Values[0].Input1;
  } else{
    value = data->Values[0].Input2;
  }
  for (int i = 0; i < data->size; ++i) {
    if (input == 1){
      tmp = data->Values[i].Input1;
    } else{
      tmp = data->Values[i].Input2;
    }

    if(tmp < value){
      value = tmp;
    }
  }
  return value;
}

void kmeansPlusPlus(Data *data, VQ *network){
  int centers[data->NbrCluster];
  int index = 0, nbrInit = 0;
  double jointDist[data->size];

  for (int l = 0; l < data->NbrCluster; ++l) {
    centers[l] = 0;
  }

  for (int l = 0; l < data->size; ++l) {
    jointDist[l] = 0;
  }

  index =  (int)floor((data->size+1)*(double)rand()/RAND_MAX);
  centers[0] = index;
  nbrInit = 1;
  network->Output.Neurons[0].Weights[0] = data->Values[index].Input1;
  network->Output.Neurons[0].Weights[1] = data->Values[index].Input2;


  for (int i = 0; i < data->NbrCluster-1; ++i) {
    for (int k = 0; k < nbrInit; ++k) {
      for (int j = 0; j < data->size; ++j) {
        jointDist[j] += getDistance(data->Values[centers[k]].Input1,data->Values[centers[k]].Input2,data->Values[j].Input1,data->Values[j].Input2);
      }
    }
    index = maxJointDist(jointDist,data->size);
    network->Output.Neurons[nbrInit].Weights[0] = data->Values[index].Input1;
    network->Output.Neurons[nbrInit].Weights[1] = data->Values[index].Input2;
    centers[nbrInit] = index;
    nbrInit++;
    for (int l = 0; l < data->size; ++l) {
      jointDist[l] = 0;
    }
  }
}

int maxJointDist(double *values, int size) {
  double tmp, max = values[0];
  int index = 0;
  for (int i = 1; i < size; ++i) {
    tmp = values[i];
    if (tmp > max){
      max = tmp;
      index = i;
    }
  }
  return index;
}