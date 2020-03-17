#include <sstream>
#include "Platform/PlatformHelper.hpp"

// Function declarations
void updateBranches(int seed);
static constexpr int NUM_BRANCHES{6};
sf::Sprite branches[NUM_BRANCHES];

// Left or right
enum side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

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
  // How fast can the bee move?
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

  // Time bar
  sf::RectangleShape timeBar;
  float timeBarStartWidth{400.0f};
  float timeBarHeight{80};
  timeBar.setSize(sf::Vector2f(timeBarStartWidth, timeBarHeight));
  timeBar.setFillColor(sf::Color::Red);
  timeBar.setPosition((1920/2) - timeBarStartWidth/2, 980);

  sf::Time gameTimeTotal;
  float timeRemaining{6.0f};
  float timeBarWidthPerSecond{timeBarStartWidth/timeRemaining};

  // Prepare branches
  sf::Texture textureBranch;
  textureBranch.loadFromFile("graphics/branch.png");
  // Set the texture for each branch sprite
  for(int i = 0; i < NUM_BRANCHES; i++) {
    branches[i].setTexture(textureBranch);
    branches[i].setPosition(-2000, -2000);
    // Set the sprite's origin to dead center
    // we can then spin it round without changing it's position
    branches[i].setOrigin(220, 20);
  }

  // Prepare the player
  sf::Texture texturePlayer;
  texturePlayer.loadFromFile("graphics/player.png");
  sf::Sprite spritePlayer;
  spritePlayer.setTexture(texturePlayer);
  spritePlayer.setPosition(580, 720);

  // The player starts on the left
  side playerSide = side::LEFT;

  // Prepare the gravestone
  sf::Texture textureRIP;
  textureRIP.loadFromFile("graphics/rip.png");
  sf::Sprite spriteRIP;
  spriteRIP.setTexture(textureRIP);
  spriteRIP.setPosition(600, 860);

  // Prepare axe
  sf::Texture textureAxe;
  textureAxe.loadFromFile("graphics/axe.png");
  sf::Sprite spriteAxe;
  spriteAxe.setTexture(textureAxe);
  spriteAxe.setPosition(700, 830);

  // Line the axe up with the tree
  constexpr float AXE_POSITION_LEFT{700};
  constexpr float AXE_POSITION_RIGHT{1075};

  // Prepare the flying log
  sf::Texture textureLog;
  textureLog.loadFromFile("graphics/log.png");
  sf::Sprite spriteLog;
  spriteLog.setTexture(textureLog);
  spriteLog.setPosition(810, 720);

  // Some other useful log related variable
  bool logActive{false};
  float logSpeedX{1000.0f};
  float logSpeedY{-1500.0f};

  // Prepare sound
  // Chop buffer
  sf::SoundBuffer chopBuffer;
  chopBuffer.loadFromFile("sound/chop.wav");
  sf::Sound chop;
  chop.setBuffer(chopBuffer);
  // Death Buffer
  sf::SoundBuffer deathBuffer;
  deathBuffer.loadFromFile("sound/death.wav");
  sf::Sound death;
  death.setBuffer(deathBuffer);
  // Out of time Buffer
  sf::SoundBuffer ootBuffer;
  ootBuffer.loadFromFile("sound/out_of_time.wav");
  sf::Sound outOfTime;
  outOfTime.setBuffer(ootBuffer);

  // Track weather the game is running
  bool paused{true};
  // Control player input
  bool acceptInput{false};

	sf::Event event;
  // Game Loop
	while (window.isOpen()) {
    /* 
    *********************************************************
    Handle user input
    *********************************************************
    */
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

      if (event.type == sf::Event::KeyReleased && !paused) {
        // Listen for key pressed again
        acceptInput = true;

        // Hide the axe
        spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
      }
		}
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
      paused = false;
      // Reset the time and the score
      score = 0;
      timeRemaining = 5;

      // Make all branches disappear
      for (int i = 1; i < NUM_BRANCHES; i++) {
        branchPositions[i] = side::NONE;
      }
      // Make sure the gravestone is hidden
      spriteRIP.setPosition(675, 2000);
      // Move player into position
      spritePlayer.setPosition(580, 720);
      acceptInput = true;
    }

    // wrap the player control to make sure we are accepting user input
    if (acceptInput) {
      // Handle pressing the right cursor key
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        // Make sure the player is on the right
        playerSide = side::RIGHT;
        score++;

        // Add to the amount of time remaining
        timeRemaining += (2 / score) * .15;

        spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
        spritePlayer.setPosition(1200, 720);

        // Update the branch
        updateBranches(score);

        // Set the log flying to the left
        spriteLog.setPosition(810, 720);
        logSpeedX = -5000;
        logActive = true;

        acceptInput = false;

        // Play sound
        chop.play();
      }

      // Handle pressing the left cursor key
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        // Make sure the player is on the right
        playerSide = side::LEFT;
        score++;

        // Add to the amount of time remaining
        timeRemaining += (2 / score) * .15;

        spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
        spritePlayer.setPosition(580, 720);

        // Update the branch
        updateBranches(score);

        // Set the log flying to the left
        spriteLog.setPosition(810, 720);
        logSpeedX = 5000;
        logActive = true;

        acceptInput = false;
        // Play sound
        chop.play();
      }
    }
    

    /* 
    *********************************************************
    Update game scene
    *********************************************************
    */
    if (!paused) {
      // Measure the time that has elapsed since the clock started
      sf::Time dt = clock.restart();

      // Substract from the amount of time remaining
      timeRemaining -= dt.asSeconds();
      // Set up the time bar
      timeBar.setSize(sf::Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

      if (timeRemaining <= 0.0f) {
        // Pause the game.
        paused = true;

        // Change the message shown to the player
        messageText.setString("Out of time!!");
        // Reposition the text based on it's new size.
        sf::FloatRect textRect = messageText.getLocalBounds();
        messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        messageText.setPosition(1920/2.0f, 1080/2.0f);

        // Play the sound for out of time.
        outOfTime.play();
      }

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

      // Update the branch sprites
      for(int i = 0; i < NUM_BRANCHES; i++) {
        float height = i * 150;
        if (branchPositions[i] == side::LEFT) {
          // Move the sprite to the left side
          branches[i].setPosition(610, height);
          // Flip the sprite round the other way
          branches[i].setRotation(180);
        } else if (branchPositions[i] == side::RIGHT) {
          // Move the sprite to the left side
          branches[i].setPosition(1330, height);
          // Flip the sprite round the other way
          branches[i].setRotation(0);
        } else {
          // Hide the branch
          branches[i].setPosition(3000, height);
        }
      }

      // Handle a flying log
      if (logActive) {
        spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

        // Has the log reached the right hand edge?
        if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().y > 2000) {
          // Set it up ready to a whole new log next frame
          logActive = false;
          spriteLog.setPosition(810, 720);
        }
      }

      if (branchPositions[5] == playerSide) {
        // Death
        paused = true;
        acceptInput = false;

        // Draw the gravestone
        spriteRIP.setPosition(525, 760);
        // Hide the player
        spritePlayer.setPosition(2000, 660);
        // Change the text of the message
        messageText.setString("SQUISHED!!");
        // Center it on the screen
        sf::FloatRect textRect = messageText.getLocalBounds();
        messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        messageText.setPosition(1920/2.0f, 1080/2.0f);

        // Play death sound
        death.play();
      }

      // updateBranches(1);
      // updateBranches(2);
      // updateBranches(3);
      // updateBranches(4);
      // updateBranches(5);

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
    // Draw player sprite
    window.draw(spritePlayer);
    // Draw axe
    window.draw(spriteAxe);
    // Draw log
    window.draw(spriteLog);
    // Draw gravestone sprite
    window.draw(spriteRIP);
    // Draw the Bee.
    window.draw(spriteBee);

    // Draw the branches
    for (int i = 0; i < NUM_BRANCHES; i++) {
      window.draw(branches[i]);
    }

    // Draw the time bar
    window.draw(timeBar);

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

// Function Declaration
void updateBranches(int seed) {
  // Move all branches down one place
  for (int j = NUM_BRANCHES-1; j > 0; j--) {
    branches[j] = branches[j-1];
  }

  // Spawn a new branch at the top of the tree
  // LEFT, RIGHT, NONE
  srand((int)time(0) + seed);
  int r = (rand() % 5);
  switch (r) {
  case 0:
    branchPositions[0] = side::LEFT;
    break;
  case 1:
    branchPositions[0] = side::RIGHT;
    break;
  default:
    branchPositions[0] = side::NONE;
    break;
  }
}