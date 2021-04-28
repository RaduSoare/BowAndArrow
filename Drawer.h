#pragma once
#include <Component/SimpleScene.h>
#include <Core/Engine.h>

class Drawer
{

public:
	Drawer();
	void DrawLine(glm::vec3 point1, glm::vec3 point2, glm::vec3 color, const char* meshName, 
		std::unordered_map<std::string, Mesh*>* meshes);
	void DrawCurvedLine(float startAngle, float endAngle, float radius, glm::vec3 color, const char* meshName, 
		std::unordered_map<std::string, Mesh*>* meshes);
	Mesh* CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices, 
		std::unordered_map<std::string, Mesh*>* meshes);
	
	void InitPowerBar(std::unordered_map<std::string, Mesh*>* meshes);
	void InitBow(std::unordered_map<std::string, Mesh*>* meshes, float bowRadius,
		float arrowLength, float arrowHeadHeight);
	void InitBalloon(std::unordered_map<std::string, Mesh*>* meshes, const char* type, float balloonRadius);
	void InitShuriken(std::unordered_map<std::string, Mesh*>* meshes, float shurikenSide);

};