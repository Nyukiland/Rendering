#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu
#include "glm/ext/matrix_clip_space.hpp"

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
    .vertex   = gl::ShaderSource::File{"res/vertex3D.glsl"},
    .fragment = gl::ShaderSource::File{"res/fragment3D.glsl"},
    }};

    auto const House_Mesh = gl::Mesh
    {{
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position3D{0 /*Index de l'attribut dans le shader, on en reparle juste après*/}},
            .data   = {
                -0.25f, -0.5f, -0.25f, //0
                +0.25f, -0.5f, -0.25f, //1
                -0.25f, +0.5f, -0.25f, //2
                +0.25f, +0.5f, -0.25f, //3
                -0.25f, -0.5f, +0.25f, //4
                +0.25f, -0.5f, +0.25f, //5
                -0.25f, +0.5f, +0.25f, //6
                +0.25f, +0.5f, +0.25f, //7
                +0.f, +1.f, +0.f, //8
                +0.f, -1.f, +0.f //9
            },
        }},
        .index_buffer   = {
            0, 1, 2,
            1, 2, 3,
            4, 5, 6,
            5, 6, 7,
            1, 3, 5,
            3, 5, 7, 
            0, 2, 4,
            2, 4, 6,
            0, 1, 9,
            5, 4, 9,
            0, 4, 9,
            5, 1, 9,
            2, 3, 8,
            6, 7, 8,
            3, 7, 8,
            2, 6, 8
        },
    }};

    while (gl::window_is_open())
    {
        glm::mat4 const view_matrix = camera.view_matrix();
        glm::mat4 const projection_matrix = glm::infinitePerspective(1.f /*field of view in radians*/, gl::framebuffer_aspect_ratio() /*aspect ratio*/, 0.001f /*near plane*/);

        glClearColor(0, 0, 0, 1.f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //gl::bind_default_shader(); // On a besoin qu'un shader soit bind (i.e. "actif") avant de draw(). On en reparle dans la section d'après.
        
        shader.bind();
        shader.set_uniform("Project", glm::mat4{projection_matrix * view_matrix});
        House_Mesh.draw();
    }
}