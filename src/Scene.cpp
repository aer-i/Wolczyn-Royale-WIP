#include "Scene.hpp"

struct WD {
    arln::mat4 mtx;
    alignas(16) arln::u32 mi;
};

static inline auto nP(arln::vec4 p) noexcept -> arln::vec4 {
    return p / length(arln::vec3(p));
}

auto Scene::u() noexcept -> v0 {
    cam.sP(arln::toRadians(70), arln::f32(arln::CurrentContext()->getCurrentExtent().x) / arln::f32(arln::CurrentContext()->getCurrentExtent().y), 0.1f, 1024.f);
    cam.u();
    m_phx.u();

    std::vector<WD> wds(m_mls.size());

    m_mls.back().rot.y = glm::radians(-cam.gYa() - 90.f);

    for (size_t i = 0; i < m_mls.size(); ++i) {
        arln::f64 x[16];
        m_mls[i].rb->getTransform().getOpenGLMatrix(x);
        wds[i].mtx = glm::mat4(x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8], x[9], x[10], x[11], x[12], x[13], x[14], x[15]);
        wds[i].mi = m_mls[i].mtID;
    }

    m_ob.writeData(wds.data(), m_ob.getSize());
}

auto Scene::pmd() noexcept -> v0
{
//    for (arln::i32 x = 15; x--; )
//        for (arln::i32 y = 15; y--; )
//            for (arln::i32 z = 15; z--; ) {
//                switch (x % 4) {
//                case 3:
//                    this->lMdl("ico", "gold", {-x * 3, -y * 3, -z * 3});
//                    break;
//                case 2:
//                    this->lMdl("zuzanna", "turquoise", {-x * 3, -y * 3, -z * 3});
//                    break;
//                case 1:
//                    this->lMdl("ico", "red rubber", {-x * 3, -y * 3, -z * 3});
//                    break;
//                default:
//                    this->lMdl("cube", "cyan rubber", {-x * 3, -y * 3, -z * 3});
//                    break;
//                }
//            }

    this->lMdl("plane", "red rubber");
    m_mls.back().rb->setType(rp3d::BodyType::STATIC);
    this->lMdl("cyborg", "gold", {0, 5, -3});
}

auto Scene::pms() noexcept -> v0
{
    this->lTxt("grass", "../../assets/grass.jpg");
    this->lTxt("container", "../../assets/container.png");

    this->lMtr("gold", "container", 0.4f * 128.f);
    this->lMtr("cyan rubber", "grass" , 0.1 * 128.f);
    this->lMtr("red rubber", "grass" , 0.078125f * 128.f);
    this->lMtr("turquoise" , "grass", 0.1 * 128.f);

    this->lMhs("cube", "../../assets/cube.obj");
    this->lMhs("ico", "../../assets/ico.obj");
    this->lMhs("kitten", "../../assets/kitten.obj");
    this->lMhs("zuzanna", "../../assets/zuzanna.obj");
    this->lMhs("plane", "../../assets/plane.obj");
    this->lMhs("cyborg", "../../assets/cyborg.obj");
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
    }; pi.pushConstants << arln::PushConstantRange(arln::ShaderStageBits::eVertex, sizeof(arln::mat4) * 2, 0);
    pi.descriptors << m_dp.getFirstDescriptor();
    m_gp = arln::CurrentContext()->createGraphicsPipeline(pi);
    m_vb.recreate(arln::BufferUsageBits::eStorageBuffer, arln::MemoryType::eGpuOnly, m_vv.size() * sizeof(m_vv[0]));
    m_ib.recreate(arln::BufferUsageBits::eIndexBuffer, arln::MemoryType::eGpuOnly, m_iv.size() * sizeof(m_iv[0]));
    m_vb.writeData(m_vv.data(), m_vb.getSize());
    m_ib.writeData(m_iv.data(), m_ib.getSize());

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