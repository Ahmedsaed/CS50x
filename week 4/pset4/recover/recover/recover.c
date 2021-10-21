#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

#define BUFFER_SIZE 512

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Use: ./recover FileName.raw\n");
        return 1;
    }

    // open the file given using the command-line argument
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("file cannot be opened");
        return 1;
    }

    // create a picture
    FILE *picture = NULL;

    // declare two variables
    int njpg = 0; // njpg stores the number of jpgs found
    bool jpg_found = false; // used to keep track of a jpg
    unsigned char bytes[BUFFER_SIZE];// create buffer

    // go through cardfile until there aren't any blocks left
    while (fread(bytes, BUFFER_SIZE, 1, file) == 1)
    {
        /*  check the first 4 bytes of buffer
            if a jpg signature is detected
                then create a picture
        */
        if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && (bytes[3] & 0xf0) == 0xe0)
        {
            if (jpg_found)
            {
                //a new picture is found, create a new one
                fclose(picture);
            }
            else
            {
                jpg_found = true;
            }

            //create the name of the jpg and open it
            char filename[8];
            sprintf(filename, "%03i.jpg", njpg);
            picture = fopen(filename, "a");
            njpg++;
        }

        if (jpg_found)
        {
            // write 512 bytes to file
            fwrite(&bytes, BUFFER_SIZE, 1, picture);
        }
    }

    //close the opened files
    fclose(file);
    fclose(picture);

    return 0;
}


