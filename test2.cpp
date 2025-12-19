#include <stdio.h>
#include <windows.h>
#include <iostream>   
using namespace std;

int main(){
    int segundos=20;
    for(int i=0; i<=21; i++)
        cout << "\n";
    cout << "\t\t\t\t   CARGANDO...\n";
    for(int i=0; i<=79; i++)
        cout << "_";
    for(int i=0; i<=79; i++)
    {
        cout<<char(219);
        Sleep(segundos*1000/80);
    }
    cout<<"\nCompletado!";
return 0;
 }