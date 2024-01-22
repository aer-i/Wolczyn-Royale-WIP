#include "MeshImporter.hpp"

auto MeshImporter::lFl(std::string_view t_fp) noexcept -> void
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(t_fp.data(), aiProcess_Triangulate | aiProcess_FlipUVs);

    while (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::printf("Failed to load model: %s\n%s", t_fp.data(), importer.GetErrorString());
        std::this_thread::sleep_for(std::chrono::microseconds(1000));
    }

    this->pN(scene->mRootNode, scene);

    //
//    std::vector<Vertex> lvs;
//    std::vector<arln::u32> lis;
//
//    aiMesh* mesh = scene->mMeshes[scene->mRootNode->mMeshes[0]];
//    for (arln::u32 i = 0; i < mesh->mNumVertices; ++i) {
//        lvs.emplace_back(Vertex{
//            .vx = mesh->mVertices[i].x, .vy = mesh->mVertices[i].y, .vz = mesh->mVertices[i].z,
//            .nx = arln::u8(mesh->mNormals[i].x * 127.f + 127.5f),
//            .ny = arln::u8(mesh->mNormals[i].y * 127.f + 127.5f),
//            .nz = arln::u8(mesh->mNormals[i].z * 127.f + 127.5f),
//            .u = mesh->mTextureCoords[0][i].x,
//            .v = mesh->mTextureCoords[0][i].y
//        });
//    }
//
//    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
//    {
//        aiFace face = mesh->mFaces[i];
//        for(unsigned int j = 0; j < face.mNumIndices; ++j)
//            lis.emplace_back(face.mIndices[j]);
//    }
//
//    importer.FreeScene();
//
//    auto ixc = lis.size();
//
//    std::vector<arln::u32> rmp(ixc);
//    size_t vc = meshopt_generateVertexRemap(rmp.data(), nullptr, ixc, lvs.data(), ixc, sizeof(Vertex));
//
//    std::vector<Vertex> vi(vc);
//    std::vector<arln::u32> ii(ixc);
//
//    meshopt_remapVertexBuffer(vi.data(), lvs.data(), ixc, sizeof(Vertex), rmp.data());
//    meshopt_remapIndexBuffer(ii.data(), nullptr, ixc, rmp.data());
//
//    meshopt_optimizeVertexCache(ii.data(), ii.data(), ixc, vc);
//    meshopt_optimizeVertexFetch(vi.data(), ii.data(), ixc, vi.data(), vc, sizeof(Vertex));

//    ic = static_cast<arln::u32>(ii.size());
//    vxo = static_cast<arln::i32>(t_vb.size());
//    ixo = static_cast<arln::i32>(t_ib.size());
//    t_vb.insert(t_vb.end(), vi.begin(), vi.end());
//    t_ib.insert(t_ib.end(), ii.begin(), ii.end());
//
//    for (auto& v : vi){
//        ctr += glm::vec3(v.vx, v.vy, v.vz);
//    }
//    ctr /= arln::f32(vi.size());
//
//    for (auto& v : vi){
//        rad = std::max(rad, glm::distance(ctr, glm::vec3(v.vx, v.vy, v.vz)));
//    }
}

auto MeshImporter::pN(aiNode* t_n, aiScene const* t_s) noexcept -> v0
{
    for (unsigned int i = 0; i < t_n->mNumMeshes; i++)
    {
        aiMesh* mesh = t_s->mMeshes[t_n->mMeshes[i]];
        m_mhs.emplace_back(this->pM(mesh, t_s));
    }

    for (unsigned int i = 0; i < t_n->mNumChildren; i++)
    {
        this->pN(t_n->mChildren[i], t_s);
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
            .u = t_m->mTextureCoords[0][i].x,
            .v = t_m->mTextureCoords[0][i].y
        });
    }


    for (unsigned int i = 0; i < t_m->mNumFaces; ++i)
    {
        aiFace face = t_m->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            ixv.emplace_back(face.mIndices[j]);
    }

    auto ic = static_cast<arln::u32>(vxv.size());
    auto vxo = static_cast<arln::i32>(m_vts.size());
    auto ixo = static_cast<arln::u32>(m_ixs.size());
    m_vts.insert(m_vts.end(), vxv.begin(), vxv.end());
    m_ixs.insert(m_ixs.end(), ixv.begin(), ixv.end());

    glm::vec3 ctr{};
    arln::f32 rad{};

    for (auto& v : vxv){
        ctr += glm::vec3(v.vx, v.vy, v.vz);
    }
    ctr /= arln::f32(ic);

    for (auto& v : vxv){
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
