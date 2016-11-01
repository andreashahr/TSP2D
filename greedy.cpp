#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <new>
#include <vector>
#include <limits>
using namespace std; // for not having to type std:: everywhere

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
vector<int> greedyTour(vector<Location> v) {
  vector<int> path;
  int locIndex = 0; // start location index
  for(int n = 0; n < v.size(); n++) {
    Location* currentLocation = &v.at(locIndex); // location ptr
    currentLocation->visited = true; // location is now visited
    path.push_back(currentLocation->id); // add location to path
    int shortestPath = numeric_limits<int>::max(); // minimize shortest path
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

int main() {
  vector<Location> locations = initLocationVector();
  vector<int> path = greedyTour(locations);
  for(int i = 0; i < path.size(); i++) {
    cout << path.at(i) << "\n";
  }
  return 0;
}
