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
    cam.sP(glm::radians(70.f), arln::f32(arln::CurrentContext()->getCurrentExtent().x) / arln::f32(arln::CurrentContext()->getCurrentExtent().y), 0.1f, 1024.f);
    cam.u();
    m_phx.u();

    std::vector<WD> wds(m_mls.size());

    m_mls.back().rot.y = static_cast<arln::f32>(Time::gT());
    m_mls.back().pos.y = cosf(static_cast<arln::f32>(Time::gT()) * 2.f) * 2.f + 2.f;

    for (size_t i = 0; i < m_mls.size(); ++i) {
        wds[i].mtx = m_mls[i].gMtx();
        wds[i].mi = m_mls[i].mtID;
    }

    m_ob.writeData(wds.data(), m_ob.getSize());
}

auto Scene::pmd() noexcept -> v0
{
    this->lMdl(0, "red rubber");
    this->lMdl(1, "gold", {0, 5, -3});
}

auto Scene::pms() noexcept -> v0
{
    this->lTxt("grass", "../../assets/grass.jpg");
    this->lTxt("container", "../../assets/container.png");

    this->lMtr("gold", "container", 0.4f * 128.f);
    this->lMtr("cyan rubber", "grass" , 0.1 * 128.f);
    this->lMtr("red rubber", "grass" , 0.078125f * 128.f);
    this->lMtr("turquoise" , "grass", 0.1 * 128.f);

//    this->lMhs("cube", "../../assets/cube.obj");
//    this->lMhs("ico", "../../assets/ico.obj");
//    this->lMhs("kitten", "../../assets/kitten.obj");
//    this->lMhs("zuzanna", "../../assets/zuzanna.obj");
    this->lMhs("../../assets/plane.obj");
    this->lMhs("../../assets/cyborg.obj");
}

auto Scene::pr() noexcept -> v0
{
    m_dp = arln::CurrentContext()->createDescriptorPool();
    m_ds = m_dp.addBinding(0, arln::DescriptorType::eStorageBuffer, arln::ShaderStageBits::eVertex)
        .addBinding(1, arln::DescriptorType::eStorageBuffer, arln::ShaderStageBits::eVertex)
        .addBinding(2, arln::DescriptorType::eStorageBuffer, arln::ShaderStageBits::eFragment)
        .addBinding(3, arln::DescriptorType::eCombinedImageSampler, arln::ShaderStageBits::eFragment, 64)
        .createDescriptor();
    m_ob.recreate(arln::BufferUsageBits::eStorageBuffer, arln::MemoryType::eCpuToGpu, sizeof(WD) * m_mls.size());
    m_mb.recreate(arln::BufferUsageBits::eStorageBuffer, arln::MemoryType::eGpuOnly, sizeof(Material) * m_mtHls.size());
    m_sm = arln::CurrentContext()->createSampler(arln::SamplerOptions{
        .magFilter = arln::Filter::eLinear,
        .minFilter = arln::Filter::eLinear,
        .addressModeU = arln::SamplerAddressMode::eRepeat,
        .addressModeV = arln::SamplerAddressMode::eRepeat,
        .addressModeW = arln::SamplerAddressMode::eRepeat,
        .minLod = 0.f,
        .maxLod = 16.f
    });

    m_mb.writeData(m_mtHls.data(), m_mb.getSize());

    std::vector<WD> wds(m_mls.size());

    for (size_t i = 0; i < m_mls.size(); ++i) {
        wds[i].mtx = m_mls[i].gMtx(),
        wds[i].mi = m_mls[i].mtID;
    }

    m_ob.writeData(wds.data(), m_ob.getSize());

    auto pi = arln::GraphicsPipelineInfo{
        .vertShaderPath = "shaders/bg.vert.spv", .fragShaderPath = "shaders/bg.frag.spv",
        .depthFormat = arln::CurrentContext()->getDefaultDepthFormat(), .frontFace = arln::FrontFace::eCounterClockwise,
        .cullMode = arln::CullMode::eFront, .depthStencil = true
    }; pi.pushConstants << arln::PushConstantRange(arln::ShaderStageBits::eVertex, sizeof(glm::mat4) * 2, 0);
    pi.descriptors << m_dp.getFirstDescriptor();
    m_gp = arln::CurrentContext()->createGraphicsPipeline(pi);
    m_vb.recreate(arln::BufferUsageBits::eStorageBuffer, arln::MemoryType::eGpuOnly, m_mshImp.m_vts.size() * sizeof(m_mshImp.m_vts[0]));
    m_ib.recreate(arln::BufferUsageBits::eIndexBuffer, arln::MemoryType::eGpuOnly, m_mshImp.m_ixs.size() * sizeof(m_mshImp.m_ixs[0]));
    m_vb.writeData(m_mshImp.m_vts.data(), m_vb.getSize());
    m_ib.writeData(m_mshImp.m_ixs.data(), m_ib.getSize());

    auto dw = arln::DescriptorWriter();
    dw.addBuffer(m_ds, m_vb, 0, arln::DescriptorType::eStorageBuffer)
        .addBuffer(m_ds, m_ob, 1, arln::DescriptorType::eStorageBuffer)
        .addBuffer(m_ds, m_mb, 2, arln::DescriptorType::eStorageBuffer);

    for (arln::u32 i = 0; i < m_txHls.size(); ++i) {
        dw.addImage(m_ds, m_txHls[i], m_sm, 3, arln::DescriptorType::eCombinedImageSampler, i);
    }
    dw.write();
    dw.clear();

    gIdb();
}