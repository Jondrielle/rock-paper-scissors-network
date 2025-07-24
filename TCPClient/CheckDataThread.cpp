#include "CheckDataThread.h"
#include "Client.h"   // Make sure this is included to access TForm2
#include <System.SysUtils.hpp>
#include <fmx.h>

// Constructor definition
__fastcall CheckDataThread::CheckDataThread(TIdTCPClient* client, TForm2* Form2)
    : TThread(true)  // Start the thread in suspended mode
{
    Client = client;        // Store the reference to the TCP client
    form2Instance = Form2;  // Store the reference to the Form2 instance
}

// Execute method definition
void __fastcall CheckDataThread::Execute()
{
    try
    {
        while (!Terminated)  // Keep checking until the thread is terminated
        {
            String message = Client->IOHandler->ReadString(1024);   \
            message.Trim();
            form2Instance->Memo1->Lines->Add("Message : " + message );
            // Check if there is any data in the buffer
            if (Client->IOHandler->CheckForDataOnSource())
            {
            	form2Instance->Memo1->Lines->Add("Inside check if thread");
                String message = Client->IOHandler->ReadLn();  // Read the message
                message.Trim();

                // If the message is "FILE_READY", you can start the JSONReaderThread or handle it
                if (message == "FILE_READY")
                {
                    Synchronize([this, message]() {
                        form2Instance->Memo1->Lines->Add("Received message: '" + message + "'");
                    });
                    // Synchronize with the main thread to process the message
                    Synchronize([this]() {
                        // This will execute in the main thread (UI thread)
                        form2Instance->StartJSONReaderThread();
                    });
                }
                else
                {
                    Synchronize([this, message]() {
                        // Handle other messages in the UI thread
                        form2Instance->Memo1->Lines->Add("Received message: " + message);
                    });
                }
            }

            // Optional: You can add a delay here to prevent excessive CPU usage
            Sleep(100);  // Sleep for 100 milliseconds before checking again
        }
    }
    catch (const Exception &e)
    {
        // Handle errors (e.g., connection loss)
        Synchronize([this, &e]() {
            form2Instance->Memo1->Lines->Add("Error in thread: " + e.Message);
        });
    }
}

