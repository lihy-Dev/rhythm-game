#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofBackground(0);
	
	currentState = STATE_TITLE;
	selectedDifficulty = 0;
	
	updateLaneLayout();

	laneColor[0] = ofColor(255, 60, 60);
	laneColor[1] = ofColor(60, 220, 120);
	laneColor[2] = ofColor(50, 170, 255);
	laneColor[3] = ofColor(255, 220, 60);
	laneColor[4] = ofColor(240, 240, 240);
	
	noteX = laneX[0];
	noteY = 0;
	noteSpeed = 3.0;
	
	score=0;
	noteActive=true;
	
	resultText = "";
	resultTimer = 0;
	
	scorePopup = "";
	scorePopupTimer = 0;
	
	combo = 0;
	
	pauseStartTime=0.0f;

	ofTrueTypeFontSettings tutorialsettings("NotoSansJP-Regular.ttf", 30);
	tutorialsettings.addRanges(ofAlphabet::Latin);
	tutorialsettings.addRanges(ofAlphabet::Japanese);
	tutorialFont.load(tutorialsettings);
	
	ofTrueTypeFontSettings keysettings("NotoSansJP-Regular.ttf", 28);
	keysettings.addRanges(ofAlphabet::Latin);
	keyFont.load(keysettings);
	
	ofTrueTypeFontSettings menusettings("NotoSansJP-Regular.ttf", 38);
	menusettings.addRanges(ofAlphabet::Latin);
	menusettings.addRanges(ofAlphabet::Japanese);
	menuFont.load(menusettings);
	
	ofTrueTypeFontSettings titlesettings("NotoSansJP-Regular.ttf", 55);
	titlesettings.addRanges(ofAlphabet::Latin);
	titlesettings.addRanges(ofAlphabet::Japanese);
	titleFont.load(titlesettings);
	
	ofTrueTypeFontSettings bigsettings("NotoSansJP-Regular.ttf", 65);
	bigsettings.addRanges(ofAlphabet::Latin);
	bigsettings.addRanges(ofAlphabet::Japanese);
	bigFont.load(bigsettings);
	
	ofTrueTypeFontSettings judgesettings("NotoSansJP-Regular.ttf", 40);
	judgesettings.addRanges(ofAlphabet::Latin);
	judgesettings.addRanges(ofAlphabet::Japanese);
	judgeFont.load(judgesettings);
	
	ofTrueTypeFontSettings pointsettings("NotoSansJP-Regular.ttf", 40);
	pointsettings.addRanges(ofAlphabet::Latin);
	pointsettings.addRanges(ofAlphabet::Japanese);
	pointFont.load(pointsettings);
	
	resultFont.load("Arial.ttf", 40 ,true,true);
	

	//初期画面ボタン
	float buttonW=260;
	float buttonH=70;
	float buttonX=(ofGetWidth() - buttonW)/2;
	
	tutorialButton.set(buttonX,260,buttonW,buttonH);
	startButton.set(buttonX,360,buttonW,buttonH);
	
	//難易度画面ボタン
	
	float easyY = 230;
	float normalY = 340;
	float hardY = 450;
	float backY = 590;
	
	easyButton.set(buttonX, easyY, buttonW, buttonH);
	normalButton.set(buttonX, normalY, buttonW, buttonH);
	hardButton.set(buttonX, hardY, buttonW, buttonH);
	
	resetNote();
	
	noteAppearTime = 4.0f;
	gameStartTime = 0.0f;
	
	//bgm
	bgm.load("bgm.mp3");
	bgm.setLoop(false);
	
	isCountdown = false;
	countdownSartTime = 0;
	noteAppearTime = 4.0f;
	
	//hitbgm
	hitSound.load("hit.wav");
	hitSound.setMultiPlay(true);
	hitSound.setVolume(0.5f);
	
	//tutorialbgm
	tutorialBgm.load("tutorialbgm.mp3");
	tutorialBgm.setLoop(true);
	tutorialBgm.setVolume(0.3f);
	
	
	
	//连续防止
	for(int i=0;i<5;i++){
		lastLanePressTime[i]=-999;
	}
	debounceTime=0.12f;
	menuDebounceTime=0.25f;


	//select
	isPause = false;
	

	
	//pause
	isResumeCountdown=false;
	resumeCountdownStartTime=0.0f;
	
	titleBg.load("background.png");
	titleLogo.load("title.png");
	ofEnableAlphaBlending();
	titleMenu.load("select3.png");
	
	
}

//--------------------------------------------------------------
void ofApp::updateLaneLayout(){
	float w = ofGetWidth();
	float h = ofGetHeight();

	judgeLineY = h * 0.78;

	float gameCenterX = w * 0.53;
	float gameSpacing = w * 0.095;

	for(int i = 0; i < 5; i++){
		laneX[i] = gameCenterX + (i - 2) * gameSpacing;
	}


	float tutorialLeft = w * 0.40;
	float tutorialRight = w * 0.88;
	float tutorialSpacing = (tutorialRight - tutorialLeft) / 4.0f;

	for(int i = 0; i < 5; i++){
		tutorialLaneX[i] = tutorialLeft + tutorialSpacing * i;
	}
}
//--------------------------------------------------------------
//音符reset
void ofApp::resetNote(){
	noteY = 0;
	noteX = laneX[(int)ofRandom(0,5)];
}

void ofApp::resetTutorialNote(){
//	int tutorialLaneX[5] = {620,740,860,980,1100};
//	noteY = 0;
//	noteX = tutorialLaneX[(int)ofRandom(0,5)];
	updateLaneLayout();
	noteY = 0;
	int lane = (int)ofRandom(0,5);
	noteX = tutorialLaneX[lane];
}

//--------------------------------------------------------------
void ofApp::update(){
	
	if(currentState == STATE_TUTORIAL){
		updateTutorial();
	}
	else if(currentState == STATE_PLAY){
		updateGame();
	}
	

}

//--------------------------------------------------------------
void ofApp::drawTitleScreen(){
//	float w = ofGetWidth();
//	   float h = ofGetHeight();
//
//	   ofSetColor(255);
//
//	   // ===== 背景：全屏铺满 =====
//	   if(titleBg.isAllocated()){
//		   titleBg.draw(0, 0, w, h);
//	   }
//	   else{
//		   ofBackground(5, 8, 35);
//	   }
//
//	   // ===== Logo：按画面比例缩放 =====
//	   if(titleLogo.isAllocated()){
//		   float logoW = w * 0.72f;
//		   float logoH = logoW * titleLogo.getHeight() / titleLogo.getWidth();
//
//		   // 太高时限制
//		   if(logoH > h * 0.38f){
//			   logoH = h * 0.38f;
//			   logoW = logoH * titleLogo.getWidth() / titleLogo.getHeight();
//		   }
//
//		   float logoX = (w - logoW) / 2.0f;
//		   float logoY = h * 0.02f;
//
//		   titleLogo.draw(logoX, logoY, logoW, logoH);
//	   }
//
//	   // ===== 菜单图片：红=あそびかた、绿=はじめる、下方提示 =====
//	   if(titleMenu.isAllocated()){
//		   float menuW = w * 0.82f;
//		   float menuH = menuW * titleMenu.getHeight() / titleMenu.getWidth();
//
//		   // 防止菜单在小窗口/全屏时过高
//		   if(menuH > h * 0.62f){
//			   menuH = h * 0.62f;
//			   menuW = menuH * titleMenu.getWidth() / titleMenu.getHeight();
//		   }
//
//		   float menuX = (w - menuW) / 2.0f;
//		   float menuY = h * 0.35f;
//
//		   titleMenu.draw(menuX, menuY, menuW, menuH);
//	   }
//	   else{
//		   // 图片加载失败时的备用显示
//		   string guide = "同じ色のボタンを押してください";
//		   float guideWidth = menuFont.stringWidth(guide);
//		   ofSetColor(255);
//		   menuFont.drawString(guide, w / 2 - guideWidth / 2, h * 0.78f);
//	   }
	float w = ofGetWidth();
	float h = ofGetHeight();

	ofSetColor(255);

	string title = "5KEY RHYTHM GAME";
	float titleWidth = titleFont.stringWidth(title);

	titleFont.drawString(
		title,
		w / 2 - titleWidth / 2,
		h * 0.22
	);

	string tutorialText = "チュートリアル";
	string startText = "スタート";

	float tutorialY = h * 0.45f;
	float startY = h * 0.58f;

	float iconX = w * 0.40f;
	float textX = w * 0.45f;

	drawColorButton(iconX, tutorialY, laneColor[0]);
	ofSetColor(255);
	menuFont.drawString(tutorialText, textX, tutorialY + 10);

	drawColorButton(iconX, startY, laneColor[1]);
	ofSetColor(255);
	menuFont.drawString(startText, textX, startY + 10);

	// 下方说明
	string guide = "同じ色のボタンを押してください";
	float guideWidth = menuFont.stringWidth(guide);

	ofSetColor(180);
	menuFont.drawString(
		guide,
		w / 2 - guideWidth / 2,
		h * 0.78
	);

}

//--------------------------------------------------------------
void ofApp::drawTutorialScreen(){
	ofBackground(0);
	
	float w = ofGetWidth();
	float h = ofGetHeight();
	
	float leftX = 50;
	float titleY = 80;
	float dividerX = 420;
	
	ofSetColor(255);
	string tutorialTitle = "チュートリアル";
	float tutorialTitleWidth = titleFont.stringWidth(tutorialTitle);
	titleFont.drawString(tutorialTitle, w / 2 - tutorialTitleWidth / 2, titleY);
	
	//説明

	ofSetColor(235);
	tutorialFont.drawString("あそびかた", leftX, 130);

	tutorialFont.drawString("うえから ノーツが", leftX, 190);
	tutorialFont.drawString("おちてくる", leftX, 230);

	tutorialFont.drawString("あかいせんに", leftX, 300);
	tutorialFont.drawString("きたら", leftX, 340);

	tutorialFont.drawString("おなじいろの", leftX, 410);
	tutorialFont.drawString("ボタンをおす", leftX, 450);
	
	ofSetColor(255, 255, 120);
	tutorialFont.drawString("スコア", leftX, 540);
	tutorialFont.drawString("Great: とてもよい", leftX, 590);
	tutorialFont.drawString("Good: よい", leftX, 630);
	tutorialFont.drawString("Miss :  はずれ", leftX, 670);

	drawSquareButton(leftX+25, ofGetHeight()-55, ofColor(240,240,240));
	ofSetColor(255);
	tutorialFont.drawString("押してもどる", leftX+60, ofGetHeight()-40);
	
	
	ofSetColor(255, 255, 120);
	tutorialFont.drawString("Score: " + ofToString(score), leftX, h-150);
	
	ofSetColor(90);
	ofSetLineWidth(2);
	ofDrawLine(dividerX, 90, dividerX, h - 90);
	ofSetLineWidth(1);
	
	//lane
	updateLaneLayout();

	float tutorialLaneTop = h * 0.16;
	judgeLineY = h * 0.78;
	float tutorialLaneBottom = judgeLineY + h * 0.10;

	ofSetColor(170);
	ofSetLineWidth(2);
	for(int i = 0; i < 5; i++){
		ofDrawLine(tutorialLaneX[i], tutorialLaneTop, tutorialLaneX[i], tutorialLaneBottom);
	}
	ofSetLineWidth(1);

	string tutorialKeys[5] = {"1", "2", "3", "4", "5"};

	for(int i = 0; i < 5; i++){
		ofSetColor(laneColor[i]);
		keyFont.drawString(
			tutorialKeys[i],
			tutorialLaneX[i] - keyFont.stringWidth(tutorialKeys[i]) / 2,
			tutorialLaneTop - 25
		);
	}
	//判定線
	float judgeLeft = tutorialLaneX[0] - w * 0.045;
	float judgeRight = tutorialLaneX[4] + w * 0.035;
	
	float greatRange = 30.0f;
	float goodRange = 60.0f;
	
	//good範囲
	ofSetColor(80, 255, 120, 35);
	ofDrawRectangle(judgeLeft, judgeLineY-goodRange, judgeRight-judgeLeft, goodRange*2);
	
	//great範囲
	ofSetColor(255, 220, 80, 55);
	ofDrawRectangle(judgeLeft, judgeLineY-greatRange, judgeRight-judgeLeft, greatRange*2);
	
	// Good範囲の境界線
	ofSetColor(100, 255, 140, 180);
	ofSetLineWidth(2);
	drawDashedLine(judgeLeft, judgeLineY - goodRange, judgeRight, judgeLineY - goodRange);
	drawDashedLine(judgeLeft, judgeLineY + goodRange, judgeRight, judgeLineY + goodRange);

	// Great範囲の境界線
	ofSetColor(255, 230, 100, 220);
	drawDashedLine(judgeLeft, judgeLineY - greatRange, judgeRight, judgeLineY - greatRange);
	drawDashedLine(judgeLeft, judgeLineY + greatRange, judgeRight, judgeLineY + greatRange);

	//中央の判定線
	ofSetColor(255, 0, 0);
	ofSetLineWidth(3);
	ofDrawLine(judgeLeft, judgeLineY, judgeRight, judgeLineY);
	ofSetLineWidth(1);
	
	for(int i = 0; i < 5; i++){
		float x = tutorialLaneX[i];

		ofSetColor(255, 255, 180, 40);
		ofDrawCircle(x, judgeLineY, 16);

		ofSetColor(255, 255, 180, 70);
		ofDrawCircle(x, judgeLineY, 10);

		ofSetColor(255, 240, 120, 180);
		ofDrawCircle(x, judgeLineY, 6);

		ofSetColor(255, 255, 255);
		ofDrawCircle(x, judgeLineY, 3);
	}
	
	//ラベル
	ofSetColor(255, 230, 100);
	tutorialFont.drawString("Great", judgeRight + 20, judgeLineY + 5);

	ofSetColor(100, 255, 140);
	tutorialFont.drawString("Good", judgeRight + 20, judgeLineY - goodRange + 5);
	tutorialFont.drawString("Good", judgeRight + 20, judgeLineY + goodRange + 5);

	//Note
	int noteLane = 0;
	float minDiff = 999999;

	for(int i = 0; i < 5; i++){
		float diff = abs(noteX - tutorialLaneX[i]);
		if(diff < minDiff){
			minDiff = diff;
			noteLane = i;
		}
	}

	ofSetColor(laneColor[noteLane]);

	float noteW = w * 0.035;
	float noteH = h * 0.025;
	ofDrawRectangle(noteX - noteW / 2, noteY, noteW, noteH);
	drawHitEffects();
//	int noteLane = 0;
//	for(int i = 0;i<5;i++){
//		if(noteX==tutorialLaneX[i]){
//			noteLane=i;
//			break;
//		}
//	}
//	ofSetColor(laneColor[noteLane]);
//	ofDrawRectangle(noteX-20, noteY, 40, 20);
	
	if(resultTimer > 0){
		if(resultText == "Great") ofSetColor(255, 255, 0);
		else if(resultText == "Good") ofSetColor(0, 255, 0);
		else if(resultText == "Miss") ofSetColor(255, 255, 255);
		
		float resultWidth = resultFont.stringWidth(resultText);
		resultFont.drawString(resultText, w/2 - resultWidth/2, 180);
	}
}
//--------------------------------------------------------------
void ofApp::drawDifficultyScreen(){
	float w = ofGetWidth();
		float h = ofGetHeight();

		ofSetColor(255);

		string title = "SELECT DIFFICULTY";
		float titleWidth = titleFont.stringWidth(title);

		titleFont.drawString(
			title,
			w / 2 - titleWidth / 2,
			h * 0.20
		);

		float iconX = w * 0.40;
		float textX = w * 0.45;

		float easyY = h * 0.38;
		float normalY = h * 0.50;
		float hardY = h * 0.62;

		drawColorButton(iconX, easyY, laneColor[0]);
		ofSetColor(255);
		menuFont.drawString("かんたん", textX, easyY + 10);

		drawColorButton(iconX, normalY, laneColor[1]);
		ofSetColor(255);
		menuFont.drawString("ふつう", textX, normalY + 10);

		drawColorButton(iconX, hardY, laneColor[2]);
		ofSetColor(255);
		menuFont.drawString("むずかしい", textX, hardY + 10);
	
		float returnY = h*0.82;
		drawSquareButton(iconX, returnY, ofColor(240,240,240));
		ofSetColor(180);
		menuFont.drawString("もどる", textX, returnY+10);

//		ofSetColor(180);
//		menuFont.drawString("□ Return", iconX, h * 0.82);
//	ofSetColor(255);
//	string difficultTitle = "SELECT DIFFICULTY";
//	
//	float difficultTitleWidth = titleFont.stringWidth(difficultTitle);
//	float difficultTitleY = 140;
//	
//	titleFont.drawString(difficultTitle, ofGetWidth()/2 - difficultTitleWidth/2, difficultTitleY);
//	
//	ofNoFill();
//	ofSetColor(180);
//	
//	// Easyボタン
//	ofNoFill();
//	ofSetColor(180);
//	ofDrawRectangle(easyButton);
//
//	string easyText = "Easy";
//	float easyWidth = menuFont.stringWidth(easyText);
//
//	ofFill();
//	ofSetColor(255);
//	menuFont.drawString(
//		easyText,
//		easyButton.x + easyButton.width / 2 - easyWidth / 2,
//		easyButton.y + easyButton.height / 2 + 8
//	);
//
//	// Normalボタン
//	ofNoFill();
//	ofSetColor(180);
//	ofDrawRectangle(normalButton);
//
//	string normalText = "Normal";
//	float normalWidth = menuFont.stringWidth(normalText);
//
//	ofFill();
//	ofSetColor(255);
//	menuFont.drawString(
//		normalText,
//		normalButton.x + normalButton.width / 2 - normalWidth / 2,
//		normalButton.y + normalButton.height / 2 + 8
//	);

//	// Hardボタン
//	ofNoFill();
//	ofSetColor(180);
//	ofDrawRectangle(hardButton);
//
//	string hardText = "Hard";
//	float hardWidth = menuFont.stringWidth(hardText);
//
//	ofFill();
//	ofSetColor(255);
//	menuFont.drawString(
//		hardText,
//		hardButton.x + hardButton.width / 2 - hardWidth / 2,
//		hardButton.y + hardButton.height / 2 + 8
//	);
//
//	//戻す説明
//	float backY = 590;
//	float w = ofGetWidth();
//	string backText = "Press S to go back";
//	float backWidth = menuFont.stringWidth(backText);
//	menuFont.drawString(backText, w / 2 - backWidth / 2, backY);
}



//--------------------------------------------------------------
void ofApp::draw(){
    ofClear( 0, 0, 0 ); // Black
	
	if(currentState == STATE_TITLE){
		drawTitleScreen();
	}
	else if (currentState == STATE_TUTORIAL){
		drawTutorialScreen();
	}
	else if (currentState == STATE_DIFFICULTY){
		drawDifficultyScreen();
	}
	else if(currentState == STATE_PLAY){
		drawPlayScreen();
	}
	else if (currentState == STATE_RESULT){
		drawResultScreen();
	}
}
//--------------------------------------------------------------
void ofApp::drawPlayScreen(){
	updateLaneLayout();
	ofBackground(10, 10, 20);
	float w=ofGetWidth();
	float h=ofGetHeight();
	float laneTop = h*0.12;
	float laneBottom = h*0.90;
	float laneLeft = laneX[0] - w*0.065;
	float laneRight = laneX[4] + w*0.065;
	//背景
	ofSetColor(20, 20, 35);
	ofDrawRectangle(laneLeft, laneTop, laneRight - laneLeft, laneBottom - laneTop);

	float x=60;
	float y=ofGetHeight()-40;
	
	drawSquareButton(x, y-70, ofColor(255,60,60));
	ofSetColor(180);
	menuFont.drawString("ストップ", x+40, y-60);
	
	drawSquareButton(x, y-10, ofColor(240,240,240));
	ofSetColor(180);
	menuFont.drawString("もどる", x+40, y);
	
	
	for(int i=0; i<5; i++){
		ofSetColor(80, 80, 100);
		ofSetLineWidth(3);
		ofDrawLine(laneX[i], laneTop, laneX[i], laneBottom);
	}
	
	//判定線
	ofSetColor(255, 0, 0);
	ofSetLineWidth(4);
	ofDrawLine(laneLeft, judgeLineY, laneRight, judgeLineY);
	ofSetLineWidth(1);
	
	drawJudgeMarkers(judgeLineY);
	
	
	
	ofSetColor(255, 255, 0);
	menuFont.drawString("Score: " + ofToString(score), 50, 70);
	menuFont.drawString("Combo: " + ofToString(combo), 50, 130);
	
	drawGameProgressBar();
	drawScoreGauge();
	
	if(scorePopupTimer>0){
		ofSetColor(255, 220, 80);
		pointFont.drawString(scorePopup, 60, 220);
	}
	
	string keys[5] = {"1", "2", "3", "4", "5"};

	for(int i=0; i<5; i++){
		ofSetColor(laneColor[i]);
		keyFont.drawString(keys[i], laneX[i] - keyFont.stringWidth(keys[i])/2, 70);
	}
	
	if(isCountdown){
		float countTime = ofGetElapsedTimef() - countdownSartTime;
		
		ofSetColor(255, 255, 255);
		
		if(countTime<1.0f){
			bigFont.drawString("3", ofGetWidth()/2 - bigFont.stringWidth("3")/2, 360);
		}
		else if(countTime<2.0f){
			bigFont.drawString("2", ofGetWidth()/2 - bigFont.stringWidth("2")/2, 360);
		}
		else if(countTime<3.0f){
			bigFont.drawString("1", ofGetWidth()/2 - bigFont.stringWidth("1")/2, 360);
		}
		else{
			bigFont.drawString("Start!", ofGetWidth()/2 - bigFont.stringWidth("Start!")/2, 360);
		}
		return;
	}
	
	for(int i=0;i<gameNotes.size();i++){
		if(gameNotes[i].hit) continue;
		if(gameNotes[i].judged) continue;
		
		float y = getGameNoteY(gameNotes[i]);
		if(y < laneTop - 40 || y > laneBottom) continue;
		
		int lane = gameNotes[i].lane;

		ofSetColor(laneColor[lane]);
		float noteW=w*0.065;
		float noteH=h*0.035;
		ofDrawRectangle(laneX[lane]-noteW/2, y, noteW, noteH);
	}
	
	drawHitEffects();
	
	
	if(resultTimer > 0){
		if(resultText == "Great") ofSetColor(255, 255, 0);
		else if(resultText == "Good") ofSetColor(0, 255, 0);
		else if(resultText == "Miss") ofSetColor(255, 255, 255);
		
		float judgeTextX = laneRight + w * 0.04;
		float judgeTextY = h * 0.38;

		judgeFont.drawString(resultText, judgeTextX, judgeTextY);
	}
	
	if (gameEndWaiting) {
		string text = (missCount==0) ? "FULL COMBO!":"CLEAR!";
		ofSetColor(255, 220, 80);
		bigFont.drawString(text, ofGetWidth()/2 - bigFont.stringWidth(text)/2, 360);
	}
	
	if(isPause){
		drawPauseMenu();
	}
	if(isResumeCountdown){
		float countTime=ofGetElapsedTimef()-resumeCountdownStartTime;
		ofSetColor(255);
		if(countTime<1.0f){
			bigFont.drawString("3", ofGetWidth()/2-bigFont.stringWidth("3")/2, 360);
		}
		else if(countTime<2.0f){
			bigFont.drawString("2", ofGetWidth()/2-bigFont.stringWidth("2")/2, 360);
		}
		else{
			bigFont.drawString("1", ofGetWidth()/2-bigFont.stringWidth("1")/2, 360);
		}
	}
	
}
//--------------------------------------------------------------
void ofApp::drawPauseMenu(){
	float w = ofGetWidth();
	float h = ofGetHeight();

	float boxW = w * 0.45;
	float boxH = h * 0.42;

	float boxX = w / 2 - boxW / 2;
	float boxY = h / 2 - boxH / 2;

	// 背景
	ofSetColor(0, 0, 0, 200);
	ofDrawRectangle(boxX, boxY, boxW, boxH);

	// アウトフレーム
	ofNoFill();
	ofSetColor(255);
	ofSetLineWidth(3);
	ofDrawRectangle(boxX, boxY, boxW, boxH);
	ofFill();

	// タイトル
	string title = "PAUSE";
	float titleW = titleFont.stringWidth(title);

	ofSetColor(255);
	titleFont.drawString(
		title,
		w / 2 - titleW / 2,
		boxY + boxH * 0.22
	);

	float iconX = boxX + boxW * 0.28;
	float textX = boxX + boxW * 0.40;

	float resumeY = boxY + boxH * 0.45;
	float retryY  = boxY + boxH * 0.62;
	float titleY  = boxY + boxH * 0.79;

	// 赤いボタン：続ける
	drawColorButton(
		iconX,
		resumeY,
		laneColor[0]
	);

	ofSetColor(255);
	menuFont.drawString(
		"つづける",
		textX,
		resumeY + 10
	);

	// 緑ボタン：restart
	drawColorButton(
		iconX,
		retryY,
		laneColor[1]
	);

	ofSetColor(255);
	menuFont.drawString(
		"もう一度",
		textX,
		retryY + 10
	);

	// 戻る
	drawSquareButton(
		iconX,
		titleY,
		ofColor(240,240,240)
	);

	ofSetColor(255);
	menuFont.drawString(
		"タイトルへ",
		textX,
		titleY + 10
	);
}
//--------------------------------------------------------------
void ofApp::drawColorButton(int x, int y, ofColor color){
	ofSetColor(color);
	ofDrawCircle(x, y, 22);
	
	ofSetColor(255);
	ofNoFill();
	ofSetLineWidth(3);
	ofDrawCircle(x, y, 22);
	ofFill();
}
//--------------------------------------------------------------
void ofApp::drawSquareButton(float x, float y, ofColor color){
	float size=38;
	ofSetColor(color);
	ofDrawRectangle(x-size/2, y-size/2, size, size);
	ofNoFill();
	ofSetColor(255);
	ofSetLineWidth(3);
	ofDrawRectangle(x-size/2, y-size/2, size, size);
	ofFill();
	ofSetLineWidth(1);
}
//--------------------------------------------------------------
void ofApp::drawDashedLine(float x1, float y1, float x2, float y2,float dashLength,float gapLength){
	float step=dashLength + gapLength;
	for(float x=x1;x<x2;x+=step){
		float endX = MIN(x+dashLength, x2);
		ofDrawLine(x, y1, endX, y2);
	}
}
//--------------------------------------------------------------
void ofApp::drawJudgeMarkers(float y){
	for(int i=0;i<5;i++){
		float x = laneX[i];
		
		ofSetColor(255, 255, 180,40);
		ofDrawCircle(x, y, 18);
		
		ofSetColor(255, 255, 180, 70);
		ofDrawCircle(x, y, 12);
		
		ofSetColor(255, 240, 120, 180);
		ofDrawCircle(x, y, 7);
		
		ofSetColor(255, 255, 255);
		ofDrawCircle(x, y, 3);
	}
}
//--------------------------------------------------------------
void ofApp::addHitEffect(float x, float y, int lane){
	HitEffect e;
	e.x = x;
	e.y = y;
	e.lane = lane;
	e.startTime=ofGetElapsedTimef();
	hitEffects.push_back(e);
}
void ofApp::drawHitEffects(){
		float duration = 0.35f;
		float w = ofGetWidth();
		float h = ofGetHeight();

		float noteW = w * 0.065;
		float noteH = h * 0.035;

	for(int i = hitEffects.size() - 1; i >= 0; i--){
		float t = ofGetElapsedTimef() - hitEffects[i].startTime;
		
		if(t > duration){
			hitEffects.erase(hitEffects.begin() + i);
			continue;
		}
		
		float alpha = ofMap(t, 0, duration, 160, 0, true);
		float scale = ofMap(t, 0, duration, 1.0, 1.4, true);
		
		ofColor c = laneColor[hitEffects[i].lane];
		ofSetColor(c.r, c.g, c.b, alpha);
		
		ofDrawRectangle(hitEffects[i].x - noteW * scale / 2,hitEffects[i].y - noteH * scale / 2,noteW * scale,noteH * scale);
	}
}
//--------------------------------------------------------------
void ofApp::keyPressedTitle(int key){
	if(!canPressLaneButton(key)){
			return;
		}
	//1=赤いボタン：チュートリアル
	if(key=='1'){
		resetTutorialNote();
		score=0;
		resultText="";
		resultTimer=0;
		scorePopup="";
		scorePopupTimer=0;
		
		hitEffects.clear();
		
		tutorialBgm.stop();
		tutorialBgm.setPosition(0);
		tutorialBgm.play();
		
		currentState=STATE_TUTORIAL;
		return;
	}
	//2=緑ボタン：スタート
	if(key=='2'){
		currentState=STATE_DIFFICULTY;
		return;
	}
	
	
	
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(currentState == STATE_TITLE){
		keyPressedTitle(key);
	}
	else if(currentState == STATE_TUTORIAL){
		keyPressedTutorial(key);
	}
	else if(currentState == STATE_DIFFICULTY){
		keyPressedDifficulty(key);
	}
	else if(currentState == STATE_PLAY){
		keyPressedGame(key);
	}
	else if(currentState == STATE_RESULT){
		if(key == 's' ||key == 'S'){
			currentState = STATE_TITLE;
			if(!canPressLaneButton(key)){
					return;
				}
		}
	}
	
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	//	if(currentState == STATE_TITLE){
	//		if(tutorialButton.inside(x, y)){
	//			resetTutorialNote();
	//			currentState = STATE_TUTORIAL;
	//		}
	//		else if (startButton.inside(x, y)){
	//			currentState = STATE_DIFFICULTY;
	//		}
	//	}
	//	else if(currentState == STATE_DIFFICULTY){
	//		if(easyButton.inside(x, y)){
	//			selectedDifficulty = 0;
	//			score = 0;
	//			combo = 0;
	//			resultText = "";
	//			resultTimer = 0;
	//			scorePopup = "";
	//			scorePopupTimer = 0;
	//			greatCount = 0;
	//			goodCount = 0;
	//			missCount = 0;
	//			maxCombo = 0;
	//			gameEndWaiting = false;
	//			gameEndTime = 0;
	//
	//			noteAppearTime = 4.0f; //easy speed
	//
	//			loadEasyNotes();
	//
	//			bgm.stop();
	//			bgm.setPosition(0);
	//
	//			isCountdown = true;
	//			countdownSartTime = ofGetElapsedTimef();
	//			currentState = STATE_PLAY;
	//		}
	//		else if (normalButton.inside(x, y)){
	//			selectedDifficulty = 1;
	//			score = 0;
	//			combo = 0;
	//			resultText = "";
	//			resultTimer = 0;
	//			scorePopup = "";
	//			scorePopupTimer = 0;
	//			greatCount = 0;
	//			goodCount = 0;
	//			missCount = 0;
	//			maxCombo = 0;
	//			gameEndWaiting = false;
	//			gameEndTime = 0;
	//
	//			noteAppearTime = 2.5f; // normal speed
	//
	//			loadNormalNotes();
	//
	//			bgm.stop();
	//			bgm.setPosition(0);
	//
	//			isCountdown = true;
	//			countdownSartTime = ofGetElapsedTimef();
	//			currentState = STATE_PLAY;
	//		}
	//		else if (hardButton.inside(x, y)){
	//			selectedDifficulty = 2;
	//			score = 0;
	//			combo = 0;
	//			resultText = "";
	//			resultTimer = 0;
	//			scorePopup = "";
	//			scorePopupTimer = 0;
	//			greatCount = 0;
	//			goodCount = 0;
	//			missCount = 0;
	//			maxCombo = 0;
	//			gameEndWaiting = false;
	//			gameEndTime = 0;
	//
	//			noteAppearTime = 2.5f; //hard speed
	//
	//			loadHardNotes();
	//
	//			bgm.stop();
	//			bgm.setPosition(0);
	//
	//			isCountdown = true;
	//			countdownSartTime = ofGetElapsedTimef();
	//			currentState = STATE_PLAY;
	//		}
	//	}
	//
	
}
//--------------------------------------------------------------
void ofApp::drawResultScreen(){
	ofBackground(10, 10, 20);

	float w = ofGetWidth();
	float h = ofGetHeight();

	string title;
	if(missCount == 0){
		title = "FULL COMBO!";
		ofSetColor(255, 220, 80);
	}
	else{
		title = "CLEAR!";
		ofSetColor(255);
	}

	float titleWidth = titleFont.stringWidth(title);
	titleFont.drawString(title, w / 2 - titleWidth / 2, h * 0.12f);

	float textX = w / 2 - 150;
	float startY = h * 0.32f;
	float lineH = 68.0f;

	ofSetColor(255, 255, 0);
	menuFont.drawString("Score: " + ofToString(score), textX, startY);

	ofSetColor(255);
	menuFont.drawString("Max Combo: " + ofToString(maxCombo), textX, startY + lineH);
	menuFont.drawString("Great: " + ofToString(greatCount), textX, startY + lineH * 2);
	menuFont.drawString("Good: " + ofToString(goodCount), textX, startY + lineH * 3);
	menuFont.drawString("Miss: " + ofToString(missCount), textX, startY + lineH * 4);

	float buttonY = h * 0.83f;

	string backText = "タイトルもどる";
	float textW = menuFont.stringWidth(backText);

	float groupW = 38 + 20 + textW;
	float groupX = w / 2 - groupW / 2;

	float buttonX = groupX + 19;
	float X = groupX + 38 + 20;

	drawSquareButton(buttonX, buttonY, ofColor(240,240,240));

	ofSetColor(180);
	menuFont.drawString(backText, X, buttonY + 8);
	
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
	
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	updateLaneLayout();
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
	
}
//--------------------------------------------------------------
void ofApp::updateTutorial(){
	noteY += 2.0;
	
	if(resultTimer > 0){
		resultTimer--;
	}
	
	if(noteY > judgeLineY + 60){
		resultText = "Miss";
		resultTimer = 90;
		resetTutorialNote();
	}
}
//--------------------------------------------------------------
void ofApp::keyPressedTutorial(int key){
	if(key == 's'|| key =='S'){
		tutorialBgm.stop();
		currentState = STATE_TITLE;
		return;
	}
	
	
	int pressedLane = -1;
	
	if(key == '1') pressedLane = 0;
	else if(key == '2') pressedLane = 1;
	else if(key == '3') pressedLane = 2;
	else if(key == '4') pressedLane = 3;
	else if(key == '5') pressedLane = 4;
	
	if(pressedLane == -1) return;
	
	//key押す正しいことの判断
	updateLaneLayout();
	//連続防止
	float now = ofGetElapsedTimef();
	if(now-lastLanePressTime[pressedLane]<debounceTime){
		return;
	}
	lastLanePressTime[pressedLane]=now;
	
	
	if(noteX == tutorialLaneX[pressedLane]){
		float noteH=ofGetHeight()*0.025f;
		float noteCenterY=noteY+noteH/2.0f;
		float diff=abs(noteCenterY-judgeLineY);
		
		float greatRange = 30.0f;
		float goodRange = 60.0f;
		
		if(diff <= greatRange){
			hitSound.play();
			addHitEffect(noteX, noteY, pressedLane);
			
			
			resultText = "Great";
			score += 100;
			scorePopup = "+100";
			scorePopupTimer = 90;
			resultTimer = 90;
			resetTutorialNote();
		}
		else if (diff <= goodRange){
			hitSound.play();
			addHitEffect(noteX, noteY, pressedLane);
			
			resultText = "Good";
			score += 50;
			scorePopup = "+50";
			scorePopupTimer = 90;
			resultTimer = 90;
			resetTutorialNote();
		}
		else{
			hitSound.play();
			addHitEffect(noteX, noteY, pressedLane);
		
			
			resultText = "Miss";
			scorePopup = "";
			scorePopupTimer = 0;
			resultTimer = 90;
			resetTutorialNote();
		}
	}
}
//--------------------------------------------------------------
void ofApp::updateGame(){
	if(isPause){
		return;
	}
	if(isResumeCountdown){
		float countTime=ofGetElapsedTimef()-resumeCountdownStartTime;
		if(countTime>=3.0f){
			isResumeCountdown=false;
			float pauseDuration=ofGetElapsedTimef()-pauseStartTime;
			gameStartTime+=pauseDuration;
			bgm.setPaused(false);
		}
		return;
	}
	
	if(isCountdown){
		float countTime = ofGetElapsedTimef() - countdownSartTime;
		
		if(countTime >= 4.0f){
			isCountdown = false;
			gameStartTime = ofGetElapsedTimef();
			bgm.play();
		}
		return;
	}
	
	if(resultTimer > 0){
		resultTimer--;
	}
	
	if(scorePopupTimer>0){
		scorePopupTimer--;
	}
	
	float currentTime = getCurrentGameTime();
	
	for(int i = 0; i < gameNotes.size(); i++){
		if(gameNotes[i].judged) continue;
		
		if(currentTime > gameNotes[i].time + 0.30f){
			gameNotes[i].judged = true;
			gameNotes[i].hit = false;
			resultText = "Miss";
			missCount++;
			combo = 0;
			resultTimer = 90;
		}
	}
	checkGameEnd();
	if (gameEndWaiting) {
		if (ofGetElapsedTimef() - gameEndTime >=3.0f) {
			currentState = STATE_RESULT;
		}
	}
}
//--------------------------------------------------------------
void ofApp::keyPressedGame(int key){
	
	// menu
	if(isPause){
		// 赤いボタン
		if(key == '1'){
			if(!canPressLaneButton(key)){
				return;
			}
			
			isPause = false;
			isResumeCountdown=true;
			resumeCountdownStartTime=ofGetElapsedTimef();

			return;
		}

		// 緑ボタン
		if(key == '2'){
			if(!canPressLaneButton(key)){
				return;
			}
			isPause = false;
			bgm.stop();
			bgm.setPosition(0);

			score = 0;
			combo = 0;
			resultText = "";
			resultTimer = 0;
			scorePopup = "";
			scorePopupTimer = 0;
			greatCount = 0;
			goodCount = 0;
			missCount = 0;
			maxCombo = 0;
			gameEndWaiting = false;
			gameEndTime = 0;

			if(selectedDifficulty == 0){
				noteAppearTime = 4.0f;
				loadEasyNotes();
			}
			else if(selectedDifficulty == 1){
				noteAppearTime = 2.5f;
				loadNormalNotes();
			}
			else if(selectedDifficulty == 2){
				noteAppearTime = 2.5f;
				loadHardNotes();
			}

			isCountdown = true;
			countdownSartTime = ofGetElapsedTimef();

			currentState = STATE_PLAY;
			return;
		}

		// 戻る
		if(key == 's' || key == 'S'){

			isPause = false;
			bgm.stop();
			currentState = STATE_TITLE;
			return;
		}

		return;
	}
	if(isCountdown){
		return;
	}
	if(isResumeCountdown){
		return;
	}
	if(gameEndWaiting){
		return;
	}
	
	if(key==' '){
		isPause=true;
		pauseStartTime=ofGetElapsedTimef();
		bgm.setPaused(true);
		return;
	}
	
	if(resultTimer > 0){
		resultTimer--;
	}
	
	float currentTime = getCurrentGameTime();
	
	for(int i = 0; i < gameNotes.size(); i++){
		if(gameNotes[i].judged) continue;
		
		if(currentTime > gameNotes[i].time + 0.30f){
			gameNotes[i].judged = true;
			resultText = "Miss";
			resultTimer = 90;
		}
	}
	if(key == 's' || key == 'S'){
		bgm.stop();
		currentState = STATE_DIFFICULTY;
		return;
	}
	
	int pressedLane = -1;
	
	
	if(key == '1') pressedLane = 0;
	else if(key == '2') pressedLane = 1;
	else if(key == '3') pressedLane = 2;
	else if(key == '4') pressedLane = 3;
	else if(key == '5') pressedLane = 4;
	
	
	if(pressedLane == -1) return;
	
	int targetIndex = -1;
	float bestDiff = 999999.0f;
	
	//連続防止
	float now = ofGetElapsedTimef();
	if(now-lastLanePressTime[pressedLane]<debounceTime){
		return;
	}
	lastLanePressTime[pressedLane]=now;
	
	
	
	for(int i = 0; i < gameNotes.size(); i++){
		if(gameNotes[i].judged) continue;
		if(gameNotes[i].lane != pressedLane) continue;
		
		float y = getGameNoteY(gameNotes[i]);
		
		float noteH=ofGetHeight()*0.035f;
		float noteCenterY=y+noteH/2.0f;
		float diff=abs(noteCenterY-judgeLineY);
		
		if(diff < bestDiff){
			bestDiff = diff;
			targetIndex = i;
		}
	}
	if(targetIndex == -1) return;
	
	if(bestDiff>90){
		return;
	}
	
	if(bestDiff <= 30){
		float hitY=getGameNoteY(gameNotes[targetIndex]);
		float noteH=ofGetHeight()*0.035f;
		float hitCenterY = hitY+noteH/2.0f;
		hitSound.play();
		addHitEffect(laneX[pressedLane], hitCenterY, pressedLane);
		
		
		resultText = "Great";
		score += 100;
		combo++;
		greatCount++;
		if(combo>maxCombo)maxCombo=combo;
		resultTimer = 90;
		scorePopup = "+100";
		scorePopupTimer = 90;
		gameNotes[targetIndex].hit = true;
		gameNotes[targetIndex].judged = true;
	}
	else if(bestDiff <= 60){
		float hitY=getGameNoteY(gameNotes[targetIndex]);
		float noteH=ofGetHeight()*0.035f;
		float hitCenterY = hitY+noteH/2.0f;
		hitSound.play();
		addHitEffect(laneX[pressedLane], hitCenterY, pressedLane);
		
		
		resultText = "Good";
		score += 50;
		combo++;
		goodCount++;
		if(combo>maxCombo)maxCombo=combo;
		resultTimer = 90;
		scorePopup = "+50";
		scorePopupTimer = 90;
		gameNotes[targetIndex].hit = true;
		gameNotes[targetIndex].judged = true;
	}
	else{
		float hitY=getGameNoteY(gameNotes[targetIndex]);
		float noteH=ofGetHeight()*0.035f;
		float hitCenterY = hitY+noteH/2.0f;
		hitSound.play();
		addHitEffect(laneX[pressedLane], hitCenterY, pressedLane);
	
		
		resultText = "Miss";
		combo = 0;
		resultTimer = 90;
		missCount++;
		scorePopup = "";
		scorePopupTimer = 0;
		
		gameNotes[targetIndex].judged = true;
		gameNotes[targetIndex].hit = false;
		
	}
}
//--------------------------------------------------------------
void ofApp::keyPressedDifficulty(int key){
	
	if(!canPressLaneButton(key)){
			return;
		}
	if(key == 's'||key == 'S'){
		currentState = STATE_TITLE;
	
	}
	if(key=='1'){
		selectedDifficulty=0;
		score=0;
		combo=0;
		resultText = "";
		resultTimer = 0;
		scorePopup = "";
		scorePopupTimer = 0;
		greatCount = 0;
		goodCount = 0;
		missCount = 0;
		maxCombo = 0;
		gameEndWaiting = false;
		gameEndTime = 0;
		noteAppearTime = 4.0f;
		loadEasyNotes();
		tutorialBgm.stop();
		bgm.stop();
		bgm.setPosition(0);
		isCountdown = true;
		countdownSartTime = ofGetElapsedTimef();
		currentState = STATE_PLAY;
		return;
	}
	if(key=='2'){
		selectedDifficulty = 1;
		score = 0;
		combo = 0;
		resultText = "";
		resultTimer = 0;
		scorePopup = "";
		scorePopupTimer = 0;
		greatCount = 0;
		goodCount = 0;
		missCount = 0;
		maxCombo = 0;
		gameEndWaiting = false;
		gameEndTime = 0;
		noteAppearTime = 2.5f;
		loadNormalNotes();
		tutorialBgm.stop();
		bgm.stop();
		bgm.setPosition(0);
		isCountdown = true;
		countdownSartTime = ofGetElapsedTimef();
		currentState = STATE_PLAY;
		return;
	}
	if(key=='3'){
		selectedDifficulty = 2;
		score = 0;
		combo = 0;
		resultText = "";
		resultTimer = 0;
		scorePopup = "";
		scorePopupTimer = 0;
		greatCount = 0;
		goodCount = 0;
		missCount = 0;
		maxCombo = 0;
		gameEndWaiting = false;
		gameEndTime = 0;
		noteAppearTime = 2.5f;
		loadHardNotes();
		tutorialBgm.stop();
		bgm.stop();
		bgm.setPosition(0);
		isCountdown = true;
		countdownSartTime = ofGetElapsedTimef();
		currentState = STATE_PLAY;
		return;
	}
}
//--------------------------------------------------------------
//譜面
void ofApp::loadEasyNotes(){


	
	
	gameNotes.clear();
	
	gameNotes = {
		{4.355f, 4, false, false},
		{5.323f, 1, false, false},
		{5.806f, 3, false, false},
		{7.258f, 1, false, false},
		{8.226f, 3, false, false},
		{9.194f, 4, false, false},
		{10.161f, 3, false, false},
		{11.613f, 2, false, false},
		{12.581f, 1, false, false},
		{13.548f, 0, false, false},
		{14.516f, 0, false, false},
		{15.484f, 4, false, false},
		{16.452f, 3, false, false},
		{17.419f, 3, false, false},
		{18.387f, 4, false, false},
		{19.355f, 3, false, false},
		{20.323f, 3, false, false},
		{21.290f, 1, false, false},
		{22.258f, 4, false, false},
		{22.742f, 0, false, false},
		{23.226f, 3, false, false},
		{23.710f, 1, false, false},
		{24.677f, 0, false, false},
		{26.129f, 1, false, false},
		{27.097f, 2, false, false},
		{28.065f, 3, false, false},
		{29.032f, 4, false, false},
		{30.000f, 3, false, false},
		{30.968f, 2, false, false},
		{31.452f, 4, false, false},
		{31.935f, 1, false, false},
		{32.903f, 0, false, false},
		{33.871f, 0, false, false},
		{34.839f, 4, false, false},
		{35.806f, 3, false, false},
		{36.774f, 3, false, false},
		{37.742f, 4, false, false},
		{38.710f, 3, false, false},
		{39.677f, 2, false, false},
		{40.645f, 1, false, false},
		{41.613f, 2, false, false},
		{42.581f, 3, false, false},
		{43.548f, 4, false, false},
		{44.032f, 1, false, false},
		{45.484f, 1, false, false},
		{46.452f, 2, false, false},
		{47.419f, 3, false, false},
		{48.387f, 4, false, false},
		{49.355f, 3, false, false},
		{50.323f, 2, false, false},
		{51.290f, 1, false, false},
		{52.258f, 0, false, false},
		{53.226f, 0, false, false},
		{54.194f, 4, false, false},
		{55.161f, 3, false, false},
		{56.129f, 3, false, false},
		{57.097f, 4, false, false},
		{58.065f, 3, false, false}
	};
	
}
//--------------------------------------------------------------
void ofApp::loadNormalNotes(){


	
	gameNotes.clear();
	
	gameNotes = {
		{2.903f, 1, false, false},
			{3.871f, 3, false, false},
			{4.355f, 0, false, false},
			{5.323f, 4, false, false},

			{7.258f, 1, false, false},
			{7.742f, 3, false, false},
			{8.710f, 0, false, false},
			{9.194f, 4, false, false},
			{10.161f, 2, false, false},

			{11.613f, 3, false, false},
			{12.581f, 4, false, false},
			{13.548f, 0, false, false},
			{14.516f, 2, false, false},
			{15.000f, 4, false, false},

			{16.935f, 3, false, false},
			{17.419f, 0, false, false},
			{18.387f, 4, false, false},
			{19.355f, 2, false, false},
			{20.323f, 3, false, false},
			{20.806f, 1, false, false},

			{21.774f, 4, false, false},
			{22.742f, 0, false, false},
			{23.226f, 3, false, false},
			{24.194f, 1, false, false},
			{25.161f, 4, false, false},
			{26.129f, 2, false, false},

			{27.097f, 0, false, false},
			{27.581f, 3, false, false},
			{29.032f, 4, false, false},
			{30.000f, 2, false, false},
			{30.968f, 0, false, false},
			{31.452f, 3, false, false},

			{32.419f, 1, false, false},
			{33.387f, 4, false, false},
			{34.355f, 0, false, false},
			{34.839f, 2, false, false},
			{35.806f, 3, false, false},
			{36.774f, 1, false, false},
			{37.258f, 4, false, false},

			{38.226f, 0, false, false},
			{39.194f, 2, false, false},
			{39.677f, 3, false, false},
			{40.645f, 1, false, false},
			{41.613f, 4, false, false},
			{42.581f, 0, false, false},
			{43.065f, 3, false, false},

			{44.032f, 1, false, false},
			{45.000f, 4, false, false},
			{45.484f, 2, false, false},
			{46.452f, 0, false, false},
			{47.419f, 3, false, false},
			{48.387f, 1, false, false},
			{48.871f, 4, false, false},

			{49.839f, 2, false, false},
			{50.806f, 0, false, false},
			{51.290f, 3, false, false},
			{52.258f, 1, false, false},
			{53.226f, 4, false, false},
			{54.194f, 0, false, false},
			{54.677f, 2, false, false},

			{55.645f, 3, false, false},
			{56.613f, 1, false, false},
			{57.581f, 4, false, false}
	};
}
//--------------------------------------------------------------
void ofApp::loadHardNotes(){

	
	gameNotes.clear();
	
	gameNotes = {
		{2.419f, 2, false, false},
		{2.903f, 0, false, false},
		{3.387f, 3, false, false},
		{3.871f, 1, false, false},
		{4.355f, 4, false, false},
		{4.839f, 3, false, false},
		{5.323f, 1, false, false},
		{5.806f, 3, false, false},
		{6.290f, 1, false, false},
		{6.774f, 3, false, false},
		{7.258f, 1, false, false},
		{7.742f, 0, false, false},
		{8.226f, 3, false, false},
		{8.710f, 0, false, false},
		{9.194f, 4, false, false},
		{9.677f, 2, false, false},
		{10.161f, 3, false, false},
		{11.129f, 0, false, false},
		{11.613f, 2, false, false},
		{12.097f, 4, false, false},
		{12.581f, 1, false, false},
		{13.065f, 2, false, false},
		{13.548f, 0, false, false},
		{14.032f, 4, false, false},
		{14.516f, 0, false, false},
		{15.000f, 2, false, false},
		{15.484f, 4, false, false},
		{15.968f, 2, false, false},
		{16.452f, 3, false, false},
		{16.935f, 0, false, false},
		{17.419f, 3, false, false},
		{17.903f, 1, false, false},
		{18.387f, 4, false, false},
		{18.871f, 0, false, false},
		{19.355f, 3, false, false},
		{19.839f, 1, false, false},
		{20.323f, 3, false, false},
		{20.806f, 4, false, false},
		{21.290f, 1, false, false},
		{21.774f, 2, false, false},
		{22.258f, 4, false, false},
		{22.742f, 0, false, false},
		{23.226f, 3, false, false},
		{23.710f, 1, false, false},
		{24.194f, 4, false, false},
		{24.677f, 0, false, false},
		{25.161f, 1, false, false},
		{25.645f, 3, false, false},
		{26.129f, 1, false, false},
		{26.613f, 4, false, false},
		{27.097f, 2, false, false},
		{27.581f, 0, false, false},
		{28.065f, 3, false, false},
		{28.548f, 1, false, false},
		{29.032f, 4, false, false},
		{29.516f, 3, false, false},
		{30.000f, 3, false, false},
		{30.484f, 0, false, false},
		{30.968f, 2, false, false},
		{31.452f, 4, false, false},
		{31.935f, 1, false, false},
		{32.419f, 3, false, false},
		{32.903f, 0, false, false},
		{33.387f, 4, false, false},
		{33.871f, 0, false, false},
		{34.355f, 4, false, false},
		{34.839f, 4, false, false},
		{35.323f, 1, false, false},
		{35.806f, 3, false, false},
		{36.290f, 0, false, false},
		{36.774f, 3, false, false},
		{37.258f, 1, false, false},
		{37.742f, 4, false, false},
		{38.226f, 0, false, false},
		{38.710f, 3, false, false},
		{39.194f, 2, false, false},
		{39.677f, 2, false, false},
		{40.161f, 3, false, false},
		{40.645f, 1, false, false},
		{41.129f, 2, false, false},
		{41.613f, 2, false, false},
		{42.097f, 0, false, false},
		{42.581f, 3, false, false},
		{43.065f, 1, false, false},
		{43.548f, 4, false, false},
		{44.032f, 1, false, false},
		{44.516f, 1, false, false},
		{45.000f, 3, false, false},
		{45.484f, 1, false, false},
		{45.968f, 3, false, false},
		{46.452f, 2, false, false},
		{46.935f, 0, false, false},
		{47.419f, 3, false, false},
		{47.903f, 0, false, false},
		{48.387f, 4, false, false},
		{48.871f, 2, false, false},
		{49.355f, 3, false, false},
		{49.839f, 0, false, false},
		{50.323f, 2, false, false},
		{50.806f, 4, false, false},
		{51.290f, 1, false, false},
		{51.774f, 2, false, false},
		{52.258f, 0, false, false},
		{52.742f, 4, false, false},
		{53.226f, 0, false, false},
		{53.710f, 1, false, false},
		{54.194f, 4, false, false},
		{54.677f, 1, false, false},
		{55.161f, 3, false, false},
		{55.645f, 0, false, false},
		{56.129f, 3, false, false},
		{56.613f, 1, false, false},
		{57.097f, 4, false, false},
		{57.581f, 0, false, false},
		{58.065f, 3, false, false}
	};
}
//--------------------------------------------------------------
float ofApp::getCurrentGameTime(){
	if(isPause||isResumeCountdown){
		return pauseStartTime-gameStartTime;
	}
	return ofGetElapsedTimef() - gameStartTime;
}
//--------------------------------------------------------------
float ofApp::getGameNoteY(const GameNote& note){
	float currentTime = getCurrentGameTime();
	float startY = 0.0f;
	float endY = judgeLineY;
	
	float t =(currentTime - (note.time - noteAppearTime))/noteAppearTime;
	return  startY + t *(endY - startY);
}
//--------------------------------------------------------------
void ofApp::checkGameEnd(){
	for(int i=0;i<gameNotes.size();i++){
		if(!gameNotes[i].judged){
			return;
		}
	}
	if (!gameEndWaiting) {
		gameEndWaiting = true;
		gameEndTime = ofGetElapsedTimef();
		bgm.stop();
		

	}
}
//--------------------------------------------------------------
bool ofApp::canPressLaneButton(int key){
	int pressedLane = -1;
	if(key == '1') pressedLane = 0;
	else if(key == '2') pressedLane = 1;
	else if(key == '3') pressedLane = 2;
	else if(key == '4') pressedLane = 3;
	else if(key == '5') pressedLane = 4;

	if(pressedLane == -1){
		return true;
	}

	float now = ofGetElapsedTimef();

	if(now - lastLanePressTime[pressedLane] < menuDebounceTime){
		return false;
	}

	lastLanePressTime[pressedLane] = now;
	return true;
}
//--------------------------------------------------------------
float ofApp::getGameProgress(){
	if(gameNotes.size() == 0){
			return 0.0f;
		}

		if(isCountdown){
			return 0.0f;
		}

		if(gameEndWaiting){
			return 1.0f;
		}

		float currentTime = getCurrentGameTime();

		float endTime = gameNotes.back().time + 0.3f;

		return ofClamp(currentTime / endTime, 0.0f, 1.0f);
}
//--------------------------------------------------------------
float ofApp::getScoreRate(){
	if(gameNotes.size() == 0){
			return 0.0f;
		}

		float maxScore = gameNotes.size() * 100.0f;

		return ofClamp(score / maxScore, 0.0f, 1.0f);
}
//--------------------------------------------------------------
void ofApp::drawGameProgressBar(){
	float w = ofGetWidth();
		float h = ofGetHeight();

		// ゲーム画面のレーン範囲に合わせる
		float laneLeft = laneX[0] - w * 0.065;
		float laneRight = laneX[4] + w * 0.065;

		float barX = laneLeft;
		float barY = h * 0.935f;
		float barW = laneRight - laneLeft;
		float barH = 8.0f;

		float progress = getGameProgress();

		// 背景
		ofSetColor(60, 60, 80);
		ofDrawRectangle(barX, barY, barW, barH);

		// 進行部分
		ofSetColor(255, 220, 80);
		ofDrawRectangle(barX, barY, barW * progress, barH);

		// 枠
		ofNoFill();
		ofSetColor(220);
		ofSetLineWidth(2);
		ofDrawRectangle(barX, barY, barW, barH);
		ofFill();
		ofSetLineWidth(1);

}
//--------------------------------------------------------------
void ofApp::drawScoreGauge(){
	float w = ofGetWidth();
		float h = ofGetHeight();

		float laneTop = h * 0.12f;
		float laneBottom = h * 0.90f;
		float laneLeft = laneX[0] - w * 0.065f;

		float gaugeW = 34.0f;
		float gaugeH = laneBottom - laneTop;

		// レーン左側に置く
		float gaugeX = laneLeft - 90.0f;
		float gaugeY = laneTop;

		float rate = getScoreRate();

		// 背景
		ofSetColor(35, 35, 50);
		ofDrawRectangle(gaugeX, gaugeY, gaugeW, gaugeH);

		// 下から上に増える部分
		float fillH = gaugeH * rate;
		ofSetColor(80, 200, 255);
		ofDrawRectangle(
			gaugeX,
			gaugeY + gaugeH - fillH,
			gaugeW,
			fillH
		);

		// 枠
		ofNoFill();
		ofSetColor(220);
		ofSetLineWidth(2);
		ofDrawRectangle(gaugeX, gaugeY, gaugeW, gaugeH);
		ofFill();
		ofSetLineWidth(1);

		// ラベル
		string gaugeLabel = "Score";
		float labelX = gaugeX + gaugeW / 2 - menuFont.stringWidth(gaugeLabel) / 2;
		float labelY = gaugeY - 12;

		ofSetColor(180);
		menuFont.drawString(gaugeLabel, labelX, labelY);
}
