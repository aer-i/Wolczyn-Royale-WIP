#include "SceneLoader.hpp"

SceneLoader::~SceneLoader() noexcept {
    m_ob.free();
    m_idb.free();
    m_dp.destroy();
    m_gp.destroy();
    m_smp.destroy();
}

auto SceneLoader::lMdl(std::string_view t_fp, glm::vec3 const& t_pos, glm::vec3 const& t_rot, glm::vec3 const& t_scl) noexcept -> v0 {
    auto& m = m_mls.emplace_back();
    m_mshImp.lFl(t_fp, m.mhs);
    m.pos = t_pos;
    m.rot = t_rot;
    m.scl = t_scl;
}

auto SceneLoader::gIdb() noexcept -> v0 {
    std::vector<arln::DrawIndexedIndirectCommand> ic;
    ic.reserve(m_mls.size());
    m_dc = 0;

    for (auto& m_ml : m_mls)  {
        for (auto& m : m_ml.mhs)
        {
            ic.emplace_back(arln::DrawIndexedIndirectCommand{
                .indexCount = m.ic,
                .instanceCount = 1,
                .firstIndex = m.ixo,
                .vertexOffset = m.vxo,
                .firstInstance = m_dc++
            });
        }
    }

    m_idb.recreate(arln::BufferUsageBits::eIndirectBuffer, arln::MemoryType::eGpuOnly, ic.size() * sizeof(ic[0]));
    m_idb.writeData(ic.data(), m_idb.getSize());
}