#pragma once

#include <SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>

#include<iostream>
#include<vector>
#include<sstream>
#include<Windows.h>
using namespace sf;


class obstacle {
public:
	Sprite upper_s, lower_s;
	int spacing = 450;
	//static int c;
	obstacle(Texture& upper_t, Texture& lower_t) {
		//loading sprite and texture//
		upper_s.setTexture(upper_t);
		lower_s.setTexture(lower_t);
		//setting scale//
		lower_s.setScale(Vector2f(0.35f, -1.5f));
		upper_s.setScale(Vector2f(0.35f, 1.5f));
		//lower_s.setRotation(180);
	}
	~obstacle() {
	}
	obstacle(const obstacle &copy) {
		this->lower_s = copy.lower_s;
		this->upper_s = copy.upper_s;
		this->spacing = copy.spacing;
	}
	void setPos(float x) {
		int pos_upper = rand() % 400;
		upper_s.setPosition(Vector2f(x + 700, -(pos_upper)));
		lower_s.setPosition(Vector2f(x + 700, -pos_upper + 500+750  + spacing));
	}
	float getPos() {
		return upper_s.getPosition().x;
	}
};
class Game
{
private:
	//variables//
	RenderWindow* window;
	Event event;

	//Text Stuff//
	Text Score, PlayAgain;
	Font gameFont;
	std::stringstream ss_Score;


	//Sprites//
	Sprite bird_s, background_s;
	Sprite gameOver_s, gameStart_s;
	RectangleShape overlay;

	//Textures//
	Texture bird_t, background_t;
	Texture upper_t, lower_t;
	Texture gameOver_t, gameStart_t;

	//Audio//
	SoundBuffer collision_buffer;
	SoundBuffer tabdeeli_buffer;
	SoundBuffer bg_buffer, gameOver_buffer;
	Sound bg_sound, gameOver_sound;
	Sound collision_sound,tabdeeli_sound;


	//Game Objs//
	std::vector<obstacle> obstacles;

	//Game Logic//
	bool isRunning;
	bool gameOver;
	bool fadeDone = false;
	bool gameStart;
	bool cont;
	bool restarted = false;
	int keepShrinking = 0;	//0 for before animation	1 for during	2 for afterwards

	int score;

	float gravity;
	float speed;
	float gameSpeed=9.f;


	//Viewport stuff//
	View view;
	
	//Init functions//
	void initVars();
	void initTextures();
	void initSprites();
	void initWindow();
	void initText();
	void initSounds();
	
public:
	/// Constructors and Destructors///
	Game();
	virtual ~Game();

	//Functions//
	void update();
	void updateObstacles();
	void updateBird();
	void updateCollisions();
	void collisionAnimation();
	void fadeBackground();
	void birdAnimation();	//just pass true to it
	bool birdStart(bool grown, float x);
	void updateScore();

	void reset();

	void pollEvents();

	void playMusic();

	void render();
	void renderBird();
	void renderObstacles();
	void renderText();
	bool pixelPerfectCollision(const Sprite& sprite1, const Sprite& sprite2);


	//get methods//
	RenderWindow* getWindow() { return window; }
	Event& getEvent() { return event; }

};

