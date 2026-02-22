#include <GLFW/glfw3.h>
#include <stdio.h>
#include "bus.h"
#include "Cart.h"
#include "IO.h"

GLFWwindow* window;

void drawGrid() {
    int gridSize = 64;
    float size = 2.0f / gridSize;

    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            float x = -1.0f + i * size;
            float y = -1.0f + j * size;

            glBegin(GL_QUADS);
            if ((i + j) % 2 == 0) glColor3f(0.8f, 0.8f, 0.8f);
            else glColor3f(0.5f, 0.5f, 0.5f);

            glVertex2f(x, y);
            glVertex2f(x + size, y);
            glVertex2f(x + size, y + size);
            glVertex2f(x, y + size);
            glEnd();
        }
    }
}

void draw_background()
{
    int gridSize = 64;
    float size = 2.0f / gridSize;
    uint16_t tile_counter = 0x0401;
    uint16_t rom_address = 0x0000;
    uint8_t colour = 0x00;
    int8_t scroll_offset_x = getValue(0x0800);
    int8_t scroll_offset_y = getValue(0x0801);

    while (tile_counter < 0x0481)
    {
        rom_address = 0x0000;
        rom_address |= getValue(tile_counter); // get the high bit from the VRAM
        tile_counter++; //increment the tile counter
        rom_address = rom_address << 8; //shift the high bit to it's position
        rom_address |= getValue(tile_counter); // get the low bit from the VRAM
        tile_counter++; //increment the tile counter for the next pass

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                int tile_count = (tile_counter - 0x0483)/2; //+2 because the counter already incremented
                colour = getValue(rom_address); //get the colour of the pixel at the rom address
                rom_address++;

                //colour = 0b00011001; //TEST!!!!!!

                float x = (- 1.0f + (i + 8 * (tile_count % 8)) * size); //FOR FUTURE REFRERENCE: the glVertex2f function goes from -1 to +1 with -1 being the far left and +1 being the far right
                float y = (- 1.0f + (j + 8 * (tile_count / 8)) * size); //same as above but for up and down

                //printf("offset: %d number put into x: %f x before: %f\n", scroll_offset_x, (float)scroll_offset_x / 32.0f,x);

                x += (float)scroll_offset_x / 32.0f; //smooth scrolling offset
                y += (float)scroll_offset_y / 32.0f; //smooth scrolling offset

                //printf("offset: %d number put into x: %f x after: %f\n\n", scroll_offset_x, (float)scroll_offset_x / 32.0f, x);

                while (x <= -1)
                {
                    x += 2;
                }
                while (x >= 1)
                {
                    x -= 2;
                }

                while (y <= -1)
                {
                    y += 2;
                }
                while (y >= 1)
                {
                    y -= 2;
                }

                float blue = (float)(colour & 0b00000011) / (float)0b00000011;
                colour = colour >> 2;
                float green = (float)(colour & 0b00000011) / (float)0b00000011;
                colour = colour >> 2;
                float red = (float)(colour & 0b00000011) / (float)0b00000011;

                glBegin(GL_QUADS);
                /*if ((i + j) % 2 == 0) glColor3f(0.8f, 0.8f, 0.8f);
                else glColor3f(0.5f, 0.5f, 0.5f);*/

                glColor3f(red, green, blue);

                glVertex2f(x, y);
                glVertex2f(x + size, y);
                glVertex2f(x + size, y + size);
                glVertex2f(x, y + size);
                glEnd();
            }
        }
    }
}

void draw_foreground()
{
    int gridSize = 64;
    float size = 2.0f / gridSize;
    uint16_t tile_counter = 0x0601;
    uint16_t rom_address = 0x0000;
    uint8_t colour = 0x00;
    int8_t scroll_offset_x;
    int8_t scroll_offset_y;

    while (tile_counter < 0x07FD)
    {
        scroll_offset_x = getValue(tile_counter);
        tile_counter++;
        scroll_offset_y = getValue(tile_counter);
        tile_counter++;
        rom_address = 0x0000;
        rom_address |= getValue(tile_counter); // get the high bit from the VRAM
        tile_counter++; //increment the tile counter
        rom_address = rom_address << 8; //shift the high bit to it's position
        rom_address |= getValue(tile_counter); // get the low bit from the VRAM
        tile_counter++; //increment the tile counter for the next pass

        colour = getValue(rom_address); //get the colour of the pixel at the rom address
        rom_address++;
        /*if (rom_address != 1)
        {
            printf("%d,%d,%d,%d,%d\n", rom_address,getValue(0x0601), getValue(0x0602), getValue(0x0603), getValue(0x0604));
        }*/
        if (rom_address == 0x8051)
        {
            printf("%d", colour);
        }

        if (colour >= 0x80)
        {
            float x = -1.0f + (scroll_offset_x * size); //smooth scrolling offset
            float y = -1.0f + (scroll_offset_y * size); //smooth scrolling offset

            float blue = (float)(colour & 0b00000011) / (float)0b00000011;
            colour = colour >> 2;
            float green = (float)(colour & 0b00000011) / (float)0b00000011;
            colour = colour >> 2;
            float red = (float)(colour & 0b00000011) / (float)0b00000011;

            glBegin(GL_QUADS);

            glColor3f(red, green, blue);

            glVertex2f(x, y);
            glVertex2f(x + size, y);
            glVertex2f(x + size, y + size);
            glVertex2f(x, y + size);
            glEnd();
        }
    }
}

void GetKeys()
{
    //function keys
    if (glfwGetKey(window, GLFW_KEY_F4))
    {
        exit();
    }
    if (glfwGetKey(window, GLFW_KEY_F1))
    {
        readCart();
    }
    if (glfwGetKey(window, GLFW_KEY_F2))
    {
        WriteToCart();
    }

    //keyboard keys
    if (glfwGetKey(window, GLFW_KEY_1))
    {
        IO_address[0] |= 0b10000000;
    }
    else if(IO_address[0] & 0b10000000)
    {
        IO_address[0] &= 0b01111111;
    }

    if (glfwGetKey(window, GLFW_KEY_2))
    {
        IO_address[0] |= 0b01000000;
    }
    else if(IO_address[0] & 0b01000000)
    {
        IO_address[0] &= 0b10111111;
    }

    if (glfwGetKey(window, GLFW_KEY_3))
    {
        IO_address[0] |= 0b00100000;
    }
    else if (IO_address[0] & 0b00100000)
    {
        IO_address[0] &= 0b11011111;
    }

    if (glfwGetKey(window, GLFW_KEY_4))
    {
        IO_address[0] |= 0b00010000;
    }
    else if (IO_address[0] & 0b00010000)
    {
        IO_address[0] &= 0b11101111;
    }

    if (glfwGetKey(window, GLFW_KEY_5))
    {
        IO_address[0] |= 0b00001000;
    }
    else if (IO_address[0] & 0b00001000)
    {
        IO_address[0] &= 0b11110111;
    }

    if (glfwGetKey(window, GLFW_KEY_6))
    {
        IO_address[0] |= 0b00000100;
    }
    else if (IO_address[0] & 0b00000100)
    {
        IO_address[0] &= 0b11111011;
    }

    if (glfwGetKey(window, GLFW_KEY_7))
    {
        IO_address[0] |= 0b00000010;
    }
    else if (IO_address[0] & 0b00000010)
    {
        IO_address[0] &= 0b11111101;
    }

    if (glfwGetKey(window, GLFW_KEY_8))
    {
        IO_address[0] |= 0b00000001;
    }
    else if (IO_address[0] & 0b00000001)
    {
        IO_address[0] &= 0b11111110;
    }

    //1

    if (glfwGetKey(window, GLFW_KEY_9))
    {
        IO_address[1] |= 0b10000000;
    }
    else if (IO_address[1] & 0b10000000)
    {
        IO_address[1] &= 0b01111111;
    }

    if (glfwGetKey(window, GLFW_KEY_0))
    {
        IO_address[1] |= 0b01000000;
    }
    else if (IO_address[1] & 0b01000000)
    {
        IO_address[1] &= 0b10111111;
    }

    if (glfwGetKey(window, GLFW_KEY_MINUS))
    {
        IO_address[1] |= 0b00100000;
    }
    else if (IO_address[1] & 0b00100000)
    {
        IO_address[1] &= 0b11011111;
    }

    if (glfwGetKey(window, GLFW_KEY_EQUAL))
    {
        IO_address[1] |= 0b00010000;
    }
    else if (IO_address[1] & 0b00010000)
    {
        IO_address[1] &= 0b11101111;
    }

    if (glfwGetKey(window, GLFW_KEY_BACKSPACE))
    {
        IO_address[1] |= 0b00001000;
    }
    else if (IO_address[1] & 0b00001000)
    {
        IO_address[1] &= 0b11110111;
    }

    if (glfwGetKey(window, GLFW_KEY_TAB))
    {
        IO_address[1] |= 0b00000100;
    }
    else if (IO_address[1] & 0b00000100)
    {
        IO_address[1] &= 0b11111011;
    }

    if (glfwGetKey(window, GLFW_KEY_Q))
    {
        IO_address[1] |= 0b00000010;
    }
    else if (IO_address[1] & 0b00000010)
    {
        IO_address[1] &= 0b11111101;
    }

    if (glfwGetKey(window, GLFW_KEY_W))
    {
        IO_address[1] |= 0b00000001;
    }
    else if (IO_address[1] & 0b00000001)
    {
        IO_address[1] &= 0b11111110;
    }

    //2

    if (glfwGetKey(window, GLFW_KEY_E))
    {
        IO_address[2] |= 0b10000000;
    }
    else if (IO_address[2] & 0b10000000)
    {
        IO_address[2] &= 0b01111111;
    }

    if (glfwGetKey(window, GLFW_KEY_R))
    {
        IO_address[2] |= 0b01000000;
    }
    else if (IO_address[2] & 0b01000000)
    {
        IO_address[2] &= 0b10111111;
    }

    if (glfwGetKey(window, GLFW_KEY_T))
    {
        IO_address[2] |= 0b00100000;
    }
    else if (IO_address[2] & 0b00100000)
    {
        IO_address[2] &= 0b11011111;
    }

    if (glfwGetKey(window, GLFW_KEY_Y))
    {
        IO_address[2] |= 0b00010000;
    }
    else if (IO_address[2] & 0b00010000)
    {
        IO_address[2] &= 0b11101111;
    }

    if (glfwGetKey(window, GLFW_KEY_U))
    {
        IO_address[2] |= 0b00001000;
    }
    else if (IO_address[2] & 0b00001000)
    {
        IO_address[2] &= 0b11110111;
    }

    if (glfwGetKey(window, GLFW_KEY_I))
    {
        IO_address[2] |= 0b00000100;
    }
    else if (IO_address[2] & 0b00000100)
    {
        IO_address[2] &= 0b11111011;
    }

    if (glfwGetKey(window, GLFW_KEY_O))
    {
        IO_address[2] |= 0b00000010;
    }
    else if (IO_address[2] & 0b00000010)
    {
        IO_address[2] &= 0b11111101;
    }

    if (glfwGetKey(window, GLFW_KEY_P))
    {
        IO_address[2] |= 0b00000001;
    }
    else if (IO_address[2] & 0b00000001)
    {
        IO_address[2] &= 0b11111110;
    }

    //3

    if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET))
    {
        IO_address[3] |= 0b10000000;
    }
    else if (IO_address[3] & 0b10000000)
    {
        IO_address[3] &= 0b01111111;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET))
    {
        IO_address[3] |= 0b01000000;
    }
    else if (IO_address[3] & 0b01000000)
    {
        IO_address[3] &= 0b10111111;
    }

    if (glfwGetKey(window, GLFW_KEY_BACKSLASH))
    {
        IO_address[3] |= 0b00100000;
    }
    else if (IO_address[3] & 0b00100000)
    {
        IO_address[3] &= 0b11011111;
    }

    if (glfwGetKey(window, GLFW_KEY_A))
    {
        IO_address[3] |= 0b00010000;
    }
    else if (IO_address[3] & 0b00010000)
    {
        IO_address[3] &= 0b11101111;
    }

    if (glfwGetKey(window, GLFW_KEY_S))
    {
        IO_address[3] |= 0b00001000;
    }
    else if (IO_address[3] & 0b00001000)
    {
        IO_address[3] &= 0b11110111;
    }

    if (glfwGetKey(window, GLFW_KEY_D))
    {
        IO_address[3] |= 0b00000100;
    }
    else if (IO_address[3] & 0b00000100)
    {
        IO_address[3] &= 0b11111011;
    }

    if (glfwGetKey(window, GLFW_KEY_F))
    {
        IO_address[3] |= 0b00000010;
    }
    else if (IO_address[3] & 0b00000010)
    {
        IO_address[3] &= 0b11111101;
    }

    if (glfwGetKey(window, GLFW_KEY_G))
    {
        IO_address[3] |= 0b00000001;
    }
    else if (IO_address[3] & 0b00000001)
    {
        IO_address[3] &= 0b11111110;
    }

    //4

    if (glfwGetKey(window, GLFW_KEY_H))
    {
        IO_address[4] |= 0b10000000;
    }
    else if (IO_address[4] & 0b10000000)
    {
        IO_address[4] &= 0b01111111;
    }

    if (glfwGetKey(window, GLFW_KEY_J))
    {
        IO_address[4] |= 0b01000000;
    }
    else if (IO_address[4] & 0b01000000)
    {
        IO_address[4] &= 0b10111111;
    }

    if (glfwGetKey(window, GLFW_KEY_K))
    {
        IO_address[4] |= 0b00100000;
    }
    else if (IO_address[4] & 0b00100000)
    {
        IO_address[4] &= 0b11011111;
    }

    if (glfwGetKey(window, GLFW_KEY_L))
    {
        IO_address[4] |= 0b00010000;
    }
    else if (IO_address[4] & 0b00010000)
    {
        IO_address[4] &= 0b11101111;
    }

    if (glfwGetKey(window, GLFW_KEY_SEMICOLON))
    {
        IO_address[4] |= 0b00001000;
    }
    else if (IO_address[4] & 0b00001000)
    {
        IO_address[4] &= 0b11110111;
    }

    if (glfwGetKey(window, GLFW_KEY_APOSTROPHE))
    {
        IO_address[4] |= 0b00000100;
    }
    else if (IO_address[4] & 0b00000100)
    {
        IO_address[4] &= 0b11111011;
    }

    if (glfwGetKey(window, GLFW_KEY_ENTER))
    {
        IO_address[4] |= 0b00000010;
    }
    else if (IO_address[4] & 0b00000010)
    {
        IO_address[4] &= 0b11111101;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
    {
        IO_address[4] |= 0b00000001;
    }
    else if (IO_address[4] & 0b00000001)
    {
        IO_address[4] &= 0b11111110;
    }

    //5

    if (glfwGetKey(window, GLFW_KEY_Z))
    {
        IO_address[5] |= 0b10000000;
    }
    else if (IO_address[5] & 0b10000000)
    {
        IO_address[5] &= 0b01111111;
    }

    if (glfwGetKey(window, GLFW_KEY_X))
    {
        IO_address[5] |= 0b01000000;
    }
    else if (IO_address[5] & 0b01000000)
    {
        IO_address[5] &= 0b10111111;
    }

    if (glfwGetKey(window, GLFW_KEY_C))
    {
        IO_address[5] |= 0b00100000;
    }
    else if (IO_address[5] & 0b00100000)
    {
        IO_address[5] &= 0b11011111;
    }

    if (glfwGetKey(window, GLFW_KEY_V))
    {
        IO_address[5] |= 0b00010000;
    }
    else if (IO_address[5] & 0b00010000)
    {
        IO_address[5] &= 0b11101111;
    }

    if (glfwGetKey(window, GLFW_KEY_B))
    {
        IO_address[5] |= 0b00001000;
    }
    else if (IO_address[5] & 0b00001000)
    {
        IO_address[5] &= 0b11110111;
    }

    if (glfwGetKey(window, GLFW_KEY_N))
    {
        IO_address[5] |= 0b00000100;
    }
    else if (IO_address[5] & 0b00000100)
    {
        IO_address[5] &= 0b11111011;
    }

    if (glfwGetKey(window, GLFW_KEY_M))
    {
        IO_address[5] |= 0b00000010;
    }
    else if (IO_address[5] & 0b00000010)
    {
        IO_address[5] &= 0b11111101;
    }

    if (glfwGetKey(window, GLFW_KEY_COMMA))
    {
        IO_address[5] |= 0b00000001;
    }
    else if (IO_address[5] & 0b00000001)
    {
        IO_address[5] &= 0b11111110;
    }

    //6

    if (glfwGetKey(window, GLFW_KEY_PERIOD))
    {
        IO_address[6] |= 0b10000000;
    }
    else if (IO_address[6] & 0b10000000)
    {
        IO_address[6] &= 0b01111111;
    }

    if (glfwGetKey(window, GLFW_KEY_SLASH))
    {
        IO_address[6] |= 0b01000000;
    }
    else if (IO_address[6] & 0b01000000)
    {
        IO_address[6] &= 0b10111111;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT))
    {
        IO_address[6] |= 0b00100000;
    }
    else if (IO_address[6] & 0b00100000)
    {
        IO_address[6] &= 0b11011111;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
    {
        IO_address[6] |= 0b00010000;
    }
    else if (IO_address[6] & 0b00010000)
    {
        IO_address[6] &= 0b11101111;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT))
    {
        IO_address[6] |= 0b00001000;
    }
    else if (IO_address[6] & 0b00001000)
    {
        IO_address[6] &= 0b11110111;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE))
    {
        IO_address[6] |= 0b00000100;
    }
    else if (IO_address[6] & 0b00000100)
    {
        IO_address[6] &= 0b11111011;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT))
    {
        IO_address[6] |= 0b00000010;
    }
    else if (IO_address[6] & 0b00000010)
    {
        IO_address[6] &= 0b11111101;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL))
    {
        IO_address[6] |= 0b00000001;
    }
    else if (IO_address[6] & 0b00000001)
    {
        IO_address[6] &= 0b11111110;
    }

    //7

    if (glfwGetKey(window, GLFW_KEY_INSERT))
    {
        IO_address[7] |= 0b10000000;
    }
    else if (IO_address[7] & 0b10000000)
    {
        IO_address[7] &= 0b01111111;
    }

    if (glfwGetKey(window, GLFW_KEY_HOME))
    {
        IO_address[7] |= 0b01000000;
    }
    else if (IO_address[7] & 0b01000000)
    {
        IO_address[7] &= 0b10111111;
    }

    if (glfwGetKey(window, GLFW_KEY_PAGE_UP))
    {
        IO_address[7] |= 0b00100000;
    }
    else if (IO_address[7] & 0b00100000)
    {
        IO_address[7] &= 0b11011111;
    }

    if (glfwGetKey(window, GLFW_KEY_DELETE))
    {
        IO_address[7] |= 0b00010000;
    }
    else if (IO_address[7] & 0b00010000)
    {
        IO_address[7] &= 0b11101111;
    }

    if (glfwGetKey(window, GLFW_KEY_END))
    {
        IO_address[7] |= 0b00001000;
    }
    else if (IO_address[7] & 0b00001000)
    {
        IO_address[7] &= 0b11110111;
    }

    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN))
    {
        IO_address[7] |= 0b00000100;
    }
    else if (IO_address[7] & 0b00000100)
    {
        IO_address[7] &= 0b11111011;
    }

    if (glfwGetKey(window, GLFW_KEY_UP))
    {
        IO_address[7] |= 0b00000010;
    }
    else if (IO_address[7] & 0b00000010)
    {
        IO_address[7] &= 0b11111101;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT))
    {
        IO_address[7] |= 0b00000001;
    }
    else if (IO_address[7] & 0b00000001)
    {
        IO_address[7] &= 0b11111110;
    }

    //8

    if (glfwGetKey(window, GLFW_KEY_DOWN))
    {
        IO_address[8] |= 0b10000000;
    }
    else if (IO_address[8] & 0b10000000)
    {
        IO_address[8] &= 0b01111111;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT))
    {
        IO_address[8] |= 0b01000000;
    }
    else if (IO_address[8] & 0b01000000)
    {
        IO_address[8] &= 0b10111111;
    }

    if (glfwGetKey(window, GLFW_KEY_NUM_LOCK))
    {
        IO_address[8] |= 0b00100000;
    }
    else if (IO_address[8] & 0b00100000)
    {
        IO_address[8] &= 0b11011111;
    }

    if (glfwGetKey(window, GLFW_KEY_PRINT_SCREEN))
    {
        IO_address[8] |= 0b00010000;
    }
    else if (IO_address[8] & 0b00010000)
    {
        IO_address[8] &= 0b11101111;
    }

    if (glfwGetKey(window, GLFW_KEY_SCROLL_LOCK))
    {
        IO_address[8] |= 0b00001000;
    }
    else if (IO_address[8] & 0b00001000)
    {
        IO_address[8] &= 0b11110111;
    }

    if (glfwGetKey(window, GLFW_KEY_PAUSE))
    {
        IO_address[8] |= 0b00000100;
    }
    else if (IO_address[8] & 0b00000100)
    {
        IO_address[8] &= 0b11111011;
    }

    if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT))
    {
        IO_address[8] |= 0b00000010;
    }
    else if (IO_address[8] & 0b00000010)
    {
        IO_address[8] &= 0b11111101;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_DIVIDE))
    {
        IO_address[8] |= 0b00000001;
    }
    else if (IO_address[8] & 0b00000001)
    {
        IO_address[8] &= 0b11111110;
    }

    //9

    if (glfwGetKey(window, GLFW_KEY_KP_MULTIPLY))
    {
        IO_address[9] |= 0b10000000;
    }
    else if (IO_address[9] & 0b10000000)
    {
        IO_address[9] &= 0b01111111;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT))
    {
        IO_address[9] |= 0b01000000;
    }
    else if (IO_address[9] & 0b01000000)
    {
        IO_address[9] &= 0b10111111;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_7))
    {
        IO_address[9] |= 0b00100000;
    }
    else if (IO_address[9] & 0b00100000)
    {
        IO_address[9] &= 0b11011111;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_8))
    {
        IO_address[9] |= 0b00010000;
    }
    else if (IO_address[9] & 0b00010000)
    {
        IO_address[9] &= 0b11101111;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_9))
    {
        IO_address[9] |= 0b00001000;
    }
    else if (IO_address[9] & 0b00001000)
    {
        IO_address[9] &= 0b11110111;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_ADD))
    {
        IO_address[9] |= 0b00000100;
    }
    else if (IO_address[9] & 0b00000100)
    {
        IO_address[9] &= 0b11111011;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_4))
    {
        IO_address[9] |= 0b00000010;
    }
    else if (IO_address[9] & 0b00000010)
    {
        IO_address[9] &= 0b11111101;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_5))
    {
        IO_address[9] |= 0b00000001;
    }
    else if (IO_address[9] & 0b00000001)
    {
        IO_address[9] &= 0b11111110;
    }

    //10

    if (glfwGetKey(window, GLFW_KEY_KP_6))
    {
        IO_address[10] |= 0b10000000;
    }
    else if (IO_address[10] & 0b10000000)
    {
        IO_address[10] &= 0b01111111;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_1))
    {
        IO_address[10] |= 0b01000000;
    }
    else if (IO_address[10] & 0b01000000)
    {
        IO_address[10] &= 0b10111111;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_2))
    {
        IO_address[10] |= 0b00100000;
    }
    else if (IO_address[10] & 0b00100000)
    {
        IO_address[10] &= 0b11011111;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_3))
    {
        IO_address[10] |= 0b00010000;
    }
    else if (IO_address[10] & 0b00010000)
    {
        IO_address[10] &= 0b11101111;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_ENTER))
    {
        IO_address[10] |= 0b00001000;
    }
    else if (IO_address[10] & 0b00001000)
    {
        IO_address[10] &= 0b11110111;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_0))
    {
        IO_address[10] |= 0b00000100;
    }
    else if (IO_address[10] & 0b00000100)
    {
        IO_address[10] &= 0b11111011;
    }

    if (glfwGetKey(window, GLFW_KEY_KP_DECIMAL))
    {
        IO_address[10] |= 0b00000010;
    }
    else if (IO_address[10] & 0b00000010)
    {
        IO_address[10] &= 0b11111101;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        IO_address[10] |= 0b00000001;
    }
    else if (IO_address[10] & 0b00000001)
    {
        IO_address[10] &= 0b11111110;
    }
}

void GPUtick() {
    glClear(GL_COLOR_BUFFER_BIT);
    //drawGrid();
    /*TEST*/
    //for (int i = 0; i < 128; i+=2)
    //{
    //    setValue(0x0401 + i, 0x8F);
    //    setValue(0x0402 + i, 0xFF);
    //}
    //setValue(0x0799, 0x40);
    /*END TEST*/
    draw_background();
    draw_foreground();
    glfwSwapBuffers(window);
    glfwPollEvents();
    GetKeys();
}

void GPUinit()
{
    if (!glfwInit()) return -1;
    window = glfwCreateWindow(800, 800, "Little Boy Computer", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
}