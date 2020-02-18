#include <iostream> //biblioteca principala
#include <fstream>  //fsiere
#include <string.h> //siruri de caractere
#include <conio.h>  //getch()
#include <cstdlib>  //system(), srand(), rand()
#include <ctime>    //time()
#include <unistd.h> //sleep()

using namespace std;

struct dictionar///Initializare lista de tip dinamic
{
    int numar;
    char cuvant[20];
    dictionar* urmator;
};
dictionar* cap = NULL;

/// val = 1 -> Se afiseaza cuvintele la finalul rularii
bool bool_afisare_lista = 0;

/// val = 1 -> Se modifica lista intiala de cuvinte
bool bool_modif = 0;

/// val = 1 -> Se verifica existenta cuvantului in Limba Romana
bool bool_exist_verif = 1;

/// val = 1 -> Se testeaza cuvantul generat in cazul unui cuvant introdus
bool bool_test_calc = 0;

///lista subprograme
void jocul_propriu_zis();
void citire();
int random(int, int);
char* start(dictionar*);
void spatiere(int);
char* jucator_cuvant(char*, char*, int&);
void calculator_cuvant(char*, char*, int&);
bool testare(char*, char*);
bool existenta(char*);
void adaugare_cuvant(char*);
void afisare_scor(int, int);
void resetare(dictionar*);
void modif(dictionar*);
void testare_introducere_calculator();
void afisare_lista(dictionar*);
void stergere_lista(dictionar*);

int main()
{
    jocul_propriu_zis();
}

///Subprograme
void jocul_propriu_zis()///Subprogram ce reprezinta jocul in sine
{
    int k=1, pierd_jucator=0, pierd_calculator=0;
    char cuvc[20], cuvj[20];
    citire();
    if(bool_test_calc == 1)
    {
        testare_introducere_calculator();
        goto sfarsit;
    }
    ///Afisarea de inceput
	//system("Color 0F");
    cout << "         ---------F-A-Z-A-N---------" << endl;
    cout << "                       de Tomuta Gabriel" << endl << endl;
    cout << "  Jocul se realizeaza intre dumneavoastra si calculator." << endl;
    cout << "  Scopul acestui joc este de al inchide pe celalalt " << endl;
    cout << "folosind cuvinte tot mai diverse. Singura regula ce" << endl;
    cout << "trebuie urmata este ca urmatorul cuvant introdus sa" << endl;
    cout << "inceapa cu ultimele doua litere ale cuvantului ante-" << endl;
    cout << "rior. In cazul in care nu se cunoaste cuvant, se va" << endl;
    cout << "introduce valoarea '0'." << endl;
    cout << "  Pentru fiecare pierdere, jucatorul in cauza va primi" << endl;
    cout << "o litera din cuvantul 'FAZAN', completarea cuvantului" << endl;
    cout << "aducand astfel pierderea jocului." << endl;
    cout << "  !Se pot introduce doar cuvinte cu litere mici ale" << endl;
    cout << "  !alfabetului englez." << endl;
    cout << "  Mult noroc!" << endl << endl;
    cout << "Pentru a incepe, apasati orice tasta.";getch();

    ///De aici incepe jocul
	//system("Color 0A");
    while(pierd_jucator<5 && pierd_calculator<5)
    {
        ///Afisarea scorului
        system("CLS");
        cout << "                Scor curent:" << endl;
        cout << "Calculator:                            Jucator:"<< endl;
        afisare_scor(pierd_jucator,pierd_calculator);

        ///Startul jocului
        cout << "Alegeti o litera de inceput:           ";
        strcpy(cuvc,start(cap));

        ///Se repeta pana la pierderea unuia
        while(k==1)
        {
            jucator_cuvant(cuvc,cuvj,k);
            calculator_cuvant(cuvc,cuvj,k);
        }

        ///Atribuire scor
        if(k==2)
            pierd_calculator++;
        else if(k==3)
            pierd_jucator++;
        resetare(cap);
        k=1;
    }

    ///Finalul
    system("CLS");
    cout << "                Scor curent:" << endl;
    cout << "Calculator:                            Jucator:"<< endl;
    afisare_scor(pierd_jucator,pierd_calculator);
    if(pierd_jucator==5)
        cout << "FELICITARI ATI CASTIGAT!!" << endl;
    else
        cout << "FELICITARI ATI PIERDUT!!" << endl;

sfarsit:
    if(bool_modif)
        modif(cap);
    if(bool_afisare_lista)
        afisare_lista(cap);
    stergere_lista(cap);
}

void citire()///Citirea propriu zisa a listei de cuvinte
{
    ifstream f;
    f.open("Dictionar mediu.txt");
    char cuv[20];
    while (!f.eof())
    {
        dictionar *elem = new dictionar;
        elem -> numar = 0;
        f >> cuv;
        strcpy(elem -> cuvant, cuv);
        elem -> urmator = NULL;

        if(cap==NULL)
            cap=elem;
        else
        {
            dictionar *nod = cap;
            while(nod->urmator != NULL)
                nod = nod->urmator;
            nod -> urmator = elem;
        }
    }
    f.close();
}

int random(int s, int d)///Generarea unui numar random din intervalul [s, d]
{
    srand(time(0));
    return s+(rand()%(d+1-s));
}

char* start(dictionar* cap)///Startul jocului, atribuirea primului cuvant
{
    char litera;
    cin >> litera;
    while(cap->cuvant[0]!=litera)
    {
        cap = cap -> urmator;
    }

    dictionar *inter = new dictionar;
    inter -> numar = 0;
    inter -> urmator = cap;

    int numarare=0;
    while(cap->cuvant[0]==litera)
    {
        cap = cap -> urmator;
        numarare++;
    }

    int numar_rd=random(1, numarare);
    for(int i=0;i<numar_rd;i++)
        inter = inter -> urmator;

    inter -> numar = 1;
    return inter -> cuvant;
}

void spatiere(int lungime)///Aranjarea pe doua coloane a afisarilor
{
    for(int i=39;i>lungime;i--)
        cout <<' ';
}

char* jucator_cuvant(char cuvc[20], char cuvj[20], int &k)///Reprezinta introducerea de cuvant a jucatorului
{
    bool iesire = false;
    while(!iesire && k==1)
    {

        cout <<cuvc;
        spatiere(strlen(cuvc));
        cin >> cuvj;
        iesire= false;
        if(strncmp(cuvj,"0",1)==0)
            iesire=true;
        else
        {
            bool resultat = testare(cuvc,cuvj);
            if (resultat == false)
                cout <<  "             Cuvant incorect" << endl;
            if (resultat == true && strlen(cuvj)<3)
            {
                resultat = false;
                cout << "             Cuvant prea scurt" << endl;
            }
            if (resultat == true && bool_exist_verif == 1)
                resultat = existenta(cuvj);
            if (resultat == true)
            {
                dictionar *p = new dictionar;
                p -> numar = 0;
                p -> urmator=cap;
                while (p->urmator != NULL && strcmp(p->urmator->cuvant, cuvj)<0)
                        p = p->urmator;
                if((strcmp(p->urmator->cuvant,cuvj)==0 && p->urmator->numar==0) || (strcmp(p->urmator->cuvant,cuvj)!=0))
                {
                    iesire = true;
                    adaugare_cuvant(cuvj);
                }
            }
        }
    };
    if(strncmp(cuvj,"0",1) == 0)
        k=2;
}

void calculator_cuvant(char cuvc[20], char cuvj[20], int &k)///Reprezinta introducerea de cuvant a calculatorului
{
    dictionar *p = new dictionar;
    p -> numar = 0;
    p -> urmator = cap;
    bool iesire = false;
    while (p -> urmator != NULL && !iesire)
    {
        strcpy(cuvc, p->urmator->cuvant);
        bool rezultat=testare(cuvj, cuvc);
        if(rezultat == true && p->urmator->numar == 0)
            iesire = true;
        else
            p = p->urmator;
    }

    if(iesire)
    {
        dictionar *copie = new dictionar;
        copie -> numar = 0;
        copie -> urmator = p -> urmator;

        int numarare=0;
        while (p -> urmator != NULL && iesire)
        {
            if(testare(cuvj, p->urmator->cuvant) == false)
                iesire = false;
            else
            {
                p = p->urmator;
                numarare++;
            }
        }

        dictionar *inter = new dictionar;
        inter -> numar = 0;
        strcpy(inter -> cuvant, "Eroare_cuvant_calculator_inter");
        while(!iesire)
        {
            inter -> urmator = copie;

            int numar_rd=random(1, numarare);
            for(int i=0;i<numar_rd;i++)
                inter -> urmator = inter -> urmator -> urmator;
            if(inter -> urmator -> numar == 0)
            {
                inter -> urmator -> numar = 1;
                iesire=true;
            }
            if(!iesire)
                sleep(1);
        }
        strcpy(cuvc, inter->urmator->cuvant);
    }
    if(!iesire && k==1)
        k=3;
}

bool testare(char cuvc[20], char cuvj[20])///Testarea conditiei de introducere a cuvintelor
{
    char a[20], b[20];
    strcpy(a,cuvc);
    strcpy(a,a+strlen(a)-2);
    strcpy(b,cuvj);
    if(a[0]==b[0] && a[1]==b[1])
        return true;
    else
        return false;

}

bool existenta(char cuvj[20])///Testarea existentei cuvantului in Limba Romana
{
    char sursa[]="Dictionar/Dex0.txt";
    char text[20];
    bool test=false;
    sursa[13]=cuvj[0];
    ifstream f(sursa);
    do
    {
        f >> text;
        if(strcmp(text,cuvj)==0)
        {
            test=true;
        }
    }while(!f.eof() && text<cuvj);
    f.close();
    if (test==true)
        return true;
    else
    {
        cout << "             Cuvant invalid" << endl;
        return false;
    }
}

void adaugare_cuvant(char cuv[20])///Imbogatirea listei de cuvinte cu cele noi introduse
{
    char intermediar[20];
    dictionar *p = new dictionar;
    p -> numar = 0;
    p -> urmator=cap;
    while (p->urmator != NULL && strcmp(p->urmator->cuvant, cuv)<0)
            p = p->urmator;
    if(strcmp(p->urmator->cuvant,cuv)!=0 || p->urmator==NULL)
    {
        //imbogatire(cuv);
        dictionar *elem = new dictionar;
        elem -> numar = 1;
        strcpy(elem->cuvant,cuv);
        if (cap == p)
        {
            elem->urmator = cap;
            cap = elem;
        }
        else
        {
            elem->urmator = p->urmator;
            p->urmator = elem;
        }
    }
    else if(strcmp(p->urmator->cuvant,cuv)==0)
        p->urmator->numar=1;
}

void afisare_scor(int juc, int calc)///Reprezinta scorul
{
    switch (juc)
    {
    case 1:
        cout << "F";
        break;
    case 2:
        cout << "FA";
        break;
    case 3:
        cout << "FAZ";
        break;
    case 4:
        cout << "FAZA";
        break;
    case 5:
        cout << "FAZAN";
        break;
    default:
        break;
    }
    spatiere(juc);
    switch (calc)
    {
    case 1:
        cout << "F";
        break;
    case 2:
        cout << "FA";
        break;
    case 3:
        cout << "FAZ";
        break;
    case 4:
        cout << "FAZA";
        break;
    case 5:
        cout << "FAZAN";
        break;
    default:
        break;
    }
    cout << endl << endl;
}

void resetare(dictionar* cap)///Resetarea listei de cuvinte pentru o noua partida a jocului
{
    while(cap != NULL)
    {
        if(cap->numar==1)
            cap->numar=0;
        cap = cap->urmator;
    }
}

void modif(dictionar* cap)///Imbogatirea dictionarului folosit de catre program
{
    bool test=0;
    cout << "Doriti sa adaugati cuvintele noi la dictionar?" << endl << ':';
    cin >> test;
    if(test == 1)
    {
        ofstream g("Dictionar mediu.txt");
        while(cap != NULL)
        {
            g << cap->cuvant << ' ';
            cap = cap->urmator;
        }
        g.close();
    }
}

void testare_introducere_calculator()///Program auxiliar: testarea introducerii de catre calculator
{
    int repetare, k = 1;
    char cuvj[20], cuvc[20];
    cout << "Testare introducere calculator:" << endl;
    cout << "De cate ori? "; cin >> repetare;
    cout << "Va raspunde la cuvantul: "; cin >> cuvj;
    for(int i=0;i<repetare;i++)
    {
        sleep(1);
        calculator_cuvant(cuvc, cuvj, k);
        if(k==3)
        {
            cout << "Nu mai sunt cuvinte care sa satisfaca conditia." << endl;
            break;
        }
        cout << cuvc << ' ' << "k=" << k << endl;
    }
}

void afisare_lista(dictionar* cap)///Program auxiliar: afisarea listei de cuvinte
{
    system("CLS");
    int metoda, afis;
    do{
    cout << "   Afisarea listei se va realiza:" << endl << "1.Fisier" << endl << "2.Consola" << endl << ':';
    cin >> metoda;
    }while(metoda != 1 && metoda != 2);
    do{
    cout << "   Se vor afisa:" << endl << "1.Toate cuvintele" << endl << "2.Doar cuvintele folosite" << endl << ':';
    cin >> afis;
    }while(afis != 1 && afis != 2);
    if(metoda == 1)
    {
        ofstream g("afisare.txt");
        while(cap != NULL)
        {
            if(afis == 2)
            {
                if(cap->numar != 0)
                    g << cap->cuvant << ' ';
            }else
                g << cap->cuvant << ' ';
            cap = cap->urmator;
        }
        g.close();
    }else if(metoda == 2)
    {
        cout << "Lista: " << endl;
        while(cap != NULL)
        {
            if(afis == 2)
            {
                if(cap->numar != 0)
                    cout << cap->cuvant << ' ';
            }else
                cout << cap->cuvant << ' ';
            cap = cap->urmator;
        }
    }

}

void stergere_lista(dictionar* cap)
{
    for(int i = 0; i < 2; i++)
    {
        dictionar *p = cap;
        cap = cap -> urmator;
        delete p;
    }
}
///Cam atat
