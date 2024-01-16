#include "SceneLoader.hpp"

SceneLoader::~SceneLoader() noexcept {
    m_ob.free();
    m_mb.free();
    m_vb.free();
    m_ib.free();
    m_idb.free();
    m_tx1.free();
    m_tx2.free();
    m_dp.destroy();
    m_gp.destroy();
    m_sm.destroy();
}

auto SceneLoader::lMdl(std::string_view t_msh, std::string_view t_mtr, arln::vec3 const& t_pos,
                 arln::vec3 const& t_rot, arln::vec3 const& t_scl) noexcept -> v0 {
    auto* msh = &m_mhs[t_msh.data()];
    m_mls.emplace_back(msh, m_mts[t_mtr.data()], t_pos, t_rot, t_scl);
}

auto SceneLoader::lMhs(std::string_view t_n, std::string_view t_fp) noexcept -> v0 {
    m_mhs[t_n.data()] = Mesh(t_fp, m_vv, m_iv);
}

auto SceneLoader::lMtr(std::string_view t_n, arln::f32 t_shn) noexcept -> v0 {
    m_mts[t_n.data()] = static_cast<arln::u32>(m_mtHls.size());
    m_mtHls.emplace_back(Material{
        .dt = 0,
        .st = 1,
        .shin = t_shn
    });
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