#ifndef JSONREADERTHREAD_H
#define JSONREADERTHREAD_H

#include <System.IOUtils.hpp>
#include <System.JSON.hpp>
#include <System.SyncObjs.hpp>
#include "Client.h"  // For Synchronize
 // Ensure this includes the appropriate reference to the form.
//class TForm2;
class JSONReaderThread : public TThread
{
private:
    String filePath;
    TForm2* form2;  // Pointer to the form to update UI after loading

protected:
    // Declare the Execute method
    void __fastcall Execute() override;

public:
    // Constructor with form and file path
    __fastcall JSONReaderThread(TForm2* formInstance, String path);
};

#endif // JSONREADERTHREAD_H

