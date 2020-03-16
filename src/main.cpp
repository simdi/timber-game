#include <sstream>
#include "Platform/PlatformHelper.hpp"

int main() {
	PlatformHelper platform;
  sf::RenderWindow window;
  // sf::VideoMode vm(900, 1020);
	// in Windows at least, this must be called before creating the window
  float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
  sf::VideoMode vm(1920.0f * screenScalingFactor, 1080.0f * screenScalingFactor);

	window.create(vm, "Timber!!!");
	platform.setIcon(window.getSystemHandle());

  // Create a texture background.
	sf::Texture textureBackground;
  // Create sprite
  sf::Sprite spriteBackground;
  // Load graphics into texture.
	textureBackground.loadFromFile("graphics/background.png");
  // Attach the texture to the background
  spriteBackground.setTexture(textureBackground);
  // Set the sprite to cover the screen
  spriteBackground.setPosition(0, 0);

  // Add tree texture and sprite.
  sf::Texture textureTree;
  sf::Sprite spriteTree;
  textureTree.loadFromFile("graphics/tree.png");
  spriteTree.setTexture(textureTree);
  spriteTree.setPosition(810, 0);
  
  // Add bee texture and sprite.
  sf::Texture textureBee;
  sf::Sprite spriteBee;
  textureBee.loadFromFile("graphics/bee.png");
  spriteBee.setTexture(textureBee);
  spriteBee.setPosition(0, 800);
  // Is the bee currently moving?
  bool beeActive{false};
  // // How fast can the bee move?
  float beeSpeed{0.0f};
  
  // Add clouds texture and sprite.
  sf::Texture textureCloud;
  sf::Sprite spriteCloud1;
  sf::Sprite spriteCloud2;
  sf::Sprite spriteCloud3;
  textureCloud.loadFromFile("graphics/cloud.png");
  spriteCloud1.setTexture(textureCloud);
  spriteCloud2.setTexture(textureCloud);
  spriteCloud3.setTexture(textureCloud);
  spriteCloud1.setPosition(0, 0);
  spriteCloud2.setPosition(0, 250);
  spriteCloud3.setPosition(0, 500);
  // Are the clouds currently moving?
  bool cloud1Active{false};
  bool cloud2Active{false};
  bool cloud3Active{false};
  // How fast can the clouds move?
  float cloud1Speed{0.0f};
  float cloud2Speed{0.0f};
  float cloud3Speed{0.0f};

  // Draw some text
  int score{0};
  sf::Text messageText;
  sf::Text scoreText;

  // Choose a font
  sf::Font font;
  font.loadFromFile("fonts/KOMIKAP_.ttf");
  
  // Set font to the message
  messageText.setFont(font);
  scoreText.setFont(font);

  // Assign the actual text
  messageText.setString("Press Enter to start");
  scoreText.setString("Score = 0");

  // Set the size
  messageText.setCharacterSize(55);
  scoreText.setCharacterSize(75);

  // Set color of the texts
  messageText.setFillColor(sf::Color::White);
  scoreText.setFillColor(sf::Color::White);

  // Position the texts
  sf::FloatRect textRect = messageText.getLocalBounds();
  messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
  messageText.setPosition(1920/2.0f, 1080/2.0f);
  scoreText.setPosition(20, 20);

  sf::Clock clock;

  // Track weather the game is running
  bool paused{true};

	sf::Event event;
  // Game Loop
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
      paused = false;
    
    /* 
    *********************************************************
    Update game scene
    *********************************************************
    */
    if (!paused) {
      // Measure the time that has elapsed since the clock started
      sf::Time dt = clock.restart();

      // Setup Bee movement
      if(!beeActive) {
        // How far is the bee
        srand((int)time(0) * 10);
        beeSpeed = (rand() % 200) + 200;
        // How high is the bee
        srand((int)time(0) * 10);
        float height = (rand() % 500) + 500;
        spriteBee.setPosition(2000, height);
        beeActive = true;
      } 
      else
      // Move the bee
      {
        spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
        // has the bee reached the right hand edge of the screen?
        if (spriteBee.getPosition().x < -100) {
          // Set it up to be a whole new cloud next frame
          beeActive = false;
        }
      }

      // Move the clouds
      // Cloud 1
      if(!cloud1Active) {
        // How far is the cloud
        srand((int)time(0) * 10);
        cloud1Speed = (rand() % 200);
        // How high is the cloud
        srand((int)time(0) * 10);
        float height = (rand() % 150);
        spriteCloud1.setPosition(-200, height);
        cloud1Active = true;
      } 
      else
      // Move the cloud
      {
        spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);
        // has the cloud reached the right hand edge of the screen?
        if (spriteCloud1.getPosition().x > 1920) {
          // Set it up to be a whole new cloud next frame
          cloud1Active = false;
        }
      }
      
      // Cloud 2
      if(!cloud2Active) {
        // How far is the cloud
        srand((int)time(0) * 20);
        cloud2Speed = (rand() % 200);
        // How high is the cloud
        srand((int)time(0) * 20);
        float height = (rand() % 300) - 150;
        spriteCloud2.setPosition(-200, height);
        cloud2Active = true;
      } 
      else
      // Move the cloud
      {
        spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);
        // has the cloud reached the right hand edge of the screen?
        if (spriteCloud2.getPosition().x > 1920) {
          // Set it up to be a whole new cloud next frame
          cloud2Active = false;
        }
      }
      
      // Cloud 3
      if(!cloud3Active) {
        // How far is the cloud
        srand((int)time(0) * 30);
        cloud3Speed = (rand() % 200);
        // How high is the cloud
        srand((int)time(0) * 30);
        float height = (rand() % 350) - 150;
        spriteCloud3.setPosition(-200, height);
        cloud3Active = true;
      } 
      else
      // Move the cloud
      {
        spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);
        // has the cloud reached the right hand edge of the screen?
        if (spriteCloud3.getPosition().x > 1920) {
          // Set it up to be a whole new cloud next frame
          cloud3Active = false;
        }
      }

      // Update the score text
      std::stringstream ss;
      ss << "Score = " << score;
      scoreText.setString(ss.str());
    } // End if paused.

    // Clear everything from the last frame
		window.clear();
    /* 
    *********************************************************
    Draw the game scene
    *********************************************************
    */
    // Draw the background.
    window.draw(spriteBackground);
    // Draw the Clouds.
    window.draw(spriteCloud1);
    window.draw(spriteCloud2);
    window.draw(spriteCloud3);
    // Draw the Tree.
    window.draw(spriteTree);
    // Draw the Bee.
    window.draw(spriteBee);

    // Draw the score text
    window.draw(scoreText);
    if(paused)
      // Draw message text.
      window.draw(messageText);

    // Show everything we have drawn.
		window.display();
	}

	return 0;
}
