#define FAST_OBJ_IMPLEMENTATION
#include "Scene.hpp"
#include <algorithm>
#include <fast_obj.h>
#include <meshoptimizer.h>

Scene::Scene(arln::Window& t_w, arln::Context& t_c) noexcept : m_ctx{ t_c }, m_cm{ t_w, { 0.f, 0.f, -3.f}, -90.f } {
    m_cm.sP(arln::toRadians(70), arln::f32(t_w.getWidth()) / arln::f32(t_w.getHeight()));
    m_cm.u();
}

Scene::~Scene() noexcept {
    std::for_each(m_mhs.begin(), m_mhs.end(), [](Mesh& m) {
        m.mts.destroy();
        m.vb.free();
        m.ib.free();
    });
}

auto Scene::lm(const std::vector<Vertex>& t_v, const std::vector<arln::u32>& t_i) noexcept -> v0 {
    auto pi = arln::GraphicsPipelineInfo{
        .vertShaderPath = "shaders/bg.vert.spv", .fragShaderPath = "shaders/bg.frag.spv"
    }; pi.pushConstants << arln::PushConstantRange(arln::ShaderStageBits::eVertex, sizeof(PD), 0);
    auto vb = m_ctx.allocateBuffer(arln::BufferUsageBits::eStorageBuffer, arln::MemoryType::eGpu, t_v.size() * sizeof(t_v[0]));
    auto ib = m_ctx.allocateBuffer(arln::BufferUsageBits::eIndexBuffer, arln::MemoryType::eGpu, t_i.size() * sizeof(t_i[0]));
    vb.writeData(t_v.data(), vb.getSize());
    ib.writeData(t_i.data(), ib.getSize());
    m_mhs.emplace_back(m_ctx.createGraphicsPipeline(pi), vb, ib, static_cast<arln::u32>(t_i.size()));
}

auto Scene::lm(std::string_view t_pth) noexcept -> void {
    fastObjMesh* ob = fast_obj_read(t_pth.data());
    if (!ob) return;

    size_t index_count = 0;

    for (unsigned int i = 0; i < ob->face_count; ++i)
        index_count += 3 * (ob->face_vertices[i] - 2);

    std::vector<Vertex> vertices;
    std::vector<arln::u32> indices;
    vertices.resize(index_count);

    size_t vertex_offset = 0;
    size_t index_offset = 0;

    for (unsigned int i = 0; i < ob->face_count; ++i)
    {
        for (unsigned int j = 0; j < ob->face_vertices[i]; ++j)
        {
            fastObjIndex gi = ob->indices[index_offset + j];

            if (j >= 3) {
                vertices[vertex_offset + 0] = vertices[vertex_offset - 3];
                vertices[vertex_offset + 1] = vertices[vertex_offset - 1];
                vertex_offset += 2;
            }

            Vertex& v = vertices[vertex_offset++];
            v.pos.x = ob->positions[gi.p * 3 + 0];
            v.pos.y = ob->positions[gi.p * 3 + 1];
            v.pos.z = ob->positions[gi.p * 3 + 2];
            //v.nx = uint8_t(ob->normals[gi.n * 3 + 0] * 127.f + 127.5f);
            //v.ny = uint8_t(ob->normals[gi.n * 3 + 1] * 127.f + 127.5f);
            //v.nz = uint8_t(ob->normals[gi.n * 3 + 2] * 127.f + 127.5f);
            v.uvX = meshopt_quantizeHalf(ob->texcoords[gi.t * 2 + 0]);
            v.uvY = meshopt_quantizeHalf(ob->texcoords[gi.t * 2 + 1]);
        }

        index_offset += ob->face_vertices[i];
    }

    assert(vertex_offset == index_count);

    fast_obj_destroy(ob);
}

auto Scene::u() noexcept -> void {
    m_cm.u();
}