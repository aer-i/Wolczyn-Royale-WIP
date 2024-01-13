#include "Scene.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Scene::Scene(arln::Window& t_w, arln::Context& t_c) noexcept
    : m_ctx{ t_c }, m_cm{ t_w, { 0.f, 0.f, 3.f }, 90.f }, m_dc{ }, m_fru{} {
    pms();
    pmd();
    gidb();
    pr();
}

Scene::~Scene() noexcept {
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

auto Scene::pmd() noexcept -> v0
{
    for (arln::i32 x = 15; x--; )
    {
        for (arln::i32 y = 15; y--; )
        {
            for (arln::i32 z = 15; z--; )
            {
            switch (x % 4)
            {
            case 3:
//                this->lMdl("cube", "gold", {-x * 3, -y * 3, -z * 3});
//                break;
//            case 2:
//                this->lMdl("zuzanna", "turquoise", {-x * 3, -y * 3, -z * 3});
//                break;
//            case 1:
//                this->lMdl("cube", "red rubber", {-x * 3, -y * 3, -z * 3});
//                break;
            default:
                this->lMdl("cube", "cyan rubber", {-x * 3, -y * 3, -z * 3});
                break;
            }
            }
        }
    }


}

static inline auto nP(arln::vec4 p) noexcept -> arln::vec4 {
    return p / length(arln::vec3(p));
}

auto Scene::u() noexcept -> v0 {
    m_cm.sP(arln::toRadians(70), arln::f32(m_ctx.getCurrentExtent().x) / arln::f32(m_ctx.getCurrentExtent().y), 0.1f, 1024.f);
    m_cm.u();
}

auto Scene::lMdl(std::string_view t_msh, std::string_view t_mtr, arln::vec3 const& t_pos,
                 arln::vec3 const& t_rot, arln::vec3 const& t_scl) noexcept -> v0 {
    auto* msh = &m_mhs[t_msh.data()];
    m_mls.emplace_back(msh, m_mts[t_mtr.data()], t_pos, t_rot, t_scl);
}

auto Scene::lMhs(std::string_view t_n, std::string_view t_fp) noexcept -> v0 {
    m_mhs[t_n.data()] = Mesh(t_fp, m_vv, m_iv);
}

auto Scene::lMtr(std::string_view t_n, const arln::vec3& t_a, const arln::vec3& t_d, const arln::vec3& t_s, arln::f32 t_shn) noexcept -> v0 {
    m_mts[t_n.data()] = static_cast<arln::u32>(m_mtHls.size());
    m_mtHls.emplace_back(Material{
        .dt = 0,
        .st = 1,
        .shin = t_shn
    });
}

auto Scene::pr() noexcept -> v0
{
    struct WD {
        arln::mat4 mtx;
        alignas(16) arln::u32 mi;
    };

    m_dp = m_ctx.createDescriptorPool();
    m_ds = m_dp.addBinding(0, arln::DescriptorType::eStorageBuffer, arln::ShaderStageBits::eVertex)
        .addBinding(1, arln::DescriptorType::eStorageBuffer, arln::ShaderStageBits::eVertex)
        .addBinding(2, arln::DescriptorType::eStorageBuffer, arln::ShaderStageBits::eFragment)
        .addBinding(3, arln::DescriptorType::eCombinedImageSampler, arln::ShaderStageBits::eFragment, 1024)
        .createDescriptor();
    m_ob.recreate(arln::BufferUsageBits::eStorageBuffer, arln::MemoryType::eGpuOnly, sizeof(WD) * m_mls.size());
    m_mb.recreate(arln::BufferUsageBits::eStorageBuffer, arln::MemoryType::eGpuOnly, sizeof(Material) * m_mtHls.size());
    m_sm = m_ctx.createSampler(arln::SamplerOptions{
        .addressModeU = arln::SamplerAddressMode::eRepeat,
        .addressModeV = arln::SamplerAddressMode::eRepeat,
        .addressModeW = arln::SamplerAddressMode::eRepeat,
        .unnormalizedCoordinates = false
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
        .depthFormat = m_ctx.getDefaultDepthFormat(), .frontFace = arln::FrontFace::eCounterClockwise,
        .cullMode = arln::CullMode::eFront, .depthStencil = true
    }; pi.pushConstants << arln::PushConstantRange(arln::ShaderStageBits::eVertex, sizeof(arln::mat4) * 2, 0);
    pi.descriptors << m_dp.getFirstDescriptor();
    m_gp = m_ctx.createGraphicsPipeline(pi);
    m_vb.recreate(arln::BufferUsageBits::eStorageBuffer, arln::MemoryType::eGpuOnly, m_vv.size() * sizeof(m_vv[0]));
    m_ib.recreate(arln::BufferUsageBits::eIndexBuffer, arln::MemoryType::eGpuOnly, m_iv.size() * sizeof(m_iv[0]));
    m_vb.writeData(m_vv.data(), m_vb.getSize());
    m_ib.writeData(m_iv.data(), m_ib.getSize());

    int w, h, c;
    arln::u8* d = stbi_load("../../assets/container.png", &w, &h, &c, 0);
    m_tx1.recreate(w, h, m_ctx.getDefaultColorFormat(), arln::ImageUsageBits::eSampled, arln::MemoryType::eGpuOnly);
    m_tx1.transition(arln::ImageLayout::eUndefined, arln::ImageLayout::eTransferDst,
                    arln::PipelineStageBits::eTopOfPipe, arln::PipelineStageBits::eTransfer,
                    0, arln::AccessBits::eTransferWrite);
    m_tx1.writeToImage(d, w * h * c, {w, h});
    m_tx1.transition(arln::ImageLayout::eTransferDst, arln::ImageLayout::eShaderReadOnly,
                    arln::PipelineStageBits::eTransfer, arln::PipelineStageBits::eFragmentShader,
                    arln::AccessBits::eTransferWrite, arln::AccessBits::eShaderRead);
    stbi_image_free(d);

    d = stbi_load("../../assets/containerSpec.png", &w, &h, &c, 0);
    m_tx2.recreate(w, h, m_ctx.getDefaultColorFormat(), arln::ImageUsageBits::eSampled, arln::MemoryType::eGpuOnly);
    m_tx2.transition(arln::ImageLayout::eUndefined, arln::ImageLayout::eTransferDst,
                     arln::PipelineStageBits::eTopOfPipe, arln::PipelineStageBits::eTransfer,
                     0, arln::AccessBits::eTransferWrite);
    m_tx2.writeToImage(d, w * h * c, {w, h});
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
}

auto Scene::pms() noexcept -> v0
{
    this->lMtr("gold", {0.24725f, 0.1995f, 0.0745f}, {0.75164f, 0.60648f, 0.22648f}, {0.628281f, 0.555802f, 0.366065f}, 0.4f * 128.f);
    this->lMtr("cyan rubber", {0.f, 0.05f, 0.05f}, {0.4f, 0.5f, 0.5f}, {0.04f, 0.7f, 0.7f}, 0.078125f * 128.f);
    this->lMtr("red rubber", {0.05f, 0.0f, 0.0f}, {0.5f, 0.4f, 0.4f}, {0.7f, 0.04f, 0.04f}, 0.078125f * 128.f);
    this->lMtr("turquoise", {0.1f, 0.18725f, 0.1745f}, {0.396f, 0.74151f, 0.69102f}, {0.297254f, 0.30829f, 0.306678f}, 0.1 * 128.f);

    this->lMhs("cube", "../../assets/cube.obj");
    //this->lMhs("ico", "../../assets/ico.obj");
    //this->lMhs("kitten", "../../assets/kitten.obj");
    //this->lMhs("zuzanna", "../../assets/zuzanna.obj");
}

auto Scene::gidb() noexcept -> v0
{
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

