#pragma once

#include <string>

using namespace std;
using namespace ci;

class Turtle {
public:
	vec2 position = vec2(0);
	float angle = 0.0f; // [0, 1, 0] is upward
	bool isPenDown = true;
	
	int currentRepeat = -1;
	vector<vector<string>> repeats;
	vector<int> repeatCounts;
	
	bool step(string command);
	
	void reset();
};