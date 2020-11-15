#pragma once
extern "C" {
    #include <stdio.h>
    #include <stdlib.h>
}

#include "math_utils.h"

struct Vertex
{
    Vector pos;
    Vector normal;
    FixedPoint u,v;
};

class Mesh
{
private:
    bool get_line(FILE * file, char * buffer, int max) {
        
        char c;
        if (feof(file) != 0 || fread(&c, 1, 1, file) != 0) {
            return false;
        }

        int idx = 0;
        while ( c != '\n' && idx < max) {
            
            buffer[idx] = c;
            idx++;
            if (fread(&c, 1, 1, file) == 0) {
                break;
            }
        }
        // null terminator
        buffer[idx] = 0;

        return true;
    }
public:
    Mesh() = default;
    ~Mesh() {
        if (vertices) {
            free(vertices);
        }
    }

    Matrix model_matrix;
    int num_vertices = 0;
    Vertex * vertices = nullptr;

    bool load_ply(const char * file_path) {
        
        return false;
    }

    // can only load binary stl
    bool load_stl(const char * file_path) {
        FILE * file = fopen(file_path, "r");
        if (file) {
            // skip 80 byte header
            fseek(file, 80, SEEK_SET);
            unsigned int num_triangles = 0;
            fread(&num_triangles, sizeof(unsigned int), 1, file);

            if (num_triangles) {
                num_vertices = num_triangles*3;

                vertices = (Vertex*)malloc(sizeof(Vertex)*num_vertices);

                for (int idx = 0 ; idx < num_triangles ; idx++) {

                    float nx, ny, nz;
                    fread(&nx, sizeof(float), 1, file);
                    fread(&ny, sizeof(float), 1, file);
                    fread(&nz, sizeof(float), 1, file);

                    for (int v = 0 ; v < 3 ; v++) {
                        float vx, vy, vz;
                        fread(&vx, sizeof(float), 1, file);
                        fread(&vy, sizeof(float), 1, file);
                        fread(&vz, sizeof(float), 1, file);

                        vertices[idx*3 + v].pos = Vector(vx,vy,vz);
                    }

                    unsigned short attribute;
                    fread(&attribute, sizeof(unsigned short), 1, file);
                }
            }
            else {
                fclose(file);
                return false;
            }

            fclose(file);
            return true;
        }
        return false;
    }

    void draw(Matrix vp)
    {
        // Matrix mvp = vp*model_matrix;
        // for (int vert_idx = 0 ; vert_idx < num_vertices  ; vert_idx+=3)
        // {
        //     GsPoly3 tri;
        //     for (int idx = 0 ; idx < 3 ; idx++) {
        //         Vector v = mvp*vertices[vert_idx + idx].pos;
                
        //         tri.x[idx] = v.screen_x();
        //         tri.y[idx] = v.screen_y();
        //         tri.r = 255;
        //         tri.g = tri.b = 0;
        //         tri.attribute = 0;
        //     }
            
        //     GsSortPoly3(&tri);  
        // }
    }
};