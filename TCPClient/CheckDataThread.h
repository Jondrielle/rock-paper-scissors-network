#ifndef CheckDataThreadH
#define CheckDataThreadH

#include <System.Classes.hpp>  // For TThread
#include <IdTCPClient.hpp>     // For TIdTCPClient

class TForm2;  // Forward declaration of Form2 (because we will use it in the thread)

class CheckDataThread : public TThread
{
private:
    TIdTCPClient *Client;      // Pointer to the TCP client
    TForm2 *form2Instance;     // Pointer to Form2 instance for interacting with the main UI

protected:
    // Override the Execute method to define what the thread should do
    void __fastcall Execute() override;

public:
    // Constructor: You should pass the TCP client object and Form2 pointer to the thread
    __fastcall CheckDataThread(TIdTCPClient *Client, TForm2 *Form2);
};

#endif  // CheckDataThreadH

