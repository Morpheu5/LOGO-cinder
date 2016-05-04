#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "Turtle.h"

#include <boost/algorithm/string.hpp>
using namespace boost::algorithm;

using namespace ci;
using namespace ci::app;
using namespace std;

class LOGOApp : public App {
  public:
	void setup() override;
	void update() override;
	void draw() override;
	
	void fileDrop(FileDropEvent event) override;
	vector<string> program;
	
	Turtle turtle;
};

void LOGOApp::setup() {
}

void LOGOApp::update() {
}

void LOGOApp::fileDrop(cinder::app::FileDropEvent event) {
	program.clear();
	fs::path filename = event.getFile(0);
	fstream inFile(filename.c_str());
	string line;
	while(getline(inFile, line)) {
		trim(line);
		program.push_back(line);
	}
}

void LOGOApp::draw() {
	// The drawn picture might not be pixel perfect, I'm not sure whether they are due to OpenGL or to floats (but glm deals with floats, so...)
	gl::clear(Color(0, 0, 0));
	gl::color(Color::white());
	{
		gl::ScopedMatrices m;
		// OK, this transform might seem odd, but it's because of a combination of Cinder's window space and OpenGL coordinates.
		gl::multViewMatrix(glm::translate(vec3(getWindowCenter(), 0.0f)) * glm::rotate(toRadians(180.0f), vec3(0, 0, 1)));
		for(string command : program) {
			if(!turtle.step(command)) {
				console() << "Some pretty dramatic error must have happened. See console." << endl;
			}
		}
	}
	// Since it's a state machine, we need to reset it before every draw call.
	turtle.reset();
}

CINDER_APP(LOGOApp, RendererGl)
