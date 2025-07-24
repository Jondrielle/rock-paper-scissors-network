//---------------------------------------------------------------------------

#pragma hdrstop

#include "Client.h"
#include "RecievingThread.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall RecievingThread::RecievingThread(TIdTCPClient *AClient, TForm *AForm)
    : TThread(true),filePath(path)  // true tells it to create it as a background thread
{
    Client = AClient;
    MainForm = AForm;
    FreeOnTerminate = true;  // Automatically clean up the thread when it's done
}


void __fastcall RecievingThread::Execute()
{
    while (Client->Connected()) {
        try {
        	String serverMessage = Client->IOHandler->ReadLn();
                Form2->Memo1->Lines->Add(serverMessage);

                // Ensure MainForm is of type TForm2 before calling UpdateGameLabelsInfo
                Synchronize([this, serverMessage]() { ParsingMessage(serverMessage);});
        } catch (Exception &e) {
            break;  // Break out of the loop if an error occurs
        }
    }
}

void __fastcall RecievingThread::ParsingMessage(const String &serverMessage){
	TStringList *message = new TStringList();
        try{
                message->Delimiter = ' ';
                message->StrictDelimiter = true;
                message->DelimitedText = serverMessage;

                for(int i = 0; i < message->Count;i++){
                    TStringList *keyValue = new TStringList();
                    try{
                        keyValue->Delimiter = ':';
                        keyValue->StrictDelimiter = true;
                        keyValue->DelimitedText = message->Strings[i];

                        String key = keyValue->Strings[0];
                        String value = keyValue->Strings[1];

                        if(key == "PlayerScore"){
                                Form2->SetPlayerScore(value);
                        }
                        else if(key == "OpponentScore") {
                                Form2->SetOpponentScore(value);
                        }
                        else if(key == "Winner"){
                                Form2->SetWinner(value);
                        }
                        else if(key == "Round"){
                                Form2->SetRoundCount(value);
                        }
                    }catch(Exception &e){}
                }
        }catch(Exception &e){}

}
