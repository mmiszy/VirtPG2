#pragma once
#include <cinder/Vector.h>
#include "helpers/make_unique.hpp"
#include <map>
#include <boost/range/adaptor/map.hpp>
#include <boost/optional.hpp>
using boost::adaptors::map_values;

#include "Network.hpp"

// TEMP should be only in drawable
#include <cinder/gl/gl.h>
namespace gl = ci::gl;

class Terrain {
public:
    float sample(ci::Vec2f) { return 42.f; }
};

class AbstractUnit {
public:
    virtual void update(boost::optional<Packet const&>) = 0;
    virtual void draw() = 0;
};

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
            unit->update(boost::none);
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


