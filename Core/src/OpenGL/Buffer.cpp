#include <Core/OpenGL/Buffer.h>

#include <glad/glad.h>

using namespace Core;

#pragma region IndexBuffer

bool IndexBuffer::Create() {
	glGenBuffers(1, &mEBO);
	if (mEBO == 0) return false;
	Bind();
	return true;
}

bool IndexBuffer::Create(const unsigned int* data, size_t count, BufferUsage usage) {
	glGenBuffers(1, &mEBO);
	if (mEBO == 0) return false;

	mCount = count;

	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, static_cast<GLenum>(usage));
	return true;
}

void IndexBuffer::Update(const unsigned int* data, size_t count, size_t offset) {
	Bind();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(unsigned int),
		count * sizeof(unsigned int), data);
}

void IndexBuffer::UpdateAndResize(const unsigned int* data, size_t count, BufferUsage usage) {
	Bind();
	mCount = count;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, static_cast<GLenum>(usage));
}

void IndexBuffer::Bind() const { 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
}

void IndexBuffer::Unbind() const { 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
}

IndexBuffer::~IndexBuffer() {
	Destroy();
}

void IndexBuffer::Destroy() {
	if (mEBO != 0) {
		glDeleteBuffers(1, &mEBO);
		mEBO = 0;
	}
	mCount = 0;
}

#pragma endregion

#pragma region VertexBuffer

bool VertexBuffer::Create(bool instanced) {
	mIsInstanced = instanced;
	glGenBuffers(1, &mVBO);
	if (mVBO == 0) return false;
	return true;
}

bool VertexBuffer::Create(const void* data, size_t size, BufferUsage usage, bool instanced) {
	mIsInstanced = instanced;
	glGenBuffers(1, &mVBO);
	if (mVBO == 0) return false;
	Bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, static_cast<GLenum>(usage));
	return true;
}

VertexBuffer::~VertexBuffer() {
	Destroy();
}

void VertexBuffer::Destroy() {
	if (mVBO) {
		glDeleteBuffers(1, &mVBO);
		mVBO = 0;
	}
	mAttributes.clear();
}

void VertexBuffer::Update(const void* data, size_t size, size_t offset) {
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void VertexBuffer::UpdateAndResize(const void* data, size_t size, BufferUsage usage) {
	Bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, static_cast<GLenum>(usage));
}

void VertexBuffer::AddVertexAttribPointer(uint32_t index, int32_t size, VertexAttribType type, uint8_t normalized, int32_t stride, size_t offset) {
	mAttributes.push_back({ index,size,type,normalized,stride, offset });
}

void VertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
}
void VertexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

#pragma endregion

#pragma region VertexArray

bool VertexArray::Create() {
	glGenVertexArrays(1, &mVAO);
	return mVAO != 0;
}

VertexArray::~VertexArray() {
	Destroy();
}

void VertexArray::Destroy() {
	if (mVAO != 0) {
		glDeleteVertexArrays(1, &mVAO);
		mVAO = 0;
	}
}

void VertexArray::AddBuffer(const IndexBuffer &buffer) {
	Bind();
	buffer.Bind();
}

void VertexArray::AddBuffer(const VertexBuffer &buffer) {
	Bind();               // VAO binden
	buffer.Bind();        // VBO binden

	// Alle gespeicherten Attribute setzen
	for (const auto& attr : buffer.GetAttributes()) {
		if ((attr.type == VertexAttribType::Int 
			|| attr.type == VertexAttribType::UnsignedInt
			|| attr.type == VertexAttribType::Byte
			|| attr.type == VertexAttribType::UnsignedByte
			|| attr.type == VertexAttribType::Short
			|| attr.type == VertexAttribType::UnsignedShort) && !attr.normalized) {
			glVertexAttribIPointer(
				attr.index,
				attr.size,
				static_cast<GLenum>(attr.type),
				attr.stride,
				(void*)attr.offset
			);
		}
		else {
			glVertexAttribPointer(
				attr.index,
				attr.size,
				static_cast<GLenum>(attr.type),
				attr.normalized,
				attr.stride,
				(void*)attr.offset
			);
		}
		glEnableVertexAttribArray(attr.index);
		glVertexAttribDivisor(attr.index, buffer.IsInstanced() ? 1 : 0);
	}

	Unbind();
}

void VertexArray::DrawArrays(DrawMode mode, int32_t count, int32_t first) const {
	Bind();
	glDrawArrays(
		static_cast<GLenum>(mode),
		first,
		count
	);
}

void VertexArray::DrawElements(DrawMode mode, int32_t count, int32_t offset) const {
	Bind();
	glDrawElements(
		static_cast<GLenum>(mode),
		count,
		GL_UNSIGNED_INT,
		(const void*)(offset * sizeof(unsigned int))
	);
}

void VertexArray::DrawArraysInstanced(DrawMode mode, int32_t count, int32_t instanceCount, int32_t first) const {
	Bind();
	glDrawArraysInstanced(
		static_cast<GLenum>(mode),
		first,
		count,
		instanceCount
	);
}

void VertexArray::DrawElementsInstanced(DrawMode mode, int32_t count, int32_t instanceCount, int32_t offset) const {
	Bind();
	glDrawElementsInstanced(
		static_cast<GLenum>(mode),
		count,
		GL_UNSIGNED_INT,
		(const void*)(offset * sizeof(unsigned int)),
		instanceCount
	);
}

void VertexArray::SetPolygonMode(PolygonFace face, PolygonMode mode) const {
	glPolygonMode(static_cast<GLenum>(face), static_cast<GLenum>(mode));
}

void VertexArray::Bind() const {
	glBindVertexArray(mVAO);
}
void VertexArray::Unbind() const {
	glBindVertexArray(0);
}

#pragma endregion

#pragma region ShaderStorageBuffer

bool ShaderStorageBuffer::Create(uint32_t bindingPoint) {
	if (mSSBO != 0) Destroy();

	glGenBuffers(1, &mSSBO);
	mBindingPoint = bindingPoint;
	return mSSBO != 0;
}

ShaderStorageBuffer::~ShaderStorageBuffer() {
	Destroy();
}

void ShaderStorageBuffer::Destroy() {
	if (mSSBO != 0) {
		glDeleteBuffers(1, &mSSBO);
		mSSBO = 0;
	}
}

void ShaderStorageBuffer::Bind() const {
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, mBindingPoint, mSSBO);
};
void ShaderStorageBuffer::Unbind() const {
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, mBindingPoint, 0);
};

void ShaderStorageBuffer::Upload(const void* data, size_t size, BufferUsage usage) {
	Bind();
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, static_cast<GLenum>(usage));
}

void ShaderStorageBuffer::Update(const void* data, size_t offset, size_t size) {
	Bind();
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
}

#pragma endregion
