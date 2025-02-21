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

    auto const myTexture = gl::Texture{
        gl::TextureSource::File{ 
            .path           = "res/chatTexture.png",
            .flip_y         = true, // Il n'y a pas de convention universelle sur la direction de l'axe Y. Les fichiers (.png, .jpeg) utilisent souvent une direction différente de celle attendue par OpenGL. Ce booléen flip_y est là pour inverser la texture si jamais elle n'apparaît pas dans le bon sens.
            .texture_format = gl::InternalFormat::RGBA8, // Format dans lequel la texture sera stockée. On pourrait par exemple utiliser RGBA16 si on voulait 16 bits par canal de couleur au lieu de 8. (Mais ça ne sert à rien dans notre cas car notre fichier ne contient que 8 bits par canal, donc on ne gagnerait pas de précision). On pourrait aussi stocker en RGB8 si on ne voulait pas de canal alpha. On utilise aussi parfois des textures avec un seul canal (R8) pour des usages spécifiques.
        },
        gl::TextureOptions{
            .minification_filter  = gl::Filter::Linear, // Comment on va moyenner les pixels quand on voit l'image de loin ?
            .magnification_filter = gl::Filter::Linear, // Comment on va interpoler entre les pixels quand on zoom dans l'image ?
            .wrap_x               = gl::Wrap::Repeat,   // Quelle couleur va-t-on lire si jamais on essaye de lire en dehors de la texture ?
            .wrap_y               = gl::Wrap::Repeat,   // Idem, mais sur l'axe Y. En général on met le même wrap mode sur les deux axes.
        }
    };

    while (gl::window_is_open())
    {
        glm::mat4 const view_matrix = camera.view_matrix();
        glm::mat4 const projection_matrix = glm::infinitePerspective(1.f /*field of view in radians*/, gl::framebuffer_aspect_ratio() /*aspect ratio*/, 0.001f /*near plane*/);

        glClearColor(0, 0, 0, 1.f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //gl::bind_default_shader(); // On a besoin qu'un shader soit bind (i.e. "actif") avant de draw(). On en reparle dans la section d'après.
        
        shader.bind();
        shader.set_uniform("Project", glm::mat4{projection_matrix * view_matrix});
        shader.set_uniform("m_texture", myTexture);
        Mesh.draw();
    }
}