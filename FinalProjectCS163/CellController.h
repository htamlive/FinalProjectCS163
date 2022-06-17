#pragma once
#include "Cell.h"
class CellController
{
private:
	float height, width, startTime, totalTime;
	int mines, cellsLeft, currentFlags, lastPlayTime;
	bool isOver, isWin;
	Clock clock;

	std::vector<std::vector<Cell*>> cells;
	std::vector<std::vector<int>> test;
	std::vector<std::vector<float>> timeOpen;
	std::vector<std::vector<float>> timeFlag;

	//CellAnimations* cellAnimations;
	//GameExecution* gameExec;
	//GameSounds* gameSound;
	//GameOptions* gameOptions;

public:
	virtual ~CellController();
	CellController();

	bool checkStarted();
	bool checkWin();
	bool checkLose();

	bool openCell(short row, short col);
	void setOpenCellStatus(int row, int col);
	void updateOpenCells();
	bool flagOrUnflagCell(short row, short col);

	void udpateMouseClick();
	void update(const sf::Vector2f mousePos);
	void render(RenderTarget* target);

	float getStartTime();
	float getPlayTime();

	float get_height();
	float get_width();

	int getCurrentFlags();
	int updateHighScore();
	void saveGame();
};

