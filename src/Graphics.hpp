#pragma once
#include <cinder/Vector.h>
#include <cinder/gl/gl.h>
namespace gl = ci::gl;

class Drawable {
public:
    virtual void draw(ci::Vec2f position) = 0;
};

class DrawableWithColor : public Drawable {
public:
    virtual void setColor(ci::Color color) = 0;
};

class DrawablePoint : public Drawable {
public:
    virtual void draw(ci::Vec2f position) {
        gl::begin(GL_POINTS);
        gl::vertex(position);
        gl::end();
    }
};
