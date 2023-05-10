#pragma once

#include "PilkEngineCommon.h"

class Model;

const int max_shaders = 20;

/// Original Author: Thomas Beet
/// <summary>
/// Static resource manager, allows loading of shaders, textures and models. Keeps track of them
/// </summary>
class ResourceManager final
{
public:
	explicit ResourceManager() = delete;
	static bool CreateShaderProgram(unsigned int* p_sProgram, const char* p_vertFileName, const char* p_fragFileName);
	static unsigned int LoadTexture(const char* p_path, const std::string& p_directory);
	static Model* LoadModel(const char* p_path);
	static void DeleteResources();

private:
	static bool LoadShader(const char* p_fileName, std::string& p_shaderSource);
	static bool CompileShader(const GLenum& p_shaderType, const char* p_fileName, unsigned int* p_shaderBuffer, int* p_success, char* p_infoLog);
	
	static std::map<std::string, unsigned int*> m_shaderMap;
	static std::map<std::string, unsigned int*> m_textureMap;
	static std::map<std::string, Model*> m_modelMap;
};