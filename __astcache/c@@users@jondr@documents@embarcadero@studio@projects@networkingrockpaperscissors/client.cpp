//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Client.h"
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
void __fastcall TForm2::ConnectToServerClick(TObject *Sender)
{
	try{
        IdTCPClient->Connect();
		Memo1->Lines->Add("Connection Successful");
	}   catch(Exception &e){
		Memo1->Lines->Add("Connection Failed");
	}
}
//---------------------------------------------------------------------------
