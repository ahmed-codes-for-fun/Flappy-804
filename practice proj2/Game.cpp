#include "Game.h"

//Constructor and Destrcutor//
Game::Game() {
	initVars();
	initTextures();
	initSprites();
	initWindow();
	initText();
	initSounds();
}
Game::~Game() {
	delete this->window;
}
//Private Functions//
void Game::initVars()
{
	this->window = nullptr;
	isRunning = false;
	gameOver = false;
	gameStart = true;
	cont = true;
	score = 0;

	view.setCenter(Vector2f(950.f,540.f));
	view.setSize(Vector2f(1920.f,1080.f));

	obstacles.push_back(obstacle(upper_t,lower_t));
	obstacles.at(0).setPos(1000.f);
	obstacles.push_back(obstacle(upper_t, lower_t));
	obstacles.at(obstacles.size() - 1).setPos(obstacles.at(obstacles.size() - 2).getPos());
	obstacles.push_back(obstacle(upper_t, lower_t));
	obstacles.at(obstacles.size() - 1).setPos(obstacles.at(obstacles.size() - 2).getPos());
	obstacles.push_back(obstacle(upper_t, lower_t));
	obstacles.at(obstacles.size() - 1).setPos(obstacles.at(obstacles.size() - 2).getPos());

	gravity = 10.f;
}

void Game::initTextures()
{
	if (!bird_t.loadFromFile("khan.png"))
		std::cout << "Error loading Texture\n";
	if (!background_t.loadFromFile("background_jail.jpg"))
		std::cout << "Error loading texture\n";
	upper_t.loadFromFile("boot(2).png");
	lower_t.loadFromFile("boot(2).png");
	gameOver_t.loadFromFile("game_over.png");
	gameStart_t.loadFromFile("game_start.png");
}

void Game::initSprites()
{
	bird_s.setTexture(bird_t);
	bird_s.setPosition(Vector2f(480.5f, 460.f)); //centers the bird
	//bird_s.setOrigin(Vector2f(bird_s.getPosition() + Vector2f(55.f,65.f)));
	bird_s.setScale(Vector2f(0.5f, 0.6f));

	background_s.setTexture(background_t);
	background_s.setScale(Vector2f(1.071f, 1.054f));
	background_s.setPosition(Vector2f(0.f, 0.f));
	gameOver_s.setTexture(gameOver_t);
	gameOver_s.setPosition(Vector2f(710.f,290.f));
	gameOver_s.setScale(Vector2f(1.f, 1.f));

	gameStart_s.setTexture(gameStart_t);
	gameStart_s.setPosition(Vector2f(710.f, 290.f));

	overlay.setSize(Vector2f(1920.f,1080.f));
	overlay.setFillColor(Color(0, 0, 0, 0));
}



void Game::initWindow()
{
	this->window = new RenderWindow(VideoMode(1920, 1080), "Flappy 804", Style::Titlebar | Style::Close);
	window->setFramerateLimit(60);
	window->setKeyRepeatEnabled(false);
}

void Game::initText()
{
	gameFont.loadFromFile("prstart.ttf");
	Score.setFont(gameFont);
	Score.setCharacterSize(54);
	Score.setFillColor(Color::Yellow);
	Score.setOutlineColor(Color::Red);
	Score.setOutlineThickness(3);
	Score.setStyle(Text::Style::Bold);
	Score.setPosition(Vector2f(20.f, 10.f));

	PlayAgain.setFont(gameFont);
	PlayAgain.setFillColor(Color::Yellow);
	PlayAgain.setOutlineColor(Color::Red);
	PlayAgain.setOutlineThickness(3);
	PlayAgain.setCharacterSize(48);
	PlayAgain.setPosition(Vector2f(400.f, 150.f));
	PlayAgain.setString("Press Y to Play Again!!!");

	ss_Score << "Score : " << score;
	Score.setString(ss_Score.str());
}

void Game::initSounds()
{
	collision_buffer.loadFromFile("collision_audio.mp3");
	tabdeeli_buffer.loadFromFile("tabdeeli.mp3");
	bg_buffer.loadFromFile("bg_music_2.mp3");
	gameOver_buffer.loadFromFile("game_over.mp3");
	gameOver_sound.setBuffer(gameOver_buffer);
	bg_sound.setBuffer(bg_buffer);
	bg_sound.setVolume(50);
	gameOver_sound.setVolume(50);
	collision_sound.setBuffer(collision_buffer);
	tabdeeli_sound.setBuffer(tabdeeli_buffer);
}

///public functions//
void Game::update()
{
	this->pollEvents();	
	if (keepShrinking == 1)
	{
		birdStart(false, 0.23);
	}
	if(gameStart)
		birdAnimation();
	if (isRunning)
	{
		window->setView(view);
		//view.move(Vector2f(1.f, 0.f));
		view.setCenter(Vector2f(gameOver_s.getPosition().x +270, 540.f));
		updateObstacles();
		updateBird();
		updateScore();
		updateCollisions();
		if (tabdeeli_sound.Playing || collision_sound.Playing)
		{
			bg_sound.setVolume(2);
		}
		else
			bg_sound.setVolume(100);
		if (gameOver)
		{
			bg_sound.setVolume(0);
			tabdeeli_sound.setVolume(0);
			gameOver_sound.play();
		}
		//std::cout << view.getCenter().x << " " << view.getCenter().y << std::endl;
	}

}

void Game::updateObstacles()
{
	//removing older obstacles//
	if (obstacles.at(0).upper_s.getGlobalBounds().left <= (view.getCenter().x - (view.getSize().x)/2)) {
		obstacles.erase(obstacles.begin());
		std::cout << "removed ";
	}
	//making newer obstacles//
	if (obstacles.size()<=4) {
		obstacles.push_back(obstacle(upper_t, lower_t));
		obstacles.at(obstacles.size() - 1).setPos(obstacles.at(obstacles.size() - 2).getPos());
		std::cout << obstacles.size() << std::endl;
	}
}

void Game::updateBird()
{
	bird_s.move(Vector2f(gameSpeed, 0.f));

	background_s.move(Vector2f(gameSpeed, 0.f));
	gameOver_s.move(Vector2f(gameSpeed, 0.f));
	Score.move(Vector2f(gameSpeed, 0.f));
	PlayAgain.move(Vector2f(gameSpeed, 0.f));

	if (speed >= -60 && speed <= 60)
		speed += gravity * 0.11f;
	else if (speed < 0)
		speed = -40;
	else
		speed = 40;
	
	
	bird_s.move(Vector2f(0.f, speed));
	//bird_s.setOrigin(view.getCenter());
	//bird_s.setRotation((speed >= 0 ? speed : -speed) * 1);
	
}

void Game::updateCollisions()
{
	for (auto i : obstacles)
	{
		if(bird_s.getGlobalBounds().intersects(i.upper_s.getGlobalBounds()) || bird_s.getGlobalBounds().intersects(i.lower_s.getGlobalBounds()))
			if (pixelPerfectCollision(bird_s, i.upper_s) || pixelPerfectCollision(bird_s, i.lower_s))
			{
				std::cout << "collision detected\n";
				collision_sound.play();
				collisionAnimation();
				gameOver = true;
				isRunning = false;
				
			}
	}
	if (bird_s.getPosition().y >= 1080 - 177 || bird_s.getPosition().y < 0)
	{
		std::cout << "collision detected\n";
		collision_sound.play();
		collisionAnimation();
		gameOver = true;
		isRunning = false;
	}
}

void Game::collisionAnimation()
{
	for (int i = 0; i < 5; i++)
	{
		view.move(Vector2f(8.f, 0.f));
		window->setView(view);
		render();
	}
	for (int i = 0; i < 5; i++)
	{
		view.move(Vector2f(-8.f, 0.f));
		window->setView(view);
		render();
	}
}

void Game::fadeBackground()
{
	overlay.setPosition(background_s.getPosition());
	for (float i = 0; i < 120; i+=2)
	{
		overlay.setFillColor(Color(0, 0, 0, i));
		this->window->clear();
		//Draw Stuff//
		window->draw(background_s);
		window->draw(overlay);
		renderBird();
		renderObstacles();
		renderText();
		window->display();
	}
	fadeDone = true;
}

void Game::birdAnimation()
{
	if (bird_s.getPosition().y >= 445 && cont)
	{
		bird_s.move(0.f, -1.f);
		if(bird_s.getPosition().y==445)
			cont = false;
	}
	else if (bird_s.getPosition().y <= 475 && !cont)
	{
		bird_s.move(0.f, 1.f);
		if (bird_s.getPosition().y == 475)
			cont = true;
	}
}

bool Game::birdStart(bool grown, float x)
{
	static float i = 0.01;
	if (grown)
	{
		i = 0.01;
	}
	else {
		if (x + i >= 0.5)
		{
			keepShrinking = 2;
		}
		bird_s.setScale(Vector2f(0.5f - i, 0.6f - i));
		if(i==0.01)
			bird_s.setPosition(Vector2f(580.5f, 510.f));
		i += 0.01;
	}
	return grown;
}

void Game::updateScore()
{
	std::cout << obstacles.at(0).getPos() << "\n";
	if(!restarted)
		score = ((int)(bird_s.getPosition().x - 3850) / 700);
	else
		score = ((int)(bird_s.getPosition().x - 1150) / 700);
	score = score > 0 ? score : 0;
	std::cout << "Score : " << score << "\n";
	ss_Score.str("");
	ss_Score << "Score : " << score;
	Score.setString(ss_Score.str());
}

void Game::reset()
{
	gameOver_sound.stop();
	bg_sound.stop();
	keepShrinking = 0;
	fadeDone = false;
	birdStart(true, 0.23);
	restarted = true;
	obstacles.clear();
	initVars();
	//initTextures();
	initSprites();
	initWindow();
	initText();
	playMusic();
}

void Game::pollEvents()
{
	while (this->window->pollEvent(this->event))
	{
		switch (event.type) {
		case Event::Closed:
			window->close(); break;
		case Event::KeyPressed:
			if (event.key.code == Keyboard::Escape)
				window->close();
			else if (event.key.code == Keyboard::Space)
			{
				if(!gameOver)
				{
					if (gameStart)
						tabdeeli_sound.play();
					if (gameStart && keepShrinking == 0)
					{
						gameStart = false;
						isRunning = true;
						keepShrinking = 1;
					}
					if (keepShrinking == 2 && !tabdeeli_sound.Playing)
					{
						gameStart = false;
						isRunning = true;
						//keepShrinking = 1;
					}
					speed = -20;
				}
			}
			else if (event.key.code == Keyboard::Y)
			{
				if (gameOver)
				{
					reset();
				}
			}
			else if (event.key.code != Keyboard::Y)
			{
				if(gameOver)
					window->close();
			}
			break;
		}
	}
}

void Game::playMusic()
{
	Time offset(sf::seconds(6));
	//bg_sound.setPlayingOffset(offset);
	bg_sound.setVolume(10);
	bg_sound.play();
}

void Game::render()
{
	this->window->clear();
	//Draw Stuff//
	window->draw(background_s);
	window->draw(overlay);
	renderBird();
	renderObstacles();
	renderText();
	if (gameOver)
	{
		if (!fadeDone)
			fadeBackground();
		else
		{
			window->draw(gameOver_s);
		}
		//std::cout << "gameOver\n";
	}
	if (gameStart)
	{
		window->draw(gameStart_s);
		//std::cout << "gameOver\n";
	}
	window->display();
}

void Game::renderBird()
{
	window->draw(bird_s);
}

void Game::renderObstacles()
{
	for (auto i : obstacles) {
		window->draw(i.upper_s);
		window->draw(i.lower_s);
	}
}

void Game::renderText()
{
	if(!gameStart)
		window->draw(Score);
	if (gameOver && fadeDone)
		window->draw(PlayAgain);
}

bool Game::pixelPerfectCollision(const Sprite& sprite1, const Sprite& sprite2)
{
	{
		sf::FloatRect intersection;
		if (!sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds(), intersection)) {
			return false;
		}

		sf::Image image1 = sprite1.getTexture()->copyToImage();
		sf::Image image2 = sprite2.getTexture()->copyToImage();

		sf::Transform transform1 = sprite1.getTransform();
		sf::Transform transform2 = sprite2.getTransform();

		for (int x = intersection.left; x < intersection.left + intersection.width; x++) {
			for (int y = intersection.top; y < intersection.top + intersection.height; y++) {
				// Transform the global coordinates into local coordinates
				sf::Vector2f local1 = transform1.getInverse().transformPoint(x, y);
				sf::Vector2f local2 = transform2.getInverse().transformPoint(x, y);

				if (local1.x < 0 || local1.y < 0 || local2.x < 0 || local2.y < 0 ||
					local1.x >= image1.getSize().x || local1.y >= image1.getSize().y ||
					local2.x >= image2.getSize().x || local2.y >= image2.getSize().y) {
					continue;
				}

				// Check if both pixels are opaque
				if (image1.getPixel(local1.x, local1.y).a > 0 &&
					image2.getPixel(local2.x, local2.y).a > 0) {
					return true;
				}
			}
		}
		return false;
	}
}
