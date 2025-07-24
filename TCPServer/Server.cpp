#include <fmx.h>
#pragma hdrstop

#include "Server.h"
#include <System.IOUtils.hpp>

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.fmx"
TForm2 *Form2;

//---------------------------------------------------------------------------

__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
         IdTCPServer1->OnDisconnect = IdTCPServer1Disconnect;
}

//---------------------------------------------------------------------------

void __fastcall TForm2::StartServerClick(TObject *Sender)
{

    String filePath = System::Ioutils::TPath::Combine(System::Ioutils::TPath::GetDocumentsPath(), "game.json");
    try {

            if (!TFile::Exists(filePath)) {
                // Create an empty JSON file with "{}"
                TFile::WriteAllText(filePath, "{}");
                Memo1->Lines->Add("Empty JSON file created.");
            } else {
                Memo1->Lines->Add("JSON file already exists.");
            }
    } catch (const Exception &e) {
        Memo1->Lines->Add("Error creating file: " + e.Message);
        return;  // If there's an error, stop further execution.
    }

	IdTCPServer1->Active = true;
	Memo1->Lines->Add("Starting Server...");

        Memo1->Lines->Add("Created Empty Json File");
    	StartServer->Enabled = false;
}

//---------------------------------------------------------------------------

void __fastcall TForm2::IdTCPServer1Connect(TIdContext *AContext)
{
	clientCount++;

	if(clientCount == 1){
		Player[AContext] = true;
                AContext->Connection->IOHandler->WriteLn("true");
                AContext->Connection->IOHandler->WriteLn("Player1");
		Memo1->Lines->Add("Player one joined server!");
	}
	if(clientCount == 2){
		Player[AContext] = false;
                AContext->Connection->IOHandler->WriteLn("false");
                AContext->Connection->IOHandler->WriteLn("Player2");
		Memo1->Lines->Add("Player two joined server!");
	}
	else if(clientCount > 2){
		AContext->Connection->IOHandler->WriteLn("Game is full. Try again later");
		AContext->Connection->Disconnect();
        	clientCount--;
	}
}

//---------------------------------------------------------------------------

void __fastcall TForm2::IdTCPServer1Execute(TIdContext *AContext)
{
	try{
        playerMove = AContext->Connection->IOHandler->ReadLn();
        //AContext->Connection->IOHandler->WriteLn("Round:" + IntToStr(roundCount));

        if(Player[AContext] == true){
        	player1Choice = playerMove;
        }
        else{
        	player2Choice = playerMove;
        }

        if (!player1Choice.IsEmpty() && !player2Choice.IsEmpty()) {
            roundWinner = RoundWinner(player1Choice, player2Choice);
            CreateJSONFile();
            ResetGameValues();  // Reset for the next round
        } else {
            // Notify the players if one or both have not made a choice yet
            if (player1Choice.IsEmpty()) {
               // AContext->Connection->IOHandler->WriteLn("Player 1 has not made a move yet.");
            }
            if (player2Choice.IsEmpty()) {
                //AContext->Connection->IOHandler->WriteLn("Player 2 has not made a move yet.");
            }
        }

    } catch(Exception &e){
		Memo1->Lines->Add("Error receiving choices: " + e.Message);
	}
}

//---------------------------------------------------------------------------

String TForm2::RoundWinner(String &player1Choice, String &player2Choice)
{
	if( (player1Choice == "Rock" && player2Choice == "Scissors") ||
		(player1Choice == "Paper" && player2Choice == "Rock") ||
		(player1Choice == "Scissors" && player2Choice == "Paper") )
	{
		roundWinner = "Player 1";
		player1Score++;
                UpdateRound();
	}
	else if(player1Choice == player2Choice){
		roundWinner = "Draw";
	}
	else{
		roundWinner = "Player 2";
		player2Score++;
                UpdateRound();
	}

	return roundWinner;
}

//---------------------------------------------------------------------------

void TForm2::SendMessageToAllClients(String &message)
{
    for (auto it = Player.begin(); it != Player.end(); ++it)
    {
        TIdContext *context = it->first;
        context->Connection->IOHandler->WriteLn(message);
    }
}

//---------------------------------------------------------------------------


void TForm2::UpdateRound()
{
    if(roundCount < 3){
        roundCount++;
    }
    else{
        EndGame();
    }
}

//---------------------------------------------------------------------------
TForm2::GameMode TForm2::GetGameMode() const {
    return currentGameMode;
}

void TForm2::SetGameMode(TForm2::GameMode newGameMode) {
    currentGameMode = newGameMode;
}

void TForm2::CreateJSONFile()
{
    // Define a new JSON object
    TJSONObject *jsonObject = new TJSONObject;

    // Determine which player's score to use
    int playerScore, opponentScore;

    // Identify which player is making the move based on the context
    for (auto it = Player.begin(); it != Player.end(); ++it)
    {
        TIdContext *context = it->first;
        bool isPlayer1 = it->second;

        // Assign player score based on whether they are Player 1 or Player 2
        if (isPlayer1)
        {
            playerScore = player1Score;
            opponentScore = player2Score;
            TJSONObject *player1Data = CreatePlayerData(player1Score,player2Score, roundWinner);
            jsonObject->AddPair("Player1", player1Data);
        }
        else
        {
            playerScore = player2Score;
            opponentScore = player1Score;
            TJSONObject *player2Data = CreatePlayerData(player2Score, player1Score,roundWinner);
            jsonObject->AddPair("Player2", player2Data);
        }
    }

    // Convert the JSON object to string
    String jsonString = jsonObject->ToString();

    // Define the file path where you want to store the JSON file
    String filePath = System::Ioutils::TPath::Combine(System::Ioutils::TPath::GetDocumentsPath(), "game.json");


    try
    {
        // Write the JSON string to the file using TFile::WriteAllText
        TFile::WriteAllText(filePath, jsonString);

        // Clean up the JSON object
        delete jsonObject;

        // Inform the user that the file has been saved
        Memo1->Lines->Add("Game data saved to JSON file.");

        //SendGameDataToAllClients(jsonString);
        //Memo1->Lines->Add("Game data sent to clients: " + jsonString);

        String message = "FILE_READY\n";
        message.Trim();
        SendMessageToAllClients(message);
        Memo1->Lines->Add("FILE_READY sent to all clients.");
    }
    catch (const Exception &e)
    {
        // If there's an error, display it in the memo
        Memo1->Lines->Add("Error saving JSON file: " + e.Message);
    }
}

void TForm2::SendGameDataToAllClients(String &gameData){
    for(auto it = Player.begin(); it != Player.end(); ++it){
           TIdContext *context = it->first;
           context->Connection->IOHandler->WriteLn(gameData);
    }
}

TJSONObject* TForm2::CreatePlayerData(int &playerScore,int &opponentScore, String &roundWinner){
        TJSONObject *playerData = new TJSONObject;
        playerData->AddPair("PlayerScore", playerScore);
        playerData->AddPair("OpponentScore", opponentScore);
        playerData->AddPair("RoundCount", roundCount);
        playerData->AddPair("Winner", roundWinner);
        return playerData;
}

void TForm2::EndGame(){
        Memo1->Lines->Add("Game Over");
        gameWinner = player1Score > player2Score ? "Player 1" : "Player 2";
        Memo1->Lines->Add("Winner is: " + gameWinner);
}

void TForm2::ResetGameValues()
{
    player1Choice = "";
    player2Choice = "";
    roundCount = 0;
    player1Score = 0;
    player2Score = 0;
}


void __fastcall TForm2::IdTCPServer1Disconnect(TIdContext *AContext)
{
    // Handle disconnection
    Memo1->Lines->Add("A player has disconnected.");

    // Decrease the client count
    clientCount--;

    // Optionally update your internal game state
    bool wasPlayer1 = Player[AContext];  // Assuming Player is a map that stores player info
    Player.erase(AContext);  // Remove player from map

    // Send message to all connected clients
    String message = "A player has disconnected.";
    SendMessageToAllClients(message);

    // Optionally, handle game state: if one player remains, declare them the winner
    if (clientCount == 1) {
        String winner = wasPlayer1 ? "Player 2 wins by default due to disconnection." : "Player 1 wins by default due to disconnection.";
        SendMessageToAllClients(winner);

        // Disconnect the remaining player
        AContext->Connection->Disconnect();

        EndGame();  // Optionally, end the game
    }
    ResetGameValues();
}



