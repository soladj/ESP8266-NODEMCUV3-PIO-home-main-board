#include <string.h>
#define MAX_DATA_LENGHT 256

void get_command_value_list(char* commands, char* values, char* data)
{
    char* pdata = data;
    char d;
    char dd;
    char buffer[MAX_DATA_LENGHT/8];
    char* pbuffer=buffer;
    char* pcommand=commands;
    char* picommand=commands;
    char* pcdata=values;
    char* picdata=values;
    char* pibuffer=buffer;
    size_t command_len;
    int command_set;
    size_t command_data_len;
    int ci=0;
    int vi=0;
    for(int i=0;i<MAX_DATA_LENGHT;i++)
    {
        d=*(pdata++);
        if(d==';'||d==10||d==13||d==0){
            *pbuffer=0;
            pbuffer=pibuffer;
            pcommand=&commands[ci];
            ci+=16;
            picommand=pcommand;
            pcdata=&values[vi];
            vi+=16;
            picdata=pcdata;
            command_data_len=strlen(buffer);
            command_len=0;
            command_set=0;
            if(command_data_len)
            {
                for(int j=0;j<=command_data_len;j++)
                {
                    dd=*(pbuffer++);
                    if(dd!='=' && !command_set)
                    {
                        *(pcommand++)=dd;
                        command_len++;
                    }
                    else if(dd=='=')
                    {
                        *pcommand=0;
                        command_set=1;
                    }
                    else if(dd==0 && command_len!=0)
                    {
                        *pcdata=0;
                        pbuffer=pibuffer;
                        break;
                    }
                    else
                    {
                        *(pcdata++)=dd;
                    }
                }
            }
        }
        else
        {
            *(pbuffer++)=d;
        }
        if(d==0) break;
    }
}
