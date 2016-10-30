#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <new>
#include <vector>
#include <limits>
using namespace std; // for not having to type std:: everywhere'


// r -path /cygdrive/c/Users/Henrik/Desktop/c++/TSP2D/Greedy/ < input.txt


const int MAX_TWO_OPT_DISTANCE = 40;
const int TWO_OPT_ROUNDS = 5;

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

// use stdin input as blueprint to set up locations vector
vector<Location> initLocationVector() {
  vector<Location> v; // locations to visit
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
    v.push_back(*(new Location(i,x,y))); // add location i to vector
  }
  return v;
}

// returns greedy tour as vector (pos 0 to n)
vector<Location> greedyTour(vector<Location> v) {
  vector<Location> path;
  int locIndex = 0; // start location index
  for(int n = 0; n < v.size(); n++) {
    Location* currentLocation = &v.at(locIndex); // location ptr
    currentLocation->visited = true; // location is now visited
    path.push_back(*currentLocation); // add location to path
    int shortestPath = numeric_limits<int>::max(); // minimize shortest path, maxInt
    for(int i = 0; i < v.size(); i++) {
      if(true == v.at(i).visited) continue;
      if(currentLocation->distance(v.at(i)) < shortestPath) {
        locIndex = i;
        shortestPath = currentLocation->distance(v.at(i));
      }
    }
  }
  return path;
}

int path_distance(vector<Location> path) {
  int size = path.size();
  int distance = 0;
  for(int i = 0; i < path.size(); i++) {
    distance += path[i].distance(path[(i + 1) % size]);
  }
  return distance;
}


int path_distance(vector<Location> path, int start, int path_length) {
  int size = path.size();
  int distance = 0;
  for(int i = 0; i <= path_length; i++) {
    int j = (i + start) % size;
    int next_j = (j + 1) % size;
    distance += path[j].distance(path[next_j]);
  }
  return distance;
}


vector<Location> two_opt(vector<Location> path) {
  int size = path.size();
  if (size < 4) return path;
  // set distance to path length initially
  int two_opt_distance = size - 4;
  // if distance is more than boundary, decrease to boundary
  if (two_opt_distance > MAX_TWO_OPT_DISTANCE) {
    two_opt_distance = MAX_TWO_OPT_DISTANCE;
  }

  int k = 0;
  while (k < TWO_OPT_ROUNDS) {
    k++;
    for(int u = 0; u < path.size(); u++) {

      // one edge is (u,v), the other is (w,x)
      int v = (u + 1) % size;

      // setting x to a random value
      int x = u + 3 + (rand() % two_opt_distance); // u + 0 = u, u + 1 = v, u + 2 = (maybe) w
      int path_length = x - u;
      int w = (x - 1) % size;
      x = x % size;
      int pd = path_distance(path, u, path_length);
      swap(path[v], path[w]);
      int new_pd = path_distance(path, u, path_length);
      if (new_pd > pd) {
        //change back if result was worse
        swap(path[v], path[w]);
      } 
    }
  }
  return path;
}

int main() {
  vector<Location> path = initLocationVector();
  path = greedyTour(path);
  cout << "greedy path distance " << path_distance(path) << endl;
  path = two_opt(path);
  cout << "two opt path distance " << path_distance(path) << endl;
  for(int i = 0; i < path.size(); i++) {
    cout << path[i].id << "\n";
  }
  return 0;
}
