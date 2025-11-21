#ifndef WORLD_H
#define WORLD_H
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_int3.hpp>
#include <map>
#define CHUNK_SIZE 16
#define WORLD_SIZE 1
#include <vector>
#include "../VBO/VBO.h"
#include <functional>

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

struct Chunk{
    BlockType chunk[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
};



class World{

private:
    Chunk Blocks;
    std::map<glm::ivec3, bool> isGenerated;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    static constexpr glm::vec3 facePos[6][4] = {
        { {1,0,0}, {1,1,0}, {1,1,1}, {1,0,1} },
        { {0,0,0}, {0,0,1}, {0,1,1}, {0,1,0} },
        { {0,1,0}, {0,1,1}, {1,1,1}, {1,1,0} },
        { {0,0,0}, {1,0,0}, {1,0,1}, {0,0,1} },
        { {0,0,1}, {1,0,1}, {1,1,1}, {0,1,1} },
        { {0,0,0}, {0,1,0}, {1,1,0}, {1,0,0} },
    };

    static constexpr glm::vec3 FaceNormal[6] = {
        {1,0,0},//+x
        {-1,0,0},
        {0,1,0},
        {0,-1,0},
        {0,0,1},
        {0,0,-1}
    };

public:

    void setBlocks(glm::ivec3 chunkStart);
    void emitFace(u_int8_t direction, i_vec3 coordinates) ;
    void emitGreedyFace(i_vec3 min_corner, direction dir, int height, int width) ;
    void GreedyMesh_Generic(const FaceAxis& A) ;
    void generateMesh();
    void ChunkManager(glm::vec3& cameraPosition);
    std::vector<Vertex>& getVerticesRefrence();
    std::vector<GLuint>& getIndicesRefrence();
    
};
#endif
