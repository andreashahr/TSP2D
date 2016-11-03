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
clock_t start, now;

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
  int maxdist = 0, maxdAt = 0;
  for(int i = 0; i < N-1; i++) {
    if(distanceMatrix[i][i+1] > maxdist) {
      maxdist = distanceMatrix[i][i+1];
      maxdAt = i+1;
    }
  }
  from = maxdAt;
  tour[0] = from; // set start location
  for(int i = 1; i < N; i++) {
    locations[from].visited = true; // location is now visited
    int minDistance = 1000001; // smallest distance
    int nextFrom = 0;
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
  int new_distance = tourLength(0, N, tour); // beat current tour
  int edgediff, best_diff, tempM = 0;
  int* temptour; // for swapping tour <-> newtour ptrs

  for(int i = 1; i < N-1; i++) {
    now = clock();
    if((double) (now - start)/CLOCKS_PER_SEC > 1.98) return (void) 0;

    for(int k = i+1; k < N-1; k++) {

      edgediff =
      distanceMatrix[tour[i-1]][tour[i]]+distanceMatrix[tour[k]][tour[k+1]] -
      distanceMatrix[tour[i-1]][tour[k]]-distanceMatrix[tour[k+1]][tour[i]];

      if(edgediff > 0) {
        twoOptSwap(newtour, i, k);
        best_diff = edgediff;
        tempM = k+1;
        for(int m = k+1; m < N-2; m++) {
          edgediff += distanceMatrix[newtour[k]][newtour[m+1]];
          edgediff -= distanceMatrix[newtour[k]][newtour[m+2]];
          if(edgediff <= 0) break;
          if(edgediff > best_diff) {
            best_diff = edgediff;
            tempM = m;
          }
        }
        int loctmp = newtour[k+1];
        while(tempM >= k) {
          k++
          newtour[k+1] = newtour[k+2];
        }
        newtour[k+2] = loctmp;

        temptour = tour;
        tour = newtour;
        newtour = tour;

        i = 0;
        break;
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
