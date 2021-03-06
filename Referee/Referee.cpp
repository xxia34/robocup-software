#include <math.h>
#include <qfont.h>
#include "Referee.hpp"


Referee::Referee(GameControl &gc) :
	QMainWindow()
{
	
	setupUi(this);

	/** Initialize game variables */
	Referee::currPeriod = FirstHalf;
	
	//currPeriod = 0;
	currGameState=0;
	Referee::currMilliseconds=0;
	Referee::maxMilliseconds=Referee::_preGameMsecs;

	/** Initialize goals */ 
	Referee::blueGoals=0;
	Referee::yellowGoals=0;

	/** Initialize current time out timers */
	Referee::currBlueTimeOutTimer=0;
	Referee::currYellowTimeOutTimer=0;

	/** Initialize amount of time outs left */
	Referee::blueTimeOuts=4;
	Referee::yellowTimeOuts=4;

	/** connect to timer for sending ref packets out */
	connect(&_txTimer, SIGNAL(timeout()), SLOT(txSend()));

	/** Connect to the timer that keeps track of the game state. */
	connect(&_gameTimer, SIGNAL(timeout()), SLOT(gameUpdate()));

	/** Connect to the timer that controls blue team's time outs */
	connect(&_yellowTimeOutTimer, SIGNAL(timeout()), SLOT(yellowTimeOutUpdate()));

	/** Connect to the timer that controls yellow team's time outs */
	connect(&_blueTimeOutTimer, SIGNAL(timeout()), SLOT(blueTimeOutUpdate()));
	
	/** Connect to the timer that handles idle time processing */
	connect(&_idleTimer, SIGNAL(timeout()), SLOT(idle()));

	/** setup transmit timer */
	_txTimer.start(1000/Referee::_hz);
	
	// For testing only - remove when finished
	//BluePenaltyButton->setEnabled(false);
	//BluePenaltyButton->setDisabled(true);
	//BlueYellowCardButton->setText("Mitch");
	//TimeRemainingYellowText->setText("Mitch");
	
	//_idleTimer.start(1000);

}

Referee::~Referee()
{
	
}

///This is the function that is called when the game timer times out.  This is executed several times per second, based on the value of _hz.
void Referee::txSend()
{

}

///This function updates the timers for time outs
void Referee::blueTimeOutUpdate()
{
	printf("Debug - Testing blueTimeOutUpdate .\n");
	
	///Check to see if current time exceeds max time possible in current period.
	if(Referee::currBlueTimeOutTimer >= Referee::_timeOutMsecs) {

		///Stop game timer, set the gamestate to halted and increment the game period.
		_blueTimeOutTimer.stop();
		Referee::startGameTimer();
		///Reset the timer if there are still time outs left.
		if(blueTimeOuts>=1) {
			currBlueTimeOutTimer=0;
			Referee::updateBlueTimeOutRemaining();
			Referee::updateBlueTimeOutLabels();
		}
	
	} else {
		///Increment the timer if the game if time is still left.
		Referee::currBlueTimeOutTimer+=100;
		//DEBUG
		//printf("Current milliseconds: %d\n", Referee::currBlueTimeOutTimer);

		///Update our labels.
		Referee::updateBlueTimeOutRemaining();
		Referee::updateBlueTimeOutLabels();
	}


}

///This function updats the timers for time outs
void Referee::yellowTimeOutUpdate()
{
	printf("Debug - Testing yellowTimeOutUpdate .\n");
	
	///Check to see if current time exceeds max time possible in current period.
	if(Referee::currYellowTimeOutTimer >= Referee::_timeOutMsecs) {

		///Stop game timer, set the gamestate to halted and increment the game period.
		_yellowTimeOutTimer.stop();
		Referee::startGameTimer();
		///Reset the timer if there are still time outs left.
		if(yellowTimeOuts>=1) {
			currYellowTimeOutTimer=0;
			Referee::updateYellowTimeOutRemaining();
			Referee::updateYellowTimeOutLabels();
		}
	
	} else {
		///Increment the timer if the game if time is still left.
		Referee::currYellowTimeOutTimer+=100;
		//DEBUG
		//printf("Current milliseconds: %d\n", Referee::currYellowTimeOutTimer);

		///Update our labels.
		Referee::updateYellowTimeOutRemaining();
		Referee::updateYellowTimeOutLabels();
	}


}

///This function updates the labels of the game timers when the game is being played.
void Referee::gameUpdate()
{
	printf("Debug - Testing gameUpdate .\n");
	
	
	
	///Check to see if current time exceeds max time possible in current period.
	if(Referee::currMilliseconds >= Referee::maxMilliseconds) {

		///Stop game timer, set the gamestate to halted and increment the game period.
		Referee::stopGameTimer();
		Referee::incrementCurrentPeriod();

	} else {
		///Increment the timer if the game if time is still left.
		Referee::currMilliseconds+=100;
		//DEBUG
		//printf("Current milliseconds: %d\n", Referee::currMilliseconds);

		//Update our labels.
		Referee::updateTimeElapsed();
		Referee::updateTimeRemaining();
	}

}

///Start of blue buttons code
void Referee::on_BlueGoalButton_clicked()
{
	//bool result;
	//printf("Debug - Blue Goal.\n");
	//result = gameControl.goalScored(Blue);
	
	gameControl.goalScored(Blue);

}

void Referee::on_MinusBlueButton_clicked()
{
	//bool result;
	//printf("Debug - Blue Goal Minus.\n");
	//result = gameControl.removeGoal(Blue);
	
	gameControl.removeGoal(Blue);

}

void Referee::on_BlueTimeOutButton_clicked()
{
	
	///Make sure blue timer is not active
//	if(_blueTimeOutTimer.timerId() == -1) {
//		///Make sure there are enough time outs, that another time out clock isn't running, and that the game is playing
//		if( (blueTimeOuts >= 1) && (_yellowTimeOutTimer.timerId()==-1) && (currGameState==1)) {
//			_blueTimeOutTimer.start(100);
//			Referee::stopGameTimer();
//			Referee::blueTimeOuts--;
//			Referee::updateBlueTimeOutLabels();
//		}
//	}
	
	//bool result;
	//printf("Debug - Blue Time Out.\n");
	//result = gameControl.beginTimeout(Blue);
	
	gameControl.beginTimeout(Blue);
}

void Referee::on_BluePenaltyButton_clicked()
{

	//printf("Debug.\n");
	//printf("Debug - Blue Penalty.\n");
	
	gameControl.setPenalty(Blue);

}

void Referee::on_BlueKickOffButton_clicked()
{
	//bool result;
	//printf("Debug - Blue Kickoff.\n");
	//result = gameControl.setKickoff(Blue);
	
	gameControl.setKickoff(Blue);

}

void Referee:: on_BlueFreeKickButton_clicked()
{
	//bool result;
	//printf("Debug - Blue Free Kick.\n");
	//result = gameControl.setDirect(Blue);
	
	gameControl.setKickoff(Blue);

}

void Referee::on_BlueIndirectButton_clicked()
{
	//bool result;
	//printf("Debug - Blue Indirect.\n");
	//result = gameControl.setIndirect(Blue);
	
	gameControl.setIndirect(Blue);

}

void Referee::on_BlueYellowCardButton_clicked()
{
	//bool result;
	//printf("Debug - Blue Yellow Card.\n");
	//result = gameControl.awardYellowCard(Blue);
	
	gameControl.setIndirect(Blue);

}

void Referee::on_BlueRedCardButton_clicked()
{
	//bool result;
	//printf("Debug - Blue Red Card.\n");
	//result = gameControl.awardRedCard(Blue);
	
	gameControl.awardRedCard(Blue);

}

/// Start of yellow buttons code ///

void Referee::on_YellowGoalButton_clicked()
{
	//bool result;
	//printf("Debug - Yellow Goal .\n");
	//result = gameControl.goalScored(Yellow);
	
	gameControl.goalScored(Yellow);

}

void Referee::on_MinusYellowButton_clicked()
{
	//printf("Debug - Yellow Goal Minus.\n");
	
	gameControl.removeGoal(Yellow);
}

void Referee::on_YellowTimeOutButton_clicked()
{
	
	///Make sure yello timer is not active
//	if(_yellowTimeOutTimer.timerId() == -1) {
		///Make sure there are enough time outs, that another time out clock isn't running, and that the game is playing
//		if( (yellowTimeOuts >= 1) && (_blueTimeOutTimer.timerId()==-1) && (currGameState==1)) {
//			_yellowTimeOutTimer.start(100);
//			Referee::stopGameTimer();
//			Referee::yellowTimeOuts--;
//			Referee::updateYellowTimeOutLabels();
//		}
//	}
//printf("Debug.\n");
	
	//bool result;
	//printf("Debug - Yellow Time Out.\n");
	//result = gameControl.beginTimeout(Yellow);
	
	gameControl.beginTimeout(Yellow);

}

void Referee::on_YellowPenaltyButton_clicked()
{
    //bool result;
	//printf("Debug - Yellow Penalty.\n");
	//result = gameControl.setPenalty(Yellow);
	//printf("Testing Set Penalty: %s\n", result ? "true" : "false");
	
	gameControl.setPenalty(Yellow);

}

void Referee::on_YellowKickOffButton_clicked()
{
	//bool result;
	//printf("Debug - Yellow Kickoff.\n");
	//result = gameControl.setKickoff(Yellow);
	
	gameControl.setKickoff(Yellow);

}

void Referee::on_YellowFreeKickButton_clicked()
{
    //bool result;
	//printf("Debug - Yellow Freekick.\n");
	//result = gameControl.setDirect(Yellow);
	
	gameControl.setDirect(Yellow);

}

void Referee::on_YellowIndirectButton_clicked()
{
	//bool result;
	//printf("Debug - Yellow Indirect.\n");
	//result = gameControl.setIndirect(Yellow);
	
	gameControl.setIndirect(Yellow);
	
}

void Referee::on_YellowYellowCardButton_clicked()
{
	//bool result;
	//printf("Debug - Yellow Yellow Card.\n");
	//result = gameControl.awardYellowCard(Yellow);
	
	gameControl.awardYellowCard(Yellow);

}

void Referee::on_YellowRedCardButton_clicked()
{
	//bool result;
	//printf("Debug - Yellow Red Card.\n");
	//result = gameControl.awardRedCard(Yellow);
	
	gameControl.awardRedCard(Yellow);

}

///Start of game buttons code
void Referee::on_StartTimeButton_clicked()
{
	//Referee::startGameTimer();
	
	// temporarily commented out
	//bool result;
	//printf("Debug - Start Time.\n");
	//result = gameControl.setReady();
	
	gameControl.setReady();

}

void Referee::on_StopTimeButton_clicked()
{
	//Referee::stopGameTimer();
	
	// temporarily commented out
	//bool result;
	//printf("Debug - Stop Time.\n");
	//result = gameControl.setHalt();
	
	gameControl.setHalt();

}

void Referee::on_StartGameButton_clicked()
{
	// temporarily commented out
	//printf("Debug.\n");
	//bool result;
	//printf("Debug - Start Game.\n");
	//result = gameControl.setStart();
	
	gameControl.setStart();

}

void Referee::on_StopGameButton_clicked()
{
    // temporarily commented out
	//printf("Debug.\n");
	//bool result;
	//printf("Debug - Stop Game.\n");
	//result = gameControl.setStop();
	
	gameControl.setStop();

}

void Referee::on_CancelButton_clicked()
{
    // temporarily commented out
	//printf("Debug.\n");
	//bool result;
	//printf("Debug - Cancel.\n");
	//result = gameControl.setCancel();
	
	gameControl.setCancel();

}

///This function updates the Time Elapsed button inside the GUI.
void Referee::updateTimeElapsed()
{
	//Create a buffer string for our sprintf
	char buf[256];

	//Convert milliseconds to minutes, seconds and tenths of seconds
	int milliseconds=Referee::currMilliseconds;

	//Find Minutes
	int minutes=floor(milliseconds/(60*1000));
	milliseconds = milliseconds - (minutes*60*1000);

	//Find seconds
	int seconds=floor(milliseconds/(1000));
	milliseconds = milliseconds - (seconds*1000);

	//Find tenths
	int tenths = floor(milliseconds/(100));

	//Format string for use in label
	if(seconds > 10) {
		snprintf(buf, 100, "%d:%d.%d", minutes, seconds, tenths);
	} else {
		snprintf(buf, 100, "%d:0%d.%d", minutes, seconds, tenths);
	}

	//Put string into label
	TimeElapsedText->setText(buf);

	//DEBUG
	//printf("Time Elapsed: %s\n", buf);
		

}

///This function updates the Time Remaining button inside the GUI.
void Referee::updateTimeRemaining()
{
	//Create a buffer string for our sprintf
	char buf[256];
	
	//Convert milliseconds to minutes, seconds and tenths of seconds
	int milliseconds=Referee::maxMilliseconds - Referee::currMilliseconds;

	//Find Minutes
	int minutes=floor(milliseconds/(60*1000));
	milliseconds = milliseconds - (minutes*60*1000);

	//Find seconds
	int seconds=floor(milliseconds/(1000));
	milliseconds = milliseconds - (seconds*1000);

	//Find tenths
	int tenths = floor(milliseconds/(100));
	
	//Format string for use in label.
	if(seconds > 10) {
		snprintf(buf, 100, "%d:%d.%d", minutes, seconds, tenths);
	} else {
		snprintf(buf, 100, "%d:0%d.%d", minutes, seconds, tenths);
	}

	//Put string into label
	TimeRemainingText->setText(buf);

	//DEBUG
	//printf("Time Remaining: %s\n", buf);

}

void Referee::updateBlueTimeOutRemaining()
{
	//Create a buffer string for our sprintf
	char buf[256];
	
	//Convert milliseconds to minutes, seconds and tenths of seconds
	int milliseconds=Referee::_timeOutMsecs - Referee::currBlueTimeOutTimer;

	//Find Minutes
	int minutes=floor(milliseconds/(60*1000));
	milliseconds = milliseconds - (minutes*60*1000);

	//Find seconds
	int seconds=floor(milliseconds/(1000));
	milliseconds = milliseconds - (seconds*1000);

	//Find tenths
	int tenths = floor(milliseconds/(100));
	
	//Format string for use in label.
	if(seconds > 10) {
		snprintf(buf, 100, "%d:%d.%d", minutes, seconds, tenths);
	} else {
		snprintf(buf, 100, "%d:0%d.%d", minutes, seconds, tenths);
	}

	//Put string into label
	TimeRemainingBlueText->setText(buf);

	//DEBUG
	//printf("Time Remaining: %s\n", buf);


}

void Referee::updateYellowTimeOutRemaining()
{
	//Create a buffer string for our sprintf
	char buf[256];
	
	//Convert milliseconds to minutes, seconds and tenths of seconds
	int milliseconds=Referee::_timeOutMsecs - Referee::currYellowTimeOutTimer;

	//Find Minutes
	int minutes=floor(milliseconds/(60*1000));
	milliseconds = milliseconds - (minutes*60*1000);

	//Find seconds
	int seconds=floor(milliseconds/(1000));
	milliseconds = milliseconds - (seconds*1000);

	//Find tenths
	int tenths = floor(milliseconds/(100));
	
	//Format string for use in label.
	if(seconds > 10) {
		snprintf(buf, 100, "%d:%d.%d", minutes, seconds, tenths);
	} else {
		snprintf(buf, 100, "%d:0%d.%d", minutes, seconds, tenths);
	}

	//Put string into label
	TimeRemainingYellowText->setText(buf);

	//DEBUG
	//printf("Time Remaining: %s\n", buf);
}

void Referee::stopGameTimer()
{
	///The timer has AIDS.  Stop it from reproducing.
	//_gameTimer.stop();
	//Referee::currGameState=0;
	_idleTimer.stop();
}

void Referee::startGameTimer()
{
	//if(_gameTimer.timerId() == -1) {
		//Start the timer if it hasn't been started yet.
	//	_gameTimer.start(100);
		///MAKE SURE YOU CHANGE THIS!!!!
	//	currGameState=1;
	//} else {
		///Re-start the timer if it has already been started.
	//	_gameTimer.start();
	//}
	if(_idleTimer.timerId() == -1) {
		//Start the timer if it hasn't been started yet.
		_idleTimer.start(100);
		///MAKE SURE YOU CHANGE THIS!!!!
		//currGameState=1;
	} else {
		///Re-start the timer if it has already been started.
		_idleTimer.start();
	}
}

void Referee::incrementBlueGoals()
{

}

void Referee::incrementYellowGoals()
{

}

void Referee::decrementBlueGoals()
{

}

void Referee::decrementYellowGoals()
{

}

void Referee::incrementCurrentPeriod()
{
	switch(Referee::currPeriod) {
		case 0:
			static_cast<Period>(Referee::currPeriod + 1);
			Referee::currMilliseconds=0;
			Referee::maxMilliseconds=_firstHalfMsecs;
			Referee::resetTimeLabels();
			Referee::updatePeriodLabels();
			break;
		case 1:
			static_cast<Period>(Referee::currPeriod + 1);
			Referee::currMilliseconds=0;
			Referee::maxMilliseconds=_halfTimeMsecs;
			Referee::resetTimeLabels();
			Referee::updatePeriodLabels();
			break;
		case 2:
			static_cast<Period>(Referee::currPeriod + 1);
			Referee::currMilliseconds=0;
			Referee::maxMilliseconds=_secondHalfMsecs;
			Referee::resetTimeLabels();
			Referee::updatePeriodLabels();
			break;
		case 3:
			static_cast<Period>(Referee::currPeriod + 1);
			Referee::currMilliseconds=0;
			Referee::maxMilliseconds=_firstOvertimeMsecs;
			Referee::resetTimeLabels();
			Referee::updatePeriodLabels();
			break;
		case 4:
			static_cast<Period>(Referee::currPeriod + 1);
			Referee::currMilliseconds=0;
			Referee::maxMilliseconds=_secondOvertimeMsecs;
			Referee::resetTimeLabels();
			Referee::updatePeriodLabels();
			break;
		case 5:
			Referee::currPeriod=PreGame;
			Referee::currMilliseconds=0;
			Referee::maxMilliseconds=_preGameMsecs;
			Referee::resetTimeLabels();
			Referee::updatePeriodLabels();
			break;
		default:
			Referee::currPeriod=PreGame;
			Referee::currMilliseconds=0;
			Referee::maxMilliseconds=_preGameMsecs;
			Referee::resetTimeLabels();
			Referee::updatePeriodLabels();
			break;
	}
}

void Referee::resetTimeLabels()
{
	Referee::updateTimeElapsed();
	Referee::updateTimeRemaining();
	Referee::updatePeriodLabels();
}

void Referee::updatePeriodLabels()
{
	switch(Referee::currPeriod) {
		case 0:
			PeriodText->setText("Pre-Game");
			break;
		case 1:
			PeriodText->setText("First Half");
			break;
		case 2:
			PeriodText->setText("Half Time");
			break;
		case 3:
			PeriodText->setText("Second Half");
			break;
		case 4:
			PeriodText->setText("First Overtime");
			break;
		case 5:
			PeriodText->setText("Pre-Game");
			break;
		default:
			PeriodText->setText("Pre-Game");
			break;
	}
}

void Referee::updateBlueTimeOutLabels()
{
	///Update blue time out labels.
	char buf[16];
	snprintf(buf, 8, "%d", blueTimeOuts);
	BlueTimeOutsLeftText->setText(buf);
}

void Referee::updateYellowTimeOutLabels()
{
	///Update yellow time out labels.
	char buf[16];
	snprintf(buf, 8, "%d", yellowTimeOuts);
	YellowTimeOutsLeftText->setText(buf);
}

void Referee::setActiveWidgets(const EnableState &es)
{
	// Test
	printf("Debug - Inside setActiveWidgets.\n");
	
	/** Game Control Buttons */
	StartTimeButton->setEnabled(es.ready);
	StopTimeButton->setEnabled(es.halt);
	StartGameButton->setEnabled(es.start);
	StopGameButton->setEnabled(es.stop);
	CancelButton->setEnabled(es.cancel);
	
	/** Yellow Team Buttons*/
	YellowGoalButton->setEnabled(es.goal[Yellow]);
	MinusYellowButton->setEnabled(es.subgoal[Yellow]);
	YellowTimeOutButton->setEnabled(es.timeout[Yellow]);
	YellowPenaltyButton->setEnabled(es.penalty[Yellow]);
	YellowKickOffButton->setEnabled(es.kickoff[Yellow]);
	YellowFreeKickButton->setEnabled(es.direct[Yellow]);
	YellowIndirectButton->setEnabled(es.indirect[Yellow]);
	YellowYellowCardButton->setEnabled(es.cards);
	YellowRedCardButton->setEnabled(es.cards);
	
	/** Blue Team Buttons*/
	BlueGoalButton->setEnabled(es.goal[Blue]);
	MinusBlueButton->setEnabled(es.subgoal[Blue]);
	BlueTimeOutButton->setEnabled(es.timeout[Blue]);
	BluePenaltyButton->setEnabled(es.penalty[Blue]);
	BlueKickOffButton->setEnabled(es.kickoff[Blue]);
	BlueFreeKickButton->setEnabled(es.direct[Blue]);
	BlueIndirectButton->setEnabled(es.indirect[Blue]);
	BlueYellowCardButton->setEnabled(es.cards);
	BlueRedCardButton->setEnabled(es.cards);
	
}

void Referee::idle()
{
	   //Test
	   printf("Debug - Inside idle().\n");
	   
	   // First get new time step
	   gameControl.stepTime();
	   char str[1024];
	   
	   QFont sansSerifFont("Arial");
	   sansSerifFont.setStyleHint(QFont::Serif);
	   sansSerifFont.setPointSize(48);
	   sansSerifFont.setWeight(QFont::Bold);
	   
	   GameInfo gi = gameControl.getGameInfo();
	   
	   // Update the Gui
	   GameStatusText->setText(gi.getStateString());

	   /** deprecated */
	   //teamname_blue.set_text( gi.game.teamNames[Blue] );
	   //teamname_yellow.set_text( gi.game.teamNames[Yellow] );
	   

	   if (gi.game.stage == PENALTY_SHOOTOUT) {
	      // Needs extra handling.
	      sprintf(str, "%2i", gi.game.goals[Yellow]);
	      YellowScoreText->setText(str);
	      YellowScoreText->setFont(sansSerifFont);

	      sprintf(str, "%2i", gi.game.goals[Blue]);
	      BlueScoreText->setText(str);
	      BlueScoreText->setFont(sansSerifFont);

	      sprintf(str, "Penalties:\n %i - %i\n", gi.game.penaltyGoals[Yellow], gi.game.penaltyGoals[Blue]);
	      //time_label.set_text( str );
	   } else {
	      sprintf(str, "%2i", gi.game.goals[Yellow]);
	      YellowScoreText->setText(str);
	      YellowScoreText->setFont(sansSerifFont);
	      
	      sprintf(str, "%2i", gi.game.goals[Blue]);
	      BlueScoreText->setText(str);
	      BlueScoreText->setFont(sansSerifFont);

	      sprintf(str, "%s\n%2i:%04.1f\n%2i:%04.1f", 
	              gi.getStageString(),
	              DISP_MIN(gi.timeTaken()),
	              DISP_SEC(gi.timeTaken()),
	              DISP_MIN(gi.timeRemaining()), 
	              DISP_SEC(gi.timeRemaining()));
	      //time_label.set_text( str );
	   }
	   
	   sprintf(str, "%i", gi.nrTimeouts(Yellow) );
	   YellowTimeOutsLeftText->setText(str);
	   sprintf(str, "%i", gi.nrTimeouts(Blue) );
	   BlueTimeOutsLeftText->setText(str);
	   
	   sprintf(str, "%2i:%04.1f",DISP_MIN(gi.timeoutRemaining(Yellow)),DISP_SEC(gi.timeoutRemaining(Yellow)));
	   TimeRemainingYellowText->setText(str);

	   if (gi.penaltyTimeRemaining(Yellow) > 0) {
	     sprintf(str, "%2i:%04.1f",DISP_MIN(gi.penaltyTimeRemaining(Yellow)),DISP_SEC(gi.penaltyTimeRemaining(Yellow)));
	     YellowYellowCardButton->setText(str);
	     YellowYellowCardButton->setEnabled(false);
	   }
	   else {
		   YellowYellowCardButton->setText("Yellow Card"); 
		   YellowYellowCardButton->setEnabled(true);
	   }
	       
	   sprintf(str, "%2i:%04.1f",DISP_MIN(gi.timeoutRemaining(Blue)),DISP_SEC(gi.timeoutRemaining(Blue)));
	   TimeRemainingBlueText->setText(str);

	   if (gi.penaltyTimeRemaining(Blue) > 0) {
	     sprintf(str, "%2i:%04.1f",DISP_MIN(gi.penaltyTimeRemaining(Blue)),DISP_SEC(gi.penaltyTimeRemaining(Blue)));
	     BlueYellowCardButton->setText(str);
	     BlueYellowCardButton->setEnabled(false);
	   }
	   else {
		 BlueYellowCardButton->setText("Yellow Card");
		 BlueYellowCardButton->setEnabled(true);
	   }
	   
	   setActiveWidgets(gameControl.getEnableState());
	   
	   usleep(50000);
	   
}
