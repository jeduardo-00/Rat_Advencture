#include <stdio.h>
#include <string.h>

typedef struct 
{
    char username[16];
    int score;
}usuario;


int main ()
{
    FILE * arq_temp = fopen("C:\\Project_gamer\\assets\\save\\score.dat","rb");
    int cont = 0;
    usuario temp;

    while (fread(&temp, sizeof(usuario), 1, arq_temp))
    {
        printf("%s\n", temp.username);
        cont++;
    }
    fclose(arq_temp);
    printf("%d",cont);

    return 0;
}

