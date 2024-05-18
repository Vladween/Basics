#include "GraphicsBasics.h"
using namespace basics;

class Test : public Application
{
public:
    void init() override
    {
        createWindow({ 1000, 650 }, "Basics works!");
        clearColor = sf::Color::White;

        texture.loadFromFile("resources/AnimationTest.png");

        shape.setSize({ 100, 100 });
        shape.setPosition(100, 100);

        manager.setTarget(shape);
        manager.endInit();

        anim1.setFramesCount(5);
        anim2.setFramesCount(5);
        for (size_t i = 0; i < 5; i++)
        {
            anim1.setFrame(i, { {100 * (int)i, 0}, {101, 101} });
            anim2.setFrame(i, { {100 * (int)i, 100}, {101, 101} });
        }
        anim1.setSpeed(0.0003);
        anim2.setSpeed(0.0003);

        anim1.setTexture(&texture);
        anim2.setTexture(&texture);

        anim1.play();
    }

    void event_update() override
    {
        if (event.type == sf::Event::KeyReleased)
        {
            if (event.key.code == sf::Keyboard::R) manager.getCurrentAnimation()->stop();
            if (event.key.code == sf::Keyboard::Num1) anim1.play();
            if (event.key.code == sf::Keyboard::Num2) anim2.play();
        }
    }

    void update() override
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && is_valid(manager.getCurrentAnimation()) && !manager.getCurrentAnimation()->isPlaying())
        {
            manager.getCurrentAnimation()->play();
        }
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::P) && is_valid(manager.getCurrentAnimation()) && manager.getCurrentAnimation()->isPlaying())
        {
            manager.getCurrentAnimation()->pause();
        }
    }

    void draw() override
    {
        window.draw(shape);
    }
private:
    sf::RectangleShape shape;
    AnimationManager manager;
    Animation anim1, anim2;
    sf::Texture texture;
};
MAIN(Test)
