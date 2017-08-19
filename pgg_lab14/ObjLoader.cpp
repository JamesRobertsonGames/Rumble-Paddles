/*!
*  \brief     OBJLoader Class.
*  \details   This class is to Load in the RAW OBJ File
*  \author    James Robertson
*  \version   1.0a
*  \date      2015
*  \copyright GNU Public License.
*/

#include "ObjLoader.h"
#include <sstream>

ObjLoader::ObjLoader() {

}

ObjLoader::~ObjLoader() {

}

void ObjLoader::Load(std::string objFileName) {

	FILE * objFile;

	fopen_s(&objFile, objFileName.c_str(), "r");

	if (NULL == objFile)
		printf("Could not open obj file: %s\n", objFileName);

	//rips the raw data out of the obj file and stores it in various std::vectors
	ReadObjFileData(objFile);

	//builds vertex and normal std::vectors based on the above
	BuildMeshVertAndNormalLists();

	fclose(objFile);
}

void ObjLoader::ReadObjFileData(FILE* objFile) {

	char buffer[256];

	while (true)
	{

		int read = fscanf_s(objFile, "%s", buffer, sizeof(buffer));

		if (read == EOF) break;

		//if first part of the line is "v"
		if (strcmp(buffer, "v") == 0) {
			//printf("Found a Vertex:\n");
			glm::vec3 vert;
			fscanf_s(objFile, "%f %f %f\n", &vert.x, &vert.y, &vert.z);
			objFileVerts.push_back(vert);
		}

		//if first part of the line is "vn"
		else if (strcmp(buffer, "vn") == 0) {
			//printf("Found a Vertex Normal:\n");
			glm::vec3 normal;
			fscanf_s(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			objFileNormals.push_back(normal);
		}

		//if first part of the line is "f"
		else if (strcmp(buffer, "f") == 0) {
			//printf("Found f:\n");

			fscanf_s(objFile, "%256[^\n]", buffer, sizeof(buffer));
			std::string s(buffer);
			std::stringstream stream(s);

			std::string split;
			std::vector<std::string> splits;
			while (stream >> split)
				splits.push_back(split);

			std::vector<FaceVertexData> tmpFaceVerts;
			for (uint32_t i = 0; i < splits.size(); i++)
				tmpFaceVerts.push_back(ExtractFaceVertexData(splits[i]));

			uint32_t i = 0;
			do {
				faceVerts.push_back(tmpFaceVerts[0]);
				faceVerts.push_back(tmpFaceVerts[i + 1]);
				faceVerts.push_back(tmpFaceVerts[i + 2]);
				i++;
			} while (i < tmpFaceVerts.size() - 2);

		}
		else {
		}
	}
}

void ObjLoader::BuildMeshVertAndNormalLists() {

	for (uint32_t i = 0; i < faceVerts.size(); i++) {
		//the ith vnp in the std::vector
		FaceVertexData* vnp = &faceVerts[i];

		//pack ith vnp's vertex data into the meshVertices list
		if (vnp->Vertex > 0) {
			meshVertices.push_back(objFileVerts[vnp->Vertex - 1].x);
			meshVertices.push_back(objFileVerts[vnp->Vertex - 1].y);
			meshVertices.push_back(objFileVerts[vnp->Vertex - 1].z);
		}

		//pack ith vnp's normal data into the meshVertices list
		if (vnp->Normal > 0) {
			meshNormals.push_back(objFileVerts[vnp->Normal - 1].x);
			meshNormals.push_back(objFileVerts[vnp->Normal - 1].y);
			meshNormals.push_back(objFileVerts[vnp->Normal - 1].z);
		}
		else {

			meshNormals.push_back(1);
			meshNormals.push_back(0);
			meshNormals.push_back(0);
		}
	}
}

FaceVertexData ObjLoader::ExtractFaceVertexData(std::string& s) {

	int slashPos[2];
	int slashCount = 0;

	for (uint32_t i = 0; i < s.size(); i++) {
		if (s[i] == '/') {
			slashPos[slashCount] = i;
			slashCount++;
		}
	}

	FaceVertexData result;

	if (slashCount == 0) {
	
		result.Vertex = std::stoi(s); 
	}
	else if (slashCount == 1) {

		result.Vertex = std::stoi(s.substr(0, slashPos[0]));	
		result.Normal = std::stoi(s.substr(slashPos[0] + 1));   
	}
	else if (slashCount == 2) {

		result.Vertex = std::stoi(s.substr(0, slashPos[0]));  
		result.Normal = std::stoi(s.substr(slashPos[1] + 1)); 

		std::string texCoordString = s.substr(slashPos[0] + 1, (slashPos[1] - (slashPos[0])) - 1);
		if (texCoordString.size() > 0)
			result.TexCoord = std::stoi(texCoordString);
	}
	return result;
}
