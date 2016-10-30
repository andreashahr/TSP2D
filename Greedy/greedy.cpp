#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <new>
#include <vector>
#include <limits>
using namespace std;

class Location {
public:
  Location(int id, double x, double y) {
    this->id = id;
    this->x = x;
    this->y = y;
    this->visited = false;
  }
  int distance(Location loc) {
    return round(sqrt((loc.x-x)*(loc.x-x)+(loc.y-y)*(loc.y-y)));
  }
  int id;
  double x;
  double y;
  bool visited;
};

vector<Location> initLocationVector() {
  vector<Location> v; // locations to visit
  string line; // input holder
  double x, y;  // location coordinates
  int space = 4, locsCnt;
  getline(cin, line); // read line from stdin
  locsCnt = stoi(line); // first is locations count
  for(int i = 0; i < locsCnt; i++, space = 4) {
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

vector<int> greedyTour(vector<Location> v) {
  vector<int> path;
  int locIndex = 0;
  for(int n = 0; n < v.size(); n++) {
    Location* currentLocation = &v.at(locIndex);
    currentLocation->visited = true;
    path.push_back(currentLocation->id);
    int shortestPath = numeric_limits<int>::max();
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
