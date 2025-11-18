#define GLM_ENABLE_EXPERIMENTAL

#include <vector>
#include <functional>
#include <array>
// Assume GLM for vec3, ivec3, cross, dot
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>  
#include "../libraries/include/glad/glad.h"
#include "../libraries/include/VBO/VBO.h"  // assume defines Vertex, etc.

#define CHUNK_SIZE 16

using vec3 = glm::vec3;
using i_vec3 = glm::ivec3;
using u_int32_t = GLuint;  // or uint32_t
using u_int8_t = uint8_t;

int windowWidth = 800;
int windowHeight = 600;

std::vector<Vertex> vertices;
std::vector<GLuint> indices;

enum class BlockType {
    SOLID,
    AIR,
    NONE
};

BlockType Blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

static const i_vec3 facePos[6][4] = {
    // 0: X+ (east) – face at x = 1
    { {1,0,0}, {1,1,0}, {1,1,1}, {1,0,1} },
    // 1: X- (west) – face at x = 0
    { {0,0,0}, {0,0,1}, {0,1,1}, {0,1,0} },
    // 2: Y+ (top) – face at y = 1
    { {0,1,0}, {0,1,1}, {1,1,1}, {1,1,0} },
    // 3: Y- (bottom) – face at y = 0
    { {0,0,0}, {1,0,0}, {1,0,1}, {0,0,1} },
    // 4: Z+ (north) – face at z = 1
    { {0,0,1}, {1,0,1}, {1,1,1}, {0,1,1} },
    // 5: Z- (south) – face at z = 0
    { {0,0,0}, {0,1,0}, {1,1,0}, {1,0,0} },
};

static const vec3 FaceNormal[6] = {
    {1,0,0},//+x
    {-1,0,0},
    {0,1,0},
    {0,-1,0},
    {0,0,1},
    {0,0,-1}
};

void setBlocks() {
    for(int x = 0; x < CHUNK_SIZE; x++) {
        for(int y = 0; y < CHUNK_SIZE; y++) {
            for(int z = 0; z < CHUNK_SIZE; z++) {
                Blocks[x][y][z] = BlockType::SOLID;
            }
        }
    }
}

enum class direction {
    POSITIVE_X = 0,
    NEGATIVE_X,
    POSITIVE_Y,
    NEGATIVE_Y,
    POSITIVE_Z,
    NEGATIVE_Z
};

void emitFace(u_int8_t direction, i_vec3 coordinates) {
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

void emitGreedyFace(i_vec3 min_corner, direction dir, int height, int width) {
    if (height <= 0 || width <= 0) return;
    GLuint start = static_cast<GLuint>(vertices.size());

    vec3 n = FaceNormal[static_cast<int>(dir)];

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

    // Unit spans for cross product sign
    vec3 unit_s1(0.0f);
    vec3 unit_s2(0.0f);
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
    vec3 cr = glm::cross(unit_s1, unit_s2);
    float dot_val = glm::dot(cr, n);
    bool flip_winding = (dot_val < 0.0f);

    // Min values for varies
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

    // Base position
    vec3 pos0(0.0f);
    if (fa == 0) {
        pos0 = vec3(static_cast<float>(fixed_val), static_cast<float>(minv1), static_cast<float>(minv2));
    } else if (fa == 1) {
        pos0 = vec3(static_cast<float>(minv1), static_cast<float>(fixed_val), static_cast<float>(minv2));
    } else {
        pos0 = vec3(static_cast<float>(minv1), static_cast<float>(minv2), static_cast<float>(fixed_val));
    }

    vec3 p0 = pos0;
    vec3 p1 = pos0;
    if (v2_axis == 0) p1.x += static_cast<float>(width);
    else if (v2_axis == 1) p1.y += static_cast<float>(width);
    else p1.z += static_cast<float>(width);

    vec3 p2 = pos0;
    if (v1_axis == 0) p2.x += static_cast<float>(height);
    else if (v1_axis == 1) p2.y += static_cast<float>(height);
    else p2.z += static_cast<float>(height);

    vec3 p3 = p2;
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

    // Indices based on winding
    if (!flip_winding) {
        // 0-2-1 , 2-3-1
        indices.push_back(start + 0); indices.push_back(start + 2); indices.push_back(start + 1);
        indices.push_back(start + 2); indices.push_back(start + 3); indices.push_back(start + 1);
    } else {
        // 0-1-2 , 1-3-2
        indices.push_back(start + 0); indices.push_back(start + 1); indices.push_back(start + 2);
        indices.push_back(start + 1); indices.push_back(start + 3); indices.push_back(start + 2);
    }
}

struct FaceAxis {
    std::function<BlockType(int, int, int)> getBlock;
    std::function<BlockType(int, int, int)> getNeighbor;
    std::function<void(i_vec3, int, int)> emitFace;
    int sizeA, sizeB;
};

void GreedyMesh_Generic(const FaceAxis& A) {
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
    // Step 2: Greedy merge
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

void generateMesh() {
    setBlocks();
    vertices.clear();
    indices.clear();

    // X+ faces
    for (int fixed = 0; fixed < CHUNK_SIZE; ++fixed) {
        auto getBlock = [fixed](int a, int b, int) -> BlockType {
            return Blocks[fixed][a][b];
        };
        auto getNeighbor = [fixed](int a, int b, int) -> BlockType {
            int nx = fixed + 1;
            if (nx < CHUNK_SIZE) return Blocks[nx][a][b];
            return BlockType::AIR;
        };
        auto emitF = [fixed](i_vec3 pos, int h, int w) {
            emitGreedyFace({fixed, pos.x, pos.y}, direction::POSITIVE_X, h, w);
        };
        FaceAxis axis{getBlock, getNeighbor, emitF, CHUNK_SIZE, CHUNK_SIZE};
        GreedyMesh_Generic(axis);
    }

    // X- faces
    for (int fixed = 0; fixed < CHUNK_SIZE; ++fixed) {
        auto getBlock = [fixed](int a, int b, int) -> BlockType {
            return Blocks[fixed][a][b];
        };
        auto getNeighbor = [fixed](int a, int b, int) -> BlockType {
            int nx = fixed - 1;
            if (nx >= 0) return Blocks[nx][a][b];
            return BlockType::AIR;
        };
        auto emitF = [fixed](i_vec3 pos, int h, int w) {
            emitGreedyFace({fixed, pos.x, pos.y}, direction::NEGATIVE_X, h, w);
        };
        FaceAxis axis{getBlock, getNeighbor, emitF, CHUNK_SIZE, CHUNK_SIZE};
        GreedyMesh_Generic(axis);
    }

    // Y+ faces
    for (int fixed = 0; fixed < CHUNK_SIZE; ++fixed) {
        auto getBlock = [fixed](int a, int b, int) -> BlockType {
            return Blocks[a][fixed][b];
        };
        auto getNeighbor = [fixed](int a, int b, int) -> BlockType {
            int ny = fixed + 1;
            if (ny < CHUNK_SIZE) return Blocks[a][ny][b];
            return BlockType::AIR;
        };
        auto emitF = [fixed](i_vec3 pos, int h, int w) {
            emitGreedyFace({pos.x, fixed, pos.y}, direction::POSITIVE_Y, h, w);
        };
        FaceAxis axis{getBlock, getNeighbor, emitF, CHUNK_SIZE, CHUNK_SIZE};
        GreedyMesh_Generic(axis);
    }

    // Y- faces
    for (int fixed = 0; fixed < CHUNK_SIZE; ++fixed) {
        auto getBlock = [fixed](int a, int b, int) -> BlockType {
            return Blocks[a][fixed][b];
        };
        auto getNeighbor = [fixed](int a, int b, int) -> BlockType {
            int ny = fixed - 1;
            if (ny >= 0) return Blocks[a][ny][b];
            return BlockType::AIR;
        };
        auto emitF = [fixed](i_vec3 pos, int h, int w) {
            emitGreedyFace({pos.x, fixed, pos.y}, direction::NEGATIVE_Y, h, w);
        };
        FaceAxis axis{getBlock, getNeighbor, emitF, CHUNK_SIZE, CHUNK_SIZE};
        GreedyMesh_Generic(axis);
    }

    // Z+ faces
    for (int fixed = 0; fixed < CHUNK_SIZE; ++fixed) {
        auto getBlock = [fixed](int a, int b, int) -> BlockType {
            return Blocks[a][b][fixed];
        };
        auto getNeighbor = [fixed](int a, int b, int) -> BlockType {
            int nz = fixed + 1;
            if (nz < CHUNK_SIZE) return Blocks[a][b][nz];
            return BlockType::AIR;
        };
        auto emitF = [fixed](i_vec3 pos, int h, int w) {
            emitGreedyFace({pos.x, pos.y, fixed}, direction::POSITIVE_Z, h, w);
        };
        FaceAxis axis{getBlock, getNeighbor, emitF, CHUNK_SIZE, CHUNK_SIZE};
        GreedyMesh_Generic(axis);
    }

    // Z- faces
    for (int fixed = 0; fixed < CHUNK_SIZE; ++fixed) {
        auto getBlock = [fixed](int a, int b, int) -> BlockType {
            return Blocks[a][b][fixed];
        };
        auto getNeighbor = [fixed](int a, int b, int) -> BlockType {
            int nz = fixed - 1;
            if (nz >= 0) return Blocks[a][b][nz];
            return BlockType::AIR;
        };
        auto emitF = [fixed](i_vec3 pos, int h, int w) {
            emitGreedyFace({pos.x, pos.y, fixed}, direction::NEGATIVE_Z, h, w);
        };
        FaceAxis axis{getBlock, getNeighbor, emitF, CHUNK_SIZE, CHUNK_SIZE};
        GreedyMesh_Generic(axis);
    }
}
