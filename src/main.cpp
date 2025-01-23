#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu

int main()
{
    // Initialisation
    gl::init("TPs de Rendering"); // On crée une fenêtre et on choisit son nom
    gl::maximize_window(); // On peut la maximiser si on veut

    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE);

    auto const shader = gl::Shader
    {{
    .vertex   = gl::ShaderSource::File{"res/vertex.glsl"},
    .fragment = gl::ShaderSource::File{"res/fragment.glsl"},
    }};

     auto const shaderBack = gl::Shader
    {{
    .vertex   = gl::ShaderSource::File{"res/baseVertex.glsl"},
    .fragment = gl::ShaderSource::File{"res/baseFragment.glsl"},
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

    auto const BackMesh = gl::Mesh
    {{
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position2D{0 /*Index de l'attribut dans le shader, on en reparle juste après*/}},
            .data   = {
                -1.f, -1.f,
                +1.f, -1.f,
                -1.f, +1.f,
                +1.f, +1.f
            },
        }},
        .index_buffer   = {
            0, 1, 2,
            1, 2, 3,
        },
    }};

    float t = 0;
    while (gl::window_is_open())
    {
        // Rendu à chaque frame
        t += 0.0001f;
        if (t>1) t = 0;

        glClearColor(t, t, t, 1.f); 
        //glClear(GL_COLOR_BUFFER_BIT);

        shaderBack.bind();
        shaderBack.set_uniform("ColorGive", glm::vec3{t,t,t});
        BackMesh.draw();

        //gl::bind_default_shader(); // On a besoin qu'un shader soit bind (i.e. "actif") avant de draw(). On en reparle dans la section d'après.
        shader.bind();
        shader.set_uniform("PositionChange", glm::vec2{(t*3) - 1.5f, sin(gl::time_in_seconds())});
        shader.set_uniform("AspectRatio", gl::framebuffer_aspect_ratio());
        House_Mesh.draw();
    }
}