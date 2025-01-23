#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu

int main()
{
    // Initialisation
    gl::init("TPs de Rendering"); // On crée une fenêtre et on choisit son nom
    gl::maximize_window(); // On peut la maximiser si on veut

    auto const shader = gl::Shader
    {{
    .vertex   = gl::ShaderSource::File{"res/vertex.glsl"},
    .fragment = gl::ShaderSource::File{"res/fragment.glsl"},
    }};

    auto const House_Mesh = gl::Mesh
    {{
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position2D{0 /*Index de l'attribut dans le shader, on en reparle juste après*/}},
            .data   = {
                -0.5f, -1.f, // Position2D du 1er sommet
                +0.5f, -1.f, // Position2D du 2ème sommet
                -0.5f, +0.f,  // Position2D du 3ème sommet
                +0.5f, +0.f,  // Position2D du 3ème sommet
                +0.f, +1.f, // Position2D du 1er sommet
            },
        }},
        .index_buffer   = {
            0, 1, 2,
            1, 2, 3,
            2, 3, 4
        },
    }};

    float t = 0;
    while (gl::window_is_open())
    {
        // Rendu à chaque frame
        t += 0.0001f;
        if (t>1) t = 0;
        glClearColor(t, t, t, 1.f); // Choisis la couleur à utiliser. Les paramètres sont R, G, B, A avec des valeurs qui vont de 0 à 1
        glClear(GL_COLOR_BUFFER_BIT);

        //gl::bind_default_shader(); // On a besoin qu'un shader soit bind (i.e. "actif") avant de draw(). On en reparle dans la section d'après.
        shader.bind();
        shader.set_uniform("PositionChange", glm::vec2{(t*3) - 1.5f, 0.f});
        House_Mesh.draw();
    }
}