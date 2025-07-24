//---------------------------------------------------------------------------

#ifndef RecievingThreadH
#define RecievingThreadH

#include <System.Classes.hpp>
#include <IdTCPClient.hpp>

class JSONReaderThread: public TThread
{
    private:
        TIdTCPClient *Client;
        String filePath;
        TForm *MainForm;
    protected:
       void __fastcall Execute();
       void __fastcall ParsingMessage(const String &serverMessage);
    public:
       __fastcall JSONReaderThread(TIdTCPClient *AClient,TForm *AForm);
};

//---------------------------------------------------------------------------
#endif
