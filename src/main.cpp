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
  // bool beeActive{false};
  // // How fast can the bee move?
  // float beeSpeed{0.0f};
  
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
  // bool cloud1Active{false};
  // bool cloud2Active{false};
  // bool cloud3Active{false};
  // // How fast can the clouds move?
  // float cloud1Speed{0.0f};
  // float cloud2Speed{0.0f};
  // float cloud3Speed{0.0f};

	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
    // Clear everything from the last frame
		window.clear();
    // Draw the game scene.
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
    // Show everything we have drawn.
		window.display();
	}

	return 0;
}
