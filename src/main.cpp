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

    auto const Mesh = gl::Mesh
    {{
        .vertex_buffers = {{
            .layout = 
            {
                gl::VertexAttribute::Position3D{0},
                gl::VertexAttribute::UV{1}
            },
            .data   = {
                //face1
                -1.0f, -1.0f, -1.0f, //0
                0, 0,

                1.0f, -1.0f, -1.0f, //1
                0,1,

                -1.0f, 1.0f, -1.0f, //2
                1,0,

                1.0f, 1.0f, -1.0f, //3
                1,1,

                //face2
                -1.0f, -1.0f, 1.0f, //4
                0, 0,

                1.0f, -1.0f, 1.0f, //5
                0,1,

                -1.0f, 1.0f, 1.0f, //6
                1,0,

                1.0f, 1.0f, 1.0f, //7
                1,1,

                //face3
                -1.0f, -1.0f, -1.0f, //8
                0, 0,

                1.0f, -1.0f, -1.0f, //9
                0,1,

                -1.0f, -1.0f, 1.0f, //10
                1,0,

                1.0f, -1.0f, 1.0f, //11
                1,1,

                //face4
                -1.0f, 1.0f, -1.0f, //12
                0, 0,

                -1.0f, 1.0f, 1.0f, //13
                0,1,

                1.0f, 1.0f, -1.0f, //14
                1,0,

                1.0f, 1.0f, 1.0f, //15
                1,1,

                //face5
                -1.0f, -1.0f, -1.0f, //16
                0, 0,

                -1.0f, -1.0f, 1.0f, //17
                0,1,

                -1.0f, 1.0f, -1.0f, //18
                1,0,

                -1.0f, 1.0f, 1.0f, //19
                1,1,

                //face6
                1.0f, 1.0f, 1.0f, //20
                0, 0,

                1.0f, -1.0f, 1.0f, //21
                0,1,

                1.0f, 1.0f, -1.0f, //22
                1,0,

                1.0f, -1.0f, -1.0f, //23
                1,1,
            },
        }},
        .index_buffer   = {
            0, 1, 2,
            1, 2, 3,

            4, 5, 6,
            5, 6, 7,

            8, 9, 10,
            9, 10, 11,

            12, 13, 14,
            13, 14, 15,

            16, 17, 18,
            17, 18, 19,

            20, 21, 22,
            21, 22, 23,
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
        Mesh.draw();
    }
}