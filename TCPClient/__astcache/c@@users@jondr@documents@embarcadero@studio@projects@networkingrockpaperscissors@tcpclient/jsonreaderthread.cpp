#include "JSONReaderThread.h"
#include <System.IOUtils.hpp>
#include <System.JSON.hpp>
#include <System.SyncObjs.hpp>  // For Synchronize and TCriticalSection

// Constructor definition
__fastcall JSONReaderThread::JSONReaderThread(TForm2* formInstance, String path)
    : TThread(true)  // Calling the base class constructor
{
    form2 = formInstance;
    filePath = path;
}

TCriticalSection *criticalSection = new TCriticalSection();

// Execute method definition
void __fastcall JSONReaderThread::Execute()
{
    form2->Memo1->Lines->Add("Started Thread");
    try {
        while (!Terminated) {
            String fileStatus = form2->IdTCPClient1->IOHandler->ReadLn();  // Read server message

            // Enter critical section to safely read and check the file status
            criticalSection->Enter();
            try {
                if (fileStatus == "FILE_READY") {
                    // The server has sent the "FILE_READY" status, so we can now read the JSON file
                    String jsonString = TFile::ReadAllText(filePath);  // Read the JSON file

                    // Process the JSON string
                    TJSONObject *jsonObject = dynamic_cast<TJSONObject*>(TJSONObject::ParseJSONValue(jsonString));
                    if (jsonObject) {
                        // Extract and display player data
                        TJSONValue* player1Value = jsonObject->GetValue("Player1");
                        TJSONValue* player2Value = jsonObject->GetValue("Player2");

                        if (form2->GetIsPlayerOne()) {
                            TJSONObject* player1Data = dynamic_cast<TJSONObject*>(player1Value);
                            auto player1ScoreValue = player1Data->GetValue("PlayerScore");
                            int playerScore = StrToInt(player1ScoreValue->Value());

                            auto player1OppScoreValue = player1Data->GetValue("OpponentScore");
                            int opponentScore = StrToInt(player1OppScoreValue->Value());

                            auto player1WinnerValue = player1Data->GetValue("Winner");
                            String winner = player1WinnerValue->Value();

                            auto player1Round = player1Data->GetValue("RoundCount");
                            int round = StrToInt(player1Round->Value());

                            Synchronize([this, playerScore,opponentScore,winner,round]() {
                                form2->PlayerScore->Text = "Player Score: " + IntToStr(playerScore);
                                form2->OpponentScore->Text = "Opponent Score: " + IntToStr(opponentScore);
                                form2->Winner->Text = "Winner: " + winner;
                                form2->RoundCount->Text = "Round " + IntToStr(round);
                            });
                        } else {
                            TJSONObject* player2Data = dynamic_cast<TJSONObject*>(player2Value);
                            auto player2ScoreValue = player2Data->GetValue("PlayerScore");
                            int playerScore = StrToInt(player2ScoreValue->Value());

                            auto player2OppScoreValue = player2Data->GetValue("OpponentScore");
                            int opponentScore = StrToInt(player2OppScoreValue->Value());

                            auto player2WinnerValue = player2Data->GetValue("Winner");
                            String winner = player2WinnerValue->Value();

                            auto player2Round = player2Data->GetValue("RoundCount");
                            int round = StrToInt(player2Round->Value());

                            Synchronize([this, playerScore,opponentScore,winner,round]() {
                                form2->PlayerScore->Text = "Player Score: " + IntToStr(playerScore);
                                form2->OpponentScore->Text = "Opponent Score: " + IntToStr(opponentScore);
                                form2->Winner->Text = "Winner: " + winner;
                                form2->RoundCount->Text = "Round " + IntToStr(round);
                            });
                        }
                        // Clean up the JSON object after processing
                        delete jsonObject;
                    } else {
                        Synchronize([this]() {
                            form2->Memo1->Lines->Add("Error parsing JSON file.");
                        });
                    }
                }
            } catch (const Exception &e) {
                Synchronize([this, &e]() {
                    form2->Memo1->Lines->Add("Error while handling file status: " + e.Message);
                });
            }
            criticalSection->Leave();  // Release the lock after processing the file status

            // Sleep to prevent continuous busy-waiting
            Sleep(1000);  // Adjust as needed
        }
    }
    catch (const Exception &e) {
        Synchronize([this, &e]() {
            form2->Memo1->Lines->Add("Error: " + e.Message);
        });
    }
}

