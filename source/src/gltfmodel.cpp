#define CGLTF_IMPLEMENTATION
#include "cube.h"
#include "gltfmodel.h"
#include "debuglog.h"
#include "cgltf.h"
#include <cstdio>

gltfmodel::gltfmodel(const char *name)
{
    mdlname = newstring(name);
    cullface = true;
    vertexlight = false;
    alphablend = false;
    alphatest = 0.9f;
    translucency = 0.25f;
    scale = 1;
    radius = 0;
    zradius = 0;
    shadowdist = 0;
    translate = vec(0,0,0);
    cachelimit = 8;
    batch = -1;
    sh = NULL;
}

gltfmodel::~gltfmodel() { cleanup(); }

bool gltfmodel::load()
{
    debuglog("gltfmodel::load called for %s", mdlname);
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

    // compute bounding box
    vec bbmin(1e9,1e9,1e9), bbmax(-1e9,-1e9,-1e9);
    for (size_t s = 0; s < submeshes.length(); s++)
    {
        for (size_t i = 0; i < submeshes[s].verts.length(); i++)
        {
            vertex &v = submeshes[s].verts[i];
            bbmin.x = min(bbmin.x, v.x); bbmin.y = min(bbmin.y, v.y); bbmin.z = min(bbmin.z, v.z);
            bbmax.x = max(bbmax.x, v.x); bbmax.y = max(bbmax.y, v.y); bbmax.z = max(bbmax.z, v.z);
        }
    }
    conoutf("gltf: bbox min(%.1f,%.1f,%.1f) max(%.1f,%.1f,%.1f)", bbmin.x, bbmin.y, bbmin.z, bbmax.x, bbmax.y, bbmax.z);
    cgltf_free(data);
    sh = loadshader("basic");
    conoutf("gltf: loaded %s (%d submeshes)", path, (int)submeshes.length());
    debuglog("gltf: loaded %s (%d submeshes)", path, (int)submeshes.length());
    return true;
}

char *gltfmodel::name() { return mdlname; }

int gltfmodel::type() { return MDL_MD3; }

void gltfmodel::cleanup()
{
    submeshes.setsize(0);
}
void gltfmodel::render(int anim, int varseed, float speed, int basetime, const vec &o, float roll, float yaw, float pitch, dynent *d, modelattach *a, float scale)
{
    glPushMatrix();
    glTranslatef(o.x, o.y, o.z);
    glRotatef(-90, 1, 0, 0);  // glTF Y-up -> engine Z-up
    glScalef(0.05f, 0.05f, 0.05f);
    glRotatef(yaw, 0, 0, 1);
    glRotatef(pitch, 0, 1, 0);
    glRotatef(roll, 1, 0, 0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    GLfloat lightpos[] = { 0.0f, 10.0f, 0.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    GLfloat ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

    if (sh) sh->use();

    for (size_t s = 0; s < submeshes.length(); s++)
    {
        submesh &sm = submeshes[s];
        if (sm.texid && sm.texid != notexture->id) { glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, sm.texid); }
        else { glDisable(GL_TEXTURE_2D); glColor3f(sm.r, sm.g, sm.b); }

        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < sm.indices.length(); i++)
        {
            vertex &v = sm.verts[sm.indices[i]];
            glNormal3f(v.nx, v.ny, v.nz);
            glTexCoord2f(v.u, v.v);
            glVertex3f(v.x, v.y, v.z);
        }
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);
    if (sh) sh->unuse();
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glPopMatrix();
}

void gltfmodel::setskin(int tex) { }

model *loadgltfmodel(const char *name)
{
    gltfmodel *m = new gltfmodel(name);
    if (!m->load()) { delete m; return NULL; }
    return m;
}

void loadgltf(char *filename)
{
    debuglog("loadgltf called: %s", filename);
    cgltf_options options = {0};
    cgltf_data *data = NULL;
    cgltf_result result = cgltf_parse_file(&options, filename, &data);
    if (result != cgltf_result_success)
    {
        conoutf("gltf: failed to parse %s", filename);
        return;
    }
    result = cgltf_load_buffers(&options, data, filename);
    if (result != cgltf_result_success)
    {
        conoutf("gltf: failed to load buffers for %s", filename);
        cgltf_free(data);
        return;
    }
    conoutf("gltf: loaded %s (%d meshes, %d nodes, %d materials, %d textures)",
            filename, (int)data->meshes_count, (int)data->nodes_count,
            (int)data->materials_count, (int)data->textures_count);
    cgltf_free(data);
}

COMMAND(loadgltf, "s");

model *gltfmapmodel = NULL;

void loadgltfmap(char *filename)
{
    debuglog("loadgltfmap called: %s", filename);
    if (gltfmapmodel) { delete gltfmapmodel; gltfmapmodel = NULL; }
    gltfmapmodel = loadgltfmodel(filename);
    if (gltfmapmodel) conoutf("gltfmap: loaded %s", filename);
    else conoutf("gltfmap: failed to load %s", filename);
}

COMMAND(loadgltfmap, "s");