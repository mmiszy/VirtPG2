#pragma once
#include <cinder/Vector.h>
#include <cinder/Quaternion.h>
#include <cinder/Rand.h>

#include "helpers/make_unique.hpp"

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


class Terrain {
public:
    float sample(ci::Vec2f) { return 42.f; }
};

class World;

class AbstractUnit {
public:
    virtual void update() = 0;
    virtual void draw() = 0;
};

#include <map>
#include <boost/range/adaptor/map.hpp>
using boost::adaptors::map_values;

using UnitID = unsigned int;

class World {
    std::map<UnitID, std::unique_ptr<AbstractUnit>> units;
public:
    class TerrainContext {
        Terrain& terrain;

    public:
        virtual float sample(ci::Vec2f p) const {
            return terrain.sample(std::forward<ci::Vec2f>(p));
        }
        TerrainContext(World& w) :
            terrain(w.terrain)
        { }
    };

    template<typename... Contexts>
    struct Context : public Contexts... {
        Context(World& w)
            : Contexts(w)...
        {
        }
    };

    Terrain terrain;

    void update() {
        for (auto& unit : units | map_values) {
            unit->update();
        }
    }

    void draw() {
        gl::color(1.0f, 0.5f, 0.25f);
        glPointSize(5.0f);
        for (auto& unit : units | map_values) {
            unit->draw();
        }
    }

    template<class UnitT, typename... Args>
    void addObject(Args&&... args) {
        units.insert(std::make_pair(
                                    units.size(), 
                                    std::make_unique<UnitT>(*this, std::forward<Args>(args)...)
                                   )
                    );
    }
};

class Unit : public AbstractUnit {
    std::shared_ptr<Drawable> drawable;

    World::Context<World::TerrainContext> context;

public:
    ci::Vec2f position;

    virtual void update() {
        auto height = context.sample(position);
        position += ci::randVec2f();
    }

    virtual void draw() {
        drawable->draw(position);
    }

    Unit(World& w, std::shared_ptr<Drawable> drawable) :
        context(w)
        , drawable(std::move(drawable))
        , position(ci::Vec2f(ci::randFloat() * 300.f + 200.f,
        ci::randFloat() * 300.f + 200.f))
    {
    }
};
