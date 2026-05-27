#include "ofApp.h"


void ofApp::setup(){
	
	ofBackground(0);
	
	currentState = STATE_TITLE;
	selectedDifficulty = 0;
	
	updateLaneLayout();
//	float laneSpacing = 120;
//	float centerX = ofGetWidth() / 2;
//
//	laneX[0] = centerX - laneSpacing * 2;
//	laneX[1] = centerX - laneSpacing;
//	laneX[2] = centerX;
//	laneX[3] = centerX + laneSpacing;
//	laneX[4] = centerX + laneSpacing * 2;
//	
	laneColor[0] = ofColor(255, 60, 60);
	laneColor[1] = ofColor(60, 220, 120);
	laneColor[2] = ofColor(50, 170, 255);
	laneColor[3] = ofColor(255, 220, 60);
	laneColor[4] = ofColor(240, 240, 240);
//	
//	judgeLineY = 650;
	
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

	ofTrueTypeFontSettings tutorialsettings("NotoSansJP-Regular.ttf", 18);
	tutorialsettings.addRanges(ofAlphabet::Latin);
	tutorialsettings.addRanges(ofAlphabet::Japanese);
	tutorialFont.load(tutorialsettings);
	
	ofTrueTypeFontSettings keysettings("NotoSansJP-Regular.ttf", 28);
	keysettings.addRanges(ofAlphabet::Latin);
	keyFont.load(keysettings);
	
	ofTrueTypeFontSettings menusettings("NotoSansJP-Regular.ttf", 22);
	menusettings.addRanges(ofAlphabet::Latin);
	menusettings.addRanges(ofAlphabet::Japanese);
	menuFont.load(menusettings);
	
	ofTrueTypeFontSettings titlesettings("NotoSansJP-Regular.ttf", 30);
	titlesettings.addRanges(ofAlphabet::Latin);
	titlesettings.addRanges(ofAlphabet::Japanese);
	titleFont.load(titlesettings);
	
	ofTrueTypeFontSettings bigsettings("NotoSansJP-Regular.ttf", 64);
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
	
	
	//连续防止
	for(int i=0;i<5;i++){
		lastLanePressTime[i]=-999;
	}
	debounceTime=0.02f;
	menuDebounceTime=0.25f;


	//select
	isPause = false;
	
	//log
	sendText2server("program_start");
	lastLogTime = ofGetElapsedTimeMillis();
	
	//pause
	isResumeCountdown=false;
	resumeCountdownStartTime=0.0f;
	
	
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
	
	//log
	if(ofGetElapsedTimeMillis()-lastLogTime>=300000){
		sendText2server("alive");
		lastLogTime=ofGetElapsedTimeMillis();
	}
}

//--------------------------------------------------------------
void ofApp::drawTitleScreen(){
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

	// 菜单整体宽度
	float menuGroupW = 360;

	// 菜单整体左端
	float groupX = w / 2 - menuGroupW / 2;

	float iconX = groupX + 40;
	float textX = groupX + 100;

	float tutorialY = h * 0.45;
	float startY = h * 0.58;

	drawColorButton(iconX, tutorialY, laneColor[0]);
	ofSetColor(255);
	menuFont.drawString("チュートリアル", textX, tutorialY + 10);

	drawColorButton(iconX, startY, laneColor[1]);
	ofSetColor(255);
	menuFont.drawString("スタート", textX, startY + 10);

	// 下方说明
	string guide = "同じ色のボタンを押してください";
	float guideWidth = menuFont.stringWidth(guide);

	ofSetColor(180);
	menuFont.drawString(
		guide,
		w / 2 - guideWidth / 2,
		h * 0.78
	);
//	ofNoFill();
//	ofSetColor(180);
//	ofDrawRectangle(tutorialButton);
//	
//	string text = "Tutorial";
//	float textWidth = menuFont.stringWidth(text);
//	float textHeight = menuFont.stringHeight(text);
//	
//	ofFill();
//	ofSetColor(255);
//	menuFont.drawString(text, tutorialButton.x + tutorialButton.width/2 - textWidth/2, tutorialButton.y + tutorialButton.height/2 + 8);
//	
//	
//	//Startボタン
//	ofNoFill();
//	ofSetColor(180);
//	ofDrawRectangle(startButton);
//	
//	string starttext = "Start";
//	float starttextWidth = menuFont.stringWidth(starttext);
//	
//	ofFill();
//	ofSetColor(255);
//	menuFont.drawString(starttext, startButton.x + startButton.width/2 - starttextWidth/2, startButton.y + startButton.height/2 + 8);
	
}

//--------------------------------------------------------------
void ofApp::drawTutorialScreen(){
	ofBackground(0);
	
	float w = ofGetWidth();
	float h = ofGetHeight();
	
	float leftX = 70;
	float titleY = 80;
	float dividerX = 470;
	
	ofSetColor(255);
	string tutorialTitle = "チュートリアル";
	float tutorialTitleWidth = titleFont.stringWidth(tutorialTitle);
	titleFont.drawString(tutorialTitle, w / 2 - tutorialTitleWidth / 2, titleY);
	
	//説明
	ofSetColor(235);
	tutorialFont.drawString("あそびかた", leftX, 140);
	
	tutorialFont.drawString("ノーツが上から落ちてきます。", leftX, 200);
	tutorialFont.drawString("赤い線に来たタイミングで、", leftX, 240);
	tutorialFont.drawString("対応するキーを押してください。", leftX, 280);

	tutorialFont.drawString("赤い線に近いほど評価が高くなり、", leftX, 360);
	tutorialFont.drawString("スコアも高くなります。", leftX, 400);

	tutorialFont.drawString("評価", leftX, 500);
	tutorialFont.drawString("Great：とてもよい", leftX, 550);
	tutorialFont.drawString("Good ：よい", leftX, 590);
	tutorialFont.drawString("Miss ：はずれ", leftX, 630);

	drawSquareButton(leftX+25, 685, ofColor(240,240,240));
	ofSetColor(255);
	tutorialFont.drawString("押してもどる", leftX+60, 693);
	
	ofSetColor(255, 255, 120);
	tutorialFont.drawString("Score: " + ofToString(score), leftX, 740);
	
	ofSetColor(80);
	ofDrawLine(dividerX, 120, dividerX, 700);
	
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
//	int tutorialLaneX[5];
//	tutorialLaneX[0] = 620;
//	tutorialLaneX[1] = 740;
//	tutorialLaneX[2] = 860;
//	tutorialLaneX[3] = 980;
//	tutorialLaneX[4] = 1100;
//	
//	ofSetColor(170);
//	for(int i = 0;i<5;i++){
//		ofDrawLine(tutorialLaneX[i], 150, tutorialLaneX[i], 650);
//	}
//	
//
//	string tutorialKeys[5] = {"1", "2", "3", "4", "5"};
//
//	for(int i=0; i<5; i++){
//		ofSetColor(laneColor[i]);
//		keyFont.drawString(
//			tutorialKeys[i],
//			tutorialLaneX[i] - keyFont.stringWidth(tutorialKeys[i])/2,
//			130
//		);
//	}
	//判定線
	float judgeLeft = tutorialLaneX[0] - w * 0.055;
	float judgeRight = tutorialLaneX[4] + w * 0.055;

	ofSetColor(255, 0, 0);
	ofSetLineWidth(3);
	ofDrawLine(judgeLeft, judgeLineY, judgeRight, judgeLineY);
	ofSetLineWidth(1);
//	judgeLineY = 600;
//	ofSetColor(255, 0, 0);
//	ofDrawLine(540, judgeLineY, 1180, judgeLineY);
	
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
		menuFont.drawString("Easy", textX, easyY + 10);

		drawColorButton(iconX, normalY, laneColor[1]);
		ofSetColor(255);
		menuFont.drawString("Normal", textX, normalY + 10);

		drawColorButton(iconX, hardY, laneColor[2]);
		ofSetColor(255);
		menuFont.drawString("Hard", textX, hardY + 10);
	
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
	menuFont.drawString("タンマ", x+40, y-60);
	
	drawSquareButton(x, y-10, ofColor(240,240,240));
	ofSetColor(180);
	menuFont.drawString("もどる", x+40, y);
	
	
	for(int i=0; i<5; i++){
		ofSetColor(80, 80, 100);
		ofSetLineWidth(3);
		ofDrawLine(laneX[i], laneTop, laneX[i], laneBottom);
	}
	
	ofSetColor(255, 0, 0);
	ofSetLineWidth(4);
	ofDrawLine(laneLeft, judgeLineY, laneRight, judgeLineY);
	ofSetLineWidth(1);
	
	ofSetColor(255, 255, 0);
	menuFont.drawString("Score: " + ofToString(score), 60, 80);
	menuFont.drawString("Combo: " + ofToString(combo), 60, 120);
	
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
//		ofDrawRectangle(laneX[lane]-35,y,70,24);
		float noteW=w*0.065;
		float noteH=h*0.035;
		ofDrawRectangle(laneX[lane]-noteW/2, y, noteW, noteH);
	}
	
	
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
	
	//log
	if(key=='1'){
		button1Count++;
	}
	if(key=='2'){
		button2Count++;
	}
	if(key=='3'){
		button3Count++;
	}
	if(key=='4'){
		button4Count++;
	}
	if(key=='5'){
		button5Count++;
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
	titleFont.drawString(title, w / 2 - titleWidth / 2, h * 0.18);

	float textX = w / 2 - 170;
	float startY = h * 0.30;
	float lineH = 55;

	ofSetColor(255, 255, 0);
	menuFont.drawString("Score: " + ofToString(score), textX, startY);

	ofSetColor(255);
	menuFont.drawString("Max Combo: " + ofToString(maxCombo), textX, startY + lineH);
	menuFont.drawString("Great: " + ofToString(greatCount), textX, startY + lineH * 2);
	menuFont.drawString("Good: " + ofToString(goodCount), textX, startY + lineH * 3);
	menuFont.drawString("Miss: " + ofToString(missCount), textX, startY + lineH * 4);

	float buttonY = h * 0.72;

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
//	int tutorialLaneX[5] = {620, 740, 860, 980, 1100};
	updateLaneLayout();
	//連続防止
	float now = ofGetElapsedTimef();
	if(now-lastLanePressTime[pressedLane]<debounceTime){
		return;
	}
	lastLanePressTime[pressedLane]=now;
	
	
	if(noteX == tutorialLaneX[pressedLane]){
		int diff = abs(noteY - judgeLineY);
		
		if(diff <= 30){
			resultText = "Great";
			score += 100;
			scorePopup = "+100";
			scorePopupTimer = 90;
			resultTimer = 90;
			resetTutorialNote();
		}
		else if (diff <= 60){
			resultText = "Good";
			score += 50;
			scorePopup = "+50";
			scorePopupTimer = 90;
			resultTimer = 90;
			resetTutorialNote();
		}
		else{
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
		
		if(currentTime > gameNotes[i].time + 0.15f){
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
		
		if(currentTime > gameNotes[i].time + 0.15f){
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
		float diff = abs(y - judgeLineY);
		
		if(diff < bestDiff){
			bestDiff = diff;
			targetIndex = i;
		}
	}
	if(targetIndex == -1) return;
	
	if(bestDiff <= 30){
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
	//log
	button1Count = 0;
	button2Count = 0;
	button3Count = 0;
	button4Count = 0;
	button5Count = 0;
	sendText2server("game_start_easy");
	
	
	gameNotes.clear();
	
	gameNotes = {
		{4.5f, 0, false, false},
		{6.0f, 1, false, false},
		{7.5f, 2, false, false},
		{9.0f, 3, false, false},
		{10.5f, 4, false, false},
		
		{12.0f, 1, false, false},
		{13.5f, 2, false, false},
		{15.0f, 0, false, false},
		{16.5f, 4, false, false},
		
		{18.0f, 0, false, false},
		{19.2f, 2, false, false},
		{20.6f, 1, false, false},
		{22.0f, 3, false, false},
		
		{24.0f, 0, false, false},
		{25.2f, 1, false, false},
		{26.4f, 2, false, false},
		{27.6f, 4, false, false},
		
		{30.0f, 1, false, false},
		{31.0f, 2, false, false},
		{32.0f, 3, false, false},
		{33.0f, 4, false, false},
		
		{35.0f, 0, false, false},
		{36.0f, 1, false, false},
		{37.0f, 2, false, false},
		{38.0f, 3, false, false},
		{39.0f, 4, false, false},
		
		{41.0f, 0, false, false},
		{42.0f, 2, false, false},
		{43.0f, 1, false, false},
		{44.0f, 4, false, false},
		
		{46.0f, 0, false, false},
		{47.0f, 1, false, false},
		{48.0f, 2, false, false},
		{49.0f, 3, false, false},
		{50.0f, 4, false, false},
		
		{52.0f, 1, false, false},
		{54.0f, 2, false, false},
		{56.0f, 0, false, false},
		{58.0f, 4, false, false},
		{60.0f, 1, false, false},
		{62.0f, 2, false, false}
	};
	
}
//--------------------------------------------------------------
void ofApp::loadNormalNotes(){
	//log
	button1Count = 0;
	button2Count = 0;
	button3Count = 0;
	button4Count = 0;
	button5Count = 0;
	sendText2server("game_start_normal");
	
	gameNotes.clear();
	
	gameNotes = {
		{4.5f,0,false,false},
		{5.5f,1,false,false},
		{6.5f,2,false,false},
		{7.5f,3,false,false},
		{8.5f,4,false,false},
		{9.5f,2,false,false},
		
		{11.0f,0,false,false},
		{12.0f,2,false,false},
		{13.0f,1,false,false},
		{14.0f,4,false,false},
		{15.0f,0,false,false},
		{16.0f,3,false,false},
		
		{17.0f,1,false,false},
		{18.0f,4,false,false},
		{19.0f,0,false,false},
		{20.0f,2,false,false},
		{21.0f,3,false,false},
		{22.0f,4,false,false},
		
		{24.0f,0,false,false},
		{24.8f,1,false,false},
		{25.6f,2,false,false},
		{26.4f,3,false,false},
		{27.2f,4,false,false},
		
		{28.0f,4,false,false},
		{28.8f,3,false,false},
		{29.6f,2,false,false},
		{30.4f,1,false,false},
		{31.2f,0,false,false},
		
		{32.0f,0,false,false},
		{32.8f,2,false,false},
		{33.6f,1,false,false},
		{34.4f,4,false,false},
		
		{36.0f,0,false,false},
		{36.7f,1,false,false},
		{37.4f,2,false,false},
		{38.1f,3,false,false},
		{38.8f,4,false,false},
		
		{39.0f,2,false,false},
		{39.7f,3,false,false},
		{40.4f,1,false,false},
		{41.1f,4,false,false},
		
		{42.0f,0,false,false},
		{42.7f,2,false,false},
		{43.4f,1,false,false},
		{44.1f,4,false,false},
		
		{45.0f,4,false,false},
		{45.7f,3,false,false},
		{46.4f,2,false,false},
		{47.1f,1,false,false},
		{47.8f,0,false,false},
		
		{49.0f,0,false,false},
		{50.0f,1,false,false},
		{51.0f,2,false,false},
		{52.0f,3,false,false},
		{53.0f,4,false,false},
		{54.0f,2,false,false},
		
		{56.0f,0,false,false},
		{57.0f,3,false,false},
		{58.0f,2,false,false},
		{59.0f,4,false,false},
		{60.0f,1,false,false},
		{61.0f,0,false,false}
	};
}
//--------------------------------------------------------------
void ofApp::loadHardNotes(){
	//log
	button1Count = 0;
	button2Count = 0;
	button3Count = 0;
	button4Count = 0;
	button5Count = 0;
	sendText2server("game_start_hard");
	
	gameNotes.clear();
	
	gameNotes = {
		{4.5f,0,false,false},
		{5.1f,2,false,false},
		{5.7f,1,false,false},
		{6.3f,4,false,false},
		{6.9f,0,false,false},
		{7.5f,3,false,false},
		{8.1f,1,false,false},
		{8.7f,2,false,false},
		{9.3f,4,false,false},
		
		{10.5f,2,false,false},
		{11.0f,0,false,false},
		{11.5f,4,false,false},
		{12.0f,1,false,false},
		{12.5f,2,false,false},
		{13.0f,0,false,false},
		{13.5f,3,false,false},
		{14.0f,4,false,false},
		
		{14.6f,0,false,false},
		{15.1f,2,false,false},
		{15.6f,4,false,false},
		{16.1f,1,false,false},
		{16.6f,0,false,false},
		{17.1f,3,false,false},
		{17.6f,2,false,false},
		{18.1f,4,false,false},
		
		{19.0f,0,false,false},
		{19.4f,4,false,false},
		{19.8f,1,false,false},
		{20.2f,2,false,false},
		{20.6f,0,false,false},
		{21.0f,2,false,false},
		{21.4f,3,false,false},
		{21.8f,4,false,false},
		
		{23.0f,0,false,false},
		{23.35f,2,false,false},
		{23.7f,1,false,false},
		{24.05f,4,false,false},
		{24.4f,0,false,false},
		{24.75f,3,false,false},
		{25.1f,1,false,false},
		{25.45f,2,false,false},
		
		{27.0f,0,false,false},
		{27.5f,4,false,false},
		{28.0f,1,false,false},
		{28.5f,2,false,false},
		{29.0f,0,false,false},
		{29.5f,2,false,false},
		{30.0f,1,false,false},
		{30.5f,3,false,false},
		
		{31.0f,2,false,false},
		{31.4f,0,false,false},
		{31.8f,4,false,false},
		{32.2f,1,false,false},
		{32.6f,2,false,false},
		{33.0f,0,false,false},
		{33.4f,1,false,false},
		{33.8f,3,false,false},
		
		{35.0f,0,false,false},
		{35.3f,1,false,false},
		{35.6f,4,false,false},
		{35.9f,2,false,false},
		{36.2f,0,false,false},
		{36.5f,2,false,false},
		{36.8f,1,false,false},
		{37.1f,3,false,false},
		
		{38.0f,4,false,false},
		{38.3f,1,false,false},
		{38.6f,0,false,false},
		{38.9f,2,false,false},
		{39.2f,3,false,false},
		{39.5f,0,false,false},
		{39.8f,4,false,false},
		{40.1f,1,false,false},
		
		{42.0f,0,false,false},
		{42.6f,4,false,false},
		{43.2f,1,false,false},
		{43.8f,2,false,false},
		{44.4f,0,false,false},
		{45.0f,2,false,false},
		{45.6f,3,false,false},
		{46.2f,4,false,false},
		
		{48.0f,0,false,false},
		{49.0f,4,false,false},
		{50.0f,1,false,false},
		{51.0f,2,false,false},
		{52.0f,0,false,false},
		{53.0f,3,false,false},
		{54.0f,1,false,false},
		{55.0f,4,false,false},
		{56.0f,0,false,false},
		{57.0f,3,false,false},
		{58.0f,1,false,false},
		{59.0f,2,false,false},
		{60.0f,4,false,false},
		{61.0f,3,false,false}
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
		
		//log
		string data ="game_end score=" + ofToString(score)
		+",button1=" + ofToString(button1Count)
		+",button2=" + ofToString(button2Count)
		+",button3=" + ofToString(button3Count)
		+",button4=" + ofToString(button4Count)
		+",button5=" + ofToString(button5Count);
		sendText2server(data);
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
