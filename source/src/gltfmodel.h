#ifndef GLTFMODEL_H
#define GLTFMODEL_H

#include "model.h"
#include "shader.h"

struct gltfmodel : model
{
    struct vertex { float x,y,z, nx,ny,nz, u,v; };
    struct submesh {
        vector<vertex> verts;
        vector<unsigned int> indices;
        int texid;
        float r,g,b;
    };
    vector<submesh> submeshes;
    shader *sh;
    char *mdlname;

    gltfmodel(const char *name);
    ~gltfmodel();

    bool load();
    char *name();
    int type();
    void cleanup();
    void render(int anim, int varseed, float speed, int basetime, const vec &o, float roll, float yaw, float pitch, dynent *d, modelattach *a, float scale);
    void setskin(int tex);
};

model *loadgltfmodel(const char *name);
void loadgltf(char *filename);
void loadgltfmap(char *filename);

extern model *gltfmapmodel;

#endif