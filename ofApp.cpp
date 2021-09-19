#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 50, true, true, true);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(270);

	vector<string> word_list = {
		"0", "1", "2", "3",
		"6", "5", "4",
		"7", "8", "9", "0"
	};

	for (int base_deg = 0; base_deg < 360; base_deg += 30) {

		for (int y = -240 - 60; y < 300; y += 60) {

			int word_index = ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.0001), 0, 1, 0, word_list.size());
			auto word = word_list[word_index];

			int sample_count = 60;
			vector<ofPath> word_path = this->font.getStringAsPoints(word, true, false);
			for (int word_index = 0; word_index < word_path.size(); word_index++) {

				vector<ofPolyline> outline = word_path[word_index].getOutline();
				for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

					outline[outline_index] = outline[outline_index].getResampledByCount(sample_count);
					vector<glm::vec3> vertices = outline[outline_index].getVertices();
					vector<glm::vec3> mesh_vertices;
					vector<glm::vec3> base_location_list;
					vector<glm::highp_mat4x4> rotate_vertices;

					for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

						auto base_location = glm::vec3(0, y, 100);
						base_location += glm::vec3(this->font.stringWidth(word_list[word_index]) * 0.5, this->font.stringHeight(word_list[word_index]) * 0.5, 0);

						auto deg = base_deg + ofMap(ofNoise((base_location.y + vertices[vertices_index].y * -1) * 0.001 + ofGetFrameNum() * 0.008), 0, 1, -360, 360);

						auto rotation = glm::rotate(glm::mat4(), (float)(deg * DEG_TO_RAD), glm::vec3(0, 1, 0));
						auto location = vertices[vertices_index] - glm::vec3(this->font.stringWidth(word_list[word_index]) * 0.5, this->font.stringHeight(word_list[word_index]) * 0.5, 0);

						mesh_vertices.push_back(location);
						rotate_vertices.push_back(rotation);
						base_location_list.push_back(base_location);
					}

					ofMesh face, line;
					line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
					for (int k = 0; k < mesh_vertices.size(); k++) {

						face.addVertex(glm::vec4(base_location_list[k], 0) * rotate_vertices[k] + glm::vec4(mesh_vertices[k] + glm::vec3(0, 0, -5), 0) * -rotate_vertices[k]);
						face.addVertex(glm::vec4(base_location_list[k], 0) * rotate_vertices[k] + glm::vec4(mesh_vertices[k] + glm::vec3(0, 0, 5), 0) * -rotate_vertices[k]);

						line.addVertex(glm::vec4(base_location_list[k], 0) * rotate_vertices[k] + glm::vec4(mesh_vertices[k] + glm::vec3(0, 0, -5), 0) * -rotate_vertices[k]);
						line.addVertex(glm::vec4(base_location_list[k], 0) * rotate_vertices[k] + glm::vec4(mesh_vertices[k] + glm::vec3(0, 0, 5), 0) * -rotate_vertices[k]);

						if (k > 0) {

							face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 2); face.addIndex(face.getNumVertices() - 4);
							face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 3); face.addIndex(face.getNumVertices() - 4);

							line.addIndex(line.getNumVertices() - 1); line.addIndex(line.getNumVertices() - 3);
							line.addIndex(line.getNumVertices() - 2); line.addIndex(line.getNumVertices() - 4);
						}
					}

					face.addIndex(face.getNumVertices() - 1); face.addIndex(0); face.addIndex(1);
					face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 2); face.addIndex(2);

					line.addIndex(line.getNumVertices() - 1); line.addIndex(1);
					line.addIndex(line.getNumVertices() - 2); line.addIndex(0);

					ofSetColor(39);
					face.draw();

					ofSetColor(239);
					line.drawWireframe();
				}
			}
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}