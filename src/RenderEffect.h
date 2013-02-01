/*
 * Copyright (c) 2013 Pavlo Lavrenenko
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef RENDEREFFECT_H
#define	RENDEREFFECT_H

#include "Mat4.h"
#include "Vec3.h"

#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL_opengl.h>
#include <vector>
#include <string>

namespace PolandBall {

class RenderEffect {
public:
    enum ShaderType {
        TYPE_VERTEX = GL_VERTEX_SHADER,
        TYPE_FRAGMENT = GL_FRAGMENT_SHADER
    };

    RenderEffect();
    ~RenderEffect();

    void setMVP(const Math::Mat4& matrix);
    void setLW(const Math::Mat4& matrix);  // local -> world space transformation matrix

    void attachShader(const std::string& source, ShaderType type);
    void enable();

private:
    RenderEffect(const RenderEffect& orig) = delete;
    RenderEffect& operator =(const RenderEffect&) = delete;

    std::vector<GLuint> shaderList;

    GLuint program;

    GLint mvp;  // vertex shader
    GLint lw;   // vertex shader
};

} // namespace PolandBall

#endif  // RENDEREFFECT_H
