#ifndef ServerH
#define ServerH
//---------------------------------------------------------------------------
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
#include <IdCustomTCPServer.hpp>
#include <IdTCPServer.hpp>
#include <IdContext.hpp>
#include <map>
#include <iostream>
#include <fstream>
#include <System.JSON.hpp>
#include <System.IOUtils.hpp>
#include <System.SysUtils.hpp>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
	TLabel *Server;
	TMemo *Memo1;
	TButton *StartServer;
	TIdTCPServer *IdTCPServer1;

	void __fastcall StartServerClick(TObject *Sender);
	void __fastcall IdTCPServer1Connect(TIdContext *AContext);
	void __fastcall IdTCPServer1Execute(TIdContext *AContext);
        void __fastcall IdTCPServer1Disconnect(TIdContext *AContext);

public:		// User declarations
	__fastcall TForm2(TComponent* Owner);
        int roundCount = 1;
	int clientCount;
	String gameWinner;
	String roundWinner;
	std::map<TIdContext*,bool> Player;
	String playerMove = "";
        String player1Choice = "";
	String player2Choice = "";
	int player1Score = 0;
    	int player2Score = 0;
        String filePath;
        enum GameMode{PREGAME,ROUND,POSTGAME};
        struct Player;
        struct GameSession;


    	String RoundWinner(String &player1Choice, String &player2Choice);
    	void SendMessageToAllClients(String &message);
        void UpdateRound();
        void CreateJSONFile();
        void __fastcall ReadAndUpdateJSONFile(String &section,String &key,String &newValue);
        GameMode GetGameMode() const;
        void SetGameMode(GameMode newGameMode);
        void SendUpdatedJSONToClient(const String &jsonData);
        int DeterminePlayerScore(TIdContext *AContext);
        void EndGame();
        void SendGameDataToAllClients(String &gameData);
        TJSONObject* CreatePlayerData(int &playerScore,int &opponentScore, String &roundWinner);
        void ResetGameValues();


        private:	// User declarations
        GameMode currentGameMode;

};


//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
