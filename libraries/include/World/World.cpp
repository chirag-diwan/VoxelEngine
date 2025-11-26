#include "World.h"
#include <algorithm>
#include <glm/ext/vector_int3.hpp>
#include <mutex>
#include <thread>
#include <cmath>

World::World() : m_noise(12345u), running(true) {
    heightCache.reserve(10000);  
    unsigned num_threads = std::thread::hardware_concurrency();
    if (num_threads > 0) {
        num_threads--;
    }
    for (size_t i = 0; i < num_threads; ++i) {
        workers.emplace_back([this]() {
            std::vector<Vertex> vertices;
            std::vector<GLuint> indices;
            vertices.reserve(16384);  
            indices.reserve(24576);   
            while (running) {
                glm::ivec3 ChunkCoord;
                {
                    std::unique_lock<std::mutex> workerLock(workerMutex);
                    cv.wait(workerLock, [&] { return !ChunksToGenerate.empty() || !running; });
                    if (!running) break;
                    ChunkCoord = ChunksToGenerate.front();
                    ChunksToGenerate.pop();
                }
                Chunk chunk;
                chunk.initToAir();
                setBlocks(ChunkCoord, chunk);
                generateChunkMesh(ChunkCoord, chunk, vertices, indices);
                {
                    std::unique_lock<std::mutex> resultLock(resultMutex);
                    generatedMeshes[ChunkCoord] = {std::move(ChunkCoord), std::move(vertices), std::move(indices)};
                }
                vertices.clear();
                indices.clear();
            }
        });
    }
}

void World::MergeChunks() {
    GlobalVertices.clear();
    GlobalIndices.clear();
    {
        std::unique_lock<std::mutex> lock(resultMutex);
        GLuint offset = 0;
        for (const auto& p : generatedMeshes) {
            const auto& res = p.second;
            GLuint currOffset = static_cast<GLuint>(GlobalVertices.size());
            for (GLuint idx : res.indices) {
                GlobalIndices.push_back(idx + currOffset);
            }
            GlobalVertices.insert(GlobalVertices.end(), res.vertices.begin(), res.vertices.end());
            offset += static_cast<GLuint>(res.vertices.size());
        }
    }
}

void World::setBlocks(glm::ivec3 chunkCoord, Chunk& currentChunk) {
    constexpr float scale = 0.00008f;
    constexpr int octaves = 7;
    constexpr float persistence = 0.8f;
    constexpr float baseHeight = 32.0f;
    constexpr float heightAmp = 400.0f;
    for (int lx = 0; lx < CHUNK_SIZE; ++lx) {
        for (int lz = 0; lz < CHUNK_SIZE; ++lz) {
            int globalX = chunkCoord.x * CHUNK_SIZE + lx;
            int globalZ = chunkCoord.z * CHUNK_SIZE + lz;
            glm::ivec2 key(globalX, globalZ);  
            int terrainHeight;
            {
                std::unique_lock<std::mutex> lock(heightCacheMutex);  
                auto it = heightCache.find(key);
                if (it == heightCache.end()) {
                    float globalXf = static_cast<float>(globalX);
                    float globalZf = static_cast<float>(globalZ);
                    float rawNoise = m_noise.octave2D(globalXf * scale, globalZf * scale, octaves, persistence);
                    float noiseVal = (rawNoise + 1.0f) / 2.0f;
                    float terrainHeightFloat = baseHeight + (noiseVal - 0.5f) * heightAmp * 2.0f;
                    terrainHeight = static_cast<int>(std::floor(terrainHeightFloat));
                    heightCache[key] = terrainHeight;  
                } else {
                    terrainHeight = it->second;
                }
            }
            for (int ly = 0; ly < CHUNK_SIZE; ++ly) {
                int globalY = chunkCoord.y * CHUNK_SIZE + ly;
                currentChunk.get(lx, ly, lz) = (globalY < terrainHeight) ? BlockType::SOLID : BlockType::AIR;
            }
        }
    }
    {
        std::unique_lock<std::mutex> ChunkMapLock(ChunkMapMutex);
        chunks[chunkCoord] = currentChunk;
    }
}

void World::emitFace(direction dir, glm::ivec3 localCoordinates, glm::ivec3 globalOffset, std::vector<Vertex>& vertices, std::vector<GLuint>& indices) {
    size_t directionIndex = static_cast<size_t>(dir);
    u_int32_t start = static_cast<u_int32_t>(vertices.size());
    for (int i = 0; i < 4; ++i) {
        Vertex v;
        v.Position = facePos[directionIndex][i] + glm::vec3(localCoordinates) + glm::vec3(globalOffset);
        v.Normal = FaceNormal[directionIndex];
        vertices.push_back(v);
    }
    indices.push_back(start + 0);
    indices.push_back(start + 1);
    indices.push_back(start + 2);
    indices.push_back(start + 2);
    indices.push_back(start + 3);
    indices.push_back(start + 0);
}

void World::emitGreedyFace(glm::ivec3 localMinCorner, direction dir, int height, int width, glm::ivec3 globalOffset, std::vector<Vertex>& vertices, std::vector<GLuint>& indices) {
    if (height <= 0 || width <= 0) return;
    GLuint start = static_cast<GLuint>(vertices.size());
    glm::vec3 n = FaceNormal[static_cast<int>(dir)];
    int fa = static_cast<int>(dir) / 2;
    bool is_positive = (static_cast<int>(dir) % 2 == 0);
    int offset = is_positive ? 1 : 0;
    int fixed_val = localMinCorner[fa] + offset;
    int v1_axis, v2_axis;
    if (fa == 0) {
        v1_axis = 1;
        v2_axis = 2;
    } else if (fa == 1) {
        v1_axis = 0;
        v2_axis = 2;
    } else {
        v1_axis = 0;
        v2_axis = 1;
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
        minv1 = localMinCorner.y;
        minv2 = localMinCorner.z;
    } else if (fa == 1) {
        minv1 = localMinCorner.x;
        minv2 = localMinCorner.z;
    } else {
        minv1 = localMinCorner.x;
        minv2 = localMinCorner.y;
    }
    glm::vec3 pos0(0.0f);
    if (fa == 0) {
        pos0 = glm::vec3(static_cast<float>(fixed_val), static_cast<float>(minv1), static_cast<float>(minv2));
    } else if (fa == 1) {
        pos0 = glm::vec3(static_cast<float>(minv1), static_cast<float>(fixed_val), static_cast<float>(minv2));
    } else {
        pos0 = glm::vec3(static_cast<float>(minv1), static_cast<float>(minv2), static_cast<float>(fixed_val));
    }
    pos0 += glm::vec3(globalOffset);
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


void World::greedyMeshSlice(const Chunk& current, const Chunk* neighbor, int fixed, direction dir, int localNeighCoord, i_vec3 globalOffset, std::vector<Vertex>& vertices, std::vector<GLuint>& indices) {
    bool mask[CHUNK_SIZE][CHUNK_SIZE];  
    std::fill_n(reinterpret_cast<bool*>(mask), CHUNK_SIZE * CHUNK_SIZE, false);
    int sizeA, sizeB, blockX, blockY, blockZ, neighX, neighY, neighZ;
    int axis = static_cast<int>(dir) / 2;
    bool isPos = (static_cast<int>(dir) % 2 == 0);
    if (axis == 0) {  
        sizeA = CHUNK_SIZE; sizeB = CHUNK_SIZE;  
        blockX = fixed; blockY = 0; blockZ = 0;
        neighX = isPos ? localNeighCoord : localNeighCoord; neighY = 0; neighZ = 0;
    } else if (axis == 1) {  
        sizeA = CHUNK_SIZE; sizeB = CHUNK_SIZE;  
        blockX = 0; blockY = fixed; blockZ = 0;
        neighX = 0; neighY = isPos ? localNeighCoord : localNeighCoord; neighZ = 0;
    } else {  
        sizeA = CHUNK_SIZE; sizeB = CHUNK_SIZE;  
        blockX = 0; blockY = 0; blockZ = fixed;
        neighX = 0; neighY = 0; neighZ = isPos ? localNeighCoord : localNeighCoord;
    }
    
    for (int a = 0; a < sizeA; ++a) {
        for (int b = 0; b < sizeB; ++b) {
            int lx = (axis == 0) ? fixed : ((axis == 1) ? a : a);
            int ly = (axis == 1) ? fixed : ((axis == 2) ? b : a);
            int lz = (axis == 2) ? fixed : ((axis == 0) ? b : b);
            BlockType currentBlock = current.get(lx, ly, lz);
            BlockType neighBlock;
            if (neighbor) {
                neighBlock = neighbor->get(neighX + (axis==0 ? 0 : (axis==1 ? a : a)),  
                                          neighY + (axis==1 ? 0 : (axis==2 ? b : a)),
                                          neighZ + (axis==2 ? 0 : (axis==0 ? b : b)));
            } else {
                neighBlock = BlockType::AIR;
            }
            mask[a][b] = (currentBlock == BlockType::SOLID && neighBlock == BlockType::AIR);  
        }
    }
    
    for (int a = 0; a < sizeA; ++a) {
        for (int b = 0; b < sizeB; ) {
            if (!mask[a][b]) { ++b; continue; }
            int w = 1;
            while (b + w < sizeB && mask[a][b + w]) ++w;
            int h = 1;
            while (a + h < sizeA) {
                bool ok = true;
                for (int bb = 0; bb < w; ++bb) {
                    if (!mask[a + h][b + bb]) {
                        ok = false;
                        break;
                    }
                }
                if (!ok) break;
                ++h;
            }
            
            i_vec3 pos;
            if (axis == 0) pos = {fixed, a, b};
            else if (axis == 1) pos = {a, fixed, b};
            else pos = {a, b, fixed};
            emitGreedyFace(pos, dir, h, w, globalOffset, vertices, indices);
            for (int aa = 0; aa < h; ++aa) {
                for (int bb = 0; bb < w; ++bb) {
                    mask[a + aa][b + bb] = false;
                }
            }
            b += w;
        }
    }
}


void World::generateChunkMesh(glm::ivec3 chunkCoord, Chunk& currentChunk, std::vector<Vertex>& vertices, std::vector<GLuint>& indices) {
    {
        std::unique_lock<std::mutex> lock(ChunkMapMutex);
        if (chunks.find(chunkCoord) == chunks.end()) return;
    }
    glm::ivec3 globalOffset = chunkCoord * glm::ivec3(CHUNK_SIZE);

    
    std::array<const Chunk*, 6> neighborChunks{};
    {
        std::unique_lock<std::mutex> lock(ChunkMapMutex);
        for (int d = 0; d < 6; ++d) {
            int axis = d / 2;
            int delta = (d % 2 == 0) ? 1 : -1;
            glm::ivec3 neighC = chunkCoord;
            neighC[axis] += delta;
            auto it = chunks.find(neighC);
            if (it != chunks.end()) {
                neighborChunks[d] = &it->second;
            }
        }
    }

    
    
    {
        direction dir = direction::POSITIVE_X;
        const Chunk* neigh = neighborChunks[0];
        int localN = 0;
        for (int fixed = 0; fixed < CHUNK_SIZE; ++fixed) {
            greedyMeshSlice(currentChunk, neigh, fixed, dir, localN, globalOffset, vertices, indices);
        }
    }
    
    {
        direction dir = direction::NEGATIVE_X;
        const Chunk* neigh = neighborChunks[1];
        int localN = CHUNK_SIZE - 1;
        for (int fixed = 0; fixed < CHUNK_SIZE; ++fixed) {
            greedyMeshSlice(currentChunk, neigh, fixed, dir, localN, globalOffset, vertices, indices);
        }
    }
    
    {
        direction dir = direction::POSITIVE_Y;
        const Chunk* neigh = neighborChunks[2];
        int localN = 0;
        for (int fixed = 0; fixed < CHUNK_SIZE; ++fixed) {
            greedyMeshSlice(currentChunk, neigh, fixed, dir, localN, globalOffset, vertices, indices);
        }
    }
    
    {
        direction dir = direction::NEGATIVE_Y;
        const Chunk* neigh = neighborChunks[3];
        int localN = CHUNK_SIZE - 1;
        for (int fixed = 0; fixed < CHUNK_SIZE; ++fixed) {
            greedyMeshSlice(currentChunk, neigh, fixed, dir, localN, globalOffset, vertices, indices);
        }
    }
    
    {
        direction dir = direction::POSITIVE_Z;
        const Chunk* neigh = neighborChunks[4];
        int localN = 0;
        for (int fixed = 0; fixed < CHUNK_SIZE; ++fixed) {
            greedyMeshSlice(currentChunk, neigh, fixed, dir, localN, globalOffset, vertices, indices);
        }
    }
    
    {
        direction dir = direction::NEGATIVE_Z;
        const Chunk* neigh = neighborChunks[5];
        int localN = CHUNK_SIZE - 1;
        for (int fixed = 0; fixed < CHUNK_SIZE; ++fixed) {
            greedyMeshSlice(currentChunk, neigh, fixed, dir, localN, globalOffset, vertices, indices);
        }
    }
}

void World::ChunkManager(glm::vec3& cameraPosition, int renderRadius) {
    glm::ivec3 camChunkCoord = glm::floor(cameraPosition / static_cast<float>(CHUNK_SIZE));
    std::vector<glm::ivec3> toUnload;
    {
        std::unique_lock<std::mutex> lock(ChunkMapMutex);
        for (const auto& p : chunks) {
            glm::ivec3 coord = p.first;
            glm::vec3 delta = glm::abs(glm::vec3(coord - camChunkCoord));
            float maxDist = std::max({delta.x, delta.y, delta.z});
            if (maxDist > renderRadius + 1) {
                toUnload.push_back(coord);
            }
        }
        for (const auto& coord : toUnload) {
            chunks.erase(coord);
        }
    }
    {
        std::unique_lock<std::mutex> lock(resultMutex);
        for (const auto& coord : toUnload) {
            generatedMeshes.erase(coord);
        }
    }
    std::vector<glm::ivec3> toGenerate;
    {
        std::unique_lock<std::mutex> lock(ChunkMapMutex);
        for (int dx = -renderRadius; dx <= renderRadius; ++dx) {
            for (int dy = -renderRadius; dy <= renderRadius; ++dy) {
                for (int dz = -renderRadius; dz <= renderRadius; ++dz) {
                    glm::ivec3 targetCoord = camChunkCoord + glm::ivec3(dx, dy, dz);
                    glm::vec3 delta = glm::abs(glm::vec3(glm::ivec3(dx, dy, dz)));
                    float maxDist = std::max({delta.x, delta.y, delta.z});
                    if (maxDist <= static_cast<float>(renderRadius) && chunks.find(targetCoord) == chunks.end()) {
                        toGenerate.push_back(targetCoord);
                    }
                }
            }
        }
    }
    {
        std::unique_lock<std::mutex> lock(workerMutex);
        for (const auto& coord : toGenerate) {
            ChunksToGenerate.push(coord);
        }
    }
    cv.notify_all();
}

void World::fetchMergedMesh(std::vector<Vertex>& outVertices, std::vector<GLuint>& outIndices) {
    outVertices.clear();
    outIndices.clear();
    {
        std::unique_lock<std::mutex> lock(resultMutex);
        size_t estVerts = 0;
        for (const auto& p : generatedMeshes) estVerts += p.second.vertices.size();  
        outVertices.reserve(estVerts);
        outIndices.reserve(estVerts * 1.5);  
        GLuint offset = 0;
        for (const auto& p : generatedMeshes) {
            const auto& res = p.second;
            GLuint currOffset = static_cast<GLuint>(outVertices.size());
            for (GLuint idx : res.indices) {
                outIndices.push_back(idx + currOffset);
            }
            outVertices.insert(outVertices.end(), res.vertices.begin(), res.vertices.end());
        }
    }
}

std::vector<Vertex>& World::getVerticesReference() {
    return GlobalVertices;
}
std::vector<GLuint>& World::getIndicesReference() {
    return GlobalIndices;
}

World::~World() {
    running = false;
    cv.notify_all();
    for (auto& w : workers) {
        if (w.joinable()) w.join();
    }
}
