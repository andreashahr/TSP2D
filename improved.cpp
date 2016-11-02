#include <iostream>
#include <cmath>
#include <cstdlib>
#include <new>
#include <ctime>
using namespace std;

class Location {
public:
  double x;
  double y;
  bool visited = false;
};

// functions
int distance(Location);
void greedyTour(Location*);
int tourLength(int, int, int*);
void twoOpt();
void twoOptSwap(int*, int, int);
void printTourCoordinates(Location*);
int distance(Location, Location);

// global variables
int** distanceMatrix;
int* tour;
int N;
clock_t start;

int main() {
  start = clock();
  string line; // input holder
  getline(cin, line); // read first line from stdin
  N = stoi(line); // first line is locations count

  Location* locations = new Location[N]; // allocate locations array
  tour = new int[N]; // allocate tour array

  // allocate distanceMatrix
  distanceMatrix = new int*[N];
  for(int i = 0; i < N; i++) {
    distanceMatrix[i] = new int[N];
  }

  // setup locations array
  for(int i = 0, space = 0; i < N; i++, space = 0) {
    getline(cin, line); // read next line from stdin
    while(true) {
      if(line[space] == ' ') { // find space character
        line[space] = '\0'; // end string after x-value
        break;
      }
      space++;
    }
    locations[i].x = stod(line); // x now ends at index: space
    locations[i].y = stod(&line[space+1]); // y starts at index: space+1
  }

  // setup distances matrix
  for(int i = 0; i < N-1; i++) {
    for(int j = i+1; j < N; j++) {
      distanceMatrix[i][j] = distanceMatrix[j][i] = distance(locations[i], locations[j]);
    }
  }

  greedyTour(locations);
  twoOpt();

  for(int i = 0; i < N; i++) {
    cout << tour[i] << "\n";
  }

  return 0;
}

// sets global var tour to greedy
void greedyTour(Location* locations) {
  int from = 0; // start location
  tour[0] = from; // set start location
  for(int i = 1; i < N; i++) {
    locations[from].visited = true; // location is now visited
    int minDistance = 1000001; // smallest distance
    int nextFrom = -1;
    // find smallest distance
    for(int to = 0; to < N; to++) {
      if(locations[to].visited) continue;
      if(distanceMatrix[from][to] < minDistance) {
        minDistance = distanceMatrix[from][to];
        nextFrom = to;
      }
    }
    tour[i] = nextFrom;
    from = nextFrom;
  }
}

int tourLength(int from, int to, int* atour) {
  int sum = 0;
  for(int i = from; i < to-1; i++) {
    sum += distanceMatrix[atour[i]][atour[i+1]];
  }
  return sum;
}

void twoOpt() {
  int* newtour = new int[N]; // holds improved tour attempt
  int best_distance = tourLength(0, N, tour); // beat current tour
  int new_distance; // ...with new distance
  int* temptour; // for swapping tour <-> newtour ptrs
  int optimize = N-1;
  while(optimize) {
    optimize--;
    for(int i = 0; i < N-1; i++) {
      for(int k = i+1; k < N; k++) {
        twoOptSwap(newtour, i, k);
        new_distance = tourLength(0, N, newtour);
        if(new_distance < best_distance) {
          best_distance = new_distance;
          temptour = tour;
          tour = newtour;
          newtour = temptour;
          optimize = N-1;
        }
      }
      if((((double) clock()-(double)start)/(double)CLOCKS_PER_SEC) > 1.98) {
        /*optimize = false;
        break;*/
        return (void) 0;
      }
    }
  }
}

void twoOptSwap(int* newtour, int i, int k) {
  for(int j = 0; j < i; j++) {
    newtour[j] = tour[j];
  }
  for(int j = i; j <= k; j++) {
    newtour[j] = tour[k+i-j];
  }
  for(int j = k+1; j < N; j++) {
    newtour[j] = tour[j];
  }
}

int distance(Location loc1, Location loc2) {
  return round(sqrt((loc1.x-loc2.x)*(loc1.x-loc2.x)+(loc1.y-loc2.y)*(loc1.y-loc2.y)));
}

void printTourCoordinates(Location* locations) {
  for(int i = 0; i < N; i++) {
    cout << locations[tour[i]].x << " " << locations[tour[i]].y << "\n";
  }
  cout << "\n";
}
