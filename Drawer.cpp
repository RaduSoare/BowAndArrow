#include "Drawer.h"

using namespace std;

Drawer::Drawer()
{
	
}

void Drawer::DrawLine(glm::vec3 point1, glm::vec3 point2, glm::vec3 color, const char* meshName, 
	std::unordered_map<std::string, Mesh*>* meshes)
{
	vector<VertexFormat> verticesLine = {
		VertexFormat(point1, color),
		VertexFormat(point2,  color)
	};
	vector<unsigned short> indicesLine = { 0, 1 };
	Mesh *line = CreateMesh(meshName, verticesLine, indicesLine, meshes);
	line->SetDrawMode(GL_LINE_STRIP);
} 

void Drawer::DrawCurvedLine(float startAngle, float endAngle, float radius, glm::vec3 color, const char* meshName, 
	std::unordered_map<std::string, Mesh*>* meshes)
{

	vector<VertexFormat> verticesCurvedLine;
	vector<unsigned short> indicesCurvedLine;
	for (float angle = startAngle; angle <= endAngle; angle += 0.01f) {
		verticesCurvedLine.push_back(VertexFormat(glm::vec3(radius * glm::cos(angle), radius * glm::sin(angle), 0), color));
	}
	for (auto it = verticesCurvedLine.begin(); it != verticesCurvedLine.end(); ++it) {
		int index = std::distance(verticesCurvedLine.begin(), it);
		indicesCurvedLine.push_back(index);
	}
	Mesh *curvedLine = CreateMesh(meshName, verticesCurvedLine, indicesCurvedLine, meshes);
	curvedLine->SetDrawMode(GL_LINE_STRIP);

}

void Drawer::InitPowerBar(std::unordered_map<std::string, Mesh*>* meshes) {
	glm::vec3 color = glm::vec3(1.f, 1.f, 1.f);
	vector<VertexFormat> verticesPowerbar = {
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(10, 0, 0),  color),
		VertexFormat(glm::vec3(10, 1, 0),  color),
		VertexFormat(glm::vec3(0, 1, 0),  color)
	};
	vector<unsigned short> indicesPowerbar = {
		0, 1,
		1, 2,
		2, 3,
		3, 0
	};
	Mesh *powerbar = CreateMesh("powerbar", verticesPowerbar, indicesPowerbar, meshes);
	powerbar->SetDrawMode(GL_LINE_STRIP);
}

void Drawer::InitBow(std::unordered_map<std::string, Mesh*>* meshes, float bowRadius, 
	float arrowLength, float arrowHeadHeight) {
	glm::vec3 color = glm::vec3(1.f, 1.f, 1.f);

	// Draw bow's curved line between -45 and 45 degrees
	DrawCurvedLine(-glm::pi<float>() / 2,
		glm::pi<float>() / 2,
		bowRadius,
		glm::vec3(0.6f, 0.29f, 0),
		"bowCurve", meshes);


	// Draw bow's straight line
	DrawLine(glm::vec3(0, bowRadius * glm::sin(-glm::pi<float>() / 2), 0),
		glm::vec3(0, bowRadius * glm::sin(glm::pi<float>() / 2), 0),
		color,
		"bowStraightLine", meshes);

	// Draw arrow's stick
	DrawLine(glm::vec3(0, 0, 0),
		glm::vec3(arrowLength, 0, 0),
		glm::vec3(0.6f, 0.29f, 0),
		"arrowStraightLine", meshes);

	// Draw arrowhead
	vector<VertexFormat> verticesArrowhead = {
		VertexFormat(glm::vec3(0, -arrowHeadHeight,  0), glm::vec3(0.37f, 0.37f, 0.37f)),
		VertexFormat(glm::vec3(arrowHeadHeight, 0, 0),  glm::vec3(0.37f, 0.37f, 0.37f)),
		VertexFormat(glm::vec3(0, arrowHeadHeight, 0),  glm::vec3(0.37f, 0.37f, 0.37f))
	};
	vector<unsigned short> indicesArrowhead = { 0, 1, 2 };
	Mesh *arrowHead = CreateMesh("arrowHead", verticesArrowhead, indicesArrowhead, meshes);
	arrowHead->SetDrawMode(GL_TRIANGLES); 
}

void Drawer::InitBalloon(std::unordered_map<std::string, Mesh*>* meshes, const char* type, float balloonRadius) {
	// Check for balloon type
	glm::vec3 color;
	if (strcmp(type, "balloonRed") == 0) {
		color = glm::vec3(1.f, 0, 0);
	}
	else if (strcmp(type, "balloonYellow") == 0) {
		color = glm::vec3(1.f, 1.f, 0);
	}
	else if (strcmp(type, "balloonWhite") == 0) {
		color = glm::vec3(1.f, 1.f, 1.f);
	}

	// Draw the actual balloon
	DrawCurvedLine(0,
		2 * glm::pi<float>(),
		balloonRadius,
		color,
		type, meshes);

	// Draw bow's string using 4 lines
	vector<VertexFormat> verticesBalloonBottom = {
		VertexFormat(glm::vec3(0, 0,  0), glm::vec3(1.f, 1.f, 1.f)),
		VertexFormat(glm::vec3(-10, -7, 0),  glm::vec3(1.f, 1.f, 1.f)),
		VertexFormat(glm::vec3(0, -14, 0),  glm::vec3(1.f, 1.f, 1.f)),
		VertexFormat(glm::vec3(-10, -21, 0),  glm::vec3(1.f, 1.f, 1.f)),
		VertexFormat(glm::vec3(0, -28, 0),  glm::vec3(1.f, 1.f, 1.f))
	};
	vector<unsigned short> indicesBalloonBottom = { 0, 1, 2, 3, 4 };
	Mesh *balloonBottom = CreateMesh("balloonBottom", verticesBalloonBottom, indicesBalloonBottom, meshes);
	balloonBottom->SetDrawMode(GL_LINE_STRIP);
}

void Drawer::InitShuriken(std::unordered_map<std::string, Mesh*>* meshes, float shurikenSide) {

	// Draw the shuriken using 4 triangles
	glm::vec3 color = glm::vec3(1.f, 1.f, 1.f);

	vector<VertexFormat> verticesShuriken = {
		VertexFormat(glm::vec3(0, 0,  0), color),
		VertexFormat(glm::vec3(0, -shurikenSide, 0),  color),
		VertexFormat(glm::vec3(shurikenSide, -shurikenSide, 0),  color),
		VertexFormat(glm::vec3(shurikenSide, 0, 0),  color),
		VertexFormat(glm::vec3(shurikenSide, shurikenSide, 0),  color),
		VertexFormat(glm::vec3(0, shurikenSide, 0),  color),
		VertexFormat(glm::vec3(-shurikenSide, shurikenSide, 0),  color),
		VertexFormat(glm::vec3(-shurikenSide, 0, 0),  color),
		VertexFormat(glm::vec3(-shurikenSide, -shurikenSide, 0),  color)
	};
	vector<unsigned short> indicesShuriken = {
		1, 2, 0,
		3, 4, 0,
		5, 6, 0,
		7, 8, 0
	};
	Mesh *shuriken = CreateMesh("shuriken", verticesShuriken, indicesShuriken, meshes);
	shuriken->SetDrawMode(GL_TRIANGLES);
}

Mesh* Drawer::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices, 
	std::unordered_map<std::string, Mesh*>* meshes)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// TODO: Create the VBO and bind it
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// TODO: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	// TODO: Crete the IBO and bind it
	unsigned int IBO = 0;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data
	// It will be learned later, when GLSL shaders will be introduced
	// For the moment just think that each property value from our vertex format needs to be send to a certain channel
	// in order to know how to receive it in the GLSL vertex shader

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// TODO: Unbind the VAO
	glBindVertexArray(0);
	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	(*meshes)[name] = new Mesh(name);
	(*meshes)[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	return (*meshes)[name];
}