//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls3D.hpp>
#include <FMX.Objects3D.hpp>
#include <FMX.Types.hpp>
#include <System.Math.Vectors.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Objects.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.ListView.Adapters.Base.hpp>
#include <FMX.ListView.Appearances.hpp>
#include <FMX.ListView.hpp>
#include <FMX.ListView.Types.hpp>
#include <FMX.ExtCtrls.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdCustomTCPServer.hpp>
#include <IdTCPServer.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Memo.Types.hpp>
#include <FMX.ScrollBox.hpp>
#include <IdContext.hpp>
#include <string>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TLabel *GameTitle;
	TLabel *Winner;
	TLabel *PlayerChoice;
	TLabel *ComputerChoice;
	TImageViewer *RockIcon;
	TImageViewer *PaperIcon;
	TImageViewer *ScissorsIcon;
	TLabel *PlayerScore;
	TLabel *ComputerScore;
	TLabel *Score_1;
	TLabel *Score_2;
	TLabel *PlayerDrawnHand;
	TLabel *ComputerDrawnHand;
	TIdTCPClient *IdTCPClient1;
	TIdTCPServer *IdTCPServer1;
	TMemo *Memo1;
	TButton *StartServerButton;
	TButton *Connect;
	void __fastcall ScissorsButtonClick(TObject *Sender);
	void __fastcall PaperButtonClick(TObject *Sender);
	void __fastcall RockButtonClick(TObject *Sender);
	void __fastcall StartServerButtonClick(TObject *Sender);
	void __fastcall IdTCPServer1AfterBind(TObject *Sender);
	void __fastcall IdTCPServer1Execute(TIdContext *AContext);
	void __fastcall ConnectClick(TObject *Sender);
	//void __fastcall IdTCPServer1AfterBind(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	int playerWins = 0;
	int computerWins = 0;
	int roundValue = 1;
	String ComputerRandomChoice();
	String playerChoice;
	void RoundWinner(String &playerChoice, String &computerChoice);
	void UpdateScoreText(String &roundWinner,int &score);
	void GameWinner();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif











