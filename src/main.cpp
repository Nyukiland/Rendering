#include "opengl-framework/opengl-framework.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include <vector>
#include <cmath>

// Helper function to check if the current cube should be removed
bool is_removed(int x, int y, int z) {
    return (x == 1 && y == 1 && z == 1); // The center cube is always removed
}

// Recursive function to generate Menger Sponge mesh
void generate_menger_sponge(std::vector<float>& vertices, std::vector<uint32_t>& indices, 
                             float size, float offsetX, float offsetY, float offsetZ, 
                             int iteration)
{
    if (iteration == 0) {
        // Base cube: vertices and indices
        float half_size = size * 0.5f;
        
        // Cube vertices
        vertices.push_back(offsetX - half_size); vertices.push_back(offsetY - half_size); vertices.push_back(offsetZ - half_size); // 0
        vertices.push_back(offsetX + half_size); vertices.push_back(offsetY - half_size); vertices.push_back(offsetZ - half_size); // 1
        vertices.push_back(offsetX + half_size); vertices.push_back(offsetY + half_size); vertices.push_back(offsetZ - half_size); // 2
        vertices.push_back(offsetX - half_size); vertices.push_back(offsetY + half_size); vertices.push_back(offsetZ - half_size); // 3
        vertices.push_back(offsetX - half_size); vertices.push_back(offsetY - half_size); vertices.push_back(offsetZ + half_size); // 4
        vertices.push_back(offsetX + half_size); vertices.push_back(offsetY - half_size); vertices.push_back(offsetZ + half_size); // 5
        vertices.push_back(offsetX + half_size); vertices.push_back(offsetY + half_size); vertices.push_back(offsetZ + half_size); // 6
        vertices.push_back(offsetX - half_size); vertices.push_back(offsetY + half_size); vertices.push_back(offsetZ + half_size); // 7
        
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
                    generate_menger_sponge(vertices, indices, new_size, 
                                           offsetX + dx * new_size, 
                                           offsetY + dy * new_size, 
                                           offsetZ + dz * new_size, 
                                           iteration - 1);
                }
            }
        }
    }
}

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

    // Generate Menger Sponge mesh with 4 iterations
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    generate_menger_sponge(vertices, indices, 1.0f, 0.0f, 0.0f, 0.0f, 4);

    // Create Mesh
    auto mesh = gl::Mesh
    {{
        .vertex_buffers = {{
            .layout = {
                gl::VertexAttribute::Position3D{0}
            },
            .data = vertices,
        }},
        .index_buffer = indices,
    }};

    float angle = 0.0f;
    while (gl::window_is_open()) {
        glm::mat4 const view_matrix = camera.view_matrix();
        glm::mat4 const projection_matrix = glm::infinitePerspective(1.f, gl::framebuffer_aspect_ratio(), 0.001f);


        glClearColor(0, 0, 0, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.bind();
        shader.set_uniform("Project", glm::mat4{projection_matrix * view_matrix});
        shader.set_uniform("lightDir", glm::normalize(glm::vec3(0, 1, 1)));
        shader.set_uniform("lightColor", glm::vec4(0.5f, 0, 0, 1));
        shader.set_uniform("pointLight", glm::vec3(4, 0, -4));
        shader.set_uniform("lightIntensity", 4.0f);
        shader.set_uniform("pointLightColor", glm::vec4(0, 1, 0, 1));
        mesh.draw();
    }
}