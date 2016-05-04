#include "Turtle.h"
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace ci;
using namespace ci::app; // for console

void Turtle::reset() {
	position = vec2(0);
	angle = 0.0f;
	isPenDown = false;
	currentRepeat = -1;
	repeats.clear();
}

bool Turtle::step(string command) {
	vector<string> cmdVector;
	boost::split(cmdVector, command, boost::is_any_of(" "));
	
	// There is a lot of repetition in here, but nothing that can't be improved.
	
	if(cmdVector[0].compare("END") == 0) {
		if(currentRepeat >= 0) {
			for(int i = 0; i < repeatCounts[currentRepeat]; ++i) {
				for(string command : repeats[currentRepeat]) {
					// let's fake being on the top level...
					int t = currentRepeat;
					currentRepeat = -1;
					step(command);
					currentRepeat = t;
				}
			}
			currentRepeat -= 1;
			repeatCounts.pop_back();
			repeats.pop_back();
		}
	} else if(currentRepeat >= 0) {
		repeats[currentRepeat].push_back(command);
	} else if(cmdVector[0].compare("UP") == 0) {
		isPenDown = false;
	} else if(cmdVector[0].compare("DOWN") == 0) {
		isPenDown = true;
	} else if(cmdVector[0].compare("FW") == 0) {
		if(cmdVector.size() != 2) {
			console() << "FW needs exactly one parameter" << endl;
			return false;
		}
		float param = (float)stoi(cmdVector[1]);
		mat4 t = rotate(toRadians(angle), vec3(0,0,1));
		vec2 target = position + vec2(t * vec4(0.0f, param, 0.0f, 1.0f));
		if(isPenDown) {
			gl::drawLine(position, target);
		}
		position = target;
	} else if(cmdVector[0].compare("BW") == 0) {
		if(cmdVector.size() != 2) {
			console() << "BW needs exactly one parameter" << endl;
			return false;
		}
		int param = -(float)stoi(cmdVector[1]);
		mat4 t = rotate(toRadians(angle), vec3(0,0,1));
		vec2 target = position + vec2(t * vec4(0.0f, param, 0.0f, 1.0f));
		if(isPenDown) {
			gl::drawLine(position, target);
		}
		position = target;
	} else if(cmdVector[0].compare("LEFT") == 0) {
		if(cmdVector.size() != 2) {
			console() << "LEFT needs exactly one parameter" << endl;
			return false;
		}
		float param = (float)stoi(cmdVector[1]);
		angle = angle - param;
	} else if(cmdVector[0].compare("RIGHT") == 0) {
		if(cmdVector.size() != 2) {
			console() << "RIGHT needs exactly one parameter" << endl;
			return false;
		}
		float param = (float)stoi(cmdVector[1]);
		angle = angle + param;
	} else if(cmdVector[0].compare("REPEAT") == 0) {
		// Sadly, this does not handle nested repeats. I'm sure it can be done, but the deadline is approaching.
		if(cmdVector.size() != 2) {
			console() << "REPEAT needs exactly one parameter" << endl;
			return false;
		}
		int param = stoi(cmdVector[1]);
		repeatCounts.push_back(param);
		currentRepeat = repeats.size();
		repeats.push_back(vector<string>());
	}
	return true;
}