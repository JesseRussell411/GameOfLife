#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "GameOfLife.h"
#include "Stopwatch.h"
#include <iostream>
#include<string>
using namespace JesseRussell::Diagnostics;

#include<chrono>
#include<thread>

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		// Name you application
		sAppName = "Conway's Game of Life";
	}

public:
	GameOfLife game = GameOfLife(0,0);
	double fps = 30.0;
	std::chrono::steady_clock::duration updateWait;
	Stopwatch updateWatch;
	bool paused = false;

	void setFps(double value) {
		fps = value;
		updateWait = std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::milliseconds(1000) / fps);
	}
	

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		game = GameOfLife(ScreenWidth(), ScreenHeight());
		std::cout <<
			"Game created with: \n" <<
			"Width: " << game.width() << "\n" <<
			"Height: " << game.height() << "\n" <<
			"Total Cells: " << game.width() * game.height() << "\n" <<
			"Thread Count: " << game.getThreadCount() << "\n";

		setFps(fps);
		updateWatch.restart();
		size_t x, y;
		for (x = 0; x < game.width(); ++x)
			for (y = 0; y < game.height(); ++y)
				if (std::rand() > RAND_MAX / 2)
					game.set(x, y, true);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		bool redraw = false;
		auto drawGame = [&]() {
			Clear(olc::BLACK);
			size_t x, y;
			for (x = 0; x < game.width(); ++x)
				for (y = 0; y < game.height(); ++y)
					Draw(x, y, game.get(x, y) ? olc::WHITE : olc::BLACK);
			return true;
		};

		
		if (GetMouse(2).bHeld) {
			static int32_t lastMouseX = -1, lastMouseY = -1;
			int32_t mx = GetMouseX();
			int32_t my = GetMouseY();

			if (lastMouseX != mx || lastMouseY != my)
				game.set(mx, my, !game.get(mx, my));

			lastMouseX = mx;
			lastMouseY = my;
			redraw = true;
		}

		if (GetMouse(0).bHeld) {
			game.set(GetMouseX(), GetMouseY(), true);
			redraw = true;
		}
		if (GetMouse(1).bHeld) {
			game.set(GetMouseX(), GetMouseY(), false);
			redraw = true;
		}

		if (GetKey(olc::SPACE).bReleased) {
			if (paused) {
				paused = false;
				updateWatch.start();
				std::cout << "unpaused\n";
			}
			else {
				paused = true;
				updateWatch.stop();
				std::cout << "paused\n";
			}
		}

		if (GetKey(olc::J).bReleased) {
			game.step();
			
			redraw = true;
		}

		if (GetKey(olc::R).bReleased) {
			game.reset();
			redraw = true;
		}

		if (GetKey(olc::S).bReleased) {
			game.scramble();
			redraw = true;
		}

		if (GetKey(olc::F).bReleased) {
			game.reset(true);
			redraw = true;
		}

		if (!paused && updateWatch.getElapsed() >= updateWait) {
			game.step();
			redraw = true;
			updateWatch.restart();
		}

		if (redraw) {
			drawGame();
		}

		return true;
	}
};

int main(int argc, char *argv[])
{
	size_t width = 100, height = 100, pixelWidth = 8, pixelHeight = 8;
	double fps = 60;


	if (argc == 2) {
		fps = std::stod(argv[1]);
	}
	if (argc == 3) {
		width = std::stoi(argv[1]);
		height = std::stoi(argv[2]);
	}
	if (argc >= 4) {
		width = std::stoi(argv[1]);
		height= std::stoi(argv[2]);
		fps = std::stod(argv[3]);
	}

	Example demo;
	if (demo.Construct(width, height, pixelWidth, pixelHeight)) {
		demo.setFps(fps);
		demo.Start();
	}
	else {
		return 1;
	}
	return 0;
}