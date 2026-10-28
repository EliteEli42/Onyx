#include "OpenGL/Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <iostream>

#pragma region Shader

bool Shader::CreateFromFile(ShaderType type, const char* filepath) {

	std::string source = LoadTextFromFile(filepath);
	const char* src = source.c_str();
	mID = glCreateShader(static_cast<GLenum>(type));
	glShaderSource(mID, 1, &src, NULL);
	glCompileShader(mID);

	// check for shader compile errors
	char infoLog[512];
	glGetShaderiv(mID, GL_COMPILE_STATUS, &mSuccess);
	if (!mSuccess)
	{
		glGetShaderInfoLog(mID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}
	return true;
}

bool Shader::CreateFromSource(ShaderType type, const char* source) {

	mID = glCreateShader(static_cast<GLenum>(type));
	glShaderSource(mID, 1, &source, NULL);
	glCompileShader(mID);

	// check for shader compile errors
	char infoLog[512];
	glGetShaderiv(mID, GL_COMPILE_STATUS, &mSuccess);
	if (!mSuccess)
	{
		glGetShaderInfoLog(mID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}
	return true;
}

Shader::~Shader() {
	Delete();
}

void Shader::Delete() {
	if (mID == 0) return;
	glDeleteShader(mID);
	mID = 0;
}

#pragma endregion

#pragma region ShaderProgramm

bool ShaderProgram::Create(const Shader& vertexShader, const Shader& fragmentShader) {
	mID = glCreateProgram();
	glAttachShader(mID, vertexShader.GetID());
	glAttachShader(mID, fragmentShader.GetID());
	glLinkProgram(mID);

	// check for linking errors
	char infoLog[512];
	glGetProgramiv(mID, GL_LINK_STATUS, &mSuccess);
	if (!mSuccess) {
		glGetProgramInfoLog(mID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return false;
	}
	return true;
}

ShaderProgram::~ShaderProgram() {
	Delete();
}

void ShaderProgram::Use() {
	glUseProgram(mID);
}

void ShaderProgram::Unuse() {
	glUseProgram(0);
}

void ShaderProgram::Delete() {
	if (mID == 0) return;
	glDeleteProgram(mID);
	mID = 0;
}

Uniform ShaderProgram::GetUniform(const char* name)
{
	return Uniform(this, name);
}

#pragma endregion

#pragma region Uniform

Uniform::Uniform(ShaderProgram* program, const char* name) {
	mProgram = program;
	mLocation = glGetUniformLocation(program->GetID(), name);
	if (mLocation == -1) {
		std::cerr << "Warning: uniform '" << name << "' not found in shader.\n";
	}
}

void Uniform::SetFloat(float value)
{
	mProgram->Use();
	glUniform1f(mLocation, value);
}

void Uniform::SetInt(int value)
{
	mProgram->Use();
	glUniform1i(mLocation, value);
}

void Uniform::SetUInt(uint32_t value)
{
	mProgram->Use();
	glUniform1ui(mLocation, value);
}

void Uniform::SetBool(bool value)
{
	mProgram->Use();
	glUniform1i(mLocation, value ? 1 : 0);
}

void Uniform::SetVec2(const Vec2& value)
{
	mProgram->Use();
	glUniform2fv(mLocation, 1, &value[0]);
}

void Uniform::SetVec3(const Vec3& value)
{
	mProgram->Use();
	glUniform3fv(mLocation, 1, &value[0]);
}

void Uniform::SetVec4(const Vec4& value)
{
	mProgram->Use();
	glUniform4fv(mLocation, 1, &value[0]);
}

void Uniform::SetMat4(const Mat4& m)
{
	mProgram->Use(); 
	glUniformMatrix4fv(mLocation, 1, GL_FALSE, &m[0][0]);
}

void Uniform::SetIntArray(const int* values, size_t count)
{
	mProgram->Use();
	glUniform1iv(mLocation, static_cast<GLsizei>(count), values);
}

void Uniform::SetUIntArray(const uint32_t* values, size_t count)
{
	mProgram->Use();
	glUniform1uiv(mLocation, static_cast<GLsizei>(count), values);
}

void Uniform::SetFloatArray(const float* values, size_t count)
{
	mProgram->Use();
	glUniform1fv(mLocation, static_cast<GLsizei>(count), values);
}

void Uniform::SetVec2Array(const Vec2* values, size_t count)
{
	mProgram->Use();
	glUniform2fv(mLocation, static_cast<GLsizei>(count), reinterpret_cast<const float*>(values));
}

void Uniform::SetVec3Array(const Vec3* values, size_t count)
{
	mProgram->Use();
	glUniform3fv(mLocation, static_cast<GLsizei>(count), reinterpret_cast<const float*>(values));
}

void Uniform::SetVec4Array(const Vec4* values, size_t count)
{
	mProgram->Use();
	glUniform4fv(mLocation, static_cast<GLsizei>(count), reinterpret_cast<const float*>(values));
}

#pragma endregion


std::string LoadTextFromFile(const char* FilePath) {
	std::ifstream file;
	std::string result;
	file.open(FilePath);
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			result += line + "\n";
		}
		file.close();
	}
	else {
		std::cout << "Could not open file: " << FilePath << std::endl;
	}
	return result;
}