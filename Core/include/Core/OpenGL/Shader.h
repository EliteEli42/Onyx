#pragma once
#include <string>
#include <iostream>

#include <Common/MathTypes.h>

class Uniform;

enum class ShaderType : uint32_t {
	Vertex = 0x8B31,
	Fragment = 0x8B30,
	Geometry = 0x8DD9,
	Compute = 0x91B9
};

struct Shader {
	bool CreateFromFile(ShaderType type, const char* filepath);
	bool CreateFromSource(ShaderType type, const char* source);
	~Shader();
	void Delete();
	inline bool CompilationSucceeded() const { return mSuccess == 1; }
	inline unsigned int GetID() const { return mID; }
private:
	uint32_t mID;
	int mSuccess;
};

struct ShaderProgram {
	bool Create(const Shader& vertexShader, const Shader& fragmentShader);
	inline bool CreateFromFiles(const char* vertexShaderPath, const char* fragmentShaderPath) {
		Shader vs;
		if(!vs.CreateFromFile(ShaderType::Vertex, vertexShaderPath)) return false;
		Shader fs;
		if(!fs.CreateFromFile(ShaderType::Fragment, fragmentShaderPath)) return false;
		return Create(vs, fs);
	}
	inline bool CreateFromSource(const char* vertexShaderSource, const char* fragmentShaderSource) {
		Shader vs;
		if (!vs.CreateFromSource(ShaderType::Vertex, vertexShaderSource)) return false;
		Shader fs;
		if(!fs.CreateFromSource(ShaderType::Fragment, fragmentShaderSource)) return false;
		return Create(vs, fs);
	}
	~ShaderProgram();
	void Use();
	void Unuse();
	void Delete();
	Uniform GetUniform(const char* name);
	inline bool LinkingSucceeded() const { return mSuccess == 1; }
	inline unsigned int GetID() const { return mID; }
private:
	uint32_t mID;
	int mSuccess;
};

class Uniform {
public:
	Uniform(ShaderProgram* program, const char* name);

	Uniform() = default;

	void SetFloat(float value);
	void SetInt(int value);
	void SetUInt(uint32_t value);
	void SetBool(bool value);

	void SetVec2(const Vec2& value);
	void SetVec3(const Vec3& value);
	void SetVec4(const Vec4& value);

	void SetMat4(const Mat4& m);

	void SetIntArray(const int* values, size_t count);
	void SetUIntArray(const uint32_t* values, size_t count);
	void SetFloatArray(const float* values, size_t count);

	void SetVec2Array(const Vec2* values, size_t count);
	void SetVec3Array(const Vec3* values, size_t count);
	void SetVec4Array(const Vec4* values, size_t count);

	bool IsValid() const { return mLocation != -1; }
	int32_t GetLocation() const { return mLocation; }

private:
	int32_t mLocation = -1;
	ShaderProgram* mProgram = nullptr;
};

std::string LoadTextFromFile(const char* FilePath);