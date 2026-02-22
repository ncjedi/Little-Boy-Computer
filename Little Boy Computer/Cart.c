#include <stdio.h>
#include <tchar.h>
#include "Cart.h"
#include "Windows.h"

uint8_t fullFile[0xEFFE];
TCHAR szFile[260] = { 0 };

void RecompileFullFile()
{
    int i = 0;

    while (i < 0x6FFF)
    {
        fullFile[i] = prog_rom[i];
        i++;
    }

    while (i < 0xEFFE)
    {
        fullFile[i] = graphic_rom[i - 0x6FFF];
        i++;
    }
}

void WriteToCart()
{
    FILE* cart;

    errno_t err = _tfopen_s(&cart, szFile, L"wb");
    if (err)
    {
        _tprintf(_T("szFile is: %s"), szFile);
    }
    else if (cart)
    {
        RecompileFullFile();
        fwrite(fullFile, sizeof(uint8_t), 0xEFFE, cart);
        fclose(cart);
    }
}

void splitFullFile()
{
    int i = 0;
    
    while (i < 0x6FFF)
    {
        prog_rom[i] = fullFile[i];
        i++;
    }

    while (i < 0xEFFE)
    {
        graphic_rom[i - 0x6FFF] = fullFile[i];
        i++;
    }
}

void readCart()
{
    OPENFILENAME ofn;
    char* fileName;
    HWND hwnd = NULL;
    FILE *cart;

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(*szFile);
    ofn.lpstrFilter = _T("All Files\0*.*\0Text Files\0*.TXT\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileName(&ofn) == TRUE) {
        // User selected a file, the path is in szFile
        //_tprintf(_T("Selected file: %s\n"), szFile);
        // You can now use standard C file handling functions like fopen() with szFile
        //MY CODE!!!!!!
        errno_t err = _tfopen_s(&cart, szFile, L"rb");
        if (err)
        {
            _tprintf(_T("szFile is: %s"), szFile);
        }
        else if(cart)
        {
            fread(fullFile, sizeof(uint8_t), 0xEFFE, cart);
            fclose(cart);
            splitFullFile();
        }
        //MY CODE!!!!!!
    }
    else {
        // User cancelled or an error occurred
        //_tprintf(_T("Operation cancelled or failed.\n"));
    }
	/*TEST FOR CORNERS
	graphic_rom[0x1000] = 0b00111011; //TEST!!!!!! bottom left 1
	graphic_rom[0x1007] = 0b00001011; //TEST!!!!!! top left 2 
	graphic_rom[0x103F] = 0b00110100; //TEST!!!!!! top right 4
	graphic_rom[0x1038] = 0b00110000; //TEST!!!!!! bottom right 3
	TEST FOR CORNERS*/
}