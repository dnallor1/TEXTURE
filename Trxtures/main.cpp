#include <iostream>
#include <memory>
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class CustomRectangleShape : public sf::RectangleShape {
public:
    CustomRectangleShape(const sf::Vector2f &size, const sf::Vector2f &position) : sf::RectangleShape(size)
    {
        setPosition(position);
    }
    void setSpeed(const int& x_speed, const int& y_speed) {
        x_speed_ = x_speed;
        y_speed_ = y_speed;
    }
    void animate(const sf::Time &elapsed){
        bouncce();
        float dt = elapsed.asSeconds();
        move(x_speed_*dt,y_speed_*dt);
        rotate(ro_speed_*dt);
    }
    void setBounds(const float& l_bound, const float& r_bound,const float& u_bound,const float& d_bound){
        l_bound_  = l_bound  ;
        r_bound_  = r_bound  ;
        u_bound_  = u_bound  ;
        d_bound_  = d_bound  ;
    }
    void moveInDirection(float dt, const sf::Keyboard::Key &key) {

        sf::FloatRect bounds = getGlobalBounds();
        if (key == sf::Keyboard::Up and bounds.top > 0) {
            bouncce();
            move(0, -y_speed_ * dt);
        } else if (key == sf::Keyboard::Down and
                   bounds.top + bounds.height > window_size.y) {
            bouncce();
            move(0, y_speed_ * dt);
        } else if (key == sf::Keyboard::Left and bounds.left > 0) {
            bouncce();
            move(-x_speed_ * dt, 0);
        } else if (key == sf::Keyboard::Right and
                   bounds.left + bounds.width > window_size.x) {
            bouncce();
            move(x_speed_ * dt, 0);
        }
    }
    bool isClicked(const sf::Vector2i &mouse_position) {
        sf::FloatRect bounds = getGlobalBounds();
        if(mouse_position.x >= bounds.left && mouse_position.x <= bounds.left + bounds.width
                && mouse_position.y >= bounds.top && mouse_position.y <= bounds.top + bounds.height) {
            return true;
        }
        return false;
    }
private:
    int x_speed_ = 0 ;
    int y_speed_ = 0 ;
    int ro_speed_ = 0 ;
    float l_bound_ = 0;
    float r_bound_ = 0;
    float u_bound_ = 0;
    float d_bound_ = 0;
    sf::Vector2u window_size;

    void bouncce(){
        sf::FloatRect rectangle_bounds = getGlobalBounds();

        if(rectangle_bounds.top <= u_bound_){
            y_speed_ = abs(y_speed_);
        }

        if(rectangle_bounds.top + rectangle_bounds.height >= d_bound_){
            y_speed_ = abs(y_speed_) * -1;
            setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
        }

        if(rectangle_bounds.left <= l_bound_ ){
           x_speed_ = abs(x_speed_);
           setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
        }

        if(rectangle_bounds.left + rectangle_bounds.width >= r_bound_){
            x_speed_ = abs(x_speed_) * -1;
            setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
        }
    }
};

int main() {
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    std::vector<std::unique_ptr<sf::Drawable>> shapes;
    sf::Vector2f size(120.0, 60.0);
    sf::Vector2f position(120.0, 60.0);
    // create some shapes
    CustomRectangleShape rectangle(size, position);
    rectangle.setPosition(500.0, 400.0);
    rectangle.setFillColor(sf::Color(100, 50, 250));
    rectangle.setSpeed(100, 150);

    sf::Texture texture_guy;
    if(!texture_guy.loadFromFile("guy.png")) { return 1; }
    sf::Sprite guy;
    guy.setTexture(texture_guy);
    guy.setTextureRect(sf::IntRect(10, 20, 20, 15)); //left, top, width, height
//    guy.setSpeed(100, 150);

    sf::Clock clock;
    int rectangle_velocity_x = 5;
    int rectangle_velocity_y = 15;
    bool flag_y = false;
    bool flag_x = false;
    // run the program as long as the window is open
    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();
//        move(x_speed_*dt,y_speed_*dt);
//        guy.move(velocity_x * dt, velocity_y * dt);
//        guy.animate(elapsed);
        rectangle.setBounds(0, window.getSize().x, 0, window.getSize().y);
        sf::FloatRect rectangle_bounds = rectangle.getGlobalBounds();
        if(rectangle_bounds.top<=0 || rectangle_bounds.top+rectangle_bounds.height>=window.getSize().y)
            {
                if(flag_y != true)
                {
                    rectangle_velocity_y *= -1;
                    rectangle.setFillColor(sf::Color(rand() % 256,
                                                     rand() % 256,
                                                     rand() % 256));
                }
                flag_y = true;
            }
            else
                flag_y = false;
            if(rectangle_bounds.left<=0 || rectangle_bounds.left+rectangle_bounds.width>=window.getSize().x)
            {
                if(flag_x!=true)
                {
                    rectangle_velocity_x *= -1;
                    rectangle.setFillColor(sf::Color(rand() % 256,
                                                     rand() % 256,
                                                     rand() % 256));
                }
                flag_x = true;
            }
            else
                flag_x = false;
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if(event.type == sf::Event::KeyPressed) {
//            sf::Sprite move = moveInDirection(dt);
                rectangle.moveInDirection(dt, event.key.code);
//                guy.moveInDirection(dt, event.key.code);
//            guy.move(velocity_x * dt, velocity_y * dt);
                guy.move(dt, event.key.code);
        }
        // clear the window with black color
        window.clear(sf::Color::Black);

        sf::Texture texture_grass;
        if (!texture_grass.loadFromFile("grass.png")) { return 1; }
        texture_grass.setRepeated(true);
        sf::Sprite grass;
        grass.setTexture(texture_grass);
        grass.setTextureRect(sf::IntRect(0, 0, 800, 600));

        sf::Texture texture_wall;
        if(!texture_wall.loadFromFile("wall.png")) { return 1; }
        texture_wall.setRepeated(true);
        sf::Sprite wall;
        wall.setTexture(texture_wall);
        wall.setScale(0.9, 0.9);
        wall.setPosition(50.0, 100.0);
        wall.setTextureRect(sf::IntRect(0, 0, 50, 300));

        sf::Sprite wall2;
        wall2.setTexture(texture_wall);
        wall2.setScale(0.9, 0.9);
        wall2.setPosition(175.0, 150.0);
        wall2.setTextureRect(sf::IntRect(0, 0, 50, 400));

        sf::Sprite wall3;
        wall3.setTexture(texture_wall);
        wall3.setScale(0.9, 0.9);
        wall3.setPosition(550.0, 100.0);
        wall3.setTextureRect(sf::IntRect(0, 0, 50, 400));

        sf::Sprite wall4;
        wall4.setTexture(texture_wall);
        wall4.setScale(0.9, 0.9);
        wall4.setPosition(275.0, 75.0);
        wall4.setTextureRect(sf::IntRect(0, 0, 200, 50));

        sf::Sprite wall5;
        wall5.setTexture(texture_wall);
        wall5.setScale(0.9, 0.9);
        wall5.setPosition(550.0, 350.0);
        wall5.setTextureRect(sf::IntRect(0, 0, 200, 50));

        // draw everything here...
        window.draw(grass);
        window.draw(wall);
        window.draw(wall2);
        window.draw(wall3);
        window.draw(wall4);
        window.draw(wall5);
        window.draw(guy);
        window.draw(rectangle);
        // end the current frame
        window.display();
    }
    return 0;
}
