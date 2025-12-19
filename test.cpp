#include <iostream>
#include <stdlib.h>
#include <windows.h>
#define color SetConsoleTextAttribute

using namespace std;

HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void menu(int x, int y){
    gotoxy(x, y);
    cout << "\t  M  E  N  U" << endl;
    gotoxy(x, y+2);
    cout << "1.- Agregar cancion" << endl;
    gotoxy(x, y+3);
    cout << "2.- Buscar Por Artista" << endl;
    gotoxy(x, y+4);
    cout << "3.- Buscar por Nombre" << endl;
    gotoxy(x, y+5);
    cout << "4.- Editar cancion" << endl;
    gotoxy(x, y+6);
    cout << "5.- Eliminar cancion" << endl;
    gotoxy(x, y+7);
    cout << "6.- Eliminar primer cancion" << endl;
    gotoxy(x, y+8);
    cout << "7.- Eliminar ultima cancion" << endl;
    gotoxy(x, y+9);
    cout << "8.- Invertir Lista" << endl;
    gotoxy(x, y+10);
    cout << "9.- Ordenar alfabeticamente" << endl;
    gotoxy(x, y+11);
    cout << "U.- Ordenar ascendente" << endl;
    gotoxy(x, y+12);
    cout << "L.- Ordenar descendente" << endl;
    gotoxy(x, y+13);
    cout << "M.- Mostrar primer cancion" << endl;
    gotoxy(x, y+14);
    cout << "N.-Mostrar ultima cancion" << endl;
    gotoxy(x, y+15);
    cout << "R.-Reproducir cancion" << endl;
    gotoxy(x, y+16);
    cout << "Q.-Salir" << endl;

}

void interfazPrincipalVacia(){
    //Barra superior
    gotoxy(2,1); for(int i = 0; i < 116; i++){
         printf("%c",205);
    }

    //Barra inferior
    gotoxy(2,30); for (int i = 0; i < 116; i++){
        printf("%c",205);
    }

    //Esquinas
    gotoxy(2,1); printf("%c",201); // Superior izquierda
    gotoxy(118,1); printf("%c",187); // Superior derecha
    gotoxy(2,30); printf("%c",200); //Inferior izquierda
    gotoxy(118,30); printf("%c",188); //Inferior derecha

    //Barra lateral izquierda
     for (int i = 2; i <30; i++){
        gotoxy(2,i); printf("%c",186);
    }

    //Barra lateral derecha
    for (int i = 2;i < 30; i++){
        gotoxy(118,i); printf("%c",186);
    }

    /*Rectangulo de arriba
    gotoxy(29,5); for (int i = 0; i < 86; i++){
         printf("%c",205);
    }*/

    //Barra vertical de adentro
    for (int i = 2; i < 30; i++){
        gotoxy(88,i); printf("%c",186);
    }
    gotoxy(88,1); printf("%c",203);
    gotoxy(88,30); printf("%c",202);

    /*Union del rectangulito de arriba
    gotoxy(115,5); printf("%c",185); //Derecho
    gotoxy(29,5); printf("%c",204); //Izquierdo
    gotoxy(29,1); printf("%c",203);//Arriba
    gotoxy(29,28); printf("%c",202); //Abajo*/

    gotoxy(118,25); printf("%c",185);
    gotoxy(88,25); printf("%c",204);
    for (int i = 89; i < 118; i++){
        gotoxy(i,25); printf("%c",205);
    }

    gotoxy(88, 20); printf("%c",185);
    gotoxy(2, 20); printf("%c",204);
    for (int i = 3; i < 88; i++){
        gotoxy(i,20); printf("%c",205);
    }
}

void progressBar(){
    gotoxy(6,24); printf("%c",218); // Superior izquierda
    gotoxy(84,24); printf("%c",191); // Superior derecha
    gotoxy(6,26); printf("%c",192); //Inferior izquierda
    gotoxy(84,26); printf("%c",217); //Inferior derecha

    gotoxy(6,25); printf("%c",179);
    gotoxy(84,25); printf("%c",179);

    gotoxy(7,24); for(int i = 7; i < 84; i++){
         printf("%c",196);
    }

    //Barra inferior
    gotoxy(7,26); for (int i = 7; i < 84; i++){
        printf("%c",196);
    }

    gotoxy(7,25); for (int i = 7; i < 84; i++){
        if(i%2 == 0)
            printf("%c",220);
        else
            printf("%c",223);
    }
}

void controls(){
    //NOW PLAYING
    gotoxy(40,21); cout << "Now Playing:";
    //PAUSE
    gotoxy(45,28); printf("%c",179);
    gotoxy(47,28); printf("%c",179);
    gotoxy(46,29); printf("%c",75);

    //NEXT
    gotoxy(53,28); printf("%c",62);
    gotoxy(55,28); printf("%c",62);
    gotoxy(54,29); printf("%c",76);

    //PREV
    gotoxy(37,28); printf("%c",60);
    gotoxy(39,28); printf("%c",60);
    gotoxy(38,29); printf("%c",74);

    //MENU
    gotoxy(78,28); cout << "MENUS";
    gotoxy(80,29); printf("%c",69);
}

void playScreen(){
    gotoxy(6,2); cout << "\tTITLE";

}

int main(){
    system("color F1");
    interfazPrincipalVacia();
    menu(90, 3);
    progressBar();
    controls();
    gotoxy(30,15);


    return 0;
}
