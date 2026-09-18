#include <stdio.h>
#include <tchar.h>
#include "Cart.h"
#include "Windows.h"
#include "bus.h"

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

    errno_t err = _tfopen_s(&cart, szFile, L"rb+");
    if (err)
    {
        _tprintf(_T("szFile is: %s"), szFile);
    }
    else if (cart)
    {
        fseek(cart, getValue(0x0400) * 0xEFFE, SEEK_SET);
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

void bankSwitch()
{
    FILE* cart;
    int num_read = 0;

    errno_t err = _tfopen_s(&cart, szFile, L"rb");
    if (err)
    {
        _tprintf(_T("szFile is: %s"), szFile);
    }
    else if (cart)
    {
        fseek(cart, 0xEFFE * getValue(0x0400), SEEK_SET);
        num_read = fread(fullFile, sizeof(uint8_t), 0xEFFE, cart);

        if (num_read < 0xEFFE)
        {
            for (int i = num_read; i < 0xEFFE; i++)
            {
                fullFile[i] = 0x00;
            }
        }

        fclose(cart);
        splitFullFile();
    }
    printf("%X, %X",num_read, 0xEFFE * getValue(0x0400));
}

void readCart()
{
    OPENFILENAME ofn;
    char* fileName;
    HWND hwnd = NULL;
    FILE *cart;
    int num_read = 0;

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
            fseek(cart, 0xEFFE * getValue(0x0400), SEEK_SET);
            num_read = fread(fullFile, sizeof(uint8_t), 0xEFFE, cart);

            if(num_read < 0xEFFE)
            {
                for (int i = num_read; i < 0xEFFE; i++)
                {
                    fullFile[i] = 0x00;
                }
            }

            fclose(cart);
            splitFullFile();
        }
        //MY CODE!!!!!!
    }
    else {
        // User cancelled or an error occurred
        //_tprintf(_T("Operation cancelled or failed.\n"));
    }
}