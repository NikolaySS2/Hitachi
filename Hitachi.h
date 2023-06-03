#pragma once

#include <string>
#include <vector>

using namespace std;

extern const string Filename;
extern vector<vector<string>> lineofwords;
extern vector<double> weather;

void putInVectors();
int* appropriateDays(int lowtemps, int hightemps, int windspeed, int humiditypercent, int numberofdays);
void createFile();
void averageValues(int templow, int temphigh, int windspeed, int humidity);
void customParameters();
void printFileContents();
