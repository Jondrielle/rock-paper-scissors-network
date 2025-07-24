#include <fmx.h>
#pragma hdrstop

#include "Client.h"
#include "CheckDataThread.h"
#include "JSONReaderThread.h"

#pragma package(smart_init)
#pragma resource "*.fmx"

TForm2 *Form2;

// Constructor
__fastcall TForm2::TForm2(TComponent* Owner)
    : TForm(Owner),jsonReaderThread(nullptr)
{
    currentGameMode = GamePhase::PREGAME;
    GameLabels["PlayerScore"] = PlayerScore;
    GameLabels["OpponentScore"] = OpponentScore;
    GameLabels["RoundCount"] = RoundCount;
    GameLabels["Winner"] = Winner;
}


void TForm2::SetPlayerScore(String score)
{
    PlayerScore->Text = score;
}

void TForm2::SetOpponentScore(String score)
{
    OpponentScore->Text = score;
}

void TForm2::SetWinner(String winner)
{
    Winner->Text = "Winner: " + winner;
}

void TForm2::SetRoundCount(String count)
{
    RoundCount->Text = "Round: " + count;
}

void __fastcall TForm2::ConnectToServerClick(TObject *Sender)
{
    // Initialize connection to the server
    try
    {
        IdTCPClient1->Connect();
        String playerValue = IdTCPClient1->IOHandler->ReadLn();
        SetIsPlayerOne(playerValue);
        SetFormName(IdTCPClient1->IOHandler->ReadLn());
        StartJSONReaderThread();
    }
    catch (const Exception &e)
    {
        Memo1->Lines->Add("Error connecting to server: " + e.Message);
    }
}

void __fastcall TForm2::RockButtonClick(TObject *Sender)
{
    // Send choice to server
    IdTCPClient1->IOHandler->WriteLn("Rock");
}

void __fastcall TForm2::PaperButtonClick(TObject *Sender)
{
    // Send choice to server
    IdTCPClient1->IOHandler->WriteLn("Paper");
}

void __fastcall TForm2::ScissorsButtonClick(TObject *Sender)
{
    // Send choice to server
    IdTCPClient1->IOHandler->WriteLn("Scissors");
}

void __fastcall TForm2::IdTCPClient1Connected(TObject *Sender)
{
    Memo1->Lines->Add("Connected to server");
}

// Define the game phase (pregame, round, postgame)
TForm2::GamePhase TForm2::GetGameMode() const
{
    return currentGameMode;
}

void __fastcall TForm2::SetGameMode(TForm2::GamePhase newGameMode)
{
    currentGameMode = newGameMode;
}

// Start the JSONReaderThread
void TForm2::StartJSONReaderThread()
{
    Memo1->Lines->Add("Starting JSONReaderThread...");
    if (!jsonReaderThread )
    {
        String filePath = System::Ioutils::TPath::Combine(System::Ioutils::TPath::GetDocumentsPath(), "game.json");
        jsonReaderThread = new JSONReaderThread(this, filePath);  // Create and pass form instance
        jsonReaderThread->Start();  // Start the thread
        Memo1->Lines->Add("JSONReaderThread started.");
    }
}

// Stop the JSONReaderThread
void TForm2::StopJSONReaderThread()
{
    if (jsonReaderThread)
    {
        //jsonReaderThread->Terminate();  // Terminate the thread
        jsonReaderThread = nullptr;     // Nullify the thread pointer
    }
}

void TForm2::SetIsPlayerOne(String &isPlayerOneStr){
        if(isPlayerOneStr == "true"){
             isPlayerOne =
              true;
        }else{
             isPlayerOne = false;
        }
        Memo1->Lines->Add("Set isPlayerOne");
}

bool TForm2::GetIsPlayerOne(){
        return isPlayerOne;
}

void TForm2::SetFormName(String newFormName){
    TThread::Synchronize(nullptr, [this, newFormName] {
        this->Caption = newFormName;
    });
}


