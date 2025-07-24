//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Server.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm2::StartServerClick(TObject *Sender)
{
	IdTCPServer1->Active = true;
	Memo1->Lines->Add("Starting Server...");
}
//---------------------------------------------------------------------------
void __fastcall TForm2::IdTCPServer1Execute(TIdContext *AContext)
{
	Memo1->Lines->Add("Client Connected");
}
//---------------------------------------------------------------------------
