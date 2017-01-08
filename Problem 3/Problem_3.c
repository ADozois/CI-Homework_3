
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define POUR_TO_ADD 0.1
#define NBR_WEIGHTS 2
#define LEARNING_RATE 0.01
#define EPOCH_MAX 1000000
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

int bestMatchingUnit(SOM *network, City city);

double getDistance(double weight_1, double weight_2, int pos_1, int pos_2);

void updateWeights(SOM *network, int index, City city, int epoch);

double getSigma_o(SOM *network);

double getLambda(int epoch, double sigma);

int getRadius(int epoch, double sigma, double lambda);

double learningRate(int epoch);

double theta(int epoch, int radius, int dist);

void train(SOM *network, Data *data);

void printPos(SOM *network);

void printRoute(SOM *network);

void printDebug(SOM *network);

int main(void) {
  char *path = "/home/gemini/TUM/CI/CI-Homework_3/Problem 3/testInput23B.txt";
  char buff[100];
  int i = 0, flag = 0;
  Data data;
  SOM network;

  srand((unsigned) time(NULL)); //Seed initialisation


  //TODO: Implement timer, check in the homework page!!!

  parseFile(path, &data);

  createSOM(&network, &data);

  printDebug(&network);

  train(&network, &data);

  printDebug(&network);

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

int bestMatchingUnit(SOM *network, City city) {
  int index;
  double min_dist, tmp_dist;

  min_dist =
      getDistance(network->layer.Neurons[0].Weights[0], network->layer.Neurons[0].Weights[1], city.Input1, city.Input2);
  index = 0;

  for (int i = 1; i < network->size; ++i) {
    tmp_dist = getDistance(network->layer.Neurons[i].Weights[0],
                           network->layer.Neurons[i].Weights[1],
                           city.Input1,
                           city.Input2);
    if (tmp_dist < min_dist) {
      min_dist = tmp_dist;
      index = i;
    }
  }

  return index;
}

double getDistance(double weight_1, double weight_2, int pos_1, int pos_2) {
  double dist, diff_1, diff_2;

  diff_1 = (weight_1 - (double) pos_1);
  diff_2 = (weight_2 - (double) pos_2);

  dist = sqrt(pow(diff_1, 2.0) + pow(diff_2, 2.0));

  return dist;
}

void updateWeights(SOM *network, int index, City city, int epoch) {
  int radius, pos;
  double learning_rate, influence;
  double sigma_o = getSigma_o(network);
  double lambda = getLambda(epoch, sigma_o);

  radius = getRadius(epoch, sigma_o, lambda);
  learning_rate = learningRate(epoch);

  for (int i = -radius; i < radius; ++i) {
    influence = theta(epoch, radius, index + i);
    if (index + i < 0) {
      pos = network->size + index + i;
    } else if (index + i >= network->size) {
      pos = index + i - network->size;
    } else {
      pos = index + i;
    }
    network->layer.Neurons[pos].Update[0] =
        learning_rate * influence * (city.Input1 - network->layer.Neurons[pos].Weights[0]);
    network->layer.Neurons[pos].Update[1] =
        learning_rate * influence * (city.Input2 - network->layer.Neurons[pos].Weights[1]);
    network->layer.Neurons[pos].Weights[0] =
        network->layer.Neurons[pos].Weights[0] + network->layer.Neurons[pos].Update[0];
    network->layer.Neurons[pos].Weights[1] =
        network->layer.Neurons[pos].Weights[1] + network->layer.Neurons[pos].Update[1];
  }

}

double getSigma_o(SOM *network) {
  return (double) network->size / 2.0;
}

double getLambda(int epoch, double sigma) {
  return (epoch) / (log(sigma));
}

int getRadius(int epoch, double sigma, double lambda) {
  return (int) ceil(sigma * exp((-epoch) / lambda));
}

double learningRate(int epoch) {
  return LEARNING_RATE * exp((-epoch) / EPOCH_MAX);
}

double theta(int epoch, int radius, int dist) {
  return exp((-pow(dist, 2.02)) / (2 * pow(radius, 2.0)));
}

void train(SOM *network, Data *data) {
  int index;
  for (int i = 0; i < EPOCH_MAX; ++i) {
    for (int j = 0; j < data->size; ++j) {
      index = bestMatchingUnit(network, data->cities[j]);
      if (network->layer.Neurons[index].Registred == 0 || network->layer.Neurons[index].CityId == data->cities[j].Id){
        updateWeights(network, index, data->cities[j], i);
        network->layer.Neurons->CityId = data->cities[j].Id;
        network->layer.Neurons->Registred = 1;
      }
    }
  }
}

void printPos(SOM *network) {
  for (int i = 0; i < network->size; ++i) {
    printf("%f, %f\n", network->layer.Neurons[i].Weights[0], network->layer.Neurons[i].Weights[1]);
  }
}

void printRoute(SOM *network){
  for (int i = 0; i < network->size; ++i) {
    printf("%d\n",network->layer.Neurons[i].CityId);
  }
}

void printDebug(SOM *network){
  printf("[");
  for (int i = 0; i < network->size; ++i) {
    printf("[%d, %f, %f]", network->layer.Neurons[i].CityId, network->layer.Neurons[i].Weights[0],network->layer.Neurons[i].Weights[1]);
    if (i != network->size - 1){
      printf(", ");
    }
  }
  printf("]\n");
}
