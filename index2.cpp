#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

enum class Direction { STOP, LEFT, UPLEFT, DOWNLEFT, RIGHT, UPRIGHT, DOWNRIGHT };

using namespace sf;
using namespace std;

class Paddle {
public:
    Paddle(float x, float y) : shape(Vector2f(20.f, 80.f)) {
        shape.setPosition(x, y);
    }

    void moveUp() {
        if (shape.getPosition().y > 0)
            shape.move(0.f, -5.0f);
    }

    void moveDown() {
        if (shape.getPosition().y + shape.getSize().y < window.getSize().y)
            shape.move(0.f, 5.0f);
    }

    void draw(RenderWindow& window) {
        window.draw(shape);
    }

    FloatRect getGlobalBounds() const {
        return shape.getGlobalBounds();
    }

private:
    RectangleShape shape;
};

class Ball {
public:
    Ball(float x, float y) : shape(11.f), direction(Direction::STOP) {
        shape.setPosition(x, y);
    }

    void reset() {
        direction = Direction::STOP;
        shape.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    }

    void update() {
        // Ball's current position
        float ballX = shape.getPosition().x;
        float ballY = shape.getPosition().y;

        // Handle ball movement and collision logic here...

        // Check if the ball hits the top wall
        if (ballY - shape.getRadius() <= 0) {
            // Reverse the vertical direction to move downwards
            direction = static_cast<Direction>((static_cast<int>(direction) + 6) % 7);
        }

        // Check if the ball hits the bottom wall
        if (ballY + shape.getRadius() >= window.getSize().y) {
            // Reverse the vertical direction to move upwards
            direction = static_cast<Direction>((static_cast<int>(direction) + 6) % 7);
        }

        // Move the ball based on the current directions
        switch (direction) {
        case Direction::LEFT:
            shape.move(-5.0f, 0.f);
            break;
        case Direction::RIGHT:
            shape.move(5.0f, 0.f);
            break;
        case Direction::UPLEFT:
            shape.move(-5.0f, -5.0f);
            break;
        case Direction::DOWNLEFT:
            shape.move(-5.0f, 5.0f);
            break;
        case Direction::UPRIGHT:
            shape.move(5.0f, -5.0f);
            break;
        case Direction::DOWNRIGHT:
            shape.move(5.0f, 5.0f);
            break;
        default:
            break;
        }

        // Check if the ball hits the paddles
        if (shape.getGlobalBounds().intersects(leftPaddle.getGlobalBounds())) {
            // Reverse the horizontal direction
            direction = Direction::RIGHT;
        }
        else if (shape.getGlobalBounds().intersects(rightPaddle.getGlobalBounds())) {
            // Reverse the horizontal direction
            direction = Direction::LEFT;
        }

        // Check if the ball went out of bounds (scored)
        if (ballX < 0) {
            // Player on the right scores a point
            rightScore++;
            cout << "Right Player Score: " << rightScore << endl;
            reset();
        }
        else if (ballX > window.getSize().x) {
            // Player on the left scores a point
            leftScore++;
            cout << "Left Player Score: " << leftScore << endl;
            reset();
        }

        // Check if the game is over
        if (leftScore >= 5 || rightScore >= 5) {
            cout << "Game Over" << endl;
            window.close();
        }
    }

    void draw(RenderWindow& window) {
        window.draw(shape);
    }

private:
    CircleShape shape;
    Direction direction;
};

class Game {
public:
    Game() : window(VideoMode(1340, 990), "index2", Style::Default),
        leftPaddle(50.f, window.getSize().y / 2),
        rightPaddle(window.getSize().x - 70.f, window.getSize().y / 2),
        ball(window.getSize().x / 2, window.getSize().y / 2),
        leftScore(0),
        rightScore(0) {
        window.setFramerateLimit(60);

        // Initialize font and text for displaying scores
        if (!font.loadFromFile("arial.ttf")) {
            cerr << "Error loading font." << endl;
        }

        leftScoreText.setFont(font);
        leftScoreText.setCharacterSize(36);
        leftScoreText.setFillColor(Color::Black);
        leftScoreText.setPosition(50, 10);

        rightScoreText.setFont(font);
        rightScoreText.setCharacterSize(36);
        rightScoreText.setFillColor(Color::White);
        rightScoreText.setPosition(window.getSize().x - 100, 10);
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
            draw();
        }
    }

private:
    RenderWindow window;
    Paddle leftPaddle;
    Paddle rightPaddle;
    Ball ball;
    int leftScore;
    int rightScore;
    Font font;
    Text leftScoreText;
    Text rightScoreText;

    void handleEvents() {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed || (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) {
                window.close();
            }
        }

        // Handle user input for paddles
        if (Keyboard::isKeyPressed(Keyboard::W)) {
            leftPaddle.moveUp();
        }
        if (Keyboard::isKeyPressed(Keyboard::S)) {
            leftPaddle.moveDown();
        }
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            rightPaddle.moveUp();
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            rightPaddle.moveDown();
        }
    }

    void update() {
        // Update game logic here...
        ball.update();

        // Update the score text
        leftScoreText.setString("Left Player: " + to_string(leftScore));
        rightScoreText.setString("Right Player: " + to_string(rightScore));
    }

    void draw() {
        window.clear(Color::Black);

        // Draw game objects
        leftPaddle.draw(window);
        rightPaddle.draw(window);
        ball.draw(window);

        // Draw the score text
        window.draw(leftScoreText);
        window.draw(rightScoreText);

        window.display();
    }
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    Game game;
    game.run();

    return 0;
}
