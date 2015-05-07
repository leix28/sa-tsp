#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#define START_T 1000.0    //start temperature
#define END_T 1e-4        //terminte temperature
#define DECEND 0.8        //decend factor of temperature
#define LOOP 10000        //loop in each temperature
using namespace std;

pair<double, double> pos[30];
double dis[30][30];             //all x to y distance
string id2name[30];
pair<int, int> actlist[1000];   //create a list of all possible swaps
int n, m, current[30];
double currentv;


class Randomizer{
  //this is a randomizer which generates int in range[0, 2^64 - 1]
  unsigned long long num;
public:
  const unsigned long long MAX;
  Randomizer() : MAX(-1) {
    num = time(0);
  }
  unsigned long long next_random() {
    return num = num * (unsigned long long)25214903917 + 11;
  }
} randomizer;


double sqr(double x) {return x * x; }

double calcdis(pair<double, double> &a, pair<double, double> &b) {
  return sqrt(sqr(a.first - b.first) + sqr(a.second - b.second));
}

void init() {

  //read position information
  for (int i = 0; i < n; i++) {
    string name;
    double x, y;
    cin >> name >> x >> y;
    id2name[i] = name;
    pos[i] = make_pair(x, y);
  }


  //calc all distance
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      dis[i][j] = calcdis(pos[i], pos[j]);


  //make action list
  for (int i = 1; i < n; i++)
    for (int j = i + 1; j < n; j++)
      actlist[m++] = make_pair(i, j);

  //initial route 0->1->2->...->(n-1)->0
  for (int i = 0; i < n; i++) {
    current[i] = i;
    currentv += dis[i][(i + 1 == n) ? 0 : (i + 1)];
  }
}


void random_update(double t) {
  //use an action in actlist to generate a new route
  pair<int, int> act = actlist[randomizer.next_random() % m];
  double v = currentv - dis[current[act.first - 1]][current[act.first]]
                      - dis[current[act.second]][current[(act.second + 1) == n ? 0 : (act.second + 1)]]
                      + dis[current[act.first - 1]][current[act.second]]
                      + dis[current[act.first]][current[(act.second + 1) == n ? 0 : (act.second + 1)]];

  //accept new status by chance
  if (v < currentv || randomizer.next_random() <= randomizer.MAX * exp((currentv - v) / t)) {
    currentv = v;
    reverse(current + act.first, current + act.second + 1);
  }
}

void sa() {
  //main Simulated Annealing loop
  double t = START_T;

  while (t > END_T) {

    for (int i = 0; i < LOOP; i++)
      random_update(t);

    t *= DECEND;
    for (int i = 0; i < n; i++)
      printf("%s ", id2name[current[i]].c_str());
    printf("%.8lf\n", currentv);
  }
}

int main(int argc, char **argv) {
  if (argc == 3) {
    freopen(argv[1], "r", stdin);
    freopen(argv[2], "w", stdout);
  }
  scanf("%d", &n);

  init();

  sa();
}
