#pragma once
#include <fstream>
#include <sstream>
#include <vector>

class ObjParser {
public:
	struct Position {
		float x{};
		float y{};
		float z{};
	};

	struct Normal {
		float x{};
		float y{};
		float z{};
	};

	struct TextureCoordinates {
		float u{};
		float v{};
	};

	struct VertexIndices {
		int v{};
		int vt{};
		int vn{};
	};

	struct Model {
		std::vector<Position> positions;
		std::vector<Normal> normals;
		std::vector<TextureCoordinates> textCoord;
		std::vector<std::vector<VertexIndices>> faces;
	};

private:
	static Position ReadPosition(std::istringstream& ss) {
		Position pos;
		std::string word;
		ss >> pos.x;
		ss >> pos.y;
		ss >> pos.z;
		return pos;
	}

	static Normal ReadNormal(std::istringstream& ss) {
		Normal norm;
		std::string word;
		ss >> norm.x;
		ss >> norm.y;
		ss >> norm.z;
		return norm;
	}

	static TextureCoordinates ReadTextureCoordinates(std::istringstream& ss) {
		TextureCoordinates textCoord;
		std::string word;
		ss >> textCoord.u;
		ss >> textCoord.v;
		return textCoord;
	}

	static VertexIndices ReadVertexIndices(const std::string& singleVertexIndices) {
		VertexIndices indices;
		std::istringstream ss(singleVertexIndices);
		std::string token;
		if (std::getline(ss, token, '/')) {
			indices.v = std::stoi(token);
		}
		if (std::getline(ss, token, '/')) {
			indices.vt = std::stoi(token);
		}
		if (std::getline(ss, token, '/')) {
			indices.vn = std::stoi(token);
		}
		return indices;
	}

	static std::vector<VertexIndices> ReadPolygonIndices(std::istringstream& ss, bool print = false) {
		std::vector<VertexIndices> indices;
		std::string singleVertexIndices;
		while (!ss.eof()) {
			ss >> singleVertexIndices;
			indices.push_back(ReadVertexIndices(singleVertexIndices));
		}
		indices.shrink_to_fit();
		return indices;
	};
public:
	static Model GetParsed(const std::string& file_name) {
		std::ifstream file;
		file.open(file_name);
		if (!file) {
			throw std::runtime_error("Couldn't load the file. Check if file's name is correct.");
		}

		// first pass: count number of elements
		int positions_count = 0;
		int normals_count = 0;
		int texture_coordinates_count = 0;
		int faces_count = 0;
		while (file) {
			// reading line
			std::string line;
			std::getline(file, line);
			std::istringstream ss(line);

			// reading single word
			std::string word;
			ss >> word;
			if (word == "v") {
				positions_count++;
			} else if (word == "vn") {
				normals_count++;
			} else if (word == "vt") {
				texture_coordinates_count++;
			} else if (word == "f") {
				faces_count++;
			}
		}

		// clear file
		file.clear();
		file.seekg(0, std::ios::beg);

		// prepare containers
		Model model;
		model.positions.reserve(positions_count);
		model.normals.reserve(normals_count);
		model.textCoord.reserve(texture_coordinates_count);
		model.faces.reserve(faces_count);


		// second pass: read and store the elements
		while (file) {
			// reading line
			std::string line;
			std::getline(file, line);
			std::istringstream ss(line);

			// reading single word
			std::string word;
			ss >> word;
			if (word == "v") {
				model.positions.push_back(ReadPosition(ss));
			} else if (word == "vn") {
				model.normals.push_back(ReadNormal(ss));
			} else if (word == "vt") {
				model.textCoord.push_back(ReadTextureCoordinates(ss));
			} else if (word == "f") {
				model.faces.push_back(ReadPolygonIndices(ss));
			}
		}
		return model;
	}
};