#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>

using namespace sf;

// Game Window Size
const int WIDTH = 600, HEIGHT = 600;

// Pac-Man Variables
CircleShape pacman(15);
Vector2f pacmanVelocity(0.f, 0.f);
float pacmanSpeed = 3.0f;

// Ghost Variables
CircleShape ghost(15);
Vector2f ghostVelocity(2.f, 0.f);

// Pellet (Food) Variables
std::vector<CircleShape> pellets;
int score = 0;

// Function to Show Title Screen
void showTitleScreen(RenderWindow &window, Font &font) {
    Text titleText, startText;

    // Title Text
    titleText.setFont(font);
    titleText.setString("Pac-Man Game");
    titleText.setCharacterSize(50);
    titleText.setFillColor(Color::Yellow);
    titleText.setPosition(150, 100);

    // Start Instructions
    startText.setFont(font);
    startText.setString("Press ENTER to Start");
    startText.setCharacterSize(30);
    startText.setFillColor(Color::White);
    startText.setPosition(170, 300);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (Keyboard::isKeyPressed(Keyboard::Enter))
                return;
        }

        window.clear();
        window.draw(titleText);
        window.draw(startText);
        window.display();
    }
}

// Function to Move Pac-Man
void movePacman() {
    pacmanVelocity = {0.f, 0.f};

    if (Keyboard::isKeyPressed(Keyboard::Left))
        pacmanVelocity.x = -pacmanSpeed;
    if (Keyboard::isKeyPressed(Keyboard::Right))
        pacmanVelocity.x = pacmanSpeed;
    if (Keyboard::isKeyPressed(Keyboard::Up))
        pacmanVelocity.y = -pacmanSpeed;
    if (Keyboard::isKeyPressed(Keyboard::Down))
        pacmanVelocity.y = pacmanSpeed;

    pacman.move(pacmanVelocity);
}

// Function to Move Ghost (AI)
void moveGhost() {
    int dir = rand() % 4; // Random Direction

    switch (dir) {
        case 0: ghostVelocity = Vector2f(2.f, 0.f); break; // Right
        case 1: ghostVelocity = Vector2f(-2.f, 0.f); break; // Left
        case 2: ghostVelocity = Vector2f(0.f, 2.f); break; // Down
        case 3: ghostVelocity = Vector2f(0.f, -2.f); break; // Up
    }

    ghost.move(ghostVelocity);
}

// Function to Check Collision
bool checkCollision(CircleShape& a, CircleShape& b) {
    return a.getGlobalBounds().intersects(b.getGlobalBounds());
}

int main() {
    // Initialize SFML Window
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Pac-Man Game");
    window.setFramerateLimit(60);

    // Load Font
    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "Error loading font\n";
        return -1;
    }

    // Show Title Screen
    showTitleScreen(window, font);

    // Initialize Pac-Man
    pacman.setFillColor(Color::Yellow);
    pacman.setPosition(WIDTH / 2, HEIGHT / 2);

    // Initialize Ghost
    ghost.setFillColor(Color::Red);
    ghost.setPosition(WIDTH / 4, HEIGHT / 4);

    // Create Pellets (Food)
    for (int i = 0; i < 20; i++) {
        CircleShape pellet(5);
        pellet.setFillColor(Color::White);
        pellet.setPosition(rand() % (WIDTH - 10), rand() % (HEIGHT - 10));
        pellets.push_back(pellet);
    }

    // Score Display
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(10, 10);

    // Game Loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        // Move Pac-Man
        movePacman();

        // Move Ghost Randomly Every 1 Second
        if (rand() % 60 == 0)
            moveGhost();

        // Check Collision with Pellets
        for (size_t i = 0; i < pellets.size(); i++) {
            if (checkCollision(pacman, pellets[i])) {
                pellets.erase(pellets.begin() + i); // Remove Pellet
                score += 10;
                break;
            }
        }

        // Check Collision with Ghost (Game Over)
        if (checkCollision(pacman, ghost)) {
            std::cout << "Game Over! Final Score: " << score << "\n";
            window.close();
        }

        // Update Score Text
        scoreText.setString("Score: " + std::to_string(score));

        // Render Objects
        window.clear();
        window.draw(pacman);
        window.draw(ghost);
        for (auto& pellet : pellets) {
            window.draw(pellet);
        }
        window.draw(scoreText);
        window.display();
    }

    return 0;
}
