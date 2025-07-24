//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	Score_1->Text = 0;
	Score_2->Text = 0;
	ComputerDrawnHand->Text = "";
	PlayerDrawnHand->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ScissorsButtonClick(TObject *Sender)
{
	String computerHand = ComputerRandomChoice();
	String playerHand = "Scissors";
    PlayerDrawnHand->Text = playerHand;
	ComputerDrawnHand->Text = computerHand;
	RoundWinner(playerHand, computerHand);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PaperButtonClick(TObject *Sender)
{
   String computerHand = ComputerRandomChoice();
   String playerHand = "Paper";
   PlayerDrawnHand->Text = playerHand;
   ComputerDrawnHand->Text = computerHand;
   RoundWinner(playerHand, computerHand);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RockButtonClick(TObject *Sender)
{
	String computerHand = ComputerRandomChoice();
	String playerHand = "Rock";
	PlayerDrawnHand->Text = playerHand;
	ComputerDrawnHand->Text = computerHand;
	RoundWinner(playerHand, computerHand);
}
//---------------------------------------------------------------------------

String TForm1::ComputerRandomChoice(){

	int randomChoice = rand() % 3;
	String computerDrawnChoice;

		if (randomChoice == 0)
			computerDrawnChoice = "Rock";
		else if(randomChoice == 1)
			computerDrawnChoice = "Paper";
		else
			computerDrawnChoice = "Scissors";

	  return computerDrawnChoice;
}
//---------------------------------------------------------------------------

void TForm1::RoundWinner(String &PlayerDrawnHand, String &ComputerDrawnHand){
	String roundWinner;

	if( (PlayerDrawnHand == "Rock" && ComputerDrawnHand == "Scissors") ||
		(PlayerDrawnHand == "Paper" && ComputerDrawnHand == "Rock") ||
		(PlayerDrawnHand == "Scissors" && ComputerDrawnHand == "Paper")
	  ){
		  Winner->Text = "Player Wins";
		  playerWins++;
		  Score_1->Text = playerWins;
	  }
	else if(PlayerDrawnHand == ComputerDrawnHand){
		Winner->Text = "Neither";
	}
	else{
		Winner->Text = "Computer Wins";
		computerWins++;
		Score_2->Text = computerWins;
	}
}

void TForm1::GameWinner(){
}

//---------------------------------------------------------------------------




void __fastcall TForm1::StartServerButtonClick(TObject *Sender)
{
	IdTCPServer1->DefaultPort = 6000;
	IdTCPServer1->Active = true;
	Memo1->Lines->Add("Starting server...");
}
//---------------------------------------------------------------------------









void __fastcall TForm1::IdTCPServer1AfterBind(TObject *Sender)
{
    //working
}
//---------------------------------------------------------------------------

void __fastcall TForm1::IdTCPServer1Execute(TIdContext *AContext)
{
	//Working
	try{
        Memo1->Lines->Clear();
        Memo1->Lines->Add("Player choose your hand");
	}   catch(const Exception &e){
        Memo1->Lines->Add(e.Message);
    }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::ConnectClick(TObject *Sender)
{
	try{

		IdTCPClient1->Connect();
        Memo1->Lines->Add("Successful Connection to server");

	}   catch(Exception &e){
		Memo1->Lines->Add("You can not connect to the server");
	}
}
//---------------------------------------------------------------------------

