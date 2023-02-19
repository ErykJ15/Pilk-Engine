#pragma once

#include "glad.h"
#include "glfw3.h"

class SceneManager
{
public:
	explicit SceneManager(const int p_width, const int p_height, char* p_windowName);
	explicit SceneManager(const SceneManager& p_sceneManager);
	~SceneManager(void);

	int Run();
	SceneManager& operator=(const SceneManager& p_rhs);


private:
	void Render(const float p_dt) const;
	void Update(const float p_dt);
	void Load();

	void processInput(GLFWwindow* p_window);

	char* m_windowName = "";
	int m_width;
	int m_height;

};
