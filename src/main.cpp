#include "opengl-framework/opengl-framework.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include <vector>
#include <cmath>


glm::vec3 compute_normal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
    glm::vec3 edge1 = v2 - v1;
    glm::vec3 edge2 = v3 - v1;
    return glm::normalize(glm::cross(edge1, edge2)); // Cross product gives the normal
}

// Helper function to determine if a cube should be removed (in the Menger Sponge pattern)
bool is_removed(int x, int y, int z) {
    return (x == 1 && y == 1 && z == 1); // The center cube is always removed
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

        // Calculate normals for each face
        glm::vec3 normal_front = compute_normal(p0, p1, p2);
        glm::vec3 normal_back = compute_normal(p4, p5, p6);
        glm::vec3 normal_left = compute_normal(p0, p3, p7);
        glm::vec3 normal_right = compute_normal(p1, p2, p6);
        glm::vec3 normal_top = compute_normal(p2, p3, p7);
        glm::vec3 normal_bottom = compute_normal(p0, p1, p4);

        // Push vertices and normals for each face of the cube
        // Front face (two triangles)
        vertices.push_back(p0.x); normals.push_back(normal_front.x);
        vertices.push_back(p0.y); normals.push_back(normal_front.y);
        vertices.push_back(p0.z); normals.push_back(normal_front.z);

        vertices.push_back(p1.x); normals.push_back(normal_front.x);
        vertices.push_back(p1.y); normals.push_back(normal_front.y);
        vertices.push_back(p1.z); normals.push_back(normal_front.z);

        vertices.push_back(p2.x); normals.push_back(normal_front.x);
        vertices.push_back(p2.y); normals.push_back(normal_front.y);
        vertices.push_back(p2.z); normals.push_back(normal_front.z);

        vertices.push_back(p0.x); normals.push_back(normal_front.x);
        vertices.push_back(p0.y); normals.push_back(normal_front.y);
        vertices.push_back(p0.z); normals.push_back(normal_front.z);

        vertices.push_back(p2.x); normals.push_back(normal_front.x);
        vertices.push_back(p2.y); normals.push_back(normal_front.y);
        vertices.push_back(p2.z); normals.push_back(normal_front.z);

        vertices.push_back(p3.x); normals.push_back(normal_front.x);
        vertices.push_back(p3.y); normals.push_back(normal_front.y);
        vertices.push_back(p3.z); normals.push_back(normal_front.z);

        // Back face (two triangles)
        vertices.push_back(p4.x); normals.push_back(normal_back.x);
        vertices.push_back(p4.y); normals.push_back(normal_back.y);
        vertices.push_back(p4.z); normals.push_back(normal_back.z);

        vertices.push_back(p5.x); normals.push_back(normal_back.x);
        vertices.push_back(p5.y); normals.push_back(normal_back.y);
        vertices.push_back(p5.z); normals.push_back(normal_back.z);

        vertices.push_back(p6.x); normals.push_back(normal_back.x);
        vertices.push_back(p6.y); normals.push_back(normal_back.y);
        vertices.push_back(p6.z); normals.push_back(normal_back.z);

        vertices.push_back(p4.x); normals.push_back(normal_back.x);
        vertices.push_back(p4.y); normals.push_back(normal_back.y);
        vertices.push_back(p4.z); normals.push_back(normal_back.z);

        vertices.push_back(p6.x); normals.push_back(normal_back.x);
        vertices.push_back(p6.y); normals.push_back(normal_back.y);
        vertices.push_back(p6.z); normals.push_back(normal_back.z);

        vertices.push_back(p7.x); normals.push_back(normal_back.x);
        vertices.push_back(p7.y); normals.push_back(normal_back.y);
        vertices.push_back(p7.z); normals.push_back(normal_back.z);

        // Left face (two triangles)
        vertices.push_back(p0.x); normals.push_back(normal_left.x);
        vertices.push_back(p0.y); normals.push_back(normal_left.y);
        vertices.push_back(p0.z); normals.push_back(normal_left.z);

        vertices.push_back(p3.x); normals.push_back(normal_left.x);
        vertices.push_back(p3.y); normals.push_back(normal_left.y);
        vertices.push_back(p3.z); normals.push_back(normal_left.z);

        vertices.push_back(p7.x); normals.push_back(normal_left.x);
        vertices.push_back(p7.y); normals.push_back(normal_left.y);
        vertices.push_back(p7.z); normals.push_back(normal_left.z);

        vertices.push_back(p0.x); normals.push_back(normal_left.x);
        vertices.push_back(p0.y); normals.push_back(normal_left.y);
        vertices.push_back(p0.z); normals.push_back(normal_left.z);

        vertices.push_back(p7.x); normals.push_back(normal_left.x);
        vertices.push_back(p7.y); normals.push_back(normal_left.y);
        vertices.push_back(p7.z); normals.push_back(normal_left.z);

        vertices.push_back(p4.x); normals.push_back(normal_left.x);
        vertices.push_back(p4.y); normals.push_back(normal_left.y);
        vertices.push_back(p4.z); normals.push_back(normal_left.z);

        // Right face (two triangles)
        vertices.push_back(p1.x); normals.push_back(normal_right.x);
        vertices.push_back(p1.y); normals.push_back(normal_right.y);
        vertices.push_back(p1.z); normals.push_back(normal_right.z);

        vertices.push_back(p2.x); normals.push_back(normal_right.x);
        vertices.push_back(p2.y); normals.push_back(normal_right.y);
        vertices.push_back(p2.z); normals.push_back(normal_right.z);

        vertices.push_back(p6.x); normals.push_back(normal_right.x);
        vertices.push_back(p6.y); normals.push_back(normal_right.y);
        vertices.push_back(p6.z); normals.push_back(normal_right.z);

        vertices.push_back(p1.x); normals.push_back(normal_right.x);
        vertices.push_back(p1.y); normals.push_back(normal_right.y);
        vertices.push_back(p1.z); normals.push_back(normal_right.z);

        vertices.push_back(p6.x); normals.push_back(normal_right.x);
        vertices.push_back(p6.y); normals.push_back(normal_right.y);
        vertices.push_back(p6.z); normals.push_back(normal_right.z);

        vertices.push_back(p5.x); normals.push_back(normal_right.x);
        vertices.push_back(p5.y); normals.push_back(normal_right.y);
        vertices.push_back(p5.z); normals.push_back(normal_right.z);

        // Top face (two triangles)
        vertices.push_back(p2.x); normals.push_back(normal_top.x);
        vertices.push_back(p2.y); normals.push_back(normal_top.y);
        vertices.push_back(p2.z); normals.push_back(normal_top.z);

        vertices.push_back(p3.x); normals.push_back(normal_top.x);
        vertices.push_back(p3.y); normals.push_back(normal_top.y);
        vertices.push_back(p3.z); normals.push_back(normal_top.z);

        vertices.push_back(p7.x); normals.push_back(normal_top.x);
        vertices.push_back(p7.y); normals.push_back(normal_top.y);
        vertices.push_back(p7.z); normals.push_back(normal_top.z);

        vertices.push_back(p2.x); normals.push_back(normal_top.x);
        vertices.push_back(p2.y); normals.push_back(normal_top.y);
        vertices.push_back(p2.z); normals.push_back(normal_top.z);

        vertices.push_back(p7.x); normals.push_back(normal_top.x);
        vertices.push_back(p7.y); normals.push_back(normal_top.y);
        vertices.push_back(p7.z); normals.push_back(normal_top.z);

        vertices.push_back(p6.x); normals.push_back(normal_top.x);
        vertices.push_back(p6.y); normals.push_back(normal_top.y);
        vertices.push_back(p6.z); normals.push_back(normal_top.z);

        // Bottom face (two triangles)
        vertices.push_back(p0.x); normals.push_back(normal_bottom.x);
        vertices.push_back(p0.y); normals.push_back(normal_bottom.y);
        vertices.push_back(p0.z); normals.push_back(normal_bottom.z);

        vertices.push_back(p1.x); normals.push_back(normal_bottom.x);
        vertices.push_back(p1.y); normals.push_back(normal_bottom.y);
        vertices.push_back(p1.z); normals.push_back(normal_bottom.z);

        vertices.push_back(p4.x); normals.push_back(normal_bottom.x);
        vertices.push_back(p4.y); normals.push_back(normal_bottom.y);
        vertices.push_back(p4.z); normals.push_back(normal_bottom.z);

        vertices.push_back(p1.x); normals.push_back(normal_bottom.x);
        vertices.push_back(p1.y); normals.push_back(normal_bottom.y);
        vertices.push_back(p1.z); normals.push_back(normal_bottom.z);

        vertices.push_back(p5.x); normals.push_back(normal_bottom.x);
        vertices.push_back(p5.y); normals.push_back(normal_bottom.y);
        vertices.push_back(p5.z); normals.push_back(normal_bottom.z);

        vertices.push_back(p4.x); normals.push_back(normal_bottom.x);
        vertices.push_back(p4.y); normals.push_back(normal_bottom.y);
        vertices.push_back(p4.z); normals.push_back(normal_bottom.z);

    } else {
        // Recursively generate sub-cubes
        float new_size = size / 3.0f;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                for (int dz = -1; dz <= 1; dz++) {
                    if (is_removed(dx, dy, dz)) continue; // Skip removed cubes
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