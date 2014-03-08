#pragma once
#include "World.hpp"
#include "Graphics.hpp"
#include <cinder/Vector.h>
#include <cinder/Rand.h>

class Unit : public AbstractUnit {
    std::shared_ptr<Drawable> drawable;

    World::Context<World::TerrainContext> context;

public:
    ci::Vec2f position;

    virtual void update(boost::optional<Packet const&> packet) {
        if (packet) {
            // auto frame = UnitFrame(packet)
            // if frame ok use frame state to update
        }
        else {
            auto height = context.sample(position);
            position += ci::randVec2f();
        }
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