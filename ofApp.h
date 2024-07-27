#pragma once
#include "ofMain.h"

class Actor {
public:
	Actor(vector<glm::vec3>& location_list, vector<vector<int>>& next_index_list, vector<int>& destination_list, ofColor color);
	void update(const int& frame_span, vector<glm::vec3>& location_list, vector<vector<int>>& next_index_list, vector<int>& destination_list);
	glm::vec3 getLocation();
	ofColor getColor();
	float getWordIndexNoiseValue();

private:

	int select_index;
	int next_index;

	float noise_param;
	float noise_step;

	glm::vec3 location;

	ofColor color;
};

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key) {};
	void keyReleased(int key) {};
	void mouseMoved(int x, int y) {};
	void mouseDragged(int x, int y, int button) {};
	void mousePressed(int x, int y, int button) {};
	void mouseReleased(int x, int y, int button) {};
	void windowResized(int w, int h) {};
	void dragEvent(ofDragInfo dragInfo) {};
	void gotMessage(ofMessage msg) {};

	vector<glm::vec3> location_list;
	vector<vector<int>> next_index_list;
	vector<int> destination_list;

	vector<unique_ptr<Actor>> actor_list;

	ofTrueTypeFont font;
	int font_size;
	vector<string> words;

	ofEasyCam cam;
};