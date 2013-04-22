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

#ifndef TRANSFORMABLE_H
#define TRANSFORMABLE_H

namespace PolandBall {

class Transformable {
public:
    virtual ~Transformable() {}

    virtual void replicateX(float factor) = 0;
    virtual void replicateY(float factor) = 0;
    virtual void replicateZ(float factor) = 0;

    virtual float getXReplicaFactor() const = 0;
    virtual float getYReplicaFactor() const = 0;
    virtual float getZReplicaFactor() const = 0;

    void replicate(float factor) {
        this->replicateX(factor);
        this->replicateY(factor);
        this->replicateZ(factor);
    }

    virtual void shearX(int slice, int totalSlices) = 0;
    virtual void shearY(int slice, int totalSlices) = 0;
    virtual void shearZ(int slice, int totalSlices) = 0;

    virtual void getXShearFactor(int& slice, int& totalSlices) const = 0;
    virtual void getYShearFactor(int& slice, int& totalSlices) const = 0;
    virtual void getZShearFactor(int& slice, int& totalSlices) const = 0;

    void shear(int slice, int totalSlices) {
        this->shearX(slice, totalSlices);
        this->shearY(slice, totalSlices);
        this->shearZ(slice, totalSlices);
    }
};

}  // namespace PolandBall

#endif  // TRANSFORMABLE_H
