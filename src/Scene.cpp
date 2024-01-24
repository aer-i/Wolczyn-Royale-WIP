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
    this->lMdl("../../assets/backpack/backpack.obj", false, glm::vec3{0.f, 2.f, 0.f});
    this->lMdl("../../assets/sponza.glb", true, {}, {}, glm::vec3{0.03f});
}

auto Scene::pr() noexcept -> v0
{
    m_smp = arln::CurrentContext()->createSampler(arln::SamplerOptions{
        .magFilter = arln::Filter::eLinear,
        .minFilter = arln::Filter::eLinear
    });
    m_dp = arln::CurrentContext()->createDescriptorPool();
    m_ds = m_dp.addBinding(0, arln::DescriptorType::eStorageBuffer, arln::ShaderStageBits::eVertex)
        .addBinding(1, arln::DescriptorType::eStorageBuffer, arln::ShaderStageBits::eVertex)
        .addBinding(2, arln::DescriptorType::eCombinedImageSampler, arln::ShaderStageBits::eFragment, 64)
        .createDescriptor();

    std::vector<WD> wds;
    for (auto& m_ml : m_mls) {
        for (auto& m : m_ml.mhs) {
            wds.emplace_back(WD{
                .mtx = m_ml.gMtx(),
                .mi = m.txs.back().id
            });
        }
    }
    m_ob.recreate(arln::BufferUsageBits::eStorageBuffer, arln::MemoryType::eGpuOnly, sizeof(WD) * wds.size());
    m_ob.writeData(wds.data(), m_ob.getSize());

    auto vs = arln::Shader("shaders/bg.vert.spv");
    auto fs = arln::Shader("shaders/bg.frag.spv");
    auto pi = arln::GraphicsPipelineInfo{
        .vertShader = vs, .fragShader = fs,
        .depthFormat = arln::CurrentContext()->getDefaultDepthFormat(), .frontFace = arln::FrontFace::eCounterClockwise,
        .cullMode = arln::CullMode::eFront, .depthStencil = true
    }; pi.pushConstants << arln::PushConstantRange(arln::ShaderStageBits::eVertex, sizeof(glm::mat4) * 2, 0);
    pi.descriptors << m_dp.getFirstDescriptor();
    m_gp = arln::CurrentContext()->createGraphicsPipeline(pi);
    fs.destroy();
    vs.destroy();
    m_mshImp.fBf();

    arln::DescriptorWriter wr;
    wr
        .addBuffer(m_ds, m_mshImp.gVb(), 0, arln::DescriptorType::eStorageBuffer)
        .addBuffer(m_ds, m_ob, 1, arln::DescriptorType::eStorageBuffer);

    for (arln::u32 i{}; i < m_mshImp.m_txs.size(); ++i) {
        wr.addImage(m_ds, m_mshImp.m_txs[i], m_smp, 2, arln::DescriptorType::eCombinedImageSampler, i);
    }
    wr.write();
    wr.clear();

    gIdb();
}