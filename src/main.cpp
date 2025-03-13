#include "opengl-framework/opengl-framework.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include <vector>
#include <cmath>


//with the help of chatgpt
bool is_removed(int x, int y, int z) {
    return (x == 1 && y == 1 && z == 1); // The center cube is always removed
}

// Helper function to calculate normals for a triangle
glm::vec3 compute_normal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
    glm::vec3 edge1 = v2 - v1;
    glm::vec3 edge2 = v3 - v1;
    return glm::normalize(glm::cross(edge1, edge2)); // Cross product gives the normal
}

// Recursive function to generate Menger Sponge mesh with normals
void generate_menger_sponge(std::vector<float>& vertices, std::vector<uint32_t>& indices, 
                             std::vector<float>& normals, float size, float offsetX, float offsetY, float offsetZ, 
                             int iteration)
{
    if (iteration == 0) {
        // Base cube: vertices and indices
        float half_size = size * 0.5f;
        
        // Cube vertices
        glm::vec3 p0(offsetX - half_size, offsetY - half_size, offsetZ - half_size);
        glm::vec3 p1(offsetX + half_size, offsetY - half_size, offsetZ - half_size);
        glm::vec3 p2(offsetX + half_size, offsetY + half_size, offsetZ - half_size);
        glm::vec3 p3(offsetX - half_size, offsetY + half_size, offsetZ - half_size);
        glm::vec3 p4(offsetX - half_size, offsetY - half_size, offsetZ + half_size);
        glm::vec3 p5(offsetX + half_size, offsetY - half_size, offsetZ + half_size);
        glm::vec3 p6(offsetX + half_size, offsetY + half_size, offsetZ + half_size);
        glm::vec3 p7(offsetX - half_size, offsetY + half_size, offsetZ + half_size);

        // Push cube vertices
        vertices.push_back(p0.x); vertices.push_back(p0.y); vertices.push_back(p0.z); 
        vertices.push_back(p1.x); vertices.push_back(p1.y); vertices.push_back(p1.z); 
        vertices.push_back(p2.x); vertices.push_back(p2.y); vertices.push_back(p2.z); 
        vertices.push_back(p3.x); vertices.push_back(p3.y); vertices.push_back(p3.z); 
        vertices.push_back(p4.x); vertices.push_back(p4.y); vertices.push_back(p4.z); 
        vertices.push_back(p5.x); vertices.push_back(p5.y); vertices.push_back(p5.z); 
        vertices.push_back(p6.x); vertices.push_back(p6.y); vertices.push_back(p6.z); 
        vertices.push_back(p7.x); vertices.push_back(p7.y); vertices.push_back(p7.z); 

        // Calculate normals for the faces
        glm::vec3 normal_front = compute_normal(p0, p1, p2);
        glm::vec3 normal_back = compute_normal(p4, p5, p6);
        glm::vec3 normal_left = compute_normal(p0, p3, p7);
        glm::vec3 normal_right = compute_normal(p1, p2, p6);
        glm::vec3 normal_top = compute_normal(p2, p3, p7);
        glm::vec3 normal_bottom = compute_normal(p0, p1, p4);

        // Push normals to match the vertices
        for (int i = 0; i < 6; i++) {
            normals.push_back(normal_front.x); normals.push_back(normal_front.y); normals.push_back(normal_front.z);
            normals.push_back(normal_back.x); normals.push_back(normal_back.y); normals.push_back(normal_back.z);
            normals.push_back(normal_left.x); normals.push_back(normal_left.y); normals.push_back(normal_left.z);
            normals.push_back(normal_right.x); normals.push_back(normal_right.y); normals.push_back(normal_right.z);
            normals.push_back(normal_top.x); normals.push_back(normal_top.y); normals.push_back(normal_top.z);
            normals.push_back(normal_bottom.x); normals.push_back(normal_bottom.y); normals.push_back(normal_bottom.z);
        }

        // Cube indices (two triangles per face)
        uint32_t base_idx = vertices.size() / 3 - 8;
        indices.push_back(base_idx + 0); indices.push_back(base_idx + 1); indices.push_back(base_idx + 2);
        indices.push_back(base_idx + 0); indices.push_back(base_idx + 2); indices.push_back(base_idx + 3);
        indices.push_back(base_idx + 4); indices.push_back(base_idx + 5); indices.push_back(base_idx + 6);
        indices.push_back(base_idx + 4); indices.push_back(base_idx + 6); indices.push_back(base_idx + 7);
        indices.push_back(base_idx + 0); indices.push_back(base_idx + 1); indices.push_back(base_idx + 5);
        indices.push_back(base_idx + 0); indices.push_back(base_idx + 5); indices.push_back(base_idx + 4);
        indices.push_back(base_idx + 1); indices.push_back(base_idx + 2); indices.push_back(base_idx + 6);
        indices.push_back(base_idx + 1); indices.push_back(base_idx + 6); indices.push_back(base_idx + 5);
        indices.push_back(base_idx + 2); indices.push_back(base_idx + 3); indices.push_back(base_idx + 7);
        indices.push_back(base_idx + 2); indices.push_back(base_idx + 7); indices.push_back(base_idx + 6);
        indices.push_back(base_idx + 3); indices.push_back(base_idx + 0); indices.push_back(base_idx + 4);
        indices.push_back(base_idx + 3); indices.push_back(base_idx + 4); indices.push_back(base_idx + 7);
    } else {
        float new_size = size / 3.0f;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                for (int dz = -1; dz <= 1; dz++) {
                    if (is_removed(dx, dy, dz)) continue; // Skip removed cubes

                    // Recursive call to generate the next level of cubes
                    generate_menger_sponge(vertices, indices, normals, new_size, 
                                           offsetX + dx * new_size, 
                                           offsetY + dy * new_size, 
                                           offsetZ + dz * new_size, 
                                           iteration - 1);
                }
            }
        }
    }
}

//-----------------------------------------------------------

int main() {
    // Initialize OpenGL
    gl::init("Menger Sponge Renderer");
    gl::maximize_window();
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE);

    auto camera = gl::Camera{};
    gl::set_events_callbacks({camera.events_callbacks()});

    auto const shader = gl::Shader
    {{
        .vertex   = gl::ShaderSource::File{"res/vertexFractal.glsl"},
        .fragment = gl::ShaderSource::File{"res/fragmentFractal.glsl"},
    }};

    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    std::vector<float> normals; // To hold normals
    generate_menger_sponge(vertices, indices, normals, 1.0f, 0.0f, 0.0f, 0.0f, 4);

    // Create Mesh
    auto mesh = gl::Mesh
    {{
        .vertex_buffers = {{
            .layout = {
                gl::VertexAttribute::Position3D{0},
                gl::VertexAttribute::Normal3D{1}
            },
            .data = vertices,
        }},
        .index_buffer = indices,
    }};

    while (gl::window_is_open()) {
        glm::mat4 const view_matrix = camera.view_matrix();
        glm::mat4 const projection_matrix = glm::infinitePerspective(1.f, gl::framebuffer_aspect_ratio(), 0.001f);

        glClearColor(0, 0, 0, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.bind();
        shader.set_uniform("Project", glm::mat4{projection_matrix * view_matrix});
        mesh.draw();
    }
}