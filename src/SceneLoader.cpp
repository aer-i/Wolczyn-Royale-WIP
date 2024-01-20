#include "SceneLoader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

SceneLoader::~SceneLoader() noexcept {
    for (auto& t : m_txHls) t.free();
    m_ob.free();
    m_mb.free();
    m_vb.free();
    m_ib.free();
    m_idb.free();
    m_dp.destroy();
    m_gp.destroy();
    m_sm.destroy();
}

auto SceneLoader::lMdl(std::string_view t_msh, std::string_view t_mtr, arln::vec3 const& t_pos,
                 arln::vec3 const& t_rot, arln::vec3 const& t_scl) noexcept -> v0 {
    auto* msh = &m_mhs[t_msh.data()];
    m_mls.emplace_back(msh, 0, m_mts[t_mtr.data()], t_pos, t_rot, t_scl, m_phx.cRB(rp3d::Transform{{t_pos.x, t_pos.y, t_pos.z}, rp3d::Quaternion::identity()}));
    if (t_pos != glm::vec3{})
    {
        m_mls.back().col = m_mls.back().rb->addCollider(Physics::cCcs(2, 3), m_mls.back().rb->getTransform());
        m_mls.back().rb->setAngularLockAxisFactor({0, 1, 0});
    }
    else
        m_mls.back().col = m_mls.back().rb->addCollider(Physics::cBcs({10, 0.1f, 10}), m_mls.back().rb->getTransform());
}

auto SceneLoader::lMhs(std::string_view t_n, std::string_view t_fp) noexcept -> v0 {
    m_mhs[t_n.data()] = Mesh(t_fp, m_vv, m_iv);
}

auto SceneLoader::lMtr(std::string_view t_n, std::string_view t_tx, arln::f32 t_shn) noexcept -> v0 {
    m_mts[t_n.data()] = static_cast<arln::u32>(m_mtHls.size());
    m_mtHls.emplace_back(Material{
        .dt = m_txs[t_tx.data()],
        .st = m_txs[t_tx.data()],
        .shin = t_shn
    });
}

auto SceneLoader::lTxt(std::string_view t_n, std::string_view t_fp) noexcept -> v0
{
    m_txs[t_n.data()] = static_cast<arln::u32>(m_txHls.size());
    m_txHls.emplace_back();
    auto& tx{ m_txHls.back() };

    int w, h, c;
    arln::u8* d = stbi_load(t_fp.data(), &w, &h, &c, STBI_rgb_alpha);
    tx.recreate(w, h, arln::Format::eR8G8B8A8Unorm, arln::ImageUsageBits::eSampled, arln::MemoryType::eGpuOnly);
    tx.transition(arln::ImageLayout::eUndefined, arln::ImageLayout::eTransferDst,
                     arln::PipelineStageBits::eTopOfPipe, arln::PipelineStageBits::eTransfer,
                     0, arln::AccessBits::eTransferWrite);
    tx.writeToImage(d, w * h * 4, {w, h});
    tx.transition(arln::ImageLayout::eTransferDst, arln::ImageLayout::eShaderReadOnly,
                     arln::PipelineStageBits::eTransfer, arln::PipelineStageBits::eFragmentShader,
                     arln::AccessBits::eTransferWrite, arln::AccessBits::eShaderRead);
    stbi_image_free(d);
}

auto SceneLoader::gIdb() noexcept -> v0 {
    std::vector<arln::DrawIndexedIndirectCommand> ic;
    ic.reserve(m_mls.size());

    for (m_dc = 0; m_dc < m_mls.size(); ++m_dc) {
        ic.emplace_back(arln::DrawIndexedIndirectCommand{
            .indexCount = m_mls[m_dc].msh->ic,
            .instanceCount = 1,
            .firstIndex = m_mls[m_dc].msh->ixo,
            .vertexOffset = m_mls[m_dc].msh->vxo,
            .firstInstance = m_dc
        });
    }

    m_idb.recreate(arln::BufferUsageBits::eIndirectBuffer, arln::MemoryType::eGpuOnly, ic.size() * sizeof(ic[0]));
    m_idb.writeData(ic.data(), m_idb.getSize());
}