#include <windows.h>
#include <digitalv.h>
#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <windows.h>

using namespace std;

int idSet = 0;
string dataPath = "D:\\UNI\\Estructura de datos\\REPRO\\data.txt";

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

class Repro{
    public:
        Repro(){paused = false;}
        ~Repro(){}

        void load(string path){
            this->path = path;
            string command = "open "+ path +" type waveaudio alias mysound";
            mciSendString(command.c_str(), NULL, 0, NULL);
        }
        void play();
        void stop();
        void pauseResume();
        void unload();

        string path;
        bool paused;
};


void Repro::play(){
    mciSendString("play mysound", NULL, 0, NULL);
}

void Repro::pauseResume(){
    if(paused){
        mciSendString("resume mysound", NULL, 0, NULL);
        paused = false;
    }
    else{
        mciSendString("pause mysound", NULL, 0, NULL);
        paused = true;
    }
}

void Repro::stop(){
    mciSendString("stop mysound", NULL, 0, NULL);
}

void Repro::unload(){
    mciSendString("close mysound", NULL, 0, NULL);
}

class Song{
    public:
        Song *next;
        Song *prev;


        int id;
        int length;
        string path;
        string title;
        string author;
        string album;
};

class SongList{
    private:
        Song *ptrHead;
    public:
        Song *Actual;
        Song *Selection;

        Song* getHead(){return ptrHead;}

        SongList(){ptrHead = NULL; Actual = ptrHead; Selection = Actual;}
        void insert_First(int, string, string, string, string);
        void insert_Last(int, string, string, string, string);
        bool empty();
        void print(int, int);
        void moveTo(int);
        void songRemove(int);
        void modify(int, string, string, string, string);
        void search_id(int);

        void sort_up();
        void sort_down();

        void download();
        void upload();

        void playScreen();

        void playSong(Repro);
};

void SongList::playSong(Repro player){
    player.unload();
    player.load(Actual->path);

    //UPDATE NOW PLAYING

    //clear line
    for(int i=3; i < 87; i++){
        gotoxy(i,23); cout << " ";
    }
    //print
    gotoxy(7,23);
    cout << Actual->author;
    gotoxy(42, 23);
    cout << Actual->title;
    gotoxy(75, 23);
    cout << Actual->album;

    player.play();
}

void SongList::download(){
    fstream file;
    string path;
    string title;
    string author;
    string album;
    file.open(dataPath, ios::in);

    getline(file, path);//GETLINE ESPACIOS
    getline(file, title);
    getline(file, author);
    getline(file, album);
    while(!file.eof()){
        insert_Last(idSet, path, title, author, album);
        getline(file, path);
        getline(file, title);
        getline(file, author);
        getline(file, album);
    }
    Actual = ptrHead;
    file.close();
}

void SongList::upload(){
    Song *temp = ptrHead;
    fstream file;
    string  cmd = "Del " + dataPath;
    system(cmd.c_str());
    file.open(dataPath, ios::out);

    do{
        file << temp->path << endl;
        file << temp->title << endl;
        file << temp->author << endl;
        file << temp->album << endl;
        temp = temp->next;
    }while(temp != ptrHead);
    file.close();
}

void SongList::insert_First(int id, string path, string title, string author, string album){
    Song *new_song = new Song();
    new_song->id = id;
    new_song->path = path;
    new_song->title = title;
    new_song->author = author;
    new_song->album = album;

    if(ptrHead == NULL){
        new_song->next = new_song;

        new_song->prev = new_song;
        ptrHead = new_song;
    }
    else{
        Song *last_song = ptrHead->prev;

        new_song->next = ptrHead;

        new_song->prev = last_song;

        ptrHead->prev = new_song;

        last_song->next = new_song;
        ptrHead = new_song;
    }
    idSet++;
}

void SongList::insert_Last(int id, string path, string title, string author, string album){
    Song *new_song = new Song();
    new_song->id = id;
    new_song->path = path;
    new_song->title = title;
    new_song->author = author;
    new_song->album = album;

    if(ptrHead == NULL){
        new_song->next = new_song;

        new_song->prev = new_song;
        ptrHead = new_song;
    }
    else{
        Song *last_song = ptrHead->prev;

        new_song->next = ptrHead;

        new_song->prev = last_song;

        ptrHead->prev = new_song;

        last_song->next = new_song;
    }
    idSet++;
}

bool SongList::empty(){
    if(ptrHead == NULL)
        return true;
    else
        return false;
}

void SongList::print(int x, int y){
    Song *temp = ptrHead;
    int i=0;
    do{
        if (!empty()){
            if(temp == Selection){
                gotoxy(x-2,y+i);
                cout << "->";
            }
            gotoxy(x, y+i);
            cout<<temp->id <<"\t" << temp->title;
            gotoxy(x+30, y+i);
            cout << temp->author;
            temp = temp->next;
            i++;
        }
    }while(temp != ptrHead);
}


void SongList::moveTo(int opc){
    if(!empty()){
        Song *temp = ptrHead;
        if(opc==1){
            cout<<"[ "<<temp->id <<" ]"<<endl;
            temp = temp->next;
            ptrHead=temp;
        }
        else if(opc==2){
            cout<<"[ "<<temp->id <<" ]"<<endl;
            temp = temp->prev;
            ptrHead=temp;
        }
    }
}

void SongList::songRemove(int id){
    Song *temp = ptrHead;
    int flag = 1;

    if(id == -1){
        id = ptrHead->id;
    }
    else if(id == -2){
        id = ptrHead->prev->id;
    }

    do{
        if(temp->id == id){
            flag = 0;
            break;
        }
        temp = temp->next;
    }while(temp != ptrHead);

    if(flag == 0){
        if(temp == ptrHead){
            ptrHead = temp->next;
            temp->next->prev = temp->prev;
            temp->prev->next = temp->next;
        }
        else{
            temp->next->prev = temp->prev;
            temp->prev->next = temp->next;
        }

        delete(temp);
    }
    else{cout << "id no encontrado" << endl;}
}

void SongList::modify(int id, string path, string title, string author, string album){
    Song *temp = ptrHead;
    int flag = 1;

    do{
        if(temp->id == id){
            flag = 0;
            break;
        }
        temp = temp->next;
    }while(temp != ptrHead);

    if(flag == 0){
        temp->id = id;
        temp->path = path;
        temp->title = title;
        temp->author = author;
        temp->album = album;
    }
    else{cout << "id no encontrado" << endl;}
}

void SongList::search_id(int id){
    Song *temp = ptrHead;
    int flag = 0;

    do{
        if(temp->id == id){
            flag = 0;
            break;
        }
        temp = temp->next;
    }while(temp != ptrHead);

    if(flag == 0){
        cout << "ID: " << temp->id << endl;
        cout << "Title: " << temp->title << endl;
        cout << "Author: " << temp->author << endl;
        cout << "Album: " << temp->album << endl;
    }
    else{cout << "id no encontrado" << endl;}
}

void SongList::sort_up(){
    Song *temp = ptrHead;
    Song *sig = temp->next;
    int flag = 0;

    do{
        flag = 0;
        temp = ptrHead;
        sig = temp->next;
        do{
            if(temp->id > sig->id){
                flag = 1;
                if(temp == ptrHead)
                    ptrHead = sig;
                else{
                    temp->prev->next = sig;
                    sig->next->prev = temp;

                    temp->next = sig->next;
                    sig->prev = temp->prev;

                    temp->prev = sig;
                    sig->next = temp;
                }
            }

            temp = sig;
            sig = temp->next;
        }while(temp != ptrHead && sig != ptrHead);
    }while(flag != 0);
}

void SongList::sort_down(){
    Song *temp = ptrHead;
    Song *sig = temp->next;
    int flag = 0;

    do{
        flag = 0;
        temp = ptrHead;
        sig = temp->next;
        do{

            if(temp->id < sig->id){
                flag = 1;
                if(temp == ptrHead)
                    ptrHead = sig;
                else{
                    temp->prev->next = sig;
                    sig->next->prev = temp;

                    temp->next = sig->next;
                    sig->prev = temp->prev;

                    temp->prev = sig;
                    sig->next = temp;
                }
            }

            temp = sig;
            sig = temp->next;
        }while(temp != ptrHead && sig != ptrHead);
    }while(flag != 0);
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
    cout << "Y.- Ordenar descendente" << endl;
    gotoxy(x, y+13);
    cout << "M.- Mostrar primer cancion" << endl;
    gotoxy(x, y+14);
    cout << "N.- Mostrar ultima cancion" << endl;
    gotoxy(x, y+15);
    cout << "R.- Reproducir cancion" << endl;
    gotoxy(x, y+16);
    cout << "Q.- Salir" << endl;

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

void SongList::playScreen(){
    Song *temp = ptrHead;
    gotoxy(6,2); cout << "\tTITLE";
    gotoxy(36,2); cout << "AUTHOR";
    gotoxy(56,2); cout << "\tGENRE";

    print(6, 4);

    gotoxy(6, 19); cout << "Prev";
    gotoxy(6, 18); cout << ",";
    gotoxy(12, 19); cout << "Next";
    gotoxy(12, 18); cout << ".";
}

void clearScreen(){
    for(int i=4; i<85; i++){
        for(int j=2; j<20; j++){
            gotoxy(i,j);
            cout << " ";
        }
    }

}


int main() {
    system("color F1");
    SongList playlist;
    playlist.download();

    playlist.Selection = playlist.Actual;

    interfazPrincipalVacia();
    menu(90, 3);
    controls();
    progressBar();
    playlist.playScreen();

    Repro player;
    playlist.playSong(player);


    //PRINT MODEL
    while(true){
        if (_kbhit()){
            char ch = _getch();
            switch(ch){
                case 'j':
                    player.stop();
                    playlist.Actual = playlist.Actual->prev;
                    playlist.playSong(player);
                    break;
                case 'k':
                    player.pauseResume();
                    break;
                case 'l':
                    player.stop();
                    playlist.Actual = playlist.Actual->next;
                    playlist.playSong(player);
                    break;
                case 'q':
                    playlist.upload();
                    mciSendString("close mysound", NULL, 0, NULL);
                    return 0;
                    break;
                case '.':
                    playlist.Selection = playlist.Selection->next;
                    clearScreen();
                    playlist.playScreen();
                    break;
                case ',':
                    playlist.Selection = playlist.Selection->prev;
                    clearScreen();
                    playlist.playScreen();
                    break;
                case 'r':
                    player.stop();
                    playlist.Actual = playlist.Selection;
                    playlist.playSong(player);
                    break;
                case 'u':
                    playlist.sort_up();
                    clearScreen();
                    playlist.playScreen();
                    break;
                case 'y':
                    playlist.sort_down();
                    clearScreen();
                    playlist.playScreen();
                    break;
                case '1':
                    break;
                case '2':
                    break;
                case '3':
                    break;
                case '4':
                    break;
                case '5':
                    int id = playlist.Selection->id;
                    playlist.Selection = playlist.Selection->next;
                    playlist.songRemove(id);
                    clearScreen();
                    playlist.playScreen();
                    //playlist
                    break;
                case '6':
                    int id = playlist.Selection->id;
                    playlist.Selection = playlist.Selection->next;
                    playlist.songRemove(id);
                    clearScreen();
                    playlist.playScreen();
                    //playlist
                    break;
                case '7':
                    break;
                case '8':
                    break;
                case '9':
                    break;
                default:
                    gotoxy(90,28); cout << "Ingrese una opcion valida.";
                    break;
            }
        }
    }

    return 0;
}

