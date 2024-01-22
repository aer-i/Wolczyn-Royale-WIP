#pragma once
#include "Model.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <meshoptimizer.h>

class MeshImporter
{
    using v0 = void;
public:
    explicit MeshImporter(std::vector<Mesh>& t_mhs) noexcept : m_mhs{ t_mhs } {}
    auto lFl(std::string_view t_fp) noexcept -> v0;
private:
    auto pN(aiNode* t_n, aiScene const* t_s) noexcept -> v0;
    auto pM(aiMesh* t_m, aiScene const* t_s) noexcept -> Mesh;
public:
    std::vector<Mesh>& m_mhs;
    std::vector<Vertex> m_vts;
    std::vector<arln::u32> m_ixs;
};