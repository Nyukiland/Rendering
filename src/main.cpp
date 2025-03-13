#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu
#include "glm/ext/matrix_clip_space.hpp"
#include <filesystem>
#include <iostream>
#include "tiny_obj_loader.h"
#include <glm/gtc/matrix_transform.hpp>

auto load_mesh(std::filesystem::path const& path) -> gl::Mesh
{
    auto reader = tinyobj::ObjReader{};
    reader.ParseFromFile(gl::make_absolute_path(path).string(), {});

    if (!reader.Error().empty())
        throw std::runtime_error("Failed to read 3D model:\n" + reader.Error());
    if (!reader.Warning().empty())
        std::cout << "Warning while reading 3D model:\n" + reader.Warning() << std::endl;

    auto const& attrib = reader.GetAttrib();
    auto const& shapes = reader.GetShapes();

    std::vector<float> vertices;
    std::vector<uint32_t> indices;

    for (auto const& shape : shapes)
    {
        for (size_t i = 0; i < shape.mesh.indices.size(); i++)
        {
            auto const& idx = shape.mesh.indices[i];

            // Position
            vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);

            // UV
            if (!attrib.texcoords.empty() && idx.texcoord_index >= 0)
            {
                vertices.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
                vertices.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
            }
            else
            {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }

            // Normal
            if (!attrib.normals.empty() && idx.normal_index >= 0)
            {
                vertices.push_back(attrib.normals[3 * idx.normal_index + 0]);
                vertices.push_back(attrib.normals[3 * idx.normal_index + 1]);
                vertices.push_back(attrib.normals[3 * idx.normal_index + 2]);
            }
            else
            {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
                vertices.push_back(1.0f);
            }

            indices.push_back(static_cast<uint32_t>(i));
        }
    }

    return gl::Mesh
    {{
        .vertex_buffers = {{
            .layout = {
                gl::VertexAttribute::Position3D{0},
                gl::VertexAttribute::UV{1},
                gl::VertexAttribute::Normal3D{2}
            },
            .data = vertices,
        }},
        .index_buffer = indices,
    }};
}

int main()
{
    // Initialisation
    gl::init("TPs de Rendering"); // On crée une fenêtre et on choisit son nom
    gl::maximize_window(); // On peut la maximiser si on veut

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE);

    auto camera = gl::Camera{};
    gl::set_events_callbacks({camera.events_callbacks()});

    auto const shader = gl::Shader
    {{
    .vertex   = gl::ShaderSource::File{"res/vertexFlower.glsl"},
    .fragment = gl::ShaderSource::File{"res/fragmentFlower.glsl"},
    }};

    auto const Mesh = load_mesh("res/Chest.obj");

    float angle = 0.0f;

    while (gl::window_is_open())
    {
        glm::mat4 const view_matrix = camera.view_matrix();
        glm::mat4 const projection_matrix = glm::infinitePerspective(1.f /*field of view in radians*/, gl::framebuffer_aspect_ratio() /*aspect ratio*/, 0.001f /*near plane*/);
        glm::mat4 rotMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));

        angle += 0.0001f;

        glClearColor(0, 0, 0, 1.f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shader.bind();
        shader.set_uniform("Project", glm::mat4{projection_matrix * view_matrix * rotMatrix});
        shader.set_uniform("TransformMatrix", glm::mat4{rotMatrix});
        shader.set_uniform("lightDir", glm::normalize(glm::vec3(0,1,1)));
        shader.set_uniform("lightColor", glm::vec4(0.5f,0,0,1));
        shader.set_uniform("pointLight", glm::vec3(4,0,-4));
        shader.set_uniform("lightIntensity", 4.0f);
        shader.set_uniform("pointLightColor", glm::vec4(0,1,0,1));
        Mesh.draw();
    }
}