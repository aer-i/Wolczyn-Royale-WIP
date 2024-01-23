#include "MeshImporter.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <cassert>

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
    m_cs = (aiScene*)importer.ReadFile(t_fp.data(), aiProcess_Triangulate | aiProcess_GenNormals);

    while (!m_cs || m_cs->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_cs->mRootNode) {
        std::printf("%s\n", importer.GetErrorString());
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    dir = t_fp.substr(0, t_fp.find_last_of('/'));
    this->pN(m_cs->mRootNode, t_mhs);
}

auto MeshImporter::pN(aiNode* t_n, std::vector<Mesh>& t_mhs) noexcept -> v0
{
    for (unsigned int i = 0; i < t_n->mNumMeshes; i++)
    {
        aiMesh* mesh = m_cs->mMeshes[t_n->mMeshes[i]];
        t_mhs.emplace_back(this->pM(mesh));
    }

    for (unsigned int i = 0; i < t_n->mNumChildren; i++)
    {
        this->pN(t_n->mChildren[i], t_mhs);
    }
}

auto MeshImporter::pM(aiMesh* t_m) noexcept -> Mesh
{
    std::vector<Vertex> vxv;
    std::vector<arln::u32> ixv;
    vxv.reserve(t_m->mNumVertices);

    for (arln::u32 i = 0; i < t_m->mNumVertices; ++i) {
        vxv.emplace_back(Vertex{
            .vx = t_m->mVertices[i].x, .vy = t_m->mVertices[i].y, .vz = t_m->mVertices[i].z,
            .nx = static_cast<arln::u8>(t_m->mNormals[i].x * 127.f + 127.5f),
            .ny = static_cast<arln::u8>(t_m->mNormals[i].y * 127.f + 127.5f),
            .nz = static_cast<arln::u8>(t_m->mNormals[i].z * 127.f + 127.5f),
            .u = t_m->mTextureCoords[0] ? t_m->mTextureCoords[0][i].x : 0.f,
            .v = t_m->mTextureCoords[0] ? t_m->mTextureCoords[0][i].y : 0.f
        });
    }

    for (arln::u32 i = 0; i < t_m->mNumFaces; ++i)
    {
        aiFace face = t_m->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            ixv.emplace_back(face.mIndices[j]);
    }

    std::vector<Texture> difM;

    if (t_m->mMaterialIndex >= 0)
    {
        auto mt = m_cs->mMaterials[t_m->mMaterialIndex];
        difM = lT(mt, aiTextureType_DIFFUSE, "texture_diffuse");
    }

    return oM(vxv, ixv, difM);
}

auto MeshImporter::oM(std::vector<Vertex>& t_vxv, std::vector<arln::u32>& t_ixv, std::vector<Texture>& t_txs) noexcept -> Mesh
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

    if constexpr (false)
    {
        constexpr arln::f32 thr = 0.5f;
        constexpr arln::f32 ter = 0.2f;
        auto tix = static_cast<size_t>((arln::f32) is * thr);
        std::vector<arln::u32> six(ix.size());
        auto oic = meshopt_simplify(six.data(), ix.data(), is, &vx[0].vx, ovc, sizeof(Vertex), tix, ter);
        six.resize(oic);
    }

    auto ic = static_cast<arln::u32>(ix.size());
    auto vxo = static_cast<arln::i32>(m_vts.size());
    auto ixo = static_cast<arln::u32>(m_ixs.size());
    m_vts.insert(m_vts.end(), vx.begin(), vx.end());
    m_ixs.insert(m_ixs.end(), ix.begin(), ix.end());

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
        .rad = rad,
        .txs = t_txs
    };
}

auto MeshImporter::lT(aiMaterial* t_mt, aiTextureType t_ty, std::string_view t_tn) noexcept -> std::vector<Texture>
{
    std::vector<Texture> txs;

    for (unsigned int i = 0; i < t_mt->GetTextureCount(t_ty); i++)
    {
        aiString str;
        t_mt->GetTexture(t_ty, i, &str);

        bool s = false;
        for (auto& m_tLd : m_tLds) {
            if (std::strcmp(m_tLd.pt.data(), str.C_Str()) == 0) {
                txs.emplace_back(m_tLd);
                s = true;
                break;
            }
        }
        if (!s) {
            auto tx = m_cs->GetEmbeddedTexture(str.C_Str());
            Texture texture;
            if (tx) texture.id = this->lIb(tx->pcData, tx->mWidth, tx->mHeight);
            else    texture.id = this->lI(str.C_Str());
            texture.ty = t_tn;
            texture.pt = str.C_Str();
            txs.emplace_back(texture);
            m_tLds.emplace_back(texture);
        }
    }
    return txs;
}

auto MeshImporter::lI(std::string_view t_fp) noexcept -> arln::u32
{
    auto id = static_cast<arln::u32>(m_txs.size());

    arln::i32 w, h, c;
    auto fp = (dir + '/' + t_fp.data());
    arln::u8* b = stbi_load(fp.c_str(), &w, &h, &c, STBI_rgb_alpha);

    assert(b);

    auto& tx = m_txs.emplace_back(
        arln::CurrentContext()->allocateImage((arln::u32)w, (arln::u32)h, arln::Format::eR8G8B8A8Unorm, arln::ImageUsageBits::eSampled, arln::MemoryType::eGpuOnly)
    );

    using namespace arln;
    tx.transition(ImageLayout::eUndefined, ImageLayout::eTransferDst, PipelineStageBits::eTopOfPipe, PipelineStageBits::eTransfer, 0, AccessBits::eTransferWrite);
    tx.writeToImage(b, w * h * 4, {w, h});
    tx.transition(ImageLayout::eTransferDst, ImageLayout::eShaderReadOnly, PipelineStageBits::eTransfer,PipelineStageBits::eFragmentShader, AccessBits::eTransferWrite, AccessBits::eShaderRead);

    stbi_image_free(b);

    return id;
}

auto MeshImporter::lIb(void* t_d, arln::u32 t_w, arln::u32 t_h) noexcept -> arln::u32
{
    auto id = static_cast<arln::u32>(m_txs.size());

    assert(t_d);

    if (!t_h) {
        arln::i32 w, h, c;
        auto d = stbi_load_from_memory((stbi_uc const*)t_d, (arln::i32)t_w, &w, &h, &c, STBI_rgb_alpha);

        assert(d);

        auto& tx = m_txs.emplace_back(
            arln::CurrentContext()->allocateImage((arln::u32)w, (arln::u32)h, arln::Format::eR8G8B8A8Unorm, arln::ImageUsageBits::eSampled, arln::MemoryType::eGpuOnly)
        );

        using namespace arln;
        tx.transition(ImageLayout::eUndefined, ImageLayout::eTransferDst, PipelineStageBits::eTopOfPipe, PipelineStageBits::eTransfer, 0, AccessBits::eTransferWrite);
        tx.writeToImage(d, w * h * 4, {w, h});
        tx.transition(ImageLayout::eTransferDst, ImageLayout::eShaderReadOnly, PipelineStageBits::eTransfer,PipelineStageBits::eFragmentShader, AccessBits::eTransferWrite, AccessBits::eShaderRead);

        stbi_image_free(d);

    } else {
        auto& tx = m_txs.emplace_back(
            arln::CurrentContext()->allocateImage((arln::u32)t_w, (arln::u32)t_h, arln::Format::eR8G8B8A8Unorm, arln::ImageUsageBits::eSampled, arln::MemoryType::eGpuOnly)
        );

        using namespace arln;
        tx.transition(ImageLayout::eUndefined, ImageLayout::eTransferDst, PipelineStageBits::eTopOfPipe, PipelineStageBits::eTransfer, 0, AccessBits::eTransferWrite);
        tx.writeToImage(t_d, t_w * t_h * 4, {t_w, t_h});
        tx.transition(ImageLayout::eTransferDst, ImageLayout::eShaderReadOnly, PipelineStageBits::eTransfer,PipelineStageBits::eFragmentShader, AccessBits::eTransferWrite, AccessBits::eShaderRead);
    }

    return id;
}
