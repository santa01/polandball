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

#include "RenderEffect.h"
#include "ShaderLoader.h"

namespace PolandBall {

RenderEffect::RenderEffect() {
    this->program = 0;
}

RenderEffect::~RenderEffect() {
    if (this->program != 0) {
        glDeleteProgram(this->program);
    }
}

void RenderEffect::setMVP(const Math::Mat4& matrix) {
    this->enable();

    if (this->mvp > -1) {
        glUniformMatrix4fv(this->mvp, 1, GL_TRUE, (GLfloat*)matrix.data());
    }
}

void RenderEffect::setLW(const Math::Mat4& matrix) {
    this->enable();
    
    if (this->lw > -1) {
        glUniformMatrix4fv(this->lw, 1, GL_TRUE, (GLfloat*)matrix.data());
    }
}

void RenderEffect::attachShader(const std::string& source, ShaderType type) {
    if (this->program == 0) {
        this->shaderList.push_back(ShaderLoader::createShader(source, type));
    }
}

void RenderEffect::enable() {
    if (this->program == 0) {
        this->program = ShaderLoader::createProgram(this->shaderList);
        glUseProgram(this->program);

        this->mvp = glGetUniformLocation(this->program, "mvp");
        this->lw = glGetUniformLocation(this->program, "lw");

        GLint samplerLocation = glGetUniformLocation(this->program, "textureSampler");
        if (samplerLocation > -1) {
            glUniform1i(samplerLocation, 0);
        }
    } else {
        glUseProgram(this->program);
    }
}

}  // namespace PolandBall
