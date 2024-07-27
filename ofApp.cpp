#include "ofApp.h"	

//--------------------------------------------------------------
Actor::Actor(vector<glm::vec3>& location_list, vector<vector<int>>& next_index_list, vector<int>& destination_list, ofColor color) {

	this->select_index = ofRandom(location_list.size());
	while (true) {

		auto itr = find(destination_list.begin(), destination_list.end(), this->select_index);
		if (itr == destination_list.end()) {

			destination_list.push_back(this->select_index);
			break;
		}

		this->select_index = (this->select_index + 1) % location_list.size();
	}

	this->next_index = this->select_index;

	this->noise_param = ofRandom(1000);
	this->noise_step = ofRandom(1000);

	this->color = color;
}

//--------------------------------------------------------------
void Actor::update(const int& frame_span, vector<glm::vec3>& location_list, vector<vector<int>>& next_index_list, vector<int>& destination_list) {

	if (ofGetFrameNum() % frame_span == 0) {

		auto tmp_index = this->select_index;
		this->select_index = this->next_index;
		int retry = next_index_list[this->select_index].size();
		this->next_index = next_index_list[this->select_index][(int)ofRandom(next_index_list[this->select_index].size())];
		while (--retry > 0) {

			auto destination_itr = find(destination_list.begin(), destination_list.end(), this->next_index);
			if (destination_itr == destination_list.end()) {

				if (tmp_index != this->next_index) {

					destination_list.push_back(this->next_index);
					break;
				}
			}

			this->next_index = next_index_list[this->select_index][(this->next_index + 1) % next_index_list[this->select_index].size()];
		}
		if (retry <= 0) {

			destination_list.push_back(this->select_index);
			this->next_index = this->select_index;
		}
	}

	auto param = ofGetFrameNum() % frame_span;
	auto distance = location_list[this->next_index] - location_list[this->select_index];
	this->location = location_list[this->select_index] + distance / frame_span * param;

	if (this->next_index != this->select_index) {

		this->noise_step += 0.5;
	}
}

//--------------------------------------------------------------
glm::vec3 Actor::getLocation() {

	return this->location;
}

//--------------------------------------------------------------
ofColor Actor::getColor() {

	return this->color;
}

//--------------------------------------------------------------
float Actor::getWordIndexNoiseValue() {

	return ofNoise(this->noise_param, this->noise_step);
}

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofSetColor(255);

	ofEnableDepthTest();

	auto span = 30;
	for (int x = -240; x <= 240; x += span) {

		for (int y = -240; y <= 240; y += span) {

			for (int z = -30; z <= 30; z += span) {

				this->location_list.push_back(glm::vec3(x, y, z));

			}
		}
	}

	for (auto& location : this->location_list) {

		vector<int> next_index = vector<int>();
		int index = -1;
		for (auto& other : this->location_list) {

			index++;
			if (location == other) { continue; }

			float distance = glm::distance(location, other);
			if (distance <= span) {

				next_index.push_back(index);
			}
		}

		this->next_index_list.push_back(next_index);
	}

	ofColor color;
	for (int i = 0; i < 750; i++) {
		
		color.setHsb(ofMap(i, 0, 900, 0, 255), 200, 255);
		this->actor_list.push_back(make_unique<Actor>(this->location_list, this->next_index_list, this->destination_list, color));
	}

	this->font_size = 25;
	ofTrueTypeFontSettings font_settings("fonts/msgothic.ttc", this->font_size);
	this->font.load(font_settings);

	this->words = {

		"A", "B", "C", "D", "E", "F", "G", "H", "J", "K", "L", "M", "N",
		"O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
	};
}

//--------------------------------------------------------------
void ofApp::update() {

	int frame_span = 10;
	int prev_index_size = 0;

	if (ofGetFrameNum() % frame_span == 0) {

		prev_index_size = this->destination_list.size();
	}

	for (auto& actor : this->actor_list) {

		actor->update(frame_span, this->location_list, this->next_index_list, this->destination_list);
	}

	if (prev_index_size != 0) {

		this->destination_list.erase(this->destination_list.begin(), this->destination_list.begin() + prev_index_size);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * -1.44);

	ofColor color;
	for (auto& actor : this->actor_list) {

		color.setHsb(ofMap(actor->getLocation().z, -30, 30, 10, 128), 150, 255);
		ofSetColor(color);

		ofPushMatrix();
		ofTranslate(glm::vec3(actor->getLocation().x - 15, actor->getLocation().y - 15, actor->getLocation().z));

		this->font.drawString(this->words[ofMap(actor->getWordIndexNoiseValue(), 0, 1, 0, this->words.size())], 0, 0);

		ofPopMatrix();
	}

	this->cam.end();

	/*
	int start = 250;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}


//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}