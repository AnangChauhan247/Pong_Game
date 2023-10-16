#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <ctime>
#include <cstdlib>
using namespace std;
using namespace sf;
class Player {
private:
    RectangleShape paddle;
    float speed;

public:
    Player(float x, float y, float width, float height, Color color, float playerSpeed)
        : speed(playerSpeed) {
        paddle.setSize(Vector2f(width, height));
        paddle.setPosition(x, y);
        paddle.setFillColor(color);
    }

    void moveUp(float deltaTime) {
        if (paddle.getPosition().y > 0) {
            paddle.move(0.f, -speed * deltaTime);
        }
    }

    void moveDown(float deltaTime, float windowHeight) {
        if (paddle.getPosition().y + paddle.getSize().y < windowHeight) {
            paddle.move(0.f, speed * deltaTime);
        }
    }

    RectangleShape& getPaddle() {
        return paddle;
    }
};

class Ball {
private:
    CircleShape ball;
    Vector2f velocity;
                
public:
    Ball(float x, float y, float radius, Color color, Vector2f playerLeftPosition, Vector2f playerRightPosition)
        : velocity(1.0f,1.0f) {
        ball.setRadius(radius);
        ball.setPosition(x, y);
        ball.setFillColor(color);
        int randomDirection = std::rand() % 4; // Values 0 to 3
        switch (randomDirection) {
        case 0: // Move left and up
            velocity.x = -5.0f;
            velocity.y = -5.0f;
            break;
        case 1: // Move left and down
            velocity.x = -5.0f;
            velocity.y = 5.0f;
            break;
        case 2: // Move right and up
            velocity.x = 5.0f;
            velocity.y = -5.0f;
            break;
        case 3: // Move right and down
            velocity.x = 5.0f;
            velocity.y = 5.0f;
            break;
        default:
            break;
        }

    }

    void move(float deltaTime) {
        ball.move(velocity * deltaTime);
    }

    CircleShape& getBall() {
        return ball;
    }

    void reverseDirectionX() {
        velocity.x = -velocity.x;
    }

    void reverseDirectionY() {
        velocity.y = -velocity.y;
    }

    Vector2f getPosition() const {
        return ball.getPosition();
    }

    void checkBoundaryCollision(float windowHeight) {
      if (ball.getPosition().y - ball.getRadius() < 0) {
        // Reverse the Y-velocity to make it bounce down
        velocity.y = std::abs(velocity.y);
    }
    // Check if the ball hits the lower wall
    else if (ball.getPosition().y + ball.getRadius() > windowHeight) {
        // Reverse the Y-velocity to make it bounce up
        velocity.y = -std::abs(velocity.y);
      }
    }

    void adjustvelocity(float factor) {
        velocity.x *= factor;
        velocity.y *= factor;
    }



};

class Game {
private:
    RenderWindow window;
    Player playerLeft;
    Player playerRight;
    Ball ball;
    Vertex line[2];
    Clock clock;
    bool ballInPlay = false;
    int dir = 0;
    bool playerLeftMovingUp;
    bool playerLeftMovingDown;
    bool playerRightMovingUp;
    bool playerRightMovingDown;
    Font font;
    int score1;
    int score2;
    bool gameIsOver;
    Text score1Text, score2Text, winnerText;
    SoundBuffer paddleHitBuffer;
    Sound paddleHitSound;

    void HandleKeyPress(sf::Keyboard::Key key) {
        if (key == sf::Keyboard::W) {
            playerLeftMovingUp = true;
        }
        else if (key == sf::Keyboard::S) {
            playerLeftMovingDown = true;
        }
        if (key == sf::Keyboard::Up) {
            playerRightMovingUp = true;
        }
        else if (key == sf::Keyboard::Down) {
            playerRightMovingDown = true;
        }
    }

    void HandleKeyRelease(sf::Keyboard::Key key) {
        if (key == sf::Keyboard::W) {
            playerLeftMovingUp = false;
        }
        else if (key == sf::Keyboard::S) {
            playerLeftMovingDown = false;
        }
        if (key == sf::Keyboard::Up) {
            playerRightMovingUp = false;
        }
        else if (key == sf::Keyboard::Down) {
            playerRightMovingDown = false;
        }
    }


public:
    Game()
        : window(VideoMode(1340, 990), "OGame-1", Style::Default),
        playerLeft(160.f, 495.f, 20.f, 80.f, Color::Blue, 500.0f),
        playerRight(1180.f, 495.f, 20.f, 80.f, Color::Red, 500.0f),
        ball(670.f, 495.f, 11.f, Color::Green, playerLeft.getPaddle().getPosition(), playerRight.getPaddle().getPosition()),score1(0),score2(0), gameIsOver(false){
        window.setFramerateLimit(60);
        line[0] = Vector2f(670.f, 0.f);
        line[1] = Vector2f(670.f, 990.f);
        playerLeftMovingUp = false;
        playerLeftMovingDown = false;
        playerRightMovingUp = false;
        playerRightMovingDown = false;


        if (!font.loadFromFile("Fonts/OpenSans-Light.ttf"))
        {
            cerr << "Error: Could not load font!" << endl;
            throw runtime_error("Font loading error");
        }
        score1Text.setFont(font);
        score1Text.setCharacterSize(34);
        score1Text.setStyle(Text::Bold);

        score2Text.setFont(font);
        score2Text.setCharacterSize(34);
        score2Text.setStyle(Text::Bold);
        score2Text.setPosition(680.f, 0.f);

        winnerText.setFont(font); // Winner text
        winnerText.setCharacterSize(48);
        winnerText.setStyle(Text::Bold);
        winnerText.setFillColor(Color::White);
        winnerText.setPosition(600.f, 420.f);
        winnerText.setString("");
        if (!paddleHitBuffer.loadFromFile("Audio/hit.wav")) {
            cerr << "Failed to load paddle hit sound!" << std::endl;
        }
        paddleHitSound.setBuffer(paddleHitBuffer);
    }

    void run() {
        while (window.isOpen()) {
            Event event;
            float deltaTime = clock.restart().asSeconds();
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }

                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                    window.close();
                }

                if (event.type == Event::KeyPressed) {
                    HandleKeyPress(event.key.code);
                }
                else if (event.type == Event::KeyReleased) {
                    HandleKeyRelease(event.key.code);
                }
            }

            Update(deltaTime);
            Draw();
        }
    }

    void Update(float deltaTime) {
        if (!gameIsOver) {
            ball.checkBoundaryCollision(window.getSize().y);
            ball.move(0.9f);

            // Player movement
            // To Update player paddle movement based on key presses
            if (playerLeftMovingUp) {
                playerLeft.moveUp(deltaTime);
            }
            if (playerLeftMovingDown) {
                playerLeft.moveDown(deltaTime, window.getSize().y);
            }
            if (playerRightMovingUp) {
                playerRight.moveUp(deltaTime);
            }
            if (playerRightMovingDown) {
                playerRight.moveDown(deltaTime, window.getSize().y);
            }

            // To Check if the ball touches the left wall (player 2 scores)
            if (ball.getPosition().x < 0) {
                score2++;
                if (score2 >= 5) {
                    gameIsOver = true;
                    winnerText.setString("Player 2 wins!");
                }
                // Reset the ball's position to the center
                ball.getBall().setPosition(670.f, 495.f);
                // Randomize the ball's initial direction
                int randomDirection = std::rand() % 4;
            }

            // To Check if the ball touches the right wall (player 1 scores)
            if (ball.getPosition().x > window.getSize().x) {
                score1++;
                if (score1 >= 5) {
                    gameIsOver = true;
                    winnerText.setString("Player 1 wins!");
                }
                // Reset the ball's position to the center
                ball.getBall().setPosition(670.f, 495.f);
                // Randomize the ball's initial direction
                int randomDirection = std::rand() % 4;
            }

            // To Handle ball collision with players
            if (ball.getBall().getGlobalBounds().intersects(playerLeft.getPaddle().getGlobalBounds())) {
                // Ball was hit by playerLeft
                // Reverse the ball's x-direction
                ball.reverseDirectionX();
                // To Modify the ball's velocity to adjust its speed
                ball.adjustvelocity(1.0f); // Increase the speed by a factor
                paddleHitSound.play();
            }

            if (ball.getBall().getGlobalBounds().intersects(playerRight.getPaddle().getGlobalBounds())) {
                // Ball was hit by playerRight
                // Reverse the ball's x-direction
                ball.reverseDirectionX();
                // Modify the ball's velocity to adjust its speed
                ball.adjustvelocity(1.0f); // Increase the speed by a factor
                paddleHitSound.play();
            }

            std::ostringstream scoreStream1, scoreStream2;
            scoreStream1 << "Player 1: " << score1;
            score1Text.setString(scoreStream1.str());

            scoreStream2 << "Player 2: " << score2;
            score2Text.setString(scoreStream2.str());
        }

    }


    void Draw() {
        window.clear(Color::Black);

        if (!gameIsOver) {
            // Draw game entities (paddles, ball, center line)
            window.draw(playerLeft.getPaddle());
            window.draw(ball.getBall());
            window.draw(playerRight.getPaddle());
            window.draw(line, 2, Lines);
        }
        else {
            // Display winner text
            window.draw(winnerText);
        }

        // Draw score text
        window.draw(score1Text);
        window.draw(score2Text);

        window.display();
    }

};

int main() {    
    std::srand(std::time(0)); // Seed the random number generator

    Game game;
    game.run();

    return 0;
}

