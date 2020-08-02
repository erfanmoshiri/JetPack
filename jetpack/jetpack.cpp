// jetpack.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "SBDL.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>


using namespace std;
////////////////////////////////////////////////////////////////
void load();
void show_back();
void declare_back();
void show_barry();
void find_obstacle();
void choose_obstacle();
void show_laser();
void show_zapper();
void show_missle();
void show_coin();
void start_menu();
void pause_menu();
void lose_menu();
void hasintersection();
void count_score();
///////////////////////////////////////////////////////////////
float S = 0.2;
struct Barry {
	Texture texture[12];
	int height = 72;
	int width = 60;
	float acceleration = 0.2;
	float speed1 = S;
	float speed2 = S;
	int x;
} barry; 
///////////////////////////////////////////////////////////////
struct Laser {
	Texture texture [3];
	int x;
	int y;
} laser;
///////////////////////////////////////////////////////////////
struct Zapper {
	Texture texture[12];
	int x;
	int y;
} zapper; 
///////////////////////////////////////////////////////////////
struct Missle {
	Texture texture[8];
	int x;
	int y;
} missle;
///////////////////////////////////////////////////////////////
struct Coin {
	Texture texture[6];
	int x;
	int y;
} coin;
///////////////////////////////////////////////////////////////
struct Menu {
	Texture texture;
	int x;
	int y;
} play, pause, setting, menu_background, quit, highscores, pause_back, retry, resume, resume2, back, lose_coins;
///////////////////////////////////////////////////////////////
struct Score {
	int value = 0;
	int x;
	int y;
	Texture texture;
	Font *font;
} score, pause_button;
///////////////////////////////////////////////////////////////
const int window_width = 1200;
const int window_height = 460;
///////////////////////////////////////////////////////////////
SDL_Rect barry_rect = { 200, window_height - 90 - barry.x, 60, 72 };
SDL_Rect laser_rect;
SDL_Rect zapper_rect;
SDL_Rect missle_rect;
SDL_Rect play_rect = { window_width / 2 - 333, 400, 166, 60 };
SDL_Rect menu_background_rect = { window_width / 2 - 333, 0, 666, 400 };
SDL_Rect highscores_rect = { (window_width / 2 - 333) + 167, 400, 166, 60 };
SDL_Rect quit_rect = { (window_width / 2 - 333) + 3 * 166 + 2, 400, 166, 60 };
SDL_Rect setting_rect = { (window_width / 2 - 333) + 2 * 166 + 2, 400, 166, 60 };
SDL_Rect back_rect; 
SDL_Rect resume_rect = { window_width / 2 - 333, 400, 166, 60 };
SDL_Rect pause_button_rect = { window_width - 110, 5, 100, 25 };
SDL_Rect retry_rect;  
SDL_Rect coin_rect[3][7];
///////////////////////////////////////////////////////////////

Music *background_music;
Sound *soundoneffect, *missle_launch, *missle_warning, *laser_warning, *zapper_warning;


Texture background[11];
int back_x[11];

int x_back = 0, y_back = 0;
int background_speed = -6;

const int FPS = 60; //frame per second
const int delay = 1000 / FPS; //delay we need at each frame

bool laser_on = false;
bool laser1 = false;
bool zapper1 = false; 
bool coin1 = false;

bool lose = false;
bool start = true;
bool pausee = false;
bool restart = false;

int interval = 1000;
int N;
int LENGTH;

int main()
{
	declare_back();
	SBDL::InitEngine("jetpack", window_width, window_height);

	srand(time(NULL));
	
	
	
	load();
	SBDL::playMusic(background_music, -1);

	while (SBDL::isRunning()) {
		int startTime = SBDL::getTime();

		SBDL::updateEvents();
		SBDL::clearRenderScreen();

		//Game logic code
		if (!lose && !start && !pausee) {
			show_back();
			show_barry();
			show_laser();
			show_zapper();
		/*	if (N == 0)
				zapper_rect = { zapper.x, zapper.y, 150 , 150 };
			else
			if (N == 1)
				zapper_rect = { zapper.x, zapper.y, 100 + LENGTH, 50 }; 
			else
			if (N == 2)
				zapper_rect = { zapper.x, zapper.y, 150 , LENGTH };  */
			show_missle();
			show_coin();
			hasintersection();
			count_score();
			restart = false;
		}
		else
			if (start && !pausee && !lose)
				start_menu();
			else
				if (pausee && !start && !lose)
					pause_menu();
				else
					if (lose && !start && !pausee)
						lose_menu();





		SBDL::updateRenderScreen();
		SBDL::delay(delay);

		//	int elapsedTime = SBDL::getTime() - startTime;
		//if (elapsedTime < delay)
		//	SBDL::delay(delay - elapsedTime);

	}

	return 0;
}

void show_back() {
	for (int i = 0; i < 11; i++) {
		if (restart) {
			declare_back();
		}
		SBDL::showTexture(background[i], { back_x[i], 0, window_width, window_height });
		back_x[i] += background_speed;
		if (back_x[i] <= -10 * window_width)
			back_x[i] = window_width;
	}
	SBDL::freeTexture(score.texture);
	score.texture = SBDL::createFontTexture(score.font, "SCORE: " + to_string(score.value), 0, 0, 0);
	SBDL::showTexture(score.texture, { 2, 2, 100, 25 });

	SBDL::freeTexture(pause_button.texture);
	pause_button.texture = SBDL::createFontTexture(score.font, "PAUSE", 0, 0, 0);
	SBDL::showTexture(pause_button.texture, { window_width - 110, 5, 100, 25 });
	if (SBDL::mouseInRect(pause_button_rect) && SBDL::Mouse.clicked()) {
		SBDL::playSound(soundoneffect, 1);
		pausee = true;
	}
}

void show_barry() {
	if (restart) {
		barry.x = 0;
		barry.speed1 = S;
			barry.speed2 = S;
	}
	if (SBDL::keyHeld(SDL_SCANCODE_SPACE)) {
		if (window_height - 90 - barry.x >= 10) {
			barry.speed2 = S;
			barry.speed1 += barry.acceleration;
			barry.x += barry.speed1;
			SBDL::showTexture(barry.texture[2], { 200, window_height - 90 - barry.x, barry.width, barry.height });
		}
		else
			SBDL::showTexture(barry.texture[2], { 200, 10, barry.width, barry.height });
	}
	else
	if (barry.x > 0) {
		barry.speed1 = S;
		barry.speed2 += barry.acceleration;
		barry.x -= barry.speed2;
		SBDL::showTexture(barry.texture[4], { 200, window_height - 90 - barry.x, barry.width, barry.height });
	}

	else
	if (barry.x == 0 || barry.x < 0) {
		barry.x = 0;
		static int count = 0;
		count++;
		if ((count % delay) < delay / 2)
			SBDL::showTexture(barry.texture[0], { 200, window_height - 90 - barry.x, barry.width, barry.height });
		else
			SBDL::showTexture(barry.texture[1], { 200, window_height - 90 - barry.x, barry.width, barry.height });
	}
}

void show_laser() {
	static int wait = 0;
	wait++;
	laser_rect = { 0, laser.y + 5, window_width, 45 };
	if (wait >= 1000) {
		if (zapper1 == false) {
			static int count_laser = 400;
			///////////////////////
			if (restart) {
				wait = 0;
				count_laser = 400;
				laser_on = false;
				laser1 = false;
			}
			///////////////////////
			if (count_laser == 400)
				laser.y = 10 + rand() % (window_height - 80);

			if (count_laser == 0)
				count_laser = 400 + rand() % 1000;

			if (count_laser < 250 && count_laser >= 150) {
				laser1 = true;
				laser_on = false;
				SBDL::playSound(laser_warning, 1);
				SBDL::showTexture(laser.texture[2], { 100, laser.y, window_width - 200, 50 });
			}
			else
				if (count_laser < 150 && count_laser >= 5) {
					laser_on = true;
					laser1 = true;
					static int go = 0;
					go++;
					if ((go % delay) < delay / 2)
						SBDL::showTexture(laser.texture[0], { 100, laser.y, window_width - 200, 50 });
					else
						SBDL::showTexture(laser.texture[1], { 100, laser.y, window_width - 200, 50 });
				}
				else
					if (count_laser <= 4 && count_laser >= 1) {
						laser1 = false;
						laser_on = false;
					}
			count_laser--;
		}
	}
}

void show_zapper() {
	static int wait = 300;
	wait--;
	static int n;
	static int length;
	if (wait <= 100) {
			static int count_zapper = 0;
			static int zapperX = -401;
			////////////////////////////////
			if (restart) {
				wait = 300;
				count_zapper = 0;
				zapperX = -401;
				zapper.x = zapperX;
				zapper1 = false;
				zapper_rect = { window_width + 10, zapper.y, 0, 0 };
			}
			else
				if (!restart) {
					////////////////////////////////
					if (zapperX == -401) {
						zapperX = window_width + 10;
						zapper.x = zapperX;
						n = rand() % 3;
						N = n;
						length = 100 + rand() % 200;
						LENGTH = length;
						zapper.y = 30 + rand() % (window_height - 80);
						count_zapper = 200 + rand() % 200;
					}

					if (count_zapper <= 200) {
						if (zapperX >= -400) {
							if (laser1 == false && coin1 == false) {
								SBDL::playSound(zapper_warning, 1);
								zapper1 = true;
								if (n == 0) {
									static int go = 0;
									go++;
									zapperX += background_speed;
									if (zapper.y >= window_height - 150)
										zapper.y -= 150;
									zapper_rect = { zapperX, zapper.y, 150 , 150 };
									if ((go % delay) < (delay / 4))
										SBDL::showTexture(zapper.texture[0], { zapperX, zapper.y, 150 , 150 });
									else
										if ((go % delay) < (2 * delay / 4))
											SBDL::showTexture(zapper.texture[1], { zapperX, zapper.y , 150 , 150 });
										else
											if ((go % delay) < (3 * delay / 4))
												SBDL::showTexture(zapper.texture[2], { zapperX, zapper.y , 150 , 150 });
											else
												if ((go % delay) < delay)
													SBDL::showTexture(zapper.texture[3], { zapperX, zapper.y ,  150 , 150 });
								}
								else
									if (n == 1) {
										static int go = 0;
										go++;
										int M = 50;
										zapperX += background_speed;
										zapper.x = zapperX;
										if (zapper.y >= window_height - 50)
											zapper.x -= 50;
										zapper_rect = { zapperX, zapper.y, 100 + length, 50 };
										if ((go % delay) < (delay / 4))
											SBDL::showTexture(zapper.texture[4], { zapperX, zapper.y, 100 + length, M });
										else
											if ((go % delay) < (2 * delay / 4))
												SBDL::showTexture(zapper.texture[5], { zapperX, zapper.y, 100 + length, M });
											else
												if ((go % delay) < (3 * delay / 4))
													SBDL::showTexture(zapper.texture[6], { zapperX, zapper.y, 100 + length, M });
												else
													if ((go % delay) < delay)
														SBDL::showTexture(zapper.texture[7], { zapperX, zapper.y, 100 + length, M });
									}
									else
										if (n == 2) {
											static int go = 0;
											go++;
											zapperX += background_speed;
											zapper.x = zapperX;
											if (zapper.y >= window_height - length)
												zapper.y -= length;
											zapper_rect = { zapperX, zapper.y, 50, length };
											if ((go % delay) < (delay / 4))
												SBDL::showTexture(zapper.texture[8], { zapperX, zapper.y, 50, length });
											else
												if ((go % delay) < (2 * delay / 4))
													SBDL::showTexture(zapper.texture[9], { zapperX, zapper.y, 50, length });
												else
													if ((go % delay) < (3 * delay / 4))
														SBDL::showTexture(zapper.texture[10], { zapperX, zapper.y, 50, length });
													else
														if ((go % delay) < delay)
															SBDL::showTexture(zapper.texture[11], { zapperX, zapper.y, 50, length });
										}
							}
						}
					}
					if (zapperX <= -401) {
						zapperX = -401;
						zapper.x = zapperX;
						zapper1 = false;
					}
					count_zapper--;
				}
	}
}
void show_missle() {
	
	static int wait = 0;
	static int count_missle;
	static int missleX = window_width - 100;
	int dif;
	if (restart) {
		wait = 0;
		count_missle = 0;
		if(missleX != ( window_width - 90))
		missleX += 940;
	}
	missle_rect = { missleX, missle.y, 123, 67 };
	wait++;
	if( wait >= 300){
		if(count_missle == 0){
						count_missle = 300 + rand() % 500;
						missle.y = window_height - 90 - barry.x;
					}
					if (count_missle < 300 && count_missle >= 150) {
						missle_rect = { missleX, missle.y, 123, 67 };
						dif = (window_height - 90 - barry.x) - missle.y;
						missle.y += dif / 12;
						SBDL::showTexture(missle.texture[0], { missleX, missle.y, 50, 50 });
					}
					else
						if (count_missle < 150 && count_missle >= 120) {
							SBDL::playSound(missle_warning, 1);
							SBDL::showTexture(missle.texture[1], { missleX, missle.y, 50, 50 });
						}
						else
							if (count_missle < 120 && count_missle >= 5) {
								missleX += 4 * background_speed;
								static int go = 0;
								go++;
								if ((go % delay) < 1 * delay / 6)
									SBDL::showTexture(missle.texture[2], { missleX, missle.y, 123, 67 });
								else
									if ((go % delay) < 2 * delay / 6)
										SBDL::showTexture(missle.texture[3], { missleX, missle.y, 123, 67 });
									else
										if ((go % delay) < 3 * delay / 6)
											SBDL::showTexture(missle.texture[4], { missleX, missle.y, 123, 67 });
										else
											if ((go % delay) < 4 * delay / 6)
												SBDL::showTexture(missle.texture[5], { missleX, missle.y, 123, 67 });
											else
												if ((go % delay) < 5 * delay / 6)
													SBDL::showTexture(missle.texture[6], { missleX, missle.y, 123, 67 });
												else
													if ((go % delay) < 6 * delay / 6)
														SBDL::showTexture(missle.texture[7], { missleX, missle.y, 123, 67 });
							}
							else
								if (count_missle <= 4 && count_missle >= 1) {
									count_missle--;
									missleX = window_width - 100;
								}

					count_missle--;
				}
}

void show_coin() {
	static int wait = 200;
	wait--;
	if (wait <= 0) {
		static int count_coin = 0;
		static int coinX = -401;
		coin.x = coinX;
		for (int b = 0; b < 3; b++) {
			for (int c = 0; c < 7; c++) {
				coin_rect[b][c] = { coin.x + c * 30 + 2, coin.y + b * coin.y + 2, 20, 20 };
			}
		}
		////////////////////////////////
		if (restart) {
			wait = 200;
			count_coin = 0;
			coinX = -251;
			coin.x = coinX;
			coin1 = false;

		}
		////////////////////////////////
		if (coinX == -251) {
			coinX = window_width + 10;
			coin.x = coinX;
			coin.y = 20 + rand() % (window_height - 80);
			count_coin = 50 + rand() % 50;
		}

		if (count_coin <= 200) {
			if (coinX >= -250) {
				if (zapper1 == false && laser1 == false) {
					coin1 = true;
					static int go = 0;
					go++;
					coinX += background_speed;
					coin.x = coinX;
					if (coin.y >= window_height - 50)
						coin.y -= 50;
					if ((go % delay) < (1 * delay / 6)) {
						for (int i = 0; i < 3; i++)
							for (int j = 0; j < 7; j++)
								SBDL::showTexture(coin.texture[0], { coinX + j * 30 + 2, coin.y + i * coin.y + 2, 20, 20});
					}
					else
					if ((go % delay) < (2 * delay / 6)) {
						for (int i = 0; i < 3; i++)
							for (int j = 0; j < 7; j++)
								SBDL::showTexture(coin.texture[1], { coinX + j * 30 + 2, coin.y + i * 30 + 2, 20, 20 });
					}
					else
					if ((go % delay) < (3 * delay / 6)) {
						for (int i = 0; i < 3; i++)
							for (int j = 0; j < 7; j++)
								SBDL::showTexture(coin.texture[2], { coinX + j * 30 + 2, coin.y + i * 30 + 2, 20, 20 });
					}
					else
					if ((go % delay) < (4 * delay / 6)) {
						for (int i = 0; i < 3; i++)
							for (int j = 0; j < 7; j++)
								SBDL::showTexture(coin.texture[3], { coinX + j * 30 + 2, coin.y + i * 30 + 2, 20, 20 });
					}
					else
					if ((go % delay) < (5 * delay / 6)) {
						for (int i = 0; i < 3; i++)
							for (int j = 0; j < 7; j++)
								SBDL::showTexture(coin.texture[4], { coinX + j * 30 + 2, coin.y + i * 30 + 2, 20, 20 });
					}
					else
					if ((go % delay) < (6 * delay / 6)) {
						for (int i = 0; i < 3; i++)
							for (int j = 0; j < 7; j++)
								SBDL::showTexture(coin.texture[5], { coinX + j * 30 + 2, coin.y + i * 30 + 2, 20, 20 });
					}
				}
			}
		}
		if (coinX <= -251) {
			coinX = -251;
			coin1 = false;
		}
		count_coin--;
	}
}

void hasintersection() {
	if ( SBDL::hasIntersectionRect(barry_rect, missle_rect) ) {
		cout << "N";
		lose = true;
	}
	if (SBDL::hasIntersectionRect(barry_rect, laser_rect) && laser_on && laser1) {
		cout << "L";
		lose = true;
	}
	for (int b = 0; b < 3; b++) {
		for (int c = 0; c < 7; c++) {
			if ((SBDL::hasIntersectionRect(coin_rect[b][c], barry_rect)))
				cout << "3";
		}
	}
}

void count_score() {
	////////////////////////
	if (restart) {
		score.value = 0;
		interval = 1000;
	}
	////////////////////////
	interval -= delay ;
	if (interval < 0){
		score.value++;
		interval = 500;
	}
}

void start_menu() {
	SBDL::stopMusic();

	SBDL::showTexture(menu_background.texture, { window_width / 2 - 333, 0, 666, 400 });
	SBDL::showTexture(play.texture, { window_width / 2 - 333, 400, 166, 60 });
	SBDL::showTexture(highscores.texture, { (window_width / 2 - 333) + 167, 400, 166, 60 });
	SBDL::showTexture(setting.texture, { (window_width / 2 - 333) + 2 * 166 + 2, 400, 166, 60 });
	SBDL::showTexture(quit.texture, { (window_width / 2 - 333) + 3 * 166 + 2, 400, 166, 60 });

	if (SBDL::mouseInRect(play_rect) && SBDL::Mouse.clicked()) {
		start = false;
		restart = true;
		lose = false;
		SBDL::playSound(soundoneffect, 1);
		SBDL::playMusic(background_music, -1);
	}
	if (SBDL::mouseInRect(quit_rect) && SBDL::Mouse.clicked()) {
		SBDL::playSound(soundoneffect, 1);
		exit(0);
	}
}
void pause_menu() {
	SBDL::stopMusic();

	back_rect = { (window_width / 2 - 333) + 2 * 166 + 2, 400, 166, 60 };
	retry_rect = { (window_width / 2 - 333) + 167, 400, 166, 60 };

	SBDL::showTexture(pause_back.texture, { window_width / 2 - 333, 0, 666, 400 });
	SBDL::showTexture(resume.texture, { window_width / 2 - 333, 400, 166, 60 });
	SBDL::showTexture(retry.texture, { (window_width / 2 - 333) + 167, 400, 166, 60 });
	SBDL::showTexture(back.texture, { (window_width / 2 - 333) + 2 * 166 + 2, 400, 166, 60 });
	SBDL::showTexture(setting.texture, { (window_width / 2 - 333) + 3 * 166 + 2, 400, 166, 60 });

	if (SBDL::mouseInRect(resume_rect) && SBDL::Mouse.clicked()) {
		SBDL::playSound(soundoneffect, 1);
		SBDL::playMusic(background_music, -1);
		pausee = false;
	}
	if (SBDL::mouseInRect(back_rect) && SBDL::Mouse.clicked()) {
		SBDL::playSound(soundoneffect, 1);
		start = true;
		pausee = false;
	}
	if (SBDL::mouseInRect(retry_rect) && SBDL::Mouse.clicked()) {
		SBDL::playSound(soundoneffect, 1);
		SBDL::playMusic(background_music, -1);
		restart = true;
		start = false;
		pausee = false;
	}
}

void lose_menu() {
	SBDL::stopMusic();

	back_rect = { (window_width / 2 - 333) + 3 * 166 + 2 - 166, 400, 166, 60 };
	retry_rect = { (window_width / 2 - 257), 400, 166, 60 };

	SBDL::freeTexture(lose_coins.texture);
	lose_coins.texture = SBDL::createFontTexture(score.font, "LOST! YOUR SCORE: "+ to_string(score.value), 0, 0, 0);
	SBDL::showTexture(lose_coins.texture, { window_width / 2 - 257, 2, 514, 98 });

	SBDL::showTexture(pause_back.texture, { window_width / 2 - 257, 100, 514, 300 });
	SBDL::showTexture(retry.texture, { (window_width / 2 - 257), 400, 166, 60 });
	SBDL::showTexture(back.texture, { (window_width / 2 - 333) + 3 * 166 + 2 - 166, 400, 166, 60 });
	SBDL::showTexture(resume2.texture, { (window_width / 2 - 72), 402, 57, 55 });

	if (SBDL::mouseInRect(back_rect) && SBDL::Mouse.clicked()) {
		SBDL::playSound(soundoneffect, 1);
		start = true;
		lose = false;
		pausee = false;
		restart = true;
	}
	if (SBDL::mouseInRect(retry_rect) && SBDL::Mouse.clicked()) {
		SBDL::playSound(soundoneffect, 1);
		start = false;
		pausee = false;
		restart = true;
		lose = false;
		SBDL::playMusic(background_music, -1);
		
	}
}

void declare_back() {
	for (int i = 0; i < 11; i++)
		back_x[i] = i * window_width;
}

void load() {
	background[0] = SBDL::loadTexture("assets\\pic\\back\\lab start.png");
	background[1] = SBDL::loadTexture("assets\\pic\\back\\lab1.png");
	background[2] = SBDL::loadTexture("assets\\pic\\back\\lab2.png");
	background[3] = SBDL::loadTexture("assets\\pic\\back\\sector start.png");
	background[4] = SBDL::loadTexture("assets\\pic\\back\\sector1.png");
	background[5] = SBDL::loadTexture("assets\\pic\\back\\sector2.png");
	background[6] = SBDL::loadTexture("assets\\pic\\back\\volcano start.png");
	background[7] = SBDL::loadTexture("assets\\pic\\back\\volcano1.png");
	background[8] = SBDL::loadTexture("assets\\pic\\back\\volcano2.png");
	background[9] = SBDL::loadTexture("assets\\pic\\back\\warehouse start.png");
	background[10] = SBDL::loadTexture("assets\\pic\\back\\warehouse.png");

	barry.texture[0] = SBDL::loadTexture("assets\\pic\\barry\\barry.png");
	barry.texture[1] = SBDL::loadTexture("assets\\pic\\barry\\barry2.png");
	barry.texture[2] = SBDL::loadTexture("assets\\pic\\barry\\barry3.png");
	barry.texture[3] = SBDL::loadTexture("assets\\pic\\barry\\barry3p.png");
	barry.texture[4] = SBDL::loadTexture("assets\\pic\\barry\\barry4.png");
	barry.texture[5] = SBDL::loadTexture("assets\\pic\\barry\\barryst.png");
	barry.texture[6] = SBDL::loadTexture("assets\\pic\\barry\\gg1.png");
	barry.texture[7] = SBDL::loadTexture("assets\\pic\\barry\\gg2.png");
	barry.texture[8] = SBDL::loadTexture("assets\\pic\\barry\\gg3.png");
	barry.texture[9] = SBDL::loadTexture("assets\\pic\\barry\\gg4.png");
	barry.texture[10] = SBDL::loadTexture("assets\\pic\\barry\\ggdown.png");
	barry.texture[11] = SBDL::loadTexture("assets\\pic\\barry\\ggup.png");

	laser.texture[0] = SBDL::loadTexture("assets\\pic\\lazer\\laser_active1.png");
	laser.texture[1] = SBDL::loadTexture("assets\\pic\\lazer\\laser_active2.png");
	laser.texture[2] = SBDL::loadTexture("assets\\pic\\lazer\\laser_noneactive.png");

	zapper.texture[0] = SBDL::loadTexture("assets\\pic\\zappers\\d1.png");
	zapper.texture[1] = SBDL::loadTexture("assets\\pic\\zappers\\d2.png");
	zapper.texture[2] = SBDL::loadTexture("assets\\pic\\zappers\\d3.png");
	zapper.texture[3] = SBDL::loadTexture("assets\\pic\\zappers\\d4.png");
	zapper.texture[4] = SBDL::loadTexture("assets\\pic\\zappers\\h1.png");
	zapper.texture[5] = SBDL::loadTexture("assets\\pic\\zappers\\h2.png");
	zapper.texture[6] = SBDL::loadTexture("assets\\pic\\zappers\\h3.png");
	zapper.texture[7] = SBDL::loadTexture("assets\\pic\\zappers\\h4.png");
	zapper.texture[8] = SBDL::loadTexture("assets\\pic\\zappers\\v1.png");
	zapper.texture[9] = SBDL::loadTexture("assets\\pic\\zappers\\v2.png");
	zapper.texture[10] = SBDL::loadTexture("assets\\pic\\zappers\\v3.png");
	zapper.texture[11] = SBDL::loadTexture("assets\\pic\\zappers\\v4.png");

	missle.texture[0] = SBDL::loadTexture("assets\\pic\\missle\\1m.png");
	missle.texture[1] = SBDL::loadTexture("assets\\pic\\missle\\2m.png");
	missle.texture[2] = SBDL::loadTexture("assets\\pic\\missle\\missle (1).png");
	missle.texture[3] = SBDL::loadTexture("assets\\pic\\missle\\missle (2).png");
	missle.texture[4] = SBDL::loadTexture("assets\\pic\\missle\\missle (3).png");
	missle.texture[5] = SBDL::loadTexture("assets\\pic\\missle\\missle (4).png");
	missle.texture[6] = SBDL::loadTexture("assets\\pic\\missle\\missle (5).png");
	missle.texture[7] = SBDL::loadTexture("assets\\pic\\missle\\missle (6).png");

	coin.texture[0] = SBDL::loadTexture("assets\\pic\\coin\\1.png");
	coin.texture[1] = SBDL::loadTexture("assets\\pic\\coin\\2.png");
	coin.texture[2] = SBDL::loadTexture("assets\\pic\\coin\\3.png");
	coin.texture[3] = SBDL::loadTexture("assets\\pic\\coin\\4.png");
	coin.texture[4] = SBDL::loadTexture("assets\\pic\\coin\\5.png");
	coin.texture[5] = SBDL::loadTexture("assets\\pic\\coin\\6.png");


	menu_background.texture = SBDL::loadTexture("assets\\pic\\menu\\Background.png");
	play.texture = SBDL::loadTexture("assets\\pic\\menu\\play.png");
	setting.texture = SBDL::loadTexture("assets\\pic\\menu\\setting1.png");
	quit.texture = SBDL::loadTexture("assets\\pic\\menu\\quit1.png");
	highscores.texture = SBDL::loadTexture("assets\\pic\\menu\\highscores1.png");
	pause_back.texture = SBDL::loadTexture("assets\\pic\\menu\\pauseb.png");
	resume.texture = SBDL::loadTexture("assets\\pic\\menu\\resume.png");
	retry.texture = SBDL::loadTexture("assets\\pic\\menu\\retry.png");
	back.texture = SBDL::loadTexture("assets\\pic\\menu\\back.png"); 
	resume2.texture = SBDL::loadTexture("assets\\pic\\menu\\resume 500.png");

	score.font = SBDL::loadFont("assets\\font\\Jetpackia.ttf", 50);
	pause_button.font = SBDL::loadFont("assets\\font\\smart_font_ui_japanese.otf", 50);

	background_music = SBDL::loadMusic("assets\\sound\\music.mp3");
	soundoneffect = SBDL::loadSound("assets\\sound\\soundonEffect.wav");
	missle_launch = SBDL::loadSound("assets\\sound\\missile_launch.wav");
	missle_warning = SBDL::loadSound("assets\\sound\\missile_warning1.wav");
	laser_warning = SBDL::loadSound("assets\\sound\\laser_warning1.wav");
	zapper_warning = SBDL::loadSound("assets\\sound\\zapper.wav");




}
