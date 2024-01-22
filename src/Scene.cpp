#include "Scene.hpp"
#include "Time.hpp"

struct WD {
    glm::mat4 mtx;
    alignas(16) arln::u32 mi;
};

static inline auto nP(glm::vec4 p) noexcept -> glm::vec4 {
    return p / length(glm::vec3(p));
}

auto Scene::u() noexcept -> v0 {
    cam.sP(glm::radians(70.f), arln::f32(arln::CurrentContext()->getCurrentExtent().x) / arln::f32(arln::CurrentContext()->getCurrentExtent().y), 0.1f, 256.f);
    cam.u();
    m_phx.u();
}

auto Scene::pmd() noexcept -> v0
{
    this->lMdl("../../assets/backpack/backpack.obj", glm::vec3{0.f, 2.f, 0.f});
    this->lMdl("../../assets/sponza.glb", {}, {}, glm::vec3{0.03f});
}

auto Scene::pr() noexcept -> v0
{
    m_dp = arln::CurrentContext()->createDescriptorPool();
    m_ds = m_dp.addBinding(0, arln::DescriptorType::eStorageBuffer, arln::ShaderStageBits::eVertex)
        .addBinding(1, arln::DescriptorType::eStorageBuffer, arln::ShaderStageBits::eVertex)
        .createDescriptor();

    std::vector<WD> wds;
    for (auto & m_ml : m_mls) {
        for (auto& m : m_ml.mhs) {
            wds.emplace_back(WD{
                .mtx = m_ml.gMtx(),
                .mi = m_ml.mtID
            });
        }
    }
    m_ob.recreate(arln::BufferUsageBits::eStorageBuffer, arln::MemoryType::eGpuOnly, sizeof(WD) * wds.size());
    m_ob.writeData(wds.data(), m_ob.getSize());

    auto pi = arln::GraphicsPipelineInfo{
        .vertShaderPath = "shaders/bg.vert.spv", .fragShaderPath = "shaders/bg.frag.spv",
        .depthFormat = arln::CurrentContext()->getDefaultDepthFormat(), .frontFace = arln::FrontFace::eCounterClockwise,
        .cullMode = arln::CullMode::eFront, .depthStencil = true
    }; pi.pushConstants << arln::PushConstantRange(arln::ShaderStageBits::eVertex, sizeof(glm::mat4) * 2, 0);
    pi.descriptors << m_dp.getFirstDescriptor();
    m_gp = arln::CurrentContext()->createGraphicsPipeline(pi);
    m_mshImp.fBf();

    arln::DescriptorWriter()
        .addBuffer(m_ds, m_mshImp.gVb(), 0, arln::DescriptorType::eStorageBuffer)
        .addBuffer(m_ds, m_ob, 1, arln::DescriptorType::eStorageBuffer)
        .write();

    gIdb();
}