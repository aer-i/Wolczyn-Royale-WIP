#pragma once
#include "Model.hpp"
#include <Arln.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <meshoptimizer.h>

class MeshImporter {
    using v0 = void;
public:
    MeshImporter() = default;
    inline ~MeshImporter() noexcept { m_vb.free(); m_ib.free(); }
    auto fBf() noexcept -> v0;
    auto lFl(std::string_view t_fp, std::vector<Mesh>& t_mhs) noexcept -> v0;
public:
    inline auto& gVb() noexcept { return m_vb; }
    inline auto& gIb() noexcept { return m_ib; }
private:
    auto pN(aiNode* t_n, aiScene const* t_s, std::vector<Mesh>& t_mhs) noexcept -> v0;
    auto pM(aiMesh* t_m, aiScene const* t_s) noexcept -> Mesh;
    auto oM(std::vector<Vertex>& t_vxv, std::vector<arln::u32>& t_ixv) noexcept -> Mesh;
public:
    arln::Buffer m_vb, m_ib;
    std::vector<Vertex> m_vts;
    std::vector<arln::u32> m_ixs;
};