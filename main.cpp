#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>

std::string generateScramble() {
  std::vector<std::string> moves = {"U", "D", "L", "R", "F", "B"};
  std::vector<std::string> rotations = {"", "'", "2"};

  std::string scramble = "";
  std::string last_move = "";
  std::string banned_move = "";

  srand(time(0));

  for (int i = 0; i < 22; ++i) {
    std::string move;
    do {
      move = moves[rand() % moves.size()];
    } while ((move[0] == last_move[0]) || (move[0] == banned_move[0]));

    std::string rotation = rotations[rand() % rotations.size()];

    if (i == 21) {
      scramble += move + rotation;
    } else {
      scramble += move + rotation + " ";
    }
    banned_move = last_move;
    last_move = move;
  }

  return scramble;
}


void drawScramble(sf::RenderWindow& window, std::string scramble, sf::Font font, const int fontSize, int x, int y) {
  sf::Text text(scramble, font, fontSize);
  text.setFillColor(sf::Color (210, 210, 210));
  sf::FloatRect textBounds = text.getLocalBounds();

  text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
  text.setPosition(x / 2.0f, y / 2.0f - (fontSize + 10));
  window.draw(text);
}


void saveScramble(std::string scramble, sf::Time time) {
  std::ofstream outputFile("times.txt", std::ios::app);
  if (!outputFile.is_open()) {
    return;
  }

  outputFile << scramble << " - " << time.asMilliseconds() << "\n";

  outputFile.close();
}


void drawTimer(sf::RenderWindow& window, sf::Time& elapsedTime, sf::Time lastTime, bool timing, bool holding, sf::Time holdingFor, sf::Font font, const int fontSize, int x, int y) {
  sf::Text text("00.000", font, fontSize);
  if (holdingFor >= sf::seconds(.4)) {
    text.setFillColor(sf::Color (78, 166, 86));
  } else if (holding) {
    text.setFillColor(sf::Color (220, 50, 47));
  } else {
    text.setFillColor(sf::Color (220, 220, 220));
  }

  std::stringstream ss;
  if (timing) {
    int currentTime = elapsedTime.asSeconds();
    int minutes = currentTime / 60;
    int seconds = currentTime % 60;

    std::stringstream ss;
    if (minutes > 0) {
      ss << std::setfill('0') << std::setw(2) << minutes << ":" << std::setw(2) << seconds << "." << std::setw(2) << static_cast<int>(elapsedTime.asMilliseconds()) % 1000 / 10;
    } else {
      ss << std::setfill('0') << std::setw(2) << seconds << "." << std::setw(2) << static_cast<int>(elapsedTime.asMilliseconds()) % 1000 / 10;
    }
    text.setString(ss.str());
    text.setCharacterSize(fontSize + 40);
  } else {
    int currentTime = lastTime.asSeconds();
    int minutes = currentTime / 60;
    int seconds = currentTime % 60;

    std::stringstream ss;
    if (minutes > 0) {
      ss << std::setfill('0') << std::setw(2) << minutes << ":" << std::setw(2) << seconds << "." << std::setw(3) << static_cast<int>(lastTime.asMilliseconds()) % 1000;
    } else {
      ss << std::setfill('0') << std::setw(2) << seconds << "." << std::setw(3) << static_cast<int>(lastTime.asMilliseconds()) % 1000;
    }
    text.setString(ss.str());
  }

  sf::Text tempText("W", font, text.getCharacterSize());
  float charWidth = tempText.getLocalBounds().width;
  float textWidth = charWidth * text.getString().getSize();

  if (timing) {
    text.setPosition((x - textWidth) / 2.0f, (y - text.getGlobalBounds().height) / 2.0f);
  } else {
    text.setPosition((x - textWidth) / 2.0f, (y - text.getGlobalBounds().height) / 2.0f);
  }
  window.draw(text);
}


int main() {
  const int SCREEN_WIDTH = 1980;
  const int SCREEN_HEIGHT = 1080;

  sf::Font regular;
  if (!regular.loadFromFile("assets/RobotoMono-Regular.ttf")) {
    return EXIT_FAILURE;
  }

  sf::Font bold;
  if (!bold.loadFromFile("assets/RobotoMono-Bold.ttf")) {
    return EXIT_FAILURE;
  }

  int fontSize = SCREEN_WIDTH / 50;

  sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Cube Timer");
  sf::Clock timer;
  sf::Color gray(36, 36, 36);
  
  std::string scramble = generateScramble();
  sf::Time elapsedTime;
  sf::Time canStart;
  sf::Time lastTime;

  bool timing = false;
  bool holding = false;
  int x = window.getSize().x;
  int y = window.getSize().y;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
        if (!timing) {
          if (!holding) {
            timer.restart();
          }
          holding = true;
          canStart = timer.getElapsedTime();
        } else {
          saveScramble(scramble, timer.getElapsedTime());
          if (timing) {
            scramble = generateScramble();
          }
          timing = false;
          lastTime = timer.getElapsedTime();
        }
      } else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
        if (holding) {
          if (timer.getElapsedTime() >= sf::seconds(.4)) {
            timing = true;
            canStart = sf::seconds(0);
            timer.restart();
          }
          holding = false;
        }
      } else if (event.type == sf::Event::Resized) {
        x = event.size.width;
        y = event.size.height;

        fontSize = x / 50;
        sf::FloatRect visibleArea(0, 0, x, y);
        window.setView(sf::View(visibleArea));
      }
    }

    if (timing) {
      elapsedTime = timer.getElapsedTime();
    }

    window.clear(gray);

    if (!timing) {
      drawScramble(window, scramble, regular, fontSize, x, y);
    }
    drawTimer(window, elapsedTime, lastTime, timing, holding, canStart, bold, fontSize * 2, x, y);

    window.display();
  }

  return 0;
}
