#include <cinder/app/AppBasic.h>
#include <list>
#include <cinder/Channel.h>

#include "World.hpp"

#include <map>
#include <string>

using namespace ci::app;

// We'll create a new Cinder Application by deriving from the AppBasic class
class BasicApp : public AppBasic {
  public:
    void mouseDrag(MouseEvent event);
    void keyDown(KeyEvent event);
    void draw();

    void update() {
        world.update();
    }

    // This will maintain a list of points which we will draw line segments between
    World world;

    std::map<std::string, std::shared_ptr<Drawable>> drawables;

    BasicApp() {
        drawables["point"] = std::make_shared<DrawablePoint>();
    }
};

void BasicApp::mouseDrag(MouseEvent event)
{
    world.addObject<Unit>(drawables["point"]);
}

void BasicApp::keyDown(KeyEvent event)
{
    if(event.getChar() == 'f')
        setFullScreen( ! isFullScreen());
}

void BasicApp::draw()
{
    gl::clear(ci::Color(0.1f, 0.1f, 0.15f));
    world.draw();
}

// This line tells Cinder to actually create the application
CINDER_APP_BASIC(BasicApp, RendererGl)