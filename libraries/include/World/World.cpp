#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <vector>
#include <functional>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>  
#include "../glad/glad.h"
#include "../VBO/VBO.h"  
#include "./World.h"


#define CHUNK_SIZE 16

void World::setBlocks(glm::ivec3 chunkStart) {
    for(int x = 0; x < CHUNK_SIZE; x++) {
        for(int y = 0; y < CHUNK_SIZE; y++) {
            for(int z = 0 ; z < CHUNK_SIZE;z++){
                Blocks.chunk[x][y][z] = BlockType::SOLID;
            }
        }
    }
}

void World::emitFace(u_int8_t direction, glm::ivec3 coordinates) {
    u_int32_t start = vertices.size();
    for(int i = 0; i < 4; i++) {
        Vertex v;
        v.Position.x = facePos[direction][i].x + coordinates.x;
        v.Position.y = facePos[direction][i].y + coordinates.y;
        v.Position.z = facePos[direction][i].z + coordinates.z;
        v.Normal = FaceNormal[direction];
        vertices.push_back(v);
    }
    indices.push_back(start + 0);
    indices.push_back(start + 1);
    indices.push_back(start + 2);
    indices.push_back(start + 2);
    indices.push_back(start + 3);
    indices.push_back(start + 0);
}

void World::emitGreedyFace(glm::ivec3 min_corner, direction dir, int height, int width) {
    if (height <= 0 || width <= 0) return;
    GLuint start = static_cast<GLuint>(vertices.size());

    glm::vec3 n = FaceNormal[static_cast<int>(dir)];

    int fa = static_cast<int>(dir) / 2;
    bool is_positive = (static_cast<int>(dir) % 2 == 0);
    int offset = is_positive ? 1 : 0;
    int fixed_val = min_corner[fa] + offset;

    int v1_axis, v2_axis;
    if (fa == 0) {
        v1_axis = 1;  // y
        v2_axis = 2;  // z
    } else if (fa == 1) {
        v1_axis = 0;  // x
        v2_axis = 2;  // z
    } else {
        v1_axis = 0;  // x
        v2_axis = 1;  // y
    }

    glm::vec3 unit_s1(0.0f);
    glm::vec3 unit_s2(0.0f);
    if (fa == 0) {
        unit_s1.y = 1.0f;
        unit_s2.z = 1.0f;
    } else if (fa == 1) {
        unit_s1.x = 1.0f;
        unit_s2.z = 1.0f;
    } else {
        unit_s1.x = 1.0f;
        unit_s2.y = 1.0f;
    }
    glm::vec3 cr = glm::cross(unit_s1, unit_s2);
    float dot_val = glm::dot(cr, n);
    bool flip_winding = (dot_val < 0.0f);

    int minv1, minv2;
    if (fa == 0) {
        minv1 = min_corner.y;
        minv2 = min_corner.z;
    } else if (fa == 1) {
        minv1 = min_corner.x;
        minv2 = min_corner.z;
    } else {
        minv1 = min_corner.x;
        minv2 = min_corner.y;
    }

    glm::vec3 pos0(0.0f);
    if (fa == 0) {
        pos0 = glm::vec3(static_cast<float>(fixed_val), static_cast<float>(minv1), static_cast<float>(minv2));
    } else if (fa == 1) {
        pos0 = glm::vec3(static_cast<float>(minv1), static_cast<float>(fixed_val), static_cast<float>(minv2));
    } else {
        pos0 = glm::vec3(static_cast<float>(minv1), static_cast<float>(minv2), static_cast<float>(fixed_val));
    }

    glm::vec3 p0 = pos0;
    glm::vec3 p1 = pos0;
    if (v2_axis == 0) p1.x += static_cast<float>(width);
    else if (v2_axis == 1) p1.y += static_cast<float>(width);
    else p1.z += static_cast<float>(width);

    glm::vec3 p2 = pos0;
    if (v1_axis == 0) p2.x += static_cast<float>(height);
    else if (v1_axis == 1) p2.y += static_cast<float>(height);
    else p2.z += static_cast<float>(height);

    glm::vec3 p3 = p2;
    if (v2_axis == 0) p3.x += static_cast<float>(width);
    else if (v2_axis == 1) p3.y += static_cast<float>(width);
    else p3.z += static_cast<float>(width);

    std::array<Vertex, 4> vs;
    vs[0].Position = p0; vs[0].Normal = n;
    vs[1].Position = p1; vs[1].Normal = n;
    vs[2].Position = p2; vs[2].Normal = n;
    vs[3].Position = p3; vs[3].Normal = n;
    for (const auto& v : vs) {
        vertices.push_back(v);
    }

    if (!flip_winding) {
        indices.push_back(start + 0); indices.push_back(start + 2); indices.push_back(start + 1);
        indices.push_back(start + 2); indices.push_back(start + 3); indices.push_back(start + 1);
    } else {
        indices.push_back(start + 0); indices.push_back(start + 1); indices.push_back(start + 2);
        indices.push_back(start + 1); indices.push_back(start + 3); indices.push_back(start + 2);
    }
}


void World::GreedyMesh_Generic(const FaceAxis& A) {
    static BlockType mask[CHUNK_SIZE][CHUNK_SIZE];
    // Step 1: Build mask
    for (int a = 0; a < A.sizeA; a++) {
        for (int b = 0; b < A.sizeB; b++) {
            BlockType current = A.getBlock(a, b, 0);
            BlockType neighbor = A.getNeighbor(a, b, 0);
            mask[a][b] = (current != BlockType::AIR && neighbor == BlockType::AIR)
                ? current
                : BlockType::NONE;
        }
    }
    for (int a = 0; a < A.sizeA; a++) {
        for (int b = 0; b < A.sizeB; ) {
            if (mask[a][b] == BlockType::NONE) { b++; continue; }
            BlockType type = mask[a][b];
            int w = 1;
            while (b + w < A.sizeB && mask[a][b + w] == type)
                w++;
            int h = 1;
            while (a + h < A.sizeA) {
                bool ok = true;
                for (int bb = 0; bb < w; bb++)
                    if (mask[a + h][b + bb] != type)
                        ok = false;
                if (!ok) break;
                h++;
            }
            // Emit
            A.emitFace({a, b, 0}, h, w);
            // Mark used
            for (int aa = 0; aa < h; aa++)
                for (int bb = 0; bb < w; bb++)
                    mask[a + aa][b + bb] = BlockType::NONE;
            b += w;
        }
    }
}

void World::generateMesh() {
    for (int fixed = 0; fixed < CHUNK_SIZE; ++fixed) {
        auto getBlock = [fixed , this](int a, int b, int) -> BlockType {
            return Blocks.chunk[fixed][a][b];
        };
        auto getNeighbor = [fixed , this](int a, int b, int) -> BlockType {
            int nx = fixed + 1;
            if (nx < CHUNK_SIZE) return Blocks.chunk[nx][a][b];
            return BlockType::AIR;
        };
        auto emitF = [fixed , this](glm::ivec3 pos, int h, int w) {
            emitGreedyFace({fixed, pos.x, pos.y}, direction::POSITIVE_X, h, w);
        };
        FaceAxis axis{getBlock, getNeighbor, emitF, CHUNK_SIZE, CHUNK_SIZE};
        GreedyMesh_Generic(axis);
    }

    for (int fixed = 0; fixed < CHUNK_SIZE; ++fixed) {
        auto getBlock = [fixed , this](int a, int b, int) -> BlockType {
            return Blocks.chunk[fixed][a][b];
        };
        auto getNeighbor = [fixed , this](int a, int b, int) -> BlockType {
            int nx = fixed - 1;
            if (nx >= 0) return Blocks.chunk[nx][a][b];
            return BlockType::AIR;
        };
        auto emitF = [fixed , this](glm::ivec3 pos, int h, int w) {
            emitGreedyFace({fixed, pos.x, pos.y}, direction::NEGATIVE_X, h, w);
        };
        FaceAxis axis{getBlock, getNeighbor, emitF, CHUNK_SIZE, CHUNK_SIZE};
        GreedyMesh_Generic(axis);
    }

    for (int fixed = 0; fixed < CHUNK_SIZE; ++fixed) {
        auto getBlock = [fixed , this](int a, int b, int) -> BlockType {
            return Blocks.chunk[a][fixed][b];
        };
        auto getNeighbor = [fixed , this](int a, int b, int) -> BlockType {
            int ny = fixed + 1;
            if (ny < CHUNK_SIZE) return Blocks.chunk[a][ny][b];
            return BlockType::AIR;
        };
        auto emitF = [fixed , this](glm::ivec3 pos, int h, int w) {
            emitGreedyFace({pos.x, fixed, pos.y}, direction::POSITIVE_Y, h, w);
        };
        FaceAxis axis{getBlock, getNeighbor, emitF, CHUNK_SIZE, CHUNK_SIZE};
        GreedyMesh_Generic(axis);
    }

    for (int fixed = 0; fixed < CHUNK_SIZE; ++fixed) {
        auto getBlock = [fixed , this](int a, int b, int) -> BlockType {
            return Blocks.chunk[a][fixed][b];
        };
        auto getNeighbor = [fixed , this](int a, int b, int) -> BlockType {
            int ny = fixed - 1;
            if (ny >= 0) return Blocks.chunk[a][ny][b];
            return BlockType::AIR;
        };
        auto emitF = [fixed , this](glm::ivec3 pos, int h, int w) {
            emitGreedyFace({pos.x, fixed, pos.y}, direction::NEGATIVE_Y, h, w);
        };
        FaceAxis axis{getBlock, getNeighbor, emitF, CHUNK_SIZE, CHUNK_SIZE};
        GreedyMesh_Generic(axis);
    }

    for (int fixed = 0; fixed < CHUNK_SIZE; ++fixed) {
        auto getBlock = [fixed , this](int a, int b, int) -> BlockType {
            return Blocks.chunk[a][b][fixed];
        };
        auto getNeighbor = [fixed , this](int a, int b, int) -> BlockType {
            int nz = fixed + 1;
            if (nz < CHUNK_SIZE) return Blocks.chunk[a][b][nz];
            return BlockType::AIR;
        };
        auto emitF = [fixed , this](glm::ivec3 pos, int h, int w) {
            emitGreedyFace({pos.x, pos.y, fixed}, direction::POSITIVE_Z, h, w);
        };
        FaceAxis axis{getBlock, getNeighbor, emitF, CHUNK_SIZE, CHUNK_SIZE};
        GreedyMesh_Generic(axis);
    }

    for (int fixed = 0; fixed < CHUNK_SIZE; ++fixed) {
        auto getBlock = [fixed , this](int a, int b, int) -> BlockType {
            return Blocks.chunk[a][b][fixed];
        };
        auto getNeighbor = [fixed , this](int a, int b, int) -> BlockType {
            int nz = fixed - 1;
            if (nz >= 0) return Blocks.chunk[a][b][nz];
            return BlockType::AIR;
        };
        auto emitF = [fixed , this](glm::ivec3 pos, int h, int w) {
            emitGreedyFace({pos.x, pos.y, fixed}, direction::NEGATIVE_Z, h, w);
        };
        FaceAxis axis{getBlock, getNeighbor, emitF, CHUNK_SIZE, CHUNK_SIZE};
        GreedyMesh_Generic(axis);
    }
}

std::vector<Vertex>& World::getVerticesRefrence(){
    return vertices;
}


std::vector<GLuint>& World::getIndicesRefrence(){
    return indices;
}
