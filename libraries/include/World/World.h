#ifndef WORLD_H
#define WORLD_H
#include <atomic>
#include <condition_variable>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_int3.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>
#include <functional>
#include <array>
#include <algorithm>
#include <cmath>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/hash.hpp>  // For ivec3/ivec2 hashing
#include "../glad/glad.h"
#include "../VBO/VBO.h"
#include "../PerlinNoise-3.0.0/PerlinNoise.hpp"
#define CHUNK_SIZE 16
using vec3 = glm::vec3;
using i_vec3 = glm::ivec3;
using i_vec2 = glm::ivec2;  // NEW: For height cache
using u_int32_t = GLuint;
using u_int8_t = uint8_t;
enum class BlockType {
    NONE,
    SOLID,
    AIR
};
enum class direction {
    POSITIVE_X = 0,
    NEGATIVE_X,
    POSITIVE_Y,
    NEGATIVE_Y,
    POSITIVE_Z,
    NEGATIVE_Z
};
struct Chunk {
    static constexpr int CS = CHUNK_SIZE;
    static constexpr int CS_SQR = CS * CS;
    std::array<BlockType, CS * CS * CS> blocks{};

    BlockType& get(int x, int y, int z) {
        return blocks[x + y * CS + z * CS_SQR];
    }
    const BlockType& get(int x, int y, int z) const {
        return blocks[x + y * CS + z * CS_SQR];
    }

    void initToAir() {
        std::fill(blocks.begin(), blocks.end(), BlockType::AIR);
    }
};
struct WorkResult{
    glm::ivec3 coord;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
};
class World {
private:
    std::vector<Vertex> GlobalVertices;
    std::vector<GLuint> GlobalIndices;
    std::unordered_map<glm::ivec3, Chunk> chunks;
    std::unordered_map<glm::ivec3, WorkResult> generatedMeshes;
    siv::PerlinNoise m_noise;
    // NEW: Height cache for noise (per global XZ column)
    std::unordered_map<glm::ivec2, int> heightCache;
    std::mutex heightCacheMutex;
    std::vector<std::thread> workers;
    std::queue<glm::ivec3> ChunksToGenerate;
    std::atomic<bool> running{true};
    std::mutex workerMutex;
    std::mutex resultMutex;
    std::mutex ChunkMapMutex;
    std::condition_variable cv;
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
    ~World();
    void setBlocks(glm::ivec3 chunkCoord , Chunk& currentChunk);
    void emitFace(direction dir, i_vec3 localCoordinates, i_vec3 globalOffset, std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
    void emitGreedyFace(i_vec3 localMinCorner, direction dir, int height, int width, i_vec3 globalOffset, std::vector<Vertex>& vertices , std::vector<GLuint>& indices);
    // UPDATED: No lambdas; direct meshing
    void greedyMeshSlice(const Chunk& current, const Chunk* neighbor, int fixed, direction dir, int localNeighCoord, i_vec3 globalOffset, std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
    void generateChunkMesh(glm::ivec3 chunkCoord , Chunk& currentChunk, std::vector<Vertex>& vertices , std::vector<GLuint>& indices) ;
    void ChunkManager(glm::vec3& cameraPosition, int renderRadius = 5);
    void MergeChunks();
    void fetchMergedMesh(std::vector<Vertex>& outVertices, std::vector<GLuint>& outIndices);
    std::vector<Vertex>& getVerticesReference();
    std::vector<GLuint>& getIndicesReference();
};
#endif
