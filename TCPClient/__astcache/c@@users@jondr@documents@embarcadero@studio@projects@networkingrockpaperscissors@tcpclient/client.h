#ifndef CLIENT_H  // Begin header guard to prevent multiple inclusions
#define CLIENT_H

#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Memo.Types.hpp>
#include <FMX.ScrollBox.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <System.JSON.hpp>
#include <System.IOUtils.hpp>
#include <memory>
//#include "JSONReaderThread.h"
#include <map>
#include <string>

// Forward declaration of classes
//class TForm2;  // Forward declaration for TForm2 class
  // Forward declaration for JSONReaderThread class
class JSONReaderThread;

//---------------------------------------------------------------------------

class TForm2 : public TForm
{
__published:	// IDE-managed Components
    TLabel *GameTitle;
    TMemo *Memo1;
    TButton *ConnectToServer;
    TIdTCPClient *IdTCPClient1;
    TButton *RockButton;
    TButton *PaperButton;
    TButton *ScissorsButton;
    TLabel *PlayerScore;
    TLabel *OpponentScore;
    TLabel *Label1;
    TLabel *Winner;
    TLabel *RoundCount;

    // Event handlers
    void __fastcall ConnectToServerClick(TObject *Sender);
    void __fastcall RockButtonClick(TObject *Sender);
    void __fastcall PaperButtonClick(TObject *Sender);
    void __fastcall ScissorsButtonClick(TObject *Sender);
    void __fastcall IdTCPClient1Connected(TObject *Sender);

public:		// User declarations
    __fastcall TForm2(TComponent* Owner);

    // Enum for GamePhase
    enum class GamePhase {
        PREGAME,
        ROUND,
        POSTGAME
    };

    // Methods to update game state
    void __fastcall UpdateGameLabelsInfo(TJSONObject *jsonObject);
    void RecieveMessagesFromServer();
    void SetRoundCount(String value);
    void SetWinner(String winner);
    void SetPlayerScore(String score);
    void SetOpponentScore(String score);
    TForm2::GamePhase GetGameMode() const;
    void __fastcall OnDataRecieved(TObject *Sender, String jsonData, GamePhase phase);
    void __fastcall SetGameMode(GamePhase newGameMode);
    void StartJSONReaderThread();
    void StopJSONReaderThread();
    void SetIsPlayerOne(String &isPlayerOneBool);
    bool GetIsPlayerOne();
    void SetFormName(String newFormName);

private:	// User declarations
    std::map<String, TLabel*> GameLabels;
    GamePhase currentGameMode;  // Variable to store current game phase
    JSONReaderThread* jsonReaderThread;
    bool isPlayerOne;
};

// End of the header guard
#endif

