#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <new>
#include <vector>
#include <limits>
using namespace std; // for not having to type std:: everywhere

const int MAX_2_OPT_DISTANCE = 20;
const int 2_OPT_ROUNDS = 100;

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


vector<Location> two_opt(vector<Location> path) {
  int path_length = path.size();
  // set distance to path length initionally
  int 2_opt_distance = path_length; 
  // if distance is more than boundry, decrease to boundry
  if (2_opt_distance > MAX_2_OPT_DISTANCE) {
    2_opt_distance = MAX_2_OPT_DISTANCE;
  }
  //init som variables
  int v, w, x, org_route, new_route, k = 0;
  
  while (k < 2_OPT_ROUNDS) {
    k++;
    for(int u = 0; u < path_length; u++) {
      // one edge is (u,v), the other is (w,x)
      v = u + 1 % path_length;
      // setting w to a random value
      w = (rand() % 2_opt_distance) % path_length;
      //changing w if (w,x) will overlap (u,v)
      if (w <= v && w >= u-1) w = (v + 1) % path_length; 
      x = (w + 1) % path_length;

      int u_v = (&path[u])->distance(path[v]);
      int w_x = (&path[w])->distance(path[x]);
      org_route = u_v + w_x;

      int u_w = (&path[u])->distance(path[w]);
      int v_x = (&path[v])->distance(path[x]);
      new_route = u_w + v_x;
      
      // Printing some shit
      cout << "u " << u << endl;
      cout << "v " << v << endl;
      cout << "w " << w << endl;
      cout << "x " << x << endl;
      cout << "org_route " << org_route << endl;
      cout << "new_route " << new_route << endl;

      // swap if new route is better
      if (new_route < org_route) {
        swap(path[w],path[v]);
      }
    }
  }
  return path;
}

int main() {
  vector<Location> locations = initLocationVector();
  //vector<Location> path = greedyTour(locations);
  vector<Location> path = two_opt(locations);
  for(int i = 0; i < path.size(); i++) {
    cout << (&path[i])->id << "\n";
  }
  return 0;
}
