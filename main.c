#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "stb_image.h"
#include "stb_image_write.h"

typedef enum file_type
{
    NONE, PNG, JPG
}file_type;

bool WriteToFile(const char* input_file, const char* output_file, file_type input_file_type, file_type output_file_type)
{
    int width, height, channels;
    unsigned char* data = stbi_load(input_file, &width, &height, &channels, (input_file_type == PNG) ? 4 : 3);
    if (!data)
    {
        printf("Error loading file!\n");
        return false;
    }
    switch(output_file_type)
    {
    case PNG:
        stbi_write_png(output_file, width, height, channels, data, width*channels);
        break;
    case JPG:
        stbi_write_jpg(output_file, width, height, channels, data, 100);
        break;
    default:
        printf("Output file type not found!\n");
        free(data);
        return false;
        break;
    }
    free(data);
    return true;
}

bool GetArgs(int argc, char** argv, char** input_file, char** output_file, file_type* input_file_type, file_type* output_file_type)
{
    *input_file = NULL;
    *output_file = NULL;
    *input_file_type = NONE;
    *output_file_type = NONE;

    //input_file_type
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--from_png") == 0)
        {
            *input_file_type = PNG;
        }
        else if (strcmp(argv[i], "--from_jpg") == 0)
        {
            *input_file_type = JPG;
        }

        //output_file_type
        else if(strcmp(argv[i], "--to_png") == 0)
        {
            *output_file_type = PNG;
        }
        else if(strcmp(argv[i], "--to_jpg") == 0)
        {
            *output_file_type = JPG;
        }

        //input_file
        else if(strncmp(argv[i], "if=", 3) == 0)
        {
            *input_file = malloc((strlen(argv[i]) - 3) * sizeof(char));
            strcpy(*input_file, argv[i] + 3 * sizeof(char));
        }

        //output_file
        else if(strncmp(argv[i], "of=", 3) == 0)
        {
            *output_file = malloc((strlen(argv[i]) - 3) * sizeof(char));
            strcpy(*output_file, argv[i] + 3 * sizeof(char));
        }

        //error
        else
        {
            printf("%s is not recognized!\n", argv[i]);
            return false;
        }
    }

    //error checking
    if (input_file == NULL)
    {
        printf("Input file not set!\n");
        return false;
    }
    else if (output_file == NULL)
    {
        printf("Output file not set!\n");
        return false;
    }
    else if (input_file_type == NONE)
    {
        printf("Input file type not set!\n");
        return false;
    }
    else if (output_file_type == NONE)
    {
        printf("Output file type not set!\n");
        return false;
    }

    return true;
}

int main(int argc, char** argv)
{
    char* input_file;
    char* output_file;

    file_type input_file_type;
    file_type output_file_type;

    if (!GetArgs(argc, argv, &input_file, &output_file, &input_file_type, &output_file_type))
    {
        free(input_file);
        free(output_file);
        return -1;
    }

    printf("Input file: %s, output file: %s!\n", input_file, output_file);
    printf("Input file type: %s, output file type: %s!\n", (input_file_type == PNG) ? "png" : "jpg", (output_file_type == PNG) ? "png" : "jpg");

    if (!WriteToFile(input_file, output_file, input_file_type, output_file_type))
    {
        printf("Writing failed!\n");
        free(input_file);
        free(output_file);
        return -1;
    }

    free(input_file);
    free(output_file);
    return 0;
}