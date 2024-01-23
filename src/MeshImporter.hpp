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
    inline ~MeshImporter() noexcept { m_vb.free(); m_ib.free(); for(auto& t : m_txs) t.free(); }
    auto fBf() noexcept -> v0;
    auto lFl(std::string_view t_fp, std::vector<Mesh>& t_mhs) noexcept -> v0;
public:
    inline auto& gVb() noexcept { return m_vb; }
    inline auto& gIb() noexcept { return m_ib; }
private:
    auto pN(aiNode* t_n, std::vector<Mesh>& t_mhs) noexcept -> v0;
    auto pM(aiMesh* t_m) noexcept -> Mesh;
    auto oM(std::vector<Vertex>& t_vxv, std::vector<arln::u32>& t_ixv, std::vector<Texture>& t_txs) noexcept -> Mesh;
    auto lT(aiMaterial* t_mt, aiTextureType t_ty, std::string_view t_tn) noexcept -> std::vector<Texture>;
    auto lI(std::string_view t_fp) noexcept -> arln::u32;
    auto lIb(void* t_d, arln::u32 t_w, arln::u32 t_h) noexcept -> arln::u32;
public:
    aiScene* m_cs{};
    arln::Buffer m_vb, m_ib;
    std::string dir;
    std::vector<Vertex> m_vts;
    std::vector<arln::u32> m_ixs;
    std::vector<arln::Image> m_txs;
    std::vector<Texture> m_tLds;
};