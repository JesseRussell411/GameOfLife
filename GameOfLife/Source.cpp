#define OLC_PGE_APPLICATION
#define HEIGHT 800
#define WIDTH 1600
#define PIXEL 1
#include "olcPixelGameEngine.h"
#include "GameOfLife.h"
#include "Stopwatch.h"
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
		sAppName = "Example";
	}

public:
	GameOfLife game = GameOfLife(WIDTH, HEIGHT);
	double fps = 60.0;
	std::chrono::steady_clock::duration updateWait;
	Stopwatch updateWatch;
	

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		updateWait = std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::milliseconds(1000) / fps);
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
			if (updateWatch.isRunning())
				updateWatch.stop();
			else
				updateWatch.start();
		}

		if (GetKey(olc::J).bReleased) {
			game.step();
			redraw = true;
		}

		if (GetKey(olc::R).bReleased) {
			game.reset();
			redraw = true;
		}

		if (updateWatch.getElapsed() >= updateWait) {
			redraw = true;
			game.step();
			updateWatch.restart();
		}

		if (redraw) {
			drawGame();
		}

		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(WIDTH, HEIGHT, PIXEL, PIXEL))
		demo.Start();
	return 0;
}