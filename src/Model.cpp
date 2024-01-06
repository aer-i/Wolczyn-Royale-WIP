#define FAST_OBJ_IMPLEMENTATION
#include "Model.hpp"
#include <algorithm>
#include <fast_obj.h>
#include <meshoptimizer.h>

Mesh::Mesh(std::string_view t_fp, std::vector<Vertex>& t_vb, std::vector<arln::u32>& t_ib) noexcept
{
    fastObjMesh* ob = fast_obj_read(t_fp.data());
    while (!ob) {
        std::printf("Failed to load model: %s\n", t_fp.data());
        std::this_thread::sleep_for(std::chrono::microseconds(1000));
    };

    size_t ixc = 0;

    for (arln::u32 i = 0; i < ob->face_count; ++i)
        ixc += 3 * (ob->face_vertices[i] - 2);

    std::vector<Vertex> lvs(ixc);
    ic = ixc;

    size_t vo = 0;
    size_t io = 0;

    for (arln::u32 i = 0; i < ob->face_count; ++i)
    {
        for (arln::u32 j = 0; j < ob->face_vertices[i]; ++j)
        {
            fastObjIndex gi = ob->indices[io + j];

            if (j >= 3) {
                lvs[vo + 0] = lvs[vo - 3];
                lvs[vo + 1] = lvs[vo - 1];
                vo += 2;
            }

            Vertex& v = lvs[vo++];
            v.vx = ob->positions[gi.p * 3 + 0];
            v.vy = ob->positions[gi.p * 3 + 1];
            v.vz = ob->positions[gi.p * 3 + 2];
            v.nx = arln::u8(ob->normals[gi.n * 3 + 0] * 127.f + 127.5f);
            v.ny = arln::u8(ob->normals[gi.n * 3 + 1] * 127.f + 127.5f);
            v.nz = arln::u8(ob->normals[gi.n * 3 + 2] * 127.f + 127.5f);
            v.tu = meshopt_quantizeHalf(ob->texcoords[gi.t * 2 + 0]);
            v.tv = meshopt_quantizeHalf(ob->texcoords[gi.t * 2 + 1]);
        }

        io += ob->face_vertices[i];
    }

    fast_obj_destroy(ob);

    std::vector<arln::u32> rmp(ixc);
    size_t vc = meshopt_generateVertexRemap(rmp.data(), nullptr, ixc, lvs.data(), ixc, sizeof(Vertex));

    std::vector<Vertex> vi(vc);
    std::vector<arln::u32> ii(ixc);

    meshopt_remapVertexBuffer(vi.data(), lvs.data(), ixc, sizeof(Vertex), rmp.data());
    meshopt_remapIndexBuffer(ii.data(), nullptr, ixc, rmp.data());

    meshopt_optimizeVertexCache(ii.data(), ii.data(), ixc, vc);
    meshopt_optimizeVertexFetch(vi.data(), ii.data(), ixc, vi.data(), vc, sizeof(Vertex));

    vxo = static_cast<arln::i32>(t_vb.size());
    t_vb.insert(t_vb.end(), vi.begin(), vi.end());
    t_ib.insert(t_ib.end(), ii.begin(), ii.end());
}
