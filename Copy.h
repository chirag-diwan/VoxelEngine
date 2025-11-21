
#ifndef WORLD_H
#define WORLD_H

#include <atomic>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_int3.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <queue>
#include <thread>
#include <unordered_map>
#include <map>
#include <vector>
#include <functional>
#include <array>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../glad/glad.h"
#include "../VBO/VBO.h"
#include "../PerlinNoise-3.0.0/PerlinNoise.hpp"

#define CHUNK_SIZE 16
#define WORLD_SIZE 1

using vec3 = glm::vec3;
using i_vec3 = glm::ivec3;
using u_int32_t = GLuint;
using u_int8_t = uint8_t;

enum class BlockType {
    SOLID,
    AIR,
    NONE
};

enum class direction {
    POSITIVE_X = 0,
    NEGATIVE_X,
    POSITIVE_Y,
    NEGATIVE_Y,
    POSITIVE_Z,
    NEGATIVE_Z
};

struct FaceAxis {
    std::function<BlockType(int, int, int)> getBlock;
    std::function<BlockType(int, int, int)> getNeighbor;
    std::function<void(i_vec3, int, int)> emitFace;
    int sizeA, sizeB;
};

struct Chunk {
    BlockType chunk[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
};


namespace std {
    template <> struct hash<glm::ivec3> {
        size_t operator()(const glm::ivec3& v) const {
            size_t h1 = hash<int>()(v.x);
            size_t h2 = hash<int>()(v.y);
            size_t h3 = hash<int>()(v.z);
            return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 2);
        }
    };
}

struct WorkResult{
    glm::ivec3 coord;
    Chunk chunk;
};

class World {
private:
    std::unordered_map<glm::ivec3, Chunk> chunks;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    siv::PerlinNoise m_noise;
    std::vector<std::thread> workers;
    std::queue<glm::ivec3> ChunksToGenerate;
    std::queue<WorkResult> GeneratedChunks;
    std::atomic<bool> running = true;

    static constexpr glm::vec3 facePos[6][4] = {
        { {1,0,0}, {1,1,0}, {1,1,1}, {1,0,1} },  
        { {0,0,0}, {0,0,1}, {0,1,1}, {0,1,0} },  
        { {0,1,0}, {0,1,1}, {1,1,1}, {1,1,0} },  
        { {0,0,0}, {1,0,0}, {1,0,1}, {0,0,1} },  
        { {0,0,1}, {1,0,1}, {1,1,1}, {0,1,1} },  
        { {0,0,0}, {0,1,0}, {1,1,0}, {1,0,0} }   
    };

    static constexpr glm::vec3 FaceNormal[6] = {
        {1,0,0},   
        {-1,0,0},  
        {0,1,0},   
        {0,-1,0},  
        {0,0,1},   
        {0,0,-1}   
    };

public:
    World();

    void setBlocks(glm::ivec3 chunkCoord);
    void emitFace(u_int8_t direction, i_vec3 localCoordinates, i_vec3 globalOffset = i_vec3(0));
    void emitGreedyFace(i_vec3 localMinCorner, direction dir, int height, int width, i_vec3 globalOffset = i_vec3(0));
    void GreedyMesh_Generic(const FaceAxis& A);
    void generateChunkMesh(glm::ivec3 chunkCoord);
    void buildGlobalMesh();
    void ChunkManager(glm::vec3& cameraPosition, int renderRadius = 5);
    std::vector<Vertex>& getVerticesRefrence();
    std::vector<GLuint>& getIndicesRefrence();
};

#endif
