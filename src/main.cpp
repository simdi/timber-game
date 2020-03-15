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

	sf::Texture textureBackground;
  // Load graphics into texture.
	textureBackground.loadFromFile("graphics/background.png");

  // Create sprite
  sf::Sprite spriteBackground;
  // Attach the texture to the background
  spriteBackground.setTexture(textureBackground);
  // Set the sprite to cover the screen
  spriteBackground.setPosition(0, 0);

	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
    // Clear everything from the last frame
		window.clear();
    // Draw the game scene.
    window.draw(spriteBackground);
    // Show everything we have drawn.
		window.display();
	}

	return 0;
}
