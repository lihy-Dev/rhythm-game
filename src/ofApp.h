#pragma once

#include "ofMain.h"

enum GameState{
	STATE_TITLE,
	STATE_TUTORIAL,
	STATE_DIFFICULTY,
	STATE_PLAY,
	STATE_RESULT
};

struct GameNote{
	float time;
	int lane;
	bool hit;
	bool judged;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
		GameState currentState;
		int selectedDifficulty;
	
		float noteX;
		float noteY;
		float noteSpeed;
	
		vector<GameNote> gameNotes;
		float noteAppearTime;
		float gameStartTime;
	
		void loadEasyNotes();
		void loadNormalNotes();
		void loadHardNotes();
		float getCurrentGameTime();
		float getGameNoteY(const GameNote& note);
		void drawPlayScreen();
	
		void resetTutorialNote();
		
		//初期画面ボタン
		ofRectangle tutorialButton;
		ofRectangle startButton;
		
		//難易度画面ボタン
		ofRectangle easyButton;
		ofRectangle normalButton;
		ofRectangle hardButton;
		
		//line
		int laneX[5];
		int judgeLineY;
	
		//ボタン
		int score;
		bool noteActive;
	
		//文字
		string resultText;
		int resultTimer;
	
		ofTrueTypeFont resultFont;
		ofTrueTypeFont tutorialFont;
		ofTrueTypeFont keyFont;
		ofTrueTypeFont menuFont;
		ofTrueTypeFont titleFont;
		ofTrueTypeFont bigFont;
		ofTrueTypeFont judgeFont;
		ofTrueTypeFont pointFont;

	
		void resetNote();
	
		//画面
		void drawTitleScreen();
		void drawTutorialScreen();
		void drawDifficultyScreen();
		void drawGameScreen();
	
		void updateTutorial();
		void updateGame();
	
		void keyPressedTitle(int key);
		void keyPressedTutorial(int key);
		void keyPressedDifficulty(int key);
		void keyPressedGame(int key);
	
		private:
			ofImage image;
	
		//bgm
		ofSoundPlayer bgm;
		bool isCountdown;
		float countdownSartTime;
		
		//combo
		string scorePopup;
		int scorePopupTimer;
		int combo;
	
		//result
		int greatCount;
		int goodCount;
		int missCount;
		int maxCombo;
		void drawResultScreen();
		void checkGameEnd();
		
		bool gameEndWaiting;
		float gameEndTime;
	
	//連続防止
	float lastLanePressTime[5];
	float debounceTime;
	bool canPressLaneButton(int key);
	float menuDebounceTime;
	
	ofColor laneColor[5];
	
	//pause
	bool isPause;
	void drawColorButton(int x, int y,ofColor color);
	void drawPauseMenu();
	void drawSquareButton(float x,float y,ofColor color);
	float pauseStartTime;
	bool isResumeCountdown;
	float resumeCountdownStartTime;
	
	
	//log
	void sendText2server( string txt );
	uint64_t lastLogTime = 0;
	int button1Count = 0;
	int button2Count = 0;
	int button3Count = 0;
	int button4Count = 0;
	int button5Count = 0;
	int buttonOKcount = 0;
	int buttonBackCount = 0;
	
	
};
