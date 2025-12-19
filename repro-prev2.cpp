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

void clearScreen(){
    for(int i=4; i<85; i++){
        for(int j=2; j<20; j++){
            gotoxy(i,j);
            cout << " ";
        }
    }

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
        void getLenght();

        string path;
        bool paused;
};

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

        void search_id(string);
        void author_search(string);
        void title_search(string);

        void sort_up();
        void sort_down();
        void invert();

        void download();
        void upload();

        void playScreen();

        void playSong();
};

Repro player;
SongList playlist;


class ProgressBar{
    public:
        ProgressBar(int x, int y){
            this->x = x;
            this->y = y;
        }

        int time;
        int actualTime;
        int x;
        int y;

        void printBar();
        void start();
        void pause();
};

ProgressBar bar(6,24);

void ProgressBar::start(){
    int id;
    //gotoxy(x+1,y+1);
    for(int i = actualTime; i < 77; i++){
        gotoxy(x+1+i, y+1);
        printf("%c",219);
        Sleep(time/77);
        if (_kbhit()){
            char ch = _getch();
            switch(ch){
                case 'k':
                    actualTime = i;
                    player.pauseResume();
                    i = 78;
                    break;
                case 'j':
                    player.stop();
                    actualTime = 0;
                    i = 78;
                    x = 6;
                    playlist.Actual = playlist.Actual->prev;
                    printBar();
                    playlist.playSong();
                    start();
                    break;
                case 'l':
                    player.stop();
                    actualTime = 0;
                    i = 78;
                    x = 6;
                    playlist.Actual = playlist.Actual->next;
                    printBar();
                    playlist.playSong();
                    start();
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
                    player.stop();\
                    i = 78;
                    playlist.Actual = playlist.Selection;
                    printBar();
                    playlist.playSong();
                    start();
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
                case '5':
                    id = playlist.Selection->id;
                    playlist.Selection = playlist.Selection->next;
                    playlist.songRemove(id);
                    clearScreen();
                    playlist.playScreen();
                    break;
                case '6':
                    playlist.Selection = playlist.getHead();
                    id = playlist.Selection->id;
                    playlist.Selection = playlist.Selection->next;
                    playlist.songRemove(id);
                    clearScreen();
                    playlist.playScreen();
                    break;
                case '7':
                    playlist.Selection = playlist.getHead();
                    playlist.Selection = playlist.Selection->prev;
                    id = playlist.Selection->id;
                    playlist.Selection = playlist.Selection->next;
                    playlist.songRemove(id);
                    clearScreen();
                    playlist.playScreen();
                    break;
                case '8':
                    playlist.invert();
                    clearScreen();
                    playlist.playScreen();
                    break;
                case '9':
                    break;
                default:
                    gotoxy(90,28); cout << "Pause para mas funciones.";
                    break;
            }
        }
    }
}

void ProgressBar::printBar(){
    gotoxy(x,y); printf("%c",218); // Superior izquierda
    gotoxy(x+78,y); printf("%c",191); // Superior derecha
    gotoxy(x,y+2); printf("%c",192); //Inferior izquierda
    gotoxy(x+78,y+2); printf("%c",217); //Inferior derecha

    gotoxy(x,y+1); printf("%c",179);
    gotoxy(x+78,y+1); printf("%c",179);

    gotoxy(x+1,y); for(int i = 0; i < 77; i++){
         printf("%c",196);
    }

    //Barra inferior
    gotoxy(x+1,y+2); for (int i = 0; i < 77; i++){
        printf("%c",196);
    }

    gotoxy(x+1,y+1); for (int i = 0; i < 77; i++){
        if(i%2 == 0)
            printf("%c",220);
        else
            printf("%c",223);
    }
}

void Repro::play(){
    mciSendString("play mysound", NULL, 0, NULL);
    paused = false;
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

void Repro::getLenght(){
    char *ret = new char('z');
    mciSendString("status mysound lenght", ret, 0, NULL);

    gotoxy(10,15);
    cout << *ret;
}

void SongList::playSong(){
    player.unload();
    player.load(Actual->path);

    bar.actualTime = 0;
    bar.x = 6;

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
        if(path != "."){temp->path = path;}
        if(title != "."){temp->title = title;}
        if(author != "."){temp->author = author;}
        if(album != "."){temp->album = album;}
    }
    else{cout << "id no encontrado" << endl;}
}

void SongList::search_id(string id){
    Song *temp = ptrHead;
    int flag = 0;

    do{
        if(to_string(temp->id) == id){
            flag = 0;
            break;
        }
        temp = temp->next;
    }while(temp != ptrHead);

    if(flag == 0){
        player.stop();
        Actual = temp;
        playSong();
    }
    else{gotoxy(90,28);cout << "id no encontrado" << endl;}
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

void SongList::author_search(string ts){
    Song *temp = ptrHead;
    int i;
    do{
        if(temp->author == ts){
            gotoxy(6, 4+i);
            cout<<temp->id <<"\t" << temp->title;
            gotoxy(6+30, 4+i);
            cout << temp->author;
            i++;
        }
        temp = temp->next;
    }while(temp != ptrHead);
}

void SongList::title_search(string ts){
    Song *temp = ptrHead;
    int i;
    do{
        if(temp->title == ts){
            gotoxy(6, 4+i);
            cout<<temp->id <<"\t" << temp->title;
            gotoxy(6+30, 4+i);
            cout << temp->author;
            i++;
        }
        temp = temp->next;
    }while(temp != ptrHead);
}

void SongList::invert(){
    Song *actual = ptrHead;
    Song *siguiente = NULL;
    Song *temp = NULL;

    do{
        siguiente = actual->next;
        actual->next = actual->prev;
        actual->prev = siguiente;
        temp = actual;
        actual = siguiente;
    }while (actual != ptrHead);
    ptrHead = temp;
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
    //cout << "M.- Mostrar primer cancion" << endl;
    cout << "R.- Reproducir cancion" << endl;
    gotoxy(x, y+14);
    //cout << "N.- Mostrar ultima cancion" << endl;
    //gotoxy(x, y+15);
    //gotoxy(x, y+16);
    cout << "Q.- Salir" << endl;

}

void progressBar(int x, int y){
    gotoxy(x,y); printf("%c",218); // Superior izquierda
    gotoxy(x+78,y); printf("%c",191); // Superior derecha
    gotoxy(x,y+2); printf("%c",192); //Inferior izquierda
    gotoxy(x+78,y+2); printf("%c",217); //Inferior derecha

    gotoxy(x,y+1); printf("%c",179);
    gotoxy(x+78,y+1); printf("%c",179);

    gotoxy(x+1,y); for(int i = 0; i < 77; i++){
         printf("%c",196);
    }

    //Barra inferior
    gotoxy(x+1,y+2); for (int i = 0; i < 77; i++){
        printf("%c",196);
    }

    gotoxy(x+1,y+1); for (int i = 0; i < 77; i++){
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

int main() {
    int id;
    int length;
    string ids;
    string path;
    string title;
    string author;
    string genre;
    system("color F1");
    //Repro player;
    //SongList playlist;
    playlist.download();

    bar.time = 60000;


    playlist.Selection = playlist.Actual;

    interfazPrincipalVacia();
    menu(90, 3);
    controls();
    bar.printBar();
    playlist.playScreen();


    playlist.playSong();

    bar.start();

    player.getLenght();
    //PRINT MODEL
    while(true){
        if (_kbhit()){
            char ch = _getch();
            switch(ch){
                case 'j':
                    player.stop();
                    bar.actualTime = 0;
                    bar.x = 6;
                    playlist.Actual = playlist.Actual->prev;
                    bar.printBar();
                    playlist.playSong();
                    bar.start();
                    break;
                case 'k':
                    player.pauseResume();
                    bar.start();
                    break;
                case 'l':
                    player.stop();
                    bar.actualTime = 0;
                    bar.x = 6;
                    playlist.Actual = playlist.Actual->next;
                    bar.printBar();
                    playlist.playSong();
                    bar.start();
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
                    playlist.playSong();
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
                    clearScreen();
                    gotoxy(5,4);
                    cout << "New path: ";
                    cin >> path;
                    gotoxy(5,6);
                    cout << "New title: ";
                    cin.ignore();
                    getline(cin, title);
                    gotoxy(5,8);
                    cout << "New author: ";
                    getline(cin, author);
                    gotoxy(5,10);
                    cout << "New genre: ";
                    getline(cin, genre);

                    playlist.insert_Last(idSet, path, title, author, genre);
                    clearScreen();
                    playlist.playScreen();
                    break;
                case '2':
                    clearScreen();
                    gotoxy(5,15);
                    cout << "Search: ";
                    getline(cin, author);
                    playlist.author_search(author);
                    gotoxy(5,17);
                    cout << "Ingrese un id o . para continuar: ";
                    cin >> ids;
                    if(ids == "."){
                        clearScreen();
                        playlist.playScreen();
                    }
                    else{
                        playlist.search_id(ids);
                        clearScreen();
                        playlist.playScreen();
                    }
                    break;
                case '3':
                    clearScreen();
                    gotoxy(5,15);
                    cout << "Search: ";
                    getline(cin, title);
                    playlist.title_search(title);
                    gotoxy(5,17);
                    cout << "Ingrese un id o . para continuar: ";
                    cin >> ids;
                    if(ids == "."){
                        clearScreen();
                        playlist.playScreen();
                    }
                    else{
                        playlist.search_id(ids);
                        clearScreen();
                        playlist.playScreen();
                    }
                    break;
                case '4':
                    clearScreen();
                    gotoxy(20,3);
                    cout << "Ingrese . para no modificar el parametro";
                    gotoxy(5,4);
                    cout << "New path: ";
                    cin >> path;
                    gotoxy(5,6);
                    cout << "New name: ";
                    cin.ignore();
                    getline(cin, title);
                    gotoxy(5,8);
                    cout << "New author: ";
                    getline(cin, author);
                    gotoxy(5,10);
                    cout << "New genre: ";
                    getline(cin, genre);

                    playlist.modify(playlist.Selection->id, path, title, author, genre);
                    clearScreen();
                    playlist.playScreen();
                    break;
                case '5':
                    id = playlist.Selection->id;
                    playlist.Selection = playlist.Selection->next;
                    playlist.songRemove(id);
                    clearScreen();
                    playlist.playScreen();
                    break;
                case '6':
                    playlist.Selection = playlist.getHead();
                    id = playlist.Selection->id;
                    playlist.Selection = playlist.Selection->next;
                    playlist.songRemove(id);
                    clearScreen();
                    playlist.playScreen();
                    break;
                case '7':
                    playlist.Selection = playlist.getHead();
                    playlist.Selection = playlist.Selection->prev;
                    id = playlist.Selection->id;
                    playlist.Selection = playlist.Selection->next;
                    playlist.songRemove(id);
                    clearScreen();
                    playlist.playScreen();
                    break;
                case '8':
                    playlist.invert();
                    clearScreen();
                    playlist.playScreen();
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

