/*!
*  \brief     OBJLoader Class.
*  \details   This class is to Load in the RAW OBJ File
*  \author    James Robertson
*  \version   1.0a
*  \date      2015
*  \copyright GNU Public License.
*/

#pragma once

#include <iostream>
#include <string>
#include <glm.hpp>
#include <vector>

struct FaceVertexData {
	int Vertex;
	int TexCoord;
	int Normal;

	FaceVertexData() {
		Vertex = 0;
		TexCoord = 0;
		Normal = 0;
	}
};

class ObjLoader {

public:
	///ctor / dtor
	ObjLoader();
	~ObjLoader();

	/// Load the Object in
	void Load(std::string objFileName);

	/// Get the Mesh Verticies & normals
	std::vector<float>& GetMeshVertices() { return meshVertices; }
	std::vector<float>& GetMeshNormals() { return meshNormals; }

private:

	//store raw data read out of a file
	std::vector<glm::vec3> objFileVerts;
	std::vector<glm::vec3> objFileNormals;
	std::vector<FaceVertexData> faceVerts;

	//extracts bits of an obj file into the above std::vectors
	void ReadObjFileData(FILE* objFile);

	void BuildMeshVertAndNormalLists();

	std::vector<float> meshVertices;
	std::vector<float> meshNormals;

	//reads a string like "3//5" and returns a VNP with 3 & 5 in it
	FaceVertexData ExtractFaceVertexData(std::string& s);

};