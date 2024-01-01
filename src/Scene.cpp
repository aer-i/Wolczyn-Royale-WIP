#include "Scene.hpp"

Scene::Scene(arln::Window& t_w, arln::Context& t_c) noexcept : m_ctx{ t_c }, m_cm{ t_w, { 0.f, 0.f, 3.f }, 90.f } {
    m_cm.sP(arln::toRadians(70), arln::f32(t_c.getCurrentExtent().x) / arln::f32(t_c.getCurrentExtent().y));
    m_cm.u();
    m_dp = m_ctx.createDescriptorPool();
    m_ob.recreate(arln::BufferUsageBits::eStorageBuffer, arln::MemoryType::eGpu, sizeof(arln::mat4) * 1000);

    auto pi = arln::GraphicsPipelineInfo{
        .vertShaderPath = "shaders/bg.vert.spv", .fragShaderPath = "shaders/bg.frag.spv",
        .depthFormat = m_ctx.getDefaultDepthFormat(), .frontFace = arln::FrontFace::eClockwise,
        .cullMode = arln::CullMode::eBack, .depthStencil = true
    }; pi.pushConstants << arln::PushConstantRange(arln::ShaderStageBits::eVertex, sizeof(arln::mat4), 0);

    this->lMhs("kitten", "../../assets/kitten.obj");
    this->lMtr("default", std::move(pi));

    this->lMdl("kitten", "default");
    this->lMdl("kitten", "default");
}

Scene::~Scene() noexcept {
    m_ob.free();
    m_dp.destroy();
    for (auto& m : m_mhs) {
        m.second.vb.free();
        m.second.ib.free();
    } for (auto& m : m_mts) {
        m.second.p.destroy();
    }
}

auto Scene::u() noexcept -> v0 {
    m_cm.sP(arln::toRadians(70), arln::f32(m_ctx.getCurrentExtent().x) / arln::f32(m_ctx.getCurrentExtent().y));
    m_cm.u();
    for (size_t i = 0; i < m_mls.size(); ++i) {
        m_ob.writeData(&m_mls[i].mtx, sizeof(arln::mat4), i * sizeof(arln::mat4));
    }
}

auto Scene::lMdl(std::string_view t_msh, std::string_view t_mtr) noexcept -> v0 {
    auto& msh = m_mhs[t_msh.data()];
    auto& mtr = m_mts[t_mtr.data()];

    arln::DescriptorWriter()
        .addBuffer(mtr.d, msh.vb, 0, arln::DescriptorType::eStorageBuffer)
        .addBuffer(mtr.d, m_ob, 1, arln::DescriptorType::eStorageBuffer).write();

    static arln::f32 cp{ };
    m_mls.emplace_back(msh, mtr, glm::translate(arln::mat4{1.f}, {cp++, 0, 0}));
}

auto Scene::lMhs(std::string_view t_n, std::string_view t_fp) noexcept -> v0 {
    m_mhs[t_n.data()] = Mesh(t_fp);
}

auto Scene::lMtr(std::string_view t_n, arln::GraphicsPipelineInfo&& t_pi) noexcept -> v0 {
    auto ds = m_dp.addBinding(0, arln::DescriptorType::eStorageBuffer, arln::ShaderStageBits::eVertex)
        .addBinding(1, arln::DescriptorType::eStorageBuffer, arln::ShaderStageBits::eVertex)
        .createDescriptor();
    t_pi.descriptors << ds;

    m_mts[t_n.data()] = Material(m_ctx.createGraphicsPipeline(t_pi), ds);
}
