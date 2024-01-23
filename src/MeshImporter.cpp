#include "MeshImporter.hpp"

auto MeshImporter::fBf() noexcept -> void
{
    m_vb.recreate(arln::BufferUsageBits::eStorageBuffer, arln::MemoryType::eGpuOnly, m_vts.size() * sizeof(m_vts[0]));
    m_ib.recreate(arln::BufferUsageBits::eIndexBuffer, arln::MemoryType::eGpuOnly, m_ixs.size() * sizeof(m_ixs[0]));
    m_vb.writeData(m_vts.data(), m_vb.getSize());
    m_ib.writeData(m_ixs.data(), m_ib.getSize());
}

auto MeshImporter::lFl(std::string_view t_fp, std::vector<Mesh>& t_mhs) noexcept -> void
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(t_fp.data(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    while (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::printf("%s\n", importer.GetErrorString());
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    this->pN(scene->mRootNode, scene, t_mhs);
}

auto MeshImporter::pN(aiNode* t_n, aiScene const* t_s, std::vector<Mesh>& t_mhs) noexcept -> v0
{
    for (unsigned int i = 0; i < t_n->mNumMeshes; i++)
    {
        aiMesh* mesh = t_s->mMeshes[t_n->mMeshes[i]];
        t_mhs.emplace_back(this->pM(mesh, t_s));
    }

    for (unsigned int i = 0; i < t_n->mNumChildren; i++)
    {
        this->pN(t_n->mChildren[i], t_s, t_mhs);
    }
}

auto MeshImporter::pM(aiMesh* t_m, const aiScene* t_s) noexcept -> Mesh
{
    std::vector<Vertex> vxv;
    std::vector<arln::u32> ixv;
    vxv.reserve(t_m->mNumVertices);

    for (unsigned int i = 0; i < t_m->mNumVertices; ++i)
    {
        vxv.emplace_back(Vertex{
            .vx = t_m->mVertices[i].x, .vy = t_m->mVertices[i].y, .vz = t_m->mVertices[i].z,
            .nx = arln::u8(t_m->mNormals[i].x * 127.f + 127.5f),
            .ny = arln::u8(t_m->mNormals[i].y * 127.f + 127.5f),
            .nz = arln::u8(t_m->mNormals[i].z * 127.f + 127.5f),
            .u = t_m->mTextureCoords[0] ? t_m->mTextureCoords[0][i].x : 0.f,
            .v = t_m->mTextureCoords[0] ? t_m->mTextureCoords[0][i].y : 0.f
        });
    }


    for (unsigned int i = 0; i < t_m->mNumFaces; ++i)
    {
        aiFace face = t_m->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            ixv.emplace_back(face.mIndices[j]);
    }

    return oM(vxv, ixv);
}

auto MeshImporter::oM(std::vector<Vertex>& t_vxv, std::vector<arln::u32>& t_ixv) noexcept -> Mesh
{
    auto vs = t_vxv.size();
    auto is = t_ixv.size();

    std::vector<arln::u32> rem(is);
    auto ovc = meshopt_generateVertexRemap(rem.data(), t_ixv.data(), is, t_vxv.data(), vs, sizeof(Vertex));

    std::vector<Vertex> vx(ovc);
    std::vector<arln::u32> ix(is);

    meshopt_remapIndexBuffer(ix.data(), t_ixv.data(), is, rem.data());
    meshopt_remapVertexBuffer(vx.data(), t_vxv.data(), vs, sizeof(Vertex), rem.data());

    meshopt_optimizeVertexCache(ix.data(), ix.data(), is, ovc);
    meshopt_optimizeOverdraw(ix.data(), ix.data(), is, &vx[0].vx, ovc, sizeof(Vertex), 1.05f);
    meshopt_optimizeVertexFetch(vx.data(), ix.data(), is, vx.data(), ovc, sizeof(Vertex));

    constexpr arln::f32 thr = 0.5f;
    constexpr arln::f32 ter = 0.2f;
    auto tix = static_cast<size_t>((arln::f32)is * thr);
    std::vector<arln::u32> six(ix.size());
    auto oic = meshopt_simplify(six.data(), ix.data(), is, &vx[0].vx, ovc, sizeof(Vertex), tix, ter);
    six.resize(oic);

    auto ic = static_cast<arln::u32>(six.size());
    auto vxo = static_cast<arln::i32>(m_vts.size());
    auto ixo = static_cast<arln::u32>(m_ixs.size());
    m_vts.insert(m_vts.end(), vx.begin(), vx.end());
    m_ixs.insert(m_ixs.end(), six.begin(), six.end());

    glm::vec3 ctr{};
    arln::f32 rad{};

    for (auto& v : vx) {
        ctr += glm::vec3(v.vx, v.vy, v.vz);
    }
    ctr /= arln::f32(ic);

    for (auto& v : vx){
        rad = std::max(rad, glm::distance(ctr, glm::vec3(v.vx, v.vy, v.vz)));
    }

    return Mesh {
        .ic = ic,
        .vxo = vxo,
        .ixo = ixo,
        .ctr = ctr,
        .rad = rad
    };
}
