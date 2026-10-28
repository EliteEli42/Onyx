#pragma once
#include <vector>
#include <array>

enum class DrawMode : uint32_t {
    Points = 0x0000,
    Lines = 0x0001,
    LineLoop = 0x0002,
    LineStrip = 0x0003,
    Triangles = 0x0004,
    TriangleStrip = 0x0005,
    TriangleFan = 0x0006
};

enum class PolygonFace : uint32_t {
    Front = 0x0404,
    Back = 0x0405,
    FrontBack = 0x0408
};

enum class PolygonMode : uint32_t {
    Point = 0x1B00,
    Line = 0x1B01,
    Fill = 0x1B02
};

enum class BufferUsage : uint32_t {
    StreamDraw = 0x88E0,
    StaticDraw = 0x88E4,
    DynamicDraw = 0x88E8
};

enum class VertexAttribType : uint32_t {
    Byte = 0x1400,
    UnsignedByte = 0x1401,
    Short = 0x1402,
    UnsignedShort = 0x1403,
    Int = 0x1404,
    UnsignedInt = 0x1405,
    Float = 0x1406,
    Double = 0x140A
};

struct VertexAttrib {
    uint32_t index;
    int32_t size;
    VertexAttribType type;
    uint8_t normalized;
    int32_t stride;
    size_t offset;
};

class IndexBuffer {
public:
    bool Create();
    bool Create(const unsigned int* data, size_t count, BufferUsage usage);

    template<typename T>
    inline bool Create(const std::vector<T>& data, BufferUsage usage) {
        return Create(reinterpret_cast<const unsigned int*>(data.data()), data.size(), usage);
    }

    ~IndexBuffer();
    void Destroy();

    void Update(const unsigned int* data, size_t count, size_t offset = 0);

    template<typename T>
    inline void Update(const std::vector<T>& data, size_t offset = 0) {
        Update(reinterpret_cast<const unsigned int*>(data.data()), data.size(), offset);
    }

    void UpdateAndResize(const unsigned int* data, size_t count, BufferUsage usage);

    template<typename T>
    inline void UpdateAndResize(const std::vector<T>& data, BufferUsage usage) {
        UpdateAndResize(reinterpret_cast<const unsigned int*>(data.data()), data.size(), usage);
    }

    void Bind() const;
    void Unbind() const;

    inline size_t GetCount() const { return mCount; }

private:
    uint32_t mEBO = 0;
    size_t mCount = 0;
};

class VertexBuffer {
public:
    bool Create(bool instanced = false);
    bool Create(const void* data, size_t size, BufferUsage usage, bool instanced = false);
    ~VertexBuffer();
    void Destroy();

    void Update(const void* data, size_t size, size_t offset = 0);

    template<typename T>
    bool Create(const std::vector<T>& data, BufferUsage usage, bool instanced = false) {
        return Create(data.data(), data.size() * sizeof(T), usage, instanced);
    }

    template<typename T>
    void Update(const std::vector<T>& data, size_t offset = 0) {
        Update(data.data(), data.size() * sizeof(T), offset);
    }

    void UpdateAndResize(const void* data, size_t size, BufferUsage usage);
    template<typename T>
    void UpdateAndResize(const std::vector<T>& data, BufferUsage usage) {
        UpdateAndResize(data.data(), data.size() * sizeof(T), usage);
    }

    /**
     * @brief Adds a vertex attribute pointer to the vertex buffer.
     *
     * Defines how OpenGL interprets vertex data for a shader attribute.
     *
     * @param index        Layout location of the attribute in the shader.
     * @param size         Number of components per vertex attribute (e.g., 3 for vec3).
     * @param type         Data type of each component (GL_FLOAT, GL_INT, etc.).
     * @param normalized   Whether fixed-point data should be normalized.
     * @param vertexSize   Total size in bytes of one vertex (distance between consecutive vertices in the buffer).
     * @param offset       Byte offset of this attribute within a single vertex.
     */
    void AddVertexAttribPointer(
        uint32_t index,         // layout location in the shader
        int32_t size,           // number of components per vertex attribute
        VertexAttribType type,  // GL_FLOAT, GL_INT, etc.
        uint8_t normalized,   // normalize fixed-point data or not
        int32_t stride,         // total size of one vertex in bytes
        size_t offset           // byte offset of this attribute in one vertex
    );

    const std::vector<VertexAttrib>& GetAttributes() const { return mAttributes; }

    void Bind() const;
    void Unbind() const;

    bool IsInstanced() const { return mIsInstanced; }
private:
    uint32_t mVBO = 0;
    bool mIsInstanced = false;
    std::vector<VertexAttrib> mAttributes; // zwischengespeicherte Attribute
};

class VertexArray {
public:
    bool Create();
    ~VertexArray();
    void Destroy();

    void AddBuffer(const IndexBuffer& buffer);
    void AddBuffer(const VertexBuffer& buffer);

    void DrawArrays(DrawMode mode, int32_t count, int32_t first = 0) const;
    void DrawElements(DrawMode mode, int32_t count, int32_t offset = 0) const;

    void DrawArraysInstanced(DrawMode mode, int32_t count, int32_t instanceCount, int32_t first = 0) const;
    void DrawElementsInstanced(DrawMode mode, int32_t count, int32_t instanceCount, int32_t offset = 0) const;

    void SetPolygonMode(PolygonFace face, PolygonMode mode) const;

    uint32_t GetVAO() const { return mVAO; }
	bool IsValid() const { return mVAO != 0; }

    void Bind() const;
    void Unbind() const;
private:
    uint32_t mVAO = 0;
};


class ShaderStorageBuffer {
public:
    bool Create(uint32_t bindingPoint = 0);
    ~ShaderStorageBuffer();
    void Destroy();

    void Bind() const;
    void Unbind() const;

    void Upload(const void* data, size_t size, BufferUsage usage = BufferUsage::StaticDraw);

    void Update(const void* data, size_t offset, size_t size);

    uint32_t GetID() const { return mSSBO; }
    uint32_t GetBindingPoint() const { return mBindingPoint; }

private:
    uint32_t mSSBO = 0;
    uint32_t mBindingPoint = 0;
};