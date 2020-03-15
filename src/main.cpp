#include "Platform/PlatformHelper.hpp"

int main()
{
	PlatformHelper platform;

	sf::RenderWindow window;
	// in Windows at least, this must be called before creating the window
	// float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
  sf::VideoMode vm(1920, 1039);
  // sf::VideoMode vm(1920.0f * screenScalingFactor, 1039.0f * screenScalingFactor);
	// Use the screenScalingFactor
	window.create(vm, "SFML works!");
	platform.setIcon(window.getSystemHandle());

	sf::CircleShape shape(window.getSize().x / 2);
	shape.setFillColor(sf::Color::White);

	sf::Texture shapeTexture;
	shapeTexture.loadFromFile("graphics/background.png");
	shape.setTexture(&shapeTexture);

	sf::Event event;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}
