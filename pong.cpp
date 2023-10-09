#include<iostream>
#include"SFML/System.hpp"
#include"SFML/Graphics.hpp"
#include"SFML/Window.hpp"
#include"SFML/Audio.hpp"
#include"SFML/Network.hpp"
#include <sstream>
using namespace std;
int score1Val = 0;
int score2Val = 0;
enum eDir { STOP = 0, LEFT = 1, UPLEFT = 2, DOWNLEFT = 3, RIGHT = 4, UPRIGHT = 5, DOWNRIGHT = 6 };
using namespace sf;
void Update(RectangleShape& playerLeft, RectangleShape& playerRight, RenderWindow& window, CircleShape& ball, Vertex line[], int& dir, bool& ballInPlay, bool& ballDirectionUp, float& playerSpeed, float& deltaTime);
void Draw(RenderWindow& window, RectangleShape& playerLeft, RectangleShape& playerRight, CircleShape& ball, Vertex line[], Text& score1, Text& score2);
int main()
{
    int KeyTime = 1;
    int dir = (rand() % 6) + 1;
    bool ballInPlay = false; // Initialize to false since the game hasn't started yet
    bool ballDirectionUp = rand() % 2 == 0; // Initialize to true, assuming the ball starts moving upwards

    RenderWindow window(VideoMode(1340, 990), "pong", Style::Default);
    window.setFramerateLimit(60);
    RectangleShape playerRight(Vector2f(20.f, 80.f));
    playerRight.setFillColor(Color::Red);
    playerRight.setPosition(window.getSize().x / 2 + 510.f, window.getSize().y / 2);

    RectangleShape playerLeft(Vector2f(20.f, 80.f));
    playerLeft.setFillColor(Color::Blue);
    playerLeft.setPosition(window.getSize().x / 2 - 530.f, window.getSize().y / 2);

    CircleShape ball(11.f);
    ball.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    ball.setFillColor(Color::Green);

    Vertex line[] = { Vector2f(670.f,0.f),Vector2f(670.f,990.f) };
    Clock clock;
    float deltaTime = 0.0f;
    float playerSpeed = 200.0f; // Adjust the player's movement speed here

    Font font;
    if (!font.loadFromFile("Fonts/OpenSans-Regular.ttf"))
        throw("Could Not Load Font");

    Text score1, score2, winnerText;
    score1.setFont(font);
    score1.setCharacterSize(34);
    score1.setStyle(Text::Bold);

    score2.setFont(font);
    score2.setCharacterSize(34);
    score2.setStyle(Text::Bold);
    score2.setPosition(680.f, 0.f);

    winnerText.setFont(font); // Winner text
    winnerText.setCharacterSize(48);
    winnerText.setStyle(Text::Bold);
    winnerText.setFillColor(Color::White);
    winnerText.setString("");
    while (window.isOpen())
    {
        Event event;
        deltaTime = clock.restart().asSeconds(); // Get the time elapsed since the last frame
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            if (event.KeyPressed && event.key.code == Keyboard::Escape)
            {
                window.close();
            }
        }

        // Check if either player wins (reaches a score of 2)
        if (score1Val == 2 || score2Val == 2)
        {
            if (score1Val == 2)
            {
                winnerText.setString("Player 1 Wins!");
            }
            else
            {
                winnerText.setString("Player 2 Wins!");
            }

            // Display the winner and stop the game
            window.clear(Color::Black);
            window.draw(winnerText);
            window.display();
            sf::sleep(sf::seconds(3)); // Pause for 3 seconds
            window.close();
            return 0; // Exit the game
        }
        //update (can make a function to update nd make a call from here)
        Update(playerLeft, playerRight, window, ball, line, dir, ballInPlay, ballDirectionUp, playerSpeed, deltaTime);
        //draw
        Draw(window, playerLeft, playerRight, ball, line, score1, score2);
    }
    return 0;
}

void Update(RectangleShape& playerLeft, RectangleShape& playerRight, RenderWindow& window, CircleShape& ball, Vertex line[], int& dir, bool& ballInPlay, bool& ballDirectionUp, float& playerSpeed, float& deltaTime)
{

    // Ball's current position
    float ballX = ball.getPosition().x;
    float ballY = ball.getPosition().y;

    if (!ballInPlay)
    {
        // Randomly choose the initial direction for the ball
        dir = (rand() % 6) + 1;

        // Randomly choose which player starts with the ball
        if (rand() % 2 == 0)
        {
            ball.setPosition(playerLeft.getPosition().x + playerLeft.getSize().x + ball.getRadius() + 1, window.getSize().y / 2);
            ballDirectionUp = true; // Start with the ball moving upwards
        }
        else
        {
            ball.setPosition(playerRight.getPosition().x - ball.getRadius() - 1, window.getSize().y / 2);
            ballDirectionUp = false; // Start with the ball moving downwards
        }

        ballInPlay = true;
    }

    // Check for collision with the upper wall
    if (ballY - ball.getRadius() <= 0)
    {
        ballDirectionUp = false; // Reverse the vertical direction to move downwards
        dir = static_cast<eDir>((dir + 1) % 7); // Reverse the horizontal direction
    }

    // Check for collision with the lower wall
    if (ballY + ball.getRadius() >= window.getSize().y)
    {
        ballDirectionUp = true; // Reverse the vertical direction to move upwards
        dir = static_cast<eDir>((dir + 1) % 7); // Reverse the horizontal direction
    }

    // Move the ball based on the current directions
    if (ballDirectionUp)
    {
        // Vertical motion
        ball.move(0.f, -5.0f);
    }
    else
    {
        // Vertical motion
        ball.move(0.f, 5.0f);
    }

    // Horizontal motion
    switch (dir)
    {
    case STOP:
        break;
    case LEFT:
        ball.move(-5.0f, 0.f);
        break;
    case RIGHT:
        ball.move(5.f, 0.f);
        break;
    case UPLEFT:
        ball.move(-5.f, -5.f);
        break;
    case DOWNLEFT:
        ball.move(-5.f, 5.f);
        break;
    case UPRIGHT:
        ball.move(5.f, -5.f);
        break;
    case DOWNRIGHT:
        ball.move(5.f, 5.f);
        break;
    default:
        break;
    }

    // Handle player movements
    if (Keyboard::isKeyPressed(Keyboard::W) && playerLeft.getPosition().y > 0)
    {
        playerLeft.move(0.f, -playerSpeed * deltaTime);
    }
    if (Keyboard::isKeyPressed(Keyboard::S) && playerLeft.getPosition().y + playerLeft.getSize().y < window.getSize().y)
    {
        playerLeft.move(0.f, playerSpeed * deltaTime);
    }
    if (Keyboard::isKeyPressed(Keyboard::Up) && playerRight.getPosition().y > 0)
    {
        playerRight.move(0.f, -playerSpeed * deltaTime);
    }
    if (Keyboard::isKeyPressed(Keyboard::Down) && playerRight.getPosition().y + playerRight.getSize().y < window.getSize().y)
    {
        playerRight.move(0.f, playerSpeed * deltaTime);
    }

    // Check if the ball hits either player
    if (ball.getGlobalBounds().intersects(playerLeft.getGlobalBounds()) || ball.getGlobalBounds().intersects(playerRight.getGlobalBounds()))
    {
        dir = static_cast<eDir>((dir + 1) % 7); // Reverse the horizontal direction
    }

    // Check if the ball went out of bounds (scored)
    if (ballX < 0 || ballX > window.getSize().x)
    {
        if (ballX < 0)
            score2Val++;
        else if (ballX > window.getSize().x)
            score1Val++;
        ballInPlay = false; // Reset the ball state

        // Reset the ball's position to the center
        ball.setPosition(window.getSize().x / 2, window.getSize().y / 2);

        // Randomly choose the initial direction for the ball again
        dir = (rand() % 6) + 1;

        // Randomly choose which player starts with the ball
        if (rand() % 2 == 0)
        {
            ball.setPosition(playerLeft.getPosition().x + playerLeft.getSize().x + ball.getRadius() + 1, window.getSize().y / 2);
            ballDirectionUp = true; // Start with the ball moving upwards
        }
        else
        {
            ball.setPosition(playerRight.getPosition().x - ball.getRadius() - 1, window.getSize().y / 2);
            ballDirectionUp = false; // Start with the ball moving downwards
        }
        ballInPlay = true;
    }
}


void Draw(RenderWindow& window, RectangleShape& playerLeft, RectangleShape& playerRight, CircleShape& ball, Vertex line[], Text& score1, Text& score2)
{
    window.clear(Color::Black);

    //Draw
    window.draw(playerLeft);
    window.draw(ball);
    window.draw(playerRight);
    window.draw(line, 2, Lines);
    stringstream ss1, ss2;
    ss1 << "P1: " << score1Val;
    score1.setString(ss1.str());

    ss2 << "P2: " << score2Val;
    score2.setString(ss2.str());
    window.draw(score1);
    window.draw(score2);
    window.display();
}