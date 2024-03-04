#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	//Let's create our buttons
	RedButton = new Button(ofGetWindowWidth()/2-20,ofGetWindowHeight()/2-260,302,239,"images/RedButton.png","sounds/RedButton.mp3");
	BlueButton = new Button(ofGetWindowWidth()/2+35,ofGetWindowHeight()/2-10,236,290,"images/BlueButton.png","sounds/BlueButton.mp3");
	YellowButton = new Button(ofGetWindowWidth()/2-260,ofGetWindowHeight()/2+40,287,239,"images/YellowButton.png","sounds/YellowButton.mp3");
	GreenButton = new Button(ofGetWindowWidth()/2-260,ofGetWindowHeight()/2-260,234,294,"images/GreenButton.png","sounds/GreenButton.mp3");

	//Load the glowing images for the buttons
	redLight.load("images/RedLight.png");
    blueLight.load("images/BlueLight.png");
    yellowLight.load("images/YellowLight.png");
    greenLight.load("images/GreenLight.png");

	//Load other images
	logo.load("images/Logo.png");
	logoLight.load("images/LogoLight.png");
	startUpScreen.load("images/StartScreen.png");
	gameOverScreen.load("images/GameOverScreen.png");

    //Load Music
	backgroundMusic.load("sounds/BackgroundMusic.mp3");
	backgroundMusic.setLoop(true);
	backgroundMusic.play();

	//Initial State
	gameState = StartUp;
}
//--------------------------------------------------------------
void ofApp::update(){

	//We will tick the buttons, aka constantly update them
	//while expecting input from the user to see if anything changed
	if(gameState == PlayerInput){
		RedButton->tick();
		BlueButton->tick();
		YellowButton->tick();
		GreenButton->tick();
		

		
		//sequence and we can proceed with the next level
		if(userIndex == sequenceLimit && !MultiplayerGameMode){ //If the amount of user input equals the sequence limit
			if(!ComputerGameModeActivated && !MultiplayerGameMode){ // if random mode is false generate sequence,
				generateSequence();
			}

			/*
			aqui voy a poner la logica de que genere both sequences para multipplayer.
			*/


			//sequence and we can proceed with the next level
			userIndex = 0;
			showingSequenceDuration = 0; 
			gameState = PlayingSequence;
			//sequence and we can proceed with the next level
		}
	}

    

	//This will take care of turning on the lights after a few
	//ticks so that they dont stay turned on forever or too long
	if(lightDisplayDuration > 0){
		lightDisplayDuration--;
		if(lightDisplayDuration <= 0){
			lightOff(RED);
			lightOff(BLUE);
			lightOff(YELLOW);
			lightOff(GREEN);
		}
	}

	// if (MultiplayerGameMode && gameState == PlayerInput) {		
	// 	// Check if it's Player 1's turn
	// 	if (currentPlayer == 1) {
	// 		currentPlayer = 2;
	// 	} else {
	// 		currentPlayer = 1;
	// 	}

	// 	// Generate a new sequence for Player 2
	// 	generateSequenceForPlayer(2);
	// 	// Set game state to PlayingSequence for Player 2's turn
	// 	userIndex = 0;
	// 	showingSequenceDuration = 0;
	// 	gameState = PlayingSequence;
	// }

}

//--------------------------------------------------------------
void ofApp::draw(){
	//Create the background
    ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));

	//Draw the buttons
	ofSetColor(255,0,0);
	RedButton->render();
	BlueButton->render();
	YellowButton->render();
	GreenButton->render();

	if(ComputerGameModeActivated){
		ofSetColor(255);
		ofDrawCircle(100, 100, 33);
		if(gameState == PlayerInput){
			ofSetColor(255, 100, 100);
		}else if(gameState == PlayingSequence){
			ofSetColor(100, 255, 100);
		}else{
			ofSetColor(255);
		}
		ofDrawCircle(100, 100, 30);
		ofSetColor(255);
	}
	
	//This whole if statement will take care of showing
	//the sequence to the user before accepting any input
	if(gameState == PlayingSequence){
		showingSequenceDuration++;
		if(showingSequenceDuration == 120){
			if(!MultiplayerGameMode){
				color = Sequence[userIndex];
			}else if(currentPlayer==1 && MultiplayerGameMode){
				color = player1Sequence[userIndex];
			}else if (currentPlayer==2 && MultiplayerGameMode){
				color = player2Sequence[userIndex];
			}
			lightOn(color);
			lightDisplayDuration = 30;
		}
	
		if(showingSequenceDuration == 140){
			lightOff(color);
			showingSequenceDuration = 60;
			userIndex++;
		}
		int limit =1;
		if(!MultiplayerGameMode){
			limit = sequenceLimit;
		}else if(currentPlayer==1){
			limit = player1Sequence.size();
		}else if (currentPlayer==2){
			limit = player2Sequence.size();
		}
		if(userIndex == limit){
			lightOff(color);
			userIndex = 0;
			gameState = PlayerInput;
		}
	}

	

	//StartUP (You dont need to pay much attention to this)
	//(This is only to create a animation effect at the start of the game)
	if(gameState == StartUp){
		showingSequenceDuration++;
		startUpSequence(showingSequenceDuration);
	}

	//If the statements to see see if the buttons should be lit up
	//If they are then we will draw the glowing images on top of them
	ofSetColor(255, 255, 255, 100);
	if (RedButton->GetIsLightUp()) {
		redLight.draw(ofGetWindowWidth()/2-60, ofGetWindowHeight()/2-305, 376, 329);
	}
	if (BlueButton->GetIsLightUp()) {
		blueLight.draw(ofGetWindowWidth()/2+5, ofGetWindowHeight()/2-60, 309, 376);
	} 
	if (YellowButton->GetIsLightUp()) {
		yellowLight.draw(ofGetWindowWidth()/2-300, ofGetWindowHeight()/2+5, 374, 318);
	} 
	if (GreenButton->GetIsLightUp()) {
		greenLight.draw(ofGetWindowWidth()/2-307, ofGetWindowHeight()/2-295, 315, 356);
	}
	ofSetColor(255);

	//Part of the Start Up
	if(logoIsReady){
		logo.draw(ofGetWindowWidth()/2-160,ofGetWindowHeight()/2-150,330,330);
	}
	// highscore logic
	//Draw the game over screen
	if(gameState == GameOver){
		gameOverScreen.draw(0,0,1024,768);
		curentScore1 = player1Sequence.size() - 1;
		curentScore2 = player2Sequence.size() - 1;
		if (MultiplayerGameMode && win == "player 1"){
			curentScore1 = curentScore1 + 1;
		}
		else if (MultiplayerGameMode && win == "player 2"){
			curentScore2 = curentScore2 + 1;
		}
		if (curentScore1 > highestScore){
			highestScore = curentScore1;
		}
		if (curentScore2 > highestScore){
			highestScore = curentScore2;
		}
	}

	//This will draw the "Press to Start" screen at the beginning
	else if(!idle && gameState == StartUp){
		startUpScreen.draw(20,0,1024,768);
	}

	//Debug
	ofDrawBitmapString("Player: " + to_string(currentPlayer), 10, 20);
	
	auto gs = gameState;
	if (gameState==StartUp) ofDrawBitmapString("State: StartUp", 10, 35);
	if (gameState==PlayingSequence) ofDrawBitmapString("State: PlayingSequence", 10, 35);
	if (gameState==PlayerInput) ofDrawBitmapString("State: PlayerInput", 10, 35);
	if (gameState==GameOver) ofDrawBitmapString("State: GameOver", 10, 35);

	if (MultiplayerGameMode) ofDrawBitmapString("Multiplayer", 10, 50);
	if (ComputerGameModeActivated) ofDrawBitmapString("Computer gamemode", 10, 65);
	ofDrawBitmapString("SS: " + to_string(showingSequenceDuration), 10, 80);
	ofDrawBitmapString("userIndex: " + to_string(userIndex), 10, 95);
}

//--------------------------------------------------------------
void ofApp::GameReset(){
	//This function will reset the game to its initial state
	lightOff(RED);
	lightOff(BLUE);
	lightOff(YELLOW);
	lightOff(GREEN);
	userIndex = 0;
	Sequence.clear();
	player1Sequence.clear();
    player2Sequence.clear();
	p1_points = 0;
	p2_points = 0;
	currentPlayer = 1;
	if(ComputerGameModeActivated){ //if computer mode was already activated it will ask for input
		// Record mode
		gameState = PlayerInput;
	}
	else if (MultiplayerGameMode){
		//if multiplayer was active it will play sequence of player one
		//its going to add a sequence for both player in the memory
		generateSequenceForPlayer(1);
		gameState = PlayingSequence;
	}
	else{
		// Normal game mode
		generateSequence();
		gameState = PlayingSequence;
	}
	showingSequenceDuration = 0;
}

void ofApp::generateSequenceForPlayer(int currentPlayer){
    int random = ofRandom(4);
    Buttons newButton = static_cast<Buttons>(random);
    if(currentPlayer == 1){
        player1Sequence.push_back(newButton);
    } else {
        player2Sequence.push_back(newButton);
    }
}

//--------------------------------------------------------------
void ofApp::generateSequence(){

	//This function will generate a random number between 0 and 3
	int random = ofRandom(4);
	
	//Depending on the random number, we will add a button to the sequence
	if(random == 0){
		Sequence.push_back(RED);
	}
	else if(random == 1){
		Sequence.push_back(GREEN);
	}
	else if(random == 2){
		Sequence.push_back(BLUE);
	}
	else if(random == 3){
		Sequence.push_back(YELLOW);
	}

	//We will adjust the sequence limit to the new size of the Sequence list
	sequenceLimit = Sequence.size();
}
//--------------------------------------------------------------
bool ofApp::checkUserInput(Buttons input){
	//This function will varify if the user input matches the color
	//of the sequence at the current index
	if(Sequence[userIndex] == input){
		return true;
	}
	else{
		return false;
	}
}

bool ofApp::checkMultyInput(Buttons input){
	if (currentPlayer == 1) {
		if (player1Sequence[userIndex] == input) {
			p1_points++;
			return true;
		} else {
			return false;
		}
	} else {
		if (player2Sequence[userIndex] == input) {
			p2_points++;
			return true;
		} else {
			return false;
		}
	}

	return false;	

	//This function will varify if the user input matches the color
	//of the sequence at the current index
}


//--------------------------------------------------------------
void ofApp::lightOn(Buttons color){
	//This function will take care of toggling the "isLightUp" variable to
	//true for the button that matches the color, and also play the button sound
	if(color == RED){
		RedButton->toggleLightOn();
        RedButton->playSound();
	}
	else if(color == BLUE){
		BlueButton->toggleLightOn();
        BlueButton->playSound();
	}
	else if(color == YELLOW){
		YellowButton->toggleLightOn();
        YellowButton->playSound();
	}
	else if(color == GREEN){
		GreenButton->toggleLightOn();
        GreenButton->playSound();
	}
}

//--------------------------------------------------------------
void ofApp::lightOff(Buttons color){
	//This function will take care of toggling the "isLightUp" variable to false
	if(color == RED){
		RedButton->toggleLightOff();
	}
	else if(color == BLUE){
		BlueButton->toggleLightOff();
	}
	else if(color == YELLOW){
		YellowButton->toggleLightOff();
	}
	else if(color == GREEN){
		GreenButton->toggleLightOff();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//As long as we're not in Idle OR the gameState is GameOver;
	//AND we press the SPACEBAR, we will reset the game
	if((!idle || gameState == GameOver) && tolower(key) == ' '){
		ComputerGameModeActivated = false;
		MultiplayerGameMode = false;
		GameReset();
	} else if(ComputerGameModeActivated && tolower(key) == 'r'){
		gameState = PlayingSequence;
		showingSequenceDuration = 0;
		userIndex = 0;
	} else if(key == OF_KEY_BACKSPACE){
		ComputerGameModeActivated = false;
		MultiplayerGameMode = false;
		gameState = StartUp;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int mouseX, int mouseY ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	//If we're not in Idle and the gameState equals PlayerInput,
	//We will pay attention to the mousePresses from the user

	
	if(gameState == StartUp){
        RedButton->setPressed(x,y);
        if(RedButton->wasPressed()){
			lightOff(RED);
			lightOff(BLUE);
			lightOff(YELLOW);
			lightOff(GREEN);
            ComputerGameModeActivated = true;
			gameState = PlayerInput;
        }
		
        BlueButton->setPressed(x,y);
        if(BlueButton->wasPressed()){
			lightOff(RED);
			lightOff(BLUE);
			lightOff(YELLOW);
			lightOff(GREEN);
			BlueButton->setIsPressed(false);
            MultiplayerGameMode = true;
			showingSequenceDuration = 0;
			player1Sequence.clear();
			player2Sequence.clear();
			generateSequenceForPlayer(1);
			p1_points = 0;
			p2_points = 0;
			currentPlayer = 1;
			userIndex = 0;
			gameState = PlayingSequence;
			idle = false;
        }
    }


	else if(!idle && gameState == PlayerInput && MultiplayerGameMode){//
		// New game mode!

		//We mark the prebbssed button as "pressed"
		RedButton->setPressed(x,y);
		BlueButton->setPressed(x,y);
		YellowButton->setPressed(x,y);
		GreenButton->setPressed(x,y);

		//here write that blue is unpressed
		//We check which button got pressed
		if(RedButton->wasPressed()){
			color = RED;
		}
		else if(BlueButton->wasPressed()){
			color = BLUE;
		}
		else if(YellowButton->wasPressed()){
			color = YELLOW;
		}
		else if(GreenButton->wasPressed()){
			color = GREEN;
		}
		lightOn(color);
		//Light up the pressed button for a curee ticks
		lightDisplayDuration = 15;
		//necesito que brille el sequence
		gameState = PlayerInput;
		if(currentPlayer == 1){
			if(checkMultyInput(color)){
				userIndex++;
				if(userIndex == player1Sequence.size()){
					currentPlayer++;
					generateSequenceForPlayer(currentPlayer);
					userIndex = 0;
					showingSequenceDuration = 0;
					gameState = PlayingSequence;
				}
			}else{
				loose = "player 1";
				win = "player 2";
				gameState = GameOver;
			}
		}else if (currentPlayer ==2){
			if(checkMultyInput(color)){
				userIndex++;
				if(userIndex == player2Sequence.size()){
					currentPlayer--;
					generateSequenceForPlayer(currentPlayer);
					userIndex = 0;
					showingSequenceDuration = 0;
					gameState = PlayingSequence;
				}

			}else{
				loose = "player 2";
				win = "player 1";
				gameState = GameOver;
			}
		}
	}
		
	else if(!idle && gameState == PlayerInput && ComputerGameModeActivated){
		// New game mode!

		//We mark the prebbssed button as "pressed"
		RedButton->setPressed(x,y);
		BlueButton->setPressed(x,y);
		YellowButton->setPressed(x,y);
		GreenButton->setPressed(x,y);
		
		//We check which button got pressed
		if(RedButton->wasPressed()){
			color = RED;
		}
		else if(BlueButton->wasPressed()){
			color = BLUE;
		}
		else if(YellowButton->wasPressed()){
			color = YELLOW;
		}
		else if(GreenButton->wasPressed()){
			color = GREEN;
		}
		//Light up the pressed button for a few ticks
		lightOn(color);
		lightDisplayDuration = 15;
		Sequence.push_back(color);
		sequenceLimit = Sequence.size();
	}
	else if(!idle && gameState == PlayerInput && !ComputerGameModeActivated && !MultiplayerGameMode){
		// Normal game mode

		//We mark the pressed button as "pressed"
		RedButton->setPressed(x,y);
		BlueButton->setPressed(x,y);
		YellowButton->setPressed(x,y);
		GreenButton->setPressed(x,y);
		
		//We check which button got pressed
		if(RedButton->wasPressed()){
			color = RED;
		}
		else if(BlueButton->wasPressed()){
			color = BLUE;
		}
		else if(YellowButton->wasPressed()){
			color = YELLOW;
		}
		else if(GreenButton->wasPressed()){
			color = GREEN;
		}
		//Light up the pressed button for a few ticks
		lightOn(color);
		lightDisplayDuration = 15;
		
		//If the user input is correct, we can continue checking
		if(checkUserInput(color)){
			userIndex++;
		}else{
			//If not, then we will terminate the game by 
			//putting it in the GameOver state.
			gameState = GameOver;	}
	}
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

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
//--------------------------------------------------------------

//You may honestly ignore this function, shouldnt be something you need
//to change or anything. It's only for the start up animation. But ofc,
//If you wish to make something out of it or make it better, be my guest.
void ofApp::startUpSequence(int count){
	
	if(count < 200){
		GreenButton->toggleLightOn();
	}
	else if(count >= 200 && count < 260){
		GreenButton->toggleLightOff();
		RedButton->toggleLightOn();
	}
	else if(count >= 260 && count < 320){
		RedButton->toggleLightOff();
		BlueButton->toggleLightOn();
	}
	else if(count >= 320 && count < 380){
		BlueButton->toggleLightOff();
		YellowButton->toggleLightOn();
	}
	else if(count >= 380 && count < 440){
		YellowButton->toggleLightOff();
	}
	else if(count >= 440 && count < 500){
		GreenButton->toggleLightOn();
		RedButton->toggleLightOn();
		YellowButton->toggleLightOn();
		BlueButton->toggleLightOn();
	}
	else if(count >= 500 && count < 560){
		GreenButton->toggleLightOff();
		RedButton->toggleLightOff();
		YellowButton->toggleLightOff();
		BlueButton->toggleLightOff();
	}
	else if(count >= 560){
		showingSequenceDuration = 400;
	}

	//Logo Drawing
	if(logoIsReady && logoCounter > 0){
		logoCounter--;
		ofSetColor(256,256,256,logoCounter);
		logoLight.draw(ofGetWindowWidth()/2-160,ofGetWindowHeight()/2-150,330,330);
		ofSetColor(256,256,256);
	}
	if((count > 375) && !logoIsReady){
		logoCounter++;

		ofSetColor(256,256,256,logoCounter);
		logoLight.draw(ofGetWindowWidth()/2-160,ofGetWindowHeight()/2-150,330,330);

		ofSetColor(256,256,256,logoCounter);
		logo.draw(ofGetWindowWidth()/2-160,ofGetWindowHeight()/2-150,330,330);

		ofSetColor(256,256,256);
	}
	if(logoCounter >=255){
		logoIsReady = true;
		idle = false;
	}
}
