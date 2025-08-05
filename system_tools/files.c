#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <system_tools.h>

TFileData read_file_data(FILE *file)
{
    TFileData result;
    if (file == NULL)
    {
        goto error;
    }

    fseek(file, 0, SEEK_END);
    long dataSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char *)malloc(dataSize + 1);
    if (data == NULL)
    {
        goto error;
    }
    fread(data, 1, dataSize, file);
    result.data = data;
    result.dataSize = dataSize;
    return result;

error:
    result.data = NULL;
    result.dataSize = 0;
    return result;
}

void delete_file_data(TFileData data){
    if (data.data == NULL) return ;
    free(data.data);
}