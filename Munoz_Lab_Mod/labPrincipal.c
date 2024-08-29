/************************************************************************
*                                                                       *
* Autor: JD                                                             *
* Materia: Sistemas Operativos                                          *
* Fecha: 29-08-24                                                       *
* Tema: Lab de compliacion modular:                                     *
*   -Primera parte: Funcionamiento del código: Completado a las 10:46   *
*   -Segunda parte: Modularización del código                           *
*   -Última parte: Creación de fichero de automatización de Compilación *
*                                                                       *
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "libreriaLab.h"

int main()
{
        int i;
        vectorDinamico editor;
        vectorInicio(&editor);

        addVector(&editor, "Hola");
        addVector(&editor, "Profesional");
        addVector(&editor, "en");
        addVector(&editor, "Formación");
        addVector(&editor, "de");
        addVector(&editor, "Ingeniería");
        
        printf("\n");
        for (i = 0; i < totalVector(&editor); i++)
        printf("%s ", (char *) getVector(&editor, i));
        
        printf("\n");
        //Borramos los 5 elementos del 1 al 5
        int c=5;
        while(c > 1)
        {
        borrarVector(&editor, c); c--;}

        setVector(&editor, 1, "Buenos");
        addVector(&editor, "Días");
        

        printf("\n");
        for (i = 0; i < totalVector(&editor); i++)
        printf("%s ", (char *) getVector(&editor, i));
        
        
        freeVector(&editor);
        printf("\n");
        return 0;

}
