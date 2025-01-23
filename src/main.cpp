#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu
#include "glm/ext/matrix_clip_space.hpp"

int main()
{
    // Initialisation
    gl::init("TPs de Rendering"); // On crée une fenêtre et on choisit son nom
    gl::maximize_window(); // On peut la maximiser si on veut

    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE);

    auto camera = gl::Camera{};
    gl::set_events_callbacks({camera.events_callbacks()});

    auto const shader = gl::Shader
    {{
    .vertex   = gl::ShaderSource::File{"res/vertexMove.glsl"},
    .fragment = gl::ShaderSource::File{"res/fragment.glsl"},
    }};

    auto const House_Mesh = gl::Mesh
    {{
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position2D{0 /*Index de l'attribut dans le shader, on en reparle juste après*/}},
            .data   = {
                -0.25f, -0.5f, 
                +0.25f, -0.5f, 
                -0.25f, +0.5f,  
                +0.25f, +0.5f,  
                +0.f, +1.f, 
                +0.f, -1.f
            },
        }},
        .index_buffer   = {
            0, 1, 2,
            1, 2, 3,
            2, 3, 4,
            0, 1, 5
        },
    }};

    while (gl::window_is_open())
    {
        glm::mat4 const view_matrix = camera.view_matrix();
        glm::mat4 const projection_matrix = glm::infinitePerspective(1.f /*field of view in radians*/, gl::framebuffer_aspect_ratio() /*aspect ratio*/, 0.001f /*near plane*/);

        glClearColor(0, 0, 0, 1.f); 
        glClear(GL_COLOR_BUFFER_BIT);

        //gl::bind_default_shader(); // On a besoin qu'un shader soit bind (i.e. "actif") avant de draw(). On en reparle dans la section d'après.
        shader.bind();
        shader.set_uniform("Project", glm::mat4{projection_matrix * view_matrix});
        House_Mesh.draw();
    }
}