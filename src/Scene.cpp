#include "Scene.hpp"

Scene::Scene(arln::Window& t_w, arln::Context& t_c) noexcept : m_ctx{ t_c }, m_cm{ t_w, { 0.f, 0.f, 3.f }, 90.f } {
    pms();
    pmd();
    gidb();
    pr();
}

Scene::~Scene() noexcept {
    m_ob.free();
    m_vb.free();
    m_ib.free();
    m_idb.free();
    m_dp.destroy();
    m_gp.destroy();
}

auto Scene::pmd() noexcept -> v0
{
    for (arln::i32 x = 10; x--; )
    {
        for (arln::i32 y = 10; y--; )
        {
            for (arln::i32 z = 10; z--; )
            {
                switch (x % 4)
                {
                    case 3:
                        this->lMdl("cube", "default", {-x * 3, -y * 3, -z * 3});
                        break;
                    case 2:
                        this->lMdl("zuzanna", "default", {-x * 3, -y * 3, -z * 3});
                        break;
                    case 1:
                        this->lMdl("kitten", "default", {-x * 3, -y * 3, -z * 3});
                        break;
                    default:
                        this->lMdl("ico", "default", {-x * 3, -y * 3, -z * 3});
                        break;
                }
            }
        }
    }
}

auto Scene::u() noexcept -> v0 {
    m_cm.sP(arln::toRadians(70), arln::f32(m_ctx.getCurrentExtent().x) / arln::f32(m_ctx.getCurrentExtent().y));
    m_cm.u();
}

auto Scene::lMdl(std::string_view t_msh, std::string_view t_mtr, arln::vec3 const& t_pos) noexcept -> v0 {
    auto* msh = &m_mhs[t_msh.data()];
    m_mls.emplace_back(msh, glm::translate(arln::mat4{1.f}, t_pos));
}

auto Scene::lMhs(std::string_view t_n, std::string_view t_fp) noexcept -> v0 {
    m_mhs[t_n.data()] = Mesh(t_fp, m_vv, m_iv);
}

auto Scene::pr() noexcept -> v0
{
    m_dp = m_ctx.createDescriptorPool();
    m_ds = m_dp.addBinding(0, arln::DescriptorType::eStorageBuffer, arln::ShaderStageBits::eVertex)
        .addBinding(1, arln::DescriptorType::eStorageBuffer, arln::ShaderStageBits::eVertex)
        .createDescriptor();
    m_ob.recreate(arln::BufferUsageBits::eStorageBuffer, arln::MemoryType::eGpu, sizeof(arln::mat4) * 100000);

    for (size_t i = 0; i < m_mls.size(); ++i) {
        m_ob.writeData(&m_mls[i].mtx, sizeof(arln::mat4), i * sizeof(arln::mat4));
    }

    auto pi = arln::GraphicsPipelineInfo{
        .vertShaderPath = "shaders/bg.vert.spv", .fragShaderPath = "shaders/bg.frag.spv",
        .depthFormat = m_ctx.getDefaultDepthFormat(), .frontFace = arln::FrontFace::eCounterClockwise,
        .cullMode = arln::CullMode::eFront, .depthStencil = true
    }; pi.pushConstants << arln::PushConstantRange(arln::ShaderStageBits::eVertex, sizeof(arln::mat4), 0);
    pi.descriptors << m_dp.getFirstDescriptor();
    m_gp = m_ctx.createGraphicsPipeline(pi);
    m_vb.recreate(arln::BufferUsageBits::eStorageBuffer, arln::MemoryType::eGpuOnly, m_vv.size() * sizeof(m_vv[0]));
    m_ib.recreate(arln::BufferUsageBits::eIndexBuffer, arln::MemoryType::eGpuOnly, m_iv.size() * sizeof(m_iv[0]));
    m_vb.writeData(m_vv.data(), m_vb.getSize());
    m_ib.writeData(m_iv.data(), m_ib.getSize());

    arln::DescriptorWriter()
        .addBuffer(m_ds, m_vb, 0, arln::DescriptorType::eStorageBuffer)
        .addBuffer(m_ds, m_ob, 1, arln::DescriptorType::eStorageBuffer).write();
}

auto Scene::pms() noexcept -> v0
{
    this->lMhs("cube", "../../assets/cube.obj");
    this->lMhs("ico", "../../assets/ico.obj");
    this->lMhs("kitten", "../../assets/kitten.obj");
    this->lMhs("zuzanna", "../../assets/zuzanna.obj");
}

auto Scene::gidb() noexcept -> v0
{
    std::vector<arln::DrawIndexedIndirectCommand> ic;
    ic.reserve(m_mls.size());

    for (arln::u32 i{}; auto& m : m_mls) {
        ic.push_back(arln::DrawIndexedIndirectCommand{
            .indexCount = m.msh->ic,
            .instanceCount = 1,
            .firstIndex = m.msh->ixo,
            .vertexOffset = m.msh->vxo,
            .firstInstance = i++
        });
    }

    m_idb.recreate(arln::BufferUsageBits::eIndirectBuffer, arln::MemoryType::eGpuOnly, ic.size() * sizeof(ic[0]));
    m_idb.writeData(ic.data(), m_idb.getSize());
}
