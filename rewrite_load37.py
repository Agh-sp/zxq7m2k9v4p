import re
p = 'source/src/gltfmodel.cpp'
s = open(p).read()
start = s.index('bool gltfmodel::load()')
end = s.index('char *gltfmodel::name()')
new = r'''bool gltfmodel::load()
{
    char path[512];
    if (strstr(mdlname, ".gltf") || strstr(mdlname, ".glb"))
    {
        snprintf(path, sizeof(path), "%s", mdlname);
    }
    else
    {
        snprintf(path, sizeof(path), "packages/models/%s/model.gltf", mdlname);
    }
    cgltf_options options = {0};
    cgltf_data *data = NULL;
    cgltf_result result = cgltf_parse_file(&options, path, &data);
    if (result != cgltf_result_success)
    {
        if (!(strstr(mdlname, ".gltf") || strstr(mdlname, ".glb")))
        {
            snprintf(path, sizeof(path), "packages/models/%s.gltf", mdlname);
            result = cgltf_parse_file(&options, path, &data);
        }
        if (result != cgltf_result_success)
        {
            conoutf("gltf: failed to parse %s", path);
            return false;
        }
    }
    result = cgltf_load_buffers(&options, data, path);
    if (result != cgltf_result_success)
    {
        conoutf("gltf: failed to load buffers for %s", path);
        cgltf_free(data);
        return false;
    }

    submeshes.setsize(0);

    auto process_prim = [&](cgltf_primitive *prim) {
        submesh sm;
        sm.texid = 0;
        sm.r = sm.g = sm.b = 1.0f;
        cgltf_accessor *pos_acc = NULL, *norm_acc = NULL, *uv_acc = NULL, *idx_acc = prim->indices;
        for (size_t a = 0; a < prim->attributes_count; a++)
        {
            if (prim->attributes[a].type == cgltf_attribute_type_position) pos_acc = prim->attributes[a].data;
            else if (prim->attributes[a].type == cgltf_attribute_type_normal) norm_acc = prim->attributes[a].data;
            else if (prim->attributes[a].type == cgltf_attribute_type_texcoord) uv_acc = prim->attributes[a].data;
        }
        if (!pos_acc) return;
        size_t nverts = pos_acc->count;
        sm.verts.reserve(nverts);
        for (size_t i = 0; i < nverts; i++)
        {
            vertex v = {0};
            cgltf_accessor_read_float(pos_acc, i, &v.x, 3);
            if (norm_acc) cgltf_accessor_read_float(norm_acc, i, &v.nx, 3);
            if (uv_acc) cgltf_accessor_read_float(uv_acc, i, &v.u, 2);
            sm.verts.add(v);
        }
        if (idx_acc)
        {
            size_t nidx = idx_acc->count;
            sm.indices.reserve(nidx);
            for (size_t i = 0; i < nidx; i++)
            {
                sm.indices.add((unsigned int)cgltf_accessor_read_index(idx_acc, i));
            }
        }
        else
        {
            for (size_t i = 0; i < nverts; i++) sm.indices.add((unsigned int)i);
        }
        if (prim->material && prim->material->has_pbr_metallic_roughness)
        {
            sm.r = prim->material->pbr_metallic_roughness.base_color_factor[0];
            sm.g = prim->material->pbr_metallic_roughness.base_color_factor[1];
            sm.b = prim->material->pbr_metallic_roughness.base_color_factor[2];
            cgltf_texture *tex = prim->material->pbr_metallic_roughness.base_color_texture.texture;
            if (tex && tex->image && tex->image->uri)
            {
                char texpath[512];
                const char *lastslash = strrchr(path, '/');
                if (!lastslash) lastslash = strrchr(path, '\\');
                if (lastslash)
                {
                    size_t dirLen = lastslash - path + 1;
                    snprintf(texpath, sizeof(texpath), "%.*s%s", (int)dirLen, path, tex->image->uri);
                }
                else
                {
                    snprintf(texpath, sizeof(texpath), "packages/models/%s", tex->image->uri);
                }
                sm.texid = textureload(texpath)->id;
            }
            else
            {
                sm.texid = 0;
            }
        }
        if (sm.texid == 0) sm.texid = notexture->id;
        submeshes.add(sm);
    };

    if (data->scene)
    {
        for (size_t ni = 0; ni < data->nodes_count; ni++)
        {
            if (data->nodes[ni].mesh)
            {
                for (size_t p = 0; p < data->nodes[ni].mesh->primitives_count; p++)
                {
                    process_prim(&data->nodes[ni].mesh->primitives[p]);
                }
            }
        }
    }
    if (submeshes.length() == 0 && data->meshes_count > 0)
    {
        for (size_t p = 0; p < data->meshes[0].primitives_count; p++)
        {
            process_prim(&data->meshes[0].primitives[p]);
        }
    }

    if (submeshes.length() == 0)
    {
        conoutf("gltf: no geometry in %s", path);
        cgltf_free(data);
        return false;
    }

    radius = 1.0f;
    zradius = 1.0f;
    shadowdist = 0;

    cgltf_free(data);
    conoutf("gltf: loaded %s (%d submeshes)", path, (int)submeshes.length());
    return true;
}

'''
s = s[:start] + new + s[end:]
open(p, 'w').write(s)
print('done')
PY