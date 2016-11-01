#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <new>
#include <vector>
#include <limits>
using namespace std; // for not having to type std:: everywhere'

class Location {
public:
  // constructor
  Location(int id, double x, double y) {
    this->id = id;
    this->x = x;
    this->y = y;
    this->visited = false;
  }
  // eucledian distance from this to arg: loc
  int distance(Location loc) {
    return round(sqrt((loc.x-x)*(loc.x-x)+(loc.y-y)*(loc.y-y)));
  }
  int id; // the nth location from stdin
  double x;
  double y;
  bool visited;
};

int size = 0;
vector<vector<int>> distances;
vector<Location> initPath;
vector<Location> path;


void initDistanceMatrix() {
  distances.reserve(size);
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < size; j++) {
      distances[i].push_back(initPath[i].distance(initPath[j]));
    }
  }
}

// use stdin input as blueprint to set up locations vector
void initLocationVector() {
  string line; // input holder
  double x, y;  // location coordinates
  int space = 0, locsCnt;
  getline(cin, line); // read line from stdin
  locsCnt = stoi(line); // first is locations count
  for(int i = 0; i < locsCnt; i++, space = 0) {
    getline(cin, line); // read next line from stdin
    while(true) {
      if(line[space] == ' ') { // find space character
        line[space] = '\0'; // end string after x-value
        break;
      }
      space++;
    }
    x = stod(line); // now ends at index: space
    y = stod(&line[space+1]); // starts at index: space+1
    initPath.push_back(*(new Location(i,x,y))); // add location i to vector
  }
  size = initPath.size();
}

int pathDistance() {
  int distance = 0;
  for(int i = 0; i < size; i++) {
    int id = path.at(i).id;
    int next_id = path.at((i + 1) % size).id;
    distance += distances[id][next_id];
  }
  return distance;
}


int pathDistance(int start, int path_length) {
  int distance = 0;
  for(int i = 0; i <= path_length; i++) {
    int j = (i + start) % size;
    int next_j = (j + 1) % size;
    int id = path.at(j).id;
    int next_id = path.at(next_j).id;
    distance += distances[id][next_id];
  }
  return distance;
}


void mix(vector<Location> v) {
  for(int i = 0; i < v.size(); i++) {
    swap(path[rand() % v.size()], path[rand() % size]);
  }
}

// returns greedy tour as vector (pos 0 to n)
void greedyTour() {
  int locIndex = 0; // start location index
  for(int n = 0; n < size; n++) {
    Location* currentLocation = &initPath.at(locIndex); // location ptr
    currentLocation->visited = true; // location is now visited
    path.push_back(*currentLocation); // add location to path
    int shortestPath = numeric_limits<int>::max(); // minimize shortest path, maxInt
    for(int i = 0; i < initPath.size(); i++) {
      if(true == initPath.at(i).visited) continue;
      if(currentLocation->distance(initPath.at(i)) < shortestPath) {
        locIndex = i;
        shortestPath = currentLocation->distance(initPath.at(i));
      }
    }
  }
}


void twoOpt() {
  if (size < 4) return;
  for(int u = 0; u < size; u++) {
    // one edge is (u,v), the other is (w,x)
    int v = (u + 1) % size;
    int pd = pathDistance();
    for (int k = 0; k < (size-3); k++) {
      int w = (u + 2 + k) % size;
      int x = (w + 1) % size;
      swap(path[v], path[w]);
      int new_pd = pathDistance();
      if (new_pd < pd) {
        u = 0; // goto start again
        break;
      } else {
        // change back if result was worse
       swap(path[v], path[w]);
     }
   }
 }
}



int main() {
  initLocationVector();
  initDistanceMatrix();
  greedyTour();
  initPath.clear();
  cout << "greedy path distance " << pathDistance() << endl;
  twoOpt();
  cout << "two opt path distance " << pathDistance() << endl;
  
  for(int i = 0; i < size; i++) {
    cout << path[i].id << "\n";
  }
  return 0;
}
