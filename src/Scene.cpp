#include "Scene.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static inline auto nP(arln::vec4 p) noexcept -> arln::vec4 {
    return p / length(arln::vec3(p));
}

auto Scene::u() noexcept -> v0 {
    cam.sP(arln::toRadians(70), arln::f32(arln::CurrentContext()->getCurrentExtent().x) / arln::f32(arln::CurrentContext()->getCurrentExtent().y), 0.1f, 1024.f);
    cam.u();
}

auto Scene::pmd() noexcept -> v0
{
    for (arln::i32 x = 15; x--; )
        for (arln::i32 y = 15; y--; )
            for (arln::i32 z = 15; z--; ) {
                switch (x % 4) {
//                case 3:
//                    this->lMdl("cube", "gold", {-x * 3, -y * 3, -z * 3});
//                    break;
//                case 2:
//                    this->lMdl("zuzanna", "turquoise", {-x * 3, -y * 3, -z * 3});
//                    break;
//                case 1:
//                    this->lMdl("cube", "red rubber", {-x * 3, -y * 3, -z * 3});
//                    break;
                default:
                    this->lMdl("cube", "cyan rubber", {-x * 3, -y * 3, -z * 3});
                    break;
                }
            }
}

auto Scene::pms() noexcept -> v0
{
    this->lMtr("gold", 0.4f * 128.f);
    this->lMtr("cyan rubber", 0.1 * 128.f);
    this->lMtr("red rubber", 0.078125f * 128.f);
    this->lMtr("turquoise", 0.1 * 128.f);

    this->lMhs("cube", "../../assets/cube.obj");
    this->lMhs("ico", "../../assets/ico.obj");
    this->lMhs("kitten", "../../assets/kitten.obj");
    this->lMhs("zuzanna", "../../assets/zuzanna.obj");
}

auto Scene::pr() noexcept -> v0
{
    struct WD {
        arln::mat4 mtx;
        alignas(16) arln::u32 mi;
    };

    m_dp = arln::CurrentContext()->createDescriptorPool();
    m_ds = m_dp.addBinding(0, arln::DescriptorType::eStorageBuffer, arln::ShaderStageBits::eVertex)
        .addBinding(1, arln::DescriptorType::eStorageBuffer, arln::ShaderStageBits::eVertex)
        .addBinding(2, arln::DescriptorType::eStorageBuffer, arln::ShaderStageBits::eFragment)
        .addBinding(3, arln::DescriptorType::eCombinedImageSampler, arln::ShaderStageBits::eFragment, 64)
        .createDescriptor();
    m_ob.recreate(arln::BufferUsageBits::eStorageBuffer, arln::MemoryType::eGpuOnly, sizeof(WD) * m_mls.size());
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
        wds[i].mtx = m_mls[i].gMtx();
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

    int w, h, c;
    arln::u8* d = stbi_load("../../assets/container.png", &w, &h, &c, STBI_rgb_alpha);
    m_tx1.recreate(w, h, arln::Format::eR8G8B8A8Unorm, arln::ImageUsageBits::eSampled, arln::MemoryType::eGpuOnly);
    m_tx1.transition(arln::ImageLayout::eUndefined, arln::ImageLayout::eTransferDst,
                    arln::PipelineStageBits::eTopOfPipe, arln::PipelineStageBits::eTransfer,
                    0, arln::AccessBits::eTransferWrite);
    m_tx1.writeToImage(d, w * h * 4, {w, h});
    m_tx1.transition(arln::ImageLayout::eTransferDst, arln::ImageLayout::eShaderReadOnly,
                    arln::PipelineStageBits::eTransfer, arln::PipelineStageBits::eFragmentShader,
                    arln::AccessBits::eTransferWrite, arln::AccessBits::eShaderRead);
    stbi_image_free(d);

    d = stbi_load("../../assets/containerSpec.png", &w, &h, &c, STBI_rgb_alpha);
    m_tx2.recreate(w, h, arln::Format::eR8G8B8A8Unorm, arln::ImageUsageBits::eSampled, arln::MemoryType::eGpuOnly);
    m_tx2.transition(arln::ImageLayout::eUndefined, arln::ImageLayout::eTransferDst,
                     arln::PipelineStageBits::eTopOfPipe, arln::PipelineStageBits::eTransfer,
                     0, arln::AccessBits::eTransferWrite);
    m_tx2.writeToImage(d, w * h * 4, {w, h});
    m_tx2.transition(arln::ImageLayout::eTransferDst, arln::ImageLayout::eShaderReadOnly,
                     arln::PipelineStageBits::eTransfer, arln::PipelineStageBits::eFragmentShader,
                     arln::AccessBits::eTransferWrite, arln::AccessBits::eShaderRead);
    stbi_image_free(d);

    arln::DescriptorWriter()
        .addBuffer(m_ds, m_vb, 0, arln::DescriptorType::eStorageBuffer)
        .addBuffer(m_ds, m_ob, 1, arln::DescriptorType::eStorageBuffer)
        .addBuffer(m_ds, m_mb, 2, arln::DescriptorType::eStorageBuffer)
        .addImage(m_ds, m_tx1, m_sm, 3, arln::DescriptorType::eCombinedImageSampler, 0)
        .addImage(m_ds, m_tx2, m_sm, 3, arln::DescriptorType::eCombinedImageSampler, 1).write();

    gIdb();
}