#version 330

#ifdef TYPE_VERTEX
    uniform mat4 mvp;
    uniform mat4 lw;
    uniform mat4 transform;

    layout(location = 0) in vec3 vertexPosition;
    layout(location = 1) in vec2 vertexUv;
    smooth out vec2 fragmentUv;

    void main () {
        fragmentUv = (transform * vec4(vertexUv, 0.0f, 1.0f)).st;
        gl_Position = mvp * lw * vec4(vertexPosition, 1.0f);
    }
#endif

#ifdef TYPE_FRAGMENT
    uniform sampler2D textureSampler;

    smooth in vec2 fragmentUv;
    out vec4 fragmentColor;

    void main() {
        fragmentColor = texture(textureSampler, fragmentUv);
    }
#endif
