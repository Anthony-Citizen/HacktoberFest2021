#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>

typedef struct orang  //Function untuk menampung data-data
{
    int kode;
    char nama[30],tanggal[30];
    char tipe;
    int hari;
    int kamar;
    int status;
    int biaya;

    struct orang *next,*prev;
}orang;

//Function prototype
//-------------------------------------------------------------------------------------------------------------------------------------------

void gotoxy(int x, int y);        //function digunakan mengatur layout menu login
void login_cek();                 //Function untuk menjalankan proses login
char menu(char *pil);             //Function untuk menampilkan menu utama
int cek_file();                   //mengecek apakah file ada isi/tidak
int isempty(orang **head);        //Function untuk cek apakah data kosong/tidak.
void swap(orang *a,orang *b);     //Swapping function
void tampil_kamar(orang *curr,int i,char pil);     //Function untuk menampilkan detail kamar yang dipesan
void read_data(orang **head,orang **tail,char pil);     //Read data di txt
void write_data(orang **node,int *total_input);         //Write data ke txt
void pesan_kamar(orang **head,orang **tail,int *total,int *total_input); //Menu ke 1, untuk memesan kamar
void list_kamar(orang **head,orang **tail,int total);                    //Menu ke 2, untuk menampilkan kamar semua/tertentu
void hapus_semua(orang **head, orang **delprev, orang **curr, orang **tail, int *total_input, int *total);        //Menu ke 3, untuk menghapus seluruh kamar yang dipesan
void check_in(orang **head,int total);                                   //Menu ke 4, untuk check_in kamar hotel
void check_out(orang **head,orang **tail,int *total,int *total_input);   //Menu ke 5, untuk Check_out kamar hotel
void cari_data(orang *head);                                             //Menu ke 6, untuk mencari data pengunjung

//-------TREE--------
orang* insert(orang *node,orang *curr);
orang* newnode(orang *curr);

//--------HASH (separate chaining)---------------
void insertToChain(int kode,char nama[30],char tanggal[30],char tipe,int hari,int kamar,int status,int biaya,orang **head);

//memasukkan ke linked list utama
void to_linkedlist(orang **head,orang **tail,int kode,char nama[30],char tanggal[30],char tipe,int hari,int kamar,int status,int biaya,int *total,int *total_input);

void dequeue_pop(orang **head);

//Stack
void push(orang **stack,int kode,char nama[30],char tanggal[30],char tipe,int hari,int kamar,int status,int biaya);  //memasukkan data ke stack
void stack_to_linkedlist(orang **head,orang **tail,orang **stack,int *total,int *total_input);      //memasukkan data dari stack ke linked list utama
//Queue
void enqueue(orang **head,orang **tail,int kode,char nama[30],char tanggal[30],char tipe,int hari,int kamar,int status,int biaya);      //memasukkan data ke queue
void queue_to_linkedlist(orang **head,orang **tail,orang **qhead,orang **qtail,int *total,int *total_input);            //memasukkan data dari queue ke linked list utama


void copy_data(orang *node,int kode,char nama[30],char tanggal[30],char tipe,int hari,int kamar,int status,int biaya);  //Copy data ke sebuah node
void masuk_data(orang **head,orang **tail,orang **qhead,orang **qtail,orang **stack,int *total,int *total_input);  //menu function untuk import data dari txt

//-----------------------------------------------------------------------------------------------------------------------------------------
//End Function Prototype

int main(int argc, char const *argv[])
{
    orang *head, *tail, *curr, *node, *delprev, *qhead, *qtail, *stack;     //Deklarasi pointer untuk proses Linked List.

    //deklarasi variabel
    char pil;
    int total=0,total_input=0;
    FILE *data;     //Variabel untuk Read/Write/Append di Notepad

    head = curr = tail = delprev = qhead = qtail = stack = NULL;    //Mengeset setiap pointer menjadi NULL
    login_cek();    //User memasukkan username dan password untuk melanjutkan

    masuk_data(&head,&tail,&qhead,&qtail,&stack,&total,&total_input);   //import data menggunakan Stack/Queue

    while(1)
    {
        fflush(stdin);
        menu(&pil);         //menu utama pada program

        switch(pil)
        {
            case '1' :  pesan_kamar(&head,&tail,&total,&total_input);   //menjalankan menu pemesanan kamar
                        break;

            case '2' :  list_kamar(&head,&tail,total);      //menjalankan menu lihat kamar
                        break;

            case '3' :  hapus_semua(&head,&delprev,&curr,&tail,&total_input,&total);        //fungsi untuk menghapus seluruh list kamar
                        break;

            case '4' :  check_in(&head,total);                  //menjalankan menu check in
                        break;

            case '5' :  check_out(&head,&tail,&total,&total_input);         //menjalankan menu check out
                        break;

            case '6' :  cari_data(head);                                    //menjalankan menu untuk mencari data
                        break;

            case '7' :  return 0;

            default  :  //error handling
                        printf("Input error\n");
                        system("pause");
                        system("cls");
                        fflush(stdin);
        }
    }

    return 0;
}

void gotoxy(int x, int y)   //function digunakan mengatur layout menu login
{
    COORD k = {x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), k);
}

void login_cek() //Function untuk menjalankan proses login
{
    char username[30], password[30];
    do      //Menu Login Dengan Default user=admin,pass=admin
    {
        system("cls");
        gotoxy(35,7);printf("---------------------------------------------");
        gotoxy(35,8);printf("| Login                                     |\n");
        gotoxy(35,9);printf("---------------------------------------------\n\n");
        gotoxy(35,10);printf("|                                           |");
        gotoxy(35,11);printf("|                                           |");
        gotoxy(35,12);printf("|           Username :                      |");
        gotoxy(35,13);printf("|           Password :                      |");
        gotoxy(35,14);printf("|                                           |");
        gotoxy(35,15);printf("|                                           |");
        gotoxy(35,16);printf("|                                           |");
        gotoxy(35,17);printf("|                                           |");
        gotoxy(35,18);printf("---------------------------------------------");
        gotoxy(35,19);printf("Hint : admin(2x)");
        gotoxy(58,12);gets(username); fflush(stdin);
        gotoxy(58,13);gets(password); fflush(stdin);


        if(strcmp("admin",username)==0&&strcmp("admin",password)==0)        //cek apakah input benar/tidak
            break;
        gotoxy(42,15);printf("Username/Password salah\n");
        gotoxy(42,17);system("pause");
    }while(1);
}

char menu(char *pil) //Function untuk menampilkan menu utama
{
    char *a=pil;
    system("cls");
    printf("-------------------------------------------------------------------------------\n");
    printf("                          Booking Kamar Hotel XYZ\n");
    printf("-------------------------------------------------------------------------------\n");
    printf("1. Booking\n");
    printf("2. Daftar Tamu\n");
    printf("3. Check in Kamar Hotel\n");
    printf("4. Check out Kamar Hotel\n");
    printf("5. Exit\n");
    printf("Input : ");scanf("%c",&*a);fflush(stdin);
}

void copy_data(orang *node,int kode,char nama[30],char tanggal[30],char tipe,int hari,int kamar,int status,int biaya)
{
    //copy data ke data node
    node->kode=kode;
    strcpy(node->nama,nama);
    strcpy(node->tanggal,tanggal);
    node->tipe=tipe;
    node->hari=hari;
    node->kamar=kamar;
    node->status=0;
    if(tipe=='S')
        biaya=hari*50000;
    else if(tipe=='D')
        biaya=hari*100000;
    node->biaya=biaya;
}

void to_linkedlist(orang **head,orang **tail,int kode,char nama[30],char tanggal[30],char tipe,int hari,int kamar,int status,int biaya,int *total,int *total_input)
{
    orang *node;
    node=(struct orang*)malloc(sizeof(struct orang));       //membuat node baru
    node->next=node;
    node->prev=node;

    copy_data(node,kode,nama,tanggal,tipe,hari,kamar,status,biaya);         //copy data ke sebuah node

    if((*head)==NULL)      //cek apakah node pertama/tidak
    {
        (*head)=node;
        (*tail)=node;
    }
    else                            //linked list berupa DOUBLE CIRCULAR linked list
    {
        (*tail)->next=node;
        node->next=(*head);
        node->prev=(*tail);
        (*head)->prev=node;
        (*tail)=node;
    }
    *total=*total+1;
    *total_input=*total_input+1;
}

void dequeue_pop(orang **head)      //function untuk dequeue dan pop
{
    orang *hapus=*head;
    *head=(*head)->next;
    free(hapus);
}

int isempty(orang **head)       //Function untuk cek apakah data kosong/tidak. 1=Kosong, 2=Tidak kosong
{
    if((*head)==NULL)
        return 1;
    else
        return 0;
}


//----------------------------QUEUE-----------------------------------

void enqueue(orang **head,orang **tail,int kode,char nama[30],char tanggal[30],char tipe,int hari,int kamar,int status,int biaya)
{
    orang *node;
    node=(struct orang*)malloc(sizeof(struct orang));       //membuat node baru
    node->next=NULL;

    copy_data(node,kode,nama,tanggal,tipe,hari,kamar,status,biaya);         //copy data ke sebuah node

    if((*head)==NULL)
        (*head) = node;
    else
        (*tail)->next=node;

    (*tail)=node;
}

void queue_to_linkedlist(orang **head,orang **tail,orang **qhead,orang **qtail,int *total,int *total_input)     //function untuk memasukkan data dari queue ke linked list utama
{
    while(!isempty(qhead))      //memasukkan data dari queue ke linked list utama sampai selesai
    {
        to_linkedlist(head,tail,(*qhead)->kode,(*qhead)->nama,(*qhead)->tanggal,(*qhead)->tipe,(*qhead)->hari,(*qhead)->kamar,(*qhead)->status,(*qhead)->biaya,total,total_input);
        dequeue_pop(qhead);
    }
}

//--------------------------------STACK-----------------------------------------

void push(orang **stack,int kode,char nama[30],char tanggal[30],char tipe,int hari,int kamar,int status,int biaya)
{
    orang *node;
    node=(struct orang*)malloc(sizeof(struct orang));       //membuat node baru
    node->next=NULL;

    copy_data(node,kode,nama,tanggal,tipe,hari,kamar,status,biaya);         //copy data ke sebuah node

    if(!isempty(stack))        //if(stack!=NULL)
        node->next = (*stack);

    (*stack)=node;
}

void stack_to_linkedlist(orang **head,orang **tail,orang **stack,int *total,int *total_input)       //function untuk memasukkan data dari stack ke linked list utama
{
    while(!isempty(stack))      //memasukkan data dari stack ke linked list utama sampai selesai
    {
        to_linkedlist(head,tail,(*stack)->kode,(*stack)->nama,(*stack)->tanggal,(*stack)->tipe,(*stack)->hari,(*stack)->kamar,(*stack)->status,(*stack)->biaya,total,total_input);
        dequeue_pop(stack);
    }
}
//---------------------------------------------------------------------------------------

void read_data(orang **head,orang **tail,char pil)      //function untuk membaca data txt dan memasukkannya ke queue/stack
{
    char nama[30],tanggal[30],tipe;
    int kode,hari,kamar,status,biaya;

    FILE *data=fopen("data.txt","r");
    while(!feof(data))
    {
        fscanf(data,"%d#%[^#]#%[^#]#%d#%c#%d#%d\n",&kode,&nama,&tanggal,&hari,&tipe,&kamar,&biaya);
        if(pil=='1')
        {
            enqueue(head,tail,kode,nama,tanggal,tipe,hari,kamar,status,biaya);      //memasukkan data yang ada di txt ke dalam queue
        }
        else if(pil=='2')
        {
            push(head,kode,nama,tanggal,tipe,hari,kamar,status,biaya);      //memasukkan data yang ada di txt ke dalam stack
        }
    }
    fclose(data);
}

void write_data(orang **node,int *total_input)          //function untuk menuliskan data ke txt
{
    int cek;

    cek=cek_file();         //cek apakah data memiliki isi/tidak
    //Write data yang diinput ke file data.txt
    FILE *data=fopen("data.txt","a");
    if((*total_input)==1 && cek==1)
        fprintf(data,"%d#%s#%s#%d#%c#%d#%d",(*node)->kode,(*node)->nama,(*node)->tanggal,(*node)->hari,(*node)->tipe,(*node)->kamar,(*node)->biaya);        //jika tidak ada data di txt dan data di txt kosong
    else
        fprintf(data,"\n%d#%s#%s#%d#%c#%d#%d",(*node)->kode,(*node)->nama,(*node)->tanggal,(*node)->hari,(*node)->tipe,(*node)->kamar,(*node)->biaya);      //jika file tidak kosong
    fclose(data);         //menutup file data.txt
}

void pesan_kamar(orang **head,orang **tail,int *total,int *total_input)     //fuction menu pemesanan kamar
{
    orang *curr,*node;
    char nama[30],tanggal[30],tipe;
    int hari,kamar,status,biaya,kode;
    bool check;

    srand(time(NULL));          //random number

    int cek=0;
    system("cls");
    printf("-------------------------------------------------------------------------------\n");
    printf("                           Input Data Pemesanan Kamar\n");
    printf("-------------------------------------------------------------------------------\n");
    printf("Nama\t\t\t\t\t:  \n");
    printf("Tanggal Check-In (DD/MM/YYYY)\t\t:\n");
    printf("Lama Menginap\t\t\t\t:    Malam\n");
    printf("Tipe Kamar (S=Single | D=Double)\t: \n");
    printf("Nomor Kamar\t\t\t\t: \n");
    gotoxy(0,12);
    printf("-------------------------------\n");
    printf("|Room Rate                    |\n");
    printf("-------------------------------\n");
    printf("|Single : Rp 50.000,00/Malam  |\n");
    printf("|Double : Rp 100.000,00/Malam |\n");
    printf("-------------------------------\n\n");
    printf("--------------------------------------\n");
    printf("|Nomor Kamar                         |\n");
    printf("--------------------------------------\n");
    printf("|Single : 101-350                    |\n");
    printf("|Double : 401-550                    |\n");
    printf("*Setiap lantai terdiri dari 50 kamar |\n");
    printf("--------------------------------------\n");
    fflush(stdin);
    gotoxy(42,3);gets(nama);    //Input Nama
    fflush(stdin);
    gotoxy(42,4);gets(tanggal);
    fflush(stdin);
    gotoxy(42,5);scanf("%d",&hari); //Input jumlah hari
    fflush(stdin);
    gotoxy(42,6);scanf("%c",&tipe); //Input Tipe Kamar
    fflush(stdin);
    tipe=toupper(tipe);
    if(tipe!='S'&&tipe!='D')        //error handling
    {
        printf("\nPilih tipe kamar dengan benar ! !\n");
        system("pause");
        return;
    }
    if(tipe=='S')               //cek apakah tipenya 'S'ingle
    {
        gotoxy(42,7);scanf("%d",&kamar);
        if(kamar<101 || (kamar > 150 && kamar < 201) || (kamar > 250 && kamar < 301) || kamar>350)             //error handling
        {
            printf("\nNomor kamar tidak tersedia !\n");
            system("pause");
            return;
        }
    }
    else if(tipe=='D')         //cek apakah tipenya 'D'ouble
    {
        gotoxy(42,7);scanf("%d",&kamar);
        if(kamar<401 || (kamar > 450 && kamar < 501) || kamar > 550 )           //error handling
        {
            printf("\nNomor kamar tidak tersedia !\n");
            system("pause");
            return;
        }
    }
    fflush(stdin);
    status=0;

    do                  //pengulangan random number generator untuk kode transaksi
    {
        check=true;
        do
        {
            kode=rand();            //meng generate kode transaksi agar > 10000
        }while(kode<10000);

        if((*head)==NULL)
            break;

        curr=(*head);
        do                          //pengulangan untuk mengecek kode transaksi, apakah ada yang sama/tidak(mencegah terjadinya 2 kode transaksi yang sama)
        {
            if(kode==curr->kode)
                check=false;

            curr=curr->next;
        }while(curr!=(*head));
    }while(check==false);


    if((*head) != NULL)       //cek apakah head bernilai NULL/tidak
    {
        curr=(*head);       //dimulai dari data pertama
        do               //cek apakah kamar sudah dibooking/belum
        {
            if(kamar==curr->kamar)      //cek data input dengan data di node
            {
                printf("\nKamar Telah Dibooking !\n");
                system("pause");
                cek=1;
                break;
            }
            curr=curr->next;
        }while(curr!=(*head));
    }

    if(cek==0)      //jika kamar belum di book maka lanjut ke sini, Lalu data akan dimasukan kedalam linked list utama
    {
        to_linkedlist(head,tail,kode,nama,tanggal,tipe,hari,kamar,status,biaya,total,total_input);
        write_data(tail,total_input);                                //menuliskan data yang diinput ke dalam txt
        printf("\nData pemesanan kamar telah berhasil diinput dengan status Booked !\n");
        system("pause");
    }
}

void tampil_kamar(orang *curr,int i,char pil)           //function untuk menampilkan kamar
{
    printf("-------------------------------------------\n");
    printf("Data ke-%d\n",i);
    printf("Kode Transaksi  : #%d\n",curr->kode);
    printf("Nama            : %s\n",curr->nama);
    printf("Tanggal         : %s\n",curr->tanggal);
    printf("Lama Menginap   : %d\n",curr->hari);

    if( pil=='1'  || (pil=='3' && curr->tipe=='S') )
        printf("Tipe Kamar      : Single\n");

    if( pil=='2'  || (pil=='3' && curr->tipe=='D') )
        printf("Tipe Kamar      : Double\n");

    printf("Nomor Kamar     : %d\n",curr->kamar);
    if(curr->status==0)
        printf("Status          : Booked\n");
    else if(curr->status==1)
        printf("Status          : Occupied\n");
    printf("Total Biaya     : Rp %d\n",curr->biaya);
}

void swap(orang *a,orang *b)            //function untuk swap data
{
    a->kode=b->kode;
    strcpy(a->nama, b->nama);
    strcpy(a->tanggal, b->tanggal);
    a->tipe=b->tipe;
    a->hari=b->hari;
    a->kamar=b->kamar;
    a->status=b->status;
    a->biaya=b->biaya;
}

void list_kamar(orang **head,orang **tail,int total)        //function untuk menu list kamar
{
    orang *temp,*delnext;

    system("cls");

    orang *curr=(*head);      //memulai dari head
    char pil;
    int i,j;
    if((*head)==NULL)      //cek apakah ada data/tidak
    {
        printf("-------------------------------------------------------------------------------\n");
        printf("                        List Kamar Hotel Yang Dipesan\n");
        printf("-------------------------------------------------------------------------------\n\n");
        printf("Tidak ada kamar yang dipesan !\n");
        system("pause");
    }
    else
    {
        while(1)
        {
            system("cls");
            printf("-------------------------------------------------------------------------------\n");
            printf("                        List Kamar Hotel Yang Dipesan\n");
            printf("-------------------------------------------------------------------------------\n");
            printf("1. Tampilkan kamar Single\n");
            printf("2. Tampilkan kamar Double\n");
            printf("3. Tampilkan semua\n");
            printf("4. Sort nomor kamar(ascending)\n");
            printf("5. Sort nomor kamar(descending)\n");
            printf("6. Exit\n\n");
            printf("Pil : ");scanf("%c",&pil);fflush(stdin);

            curr=(*head);          //memulai dari data pertama

            //memilih input
            if(pil=='1')
            {
                i=1;
                do      //menampilkan data dari head sampai tail untuk kamar single
                {
                    if(curr->tipe=='S')
                    {
                        tampil_kamar(curr,i,pil);

                        if(curr->next==curr)
                            break;
                        i=i+1;
                    }
                    curr=curr->next;        //menuju ke data selanjutnya
                }while(curr!=(*head));
                printf("-------------------------------------------\n");
                system("pause");
            }
            else if(pil=='2')
            {
                i=1;
                do      //menampilkan data dari head sampai tail untuk kamar double
                {
                    if(curr->tipe=='D')
                    {
                        tampil_kamar(curr,i,pil);

                        if(curr->next==curr)
                            break;
                        i=i+1;
                    }
                    curr=curr->next;        //menuju data selanjutnya
                }while(curr!=(*head));
                printf("-------------------------------------------\n");
                system("pause");
            }
            else if(pil=='3')
            {
                i=1;
                do      //menampilkan data dari head sampai tail untuk semua tipe kamar
                {
                    tampil_kamar(curr,i,pil);

                    if(curr->next==curr)
                        break;
                    curr=curr->next;            //menuju data selanjutnya
                    i=i+1;
                }while(curr!=(*head));
                printf("-------------------------------------------\n");
                system("pause");
            }
            else if(pil=='4')       //sorting data secara ascending
            {
                for(i=1;i<total;i++)
                {
                    curr=(*head);              //memulai dari data pertama
                    delnext=(*head)->next;         //menginisialisasikan varialbe delnext ke alamat sesudah head
                    for(j=1;j<total;j++)
                    {
                        if(curr->kamar > delnext->kamar)        //mengecek apakah nomor kamar lebih besar dari data setelahnya/tidak
                        {
                            temp=(struct orang*)malloc(sizeof(struct orang));       //membuat node baru bernama temp
                            //swap data yang tertukar
                            swap(temp,curr);
                            swap(curr,delnext);
                            swap(delnext,temp);
                        }
                        curr=curr->next;
                        delnext=delnext->next;
                    }
                }
                printf("Data berhasil disort secara ascending\n");
                system("pause");
            }
            else if(pil=='5')               //sorting data secara descending
            {
                for(i=1;i<total;i++)
                {
                    curr=(*head);              //memulai dari data pertama
                    delnext=(*head)->next;        //menginisialisasikan varialbe delnext ke alamat sesudah head
                    for(j=1;j<total;j++)
                    {
                        if(curr->kamar < delnext->kamar)        //mengecek apakah nomor kamar lebih kecil dari data setelahnya/tidak
                        {
                            temp=(struct orang*)malloc(sizeof(struct orang));       ////membuat node baru bernama temp
                            //swap data yang tertukar
                            swap(temp,curr);
                            swap(curr,delnext);
                            swap(delnext,temp);
                        }
                        curr=curr->next;
                        delnext=delnext->next;
                    }
                }
                printf("Data berhasil disort secara descending\n");
                system("pause");
            }
            else if(pil=='6')
                break;
        }
    }
}

void hapus_semua(orang **head, orang **delprev, orang **curr, orang **tail, int *total_input, int *total)       //functuon untuk menghapus seluruh list kamar
{
    int pil;
    system("cls");
    printf("-------------------------------------------------------------------------------\n");
    printf("                        Hapus Seluruh List Pemesanan Kamar\n");
    printf("-------------------------------------------------------------------------------\n\n");
    printf("Apakah anda yakin untuk menghapus seluruh list pemesanan kamar?\n");
    printf("1. Ya\n");
    printf("2. Kembali\n\n");
    printf("Pil : ");scanf("%d",&pil);

    if(pil==1)
    {
        if((*head)!=NULL)
        {
            (*delprev)=(*head);
            do                  //menghapus node dari head sampai tail
            {
                (*curr)=(*head);
                (*head)=(*head)->next;
                free(*curr);
            }while((*head)!=(*delprev));
        }
        (*head) = (*tail) = (*curr) = (*delprev) = NULL;          //mereset variabeel menjadi NULL
        *total_input=0;
        *total=0;
        printf("\nSeluruh list pemesanan kamar berhasil dihapus !\n");
        system("pause");
    }
}

void check_in(orang **head,int total)       //function untuk menu check-in kamar hotel
{
    orang *curr;     //memulai dari data pertama
    char pil;

    if((*head)==NULL)      //cek apakah ada data/tidak
    {
        system("cls");
        printf("-------------------------------------------------------------------------------\n");
        printf("                              Check in Kamar Hotel\n");
        printf("-------------------------------------------------------------------------------\n\n");
        printf("Tidak ada kamar yang dipesan !\n");
        system("pause");
    }
    else
    {
        curr=(*head);      //memulai dari head
        int i=1;
        while(1)
        {
            system("cls");
            printf("-------------------------------------------------------------------------------\n");
            printf("                              Check in Kamar Hotel\n");
            printf("-------------------------------------------------------------------------------\n");
            printf("Nomor           : %d / %d\n",i,total);
            printf("Kode Transaksi  : #%d\n",curr->kode);
            printf("Nama            : %s\n",curr->nama);
            printf("Tanggal         : %s\n",curr->tanggal);
            printf("Lama Menginap   : %d\n",curr->hari);
            if(curr->tipe=='S')
                printf("Tipe Kamar      : Single\n");
            else if(curr->tipe=='D')
                printf("Tipe Kamar      : Double\n");
            printf("Nomor Kamar     : %d\n",curr->kamar);
            if(curr->status==0)
                printf("Status          : Booked\n");
            else if(curr->status==1)
                printf("Status          : Occupied\n");
            printf("Total Biaya     : Rp %d\n",curr->biaya);
            printf("-------------------------------\n");
            printf("Menu :\n");
            printf("1. Next\n");
            printf("2. Prev\n");
            printf("3. Check in\n");
            printf("4. Exit\n");
            printf("Pilihan : ");scanf("%c",&pil);
            fflush(stdin);

            if(pil=='1')        //lanjut ke node/data selanjutnya
            {
                curr=curr->next;
                i++;
                if(i>total)
                    i=1;
            }
            else if(pil=='2')       //mundur ke node/data sebelumnya
            {
                curr=curr->prev;
                i--;
                if(i<1)
                    i=total;
            }
            else if(pil=='3')       //mengubah status dari Booked ke Occupied
            {
                curr->status=1;
                printf("Check in berhasil\n");
                system("pause");
            }
            else if(pil=='4')
                break;
            else                    //error handling
            {
                fflush(stdin);
                printf("Input Error\n");
                system("pause");
            }
        }
    }
}

void check_out(orang **head,orang **tail,int *total,int *total_input)       //function untuk menu check out kamar hotel
{
    orang *curr,*delnext,*delprev;
    int i;
    char pil;

    curr=delnext=delprev=NULL;

    while(1)
    {
        if((*head)==NULL)      //cek apakah data kosong/tidak
        {
            system("cls");
            printf("-------------------------------------------------------------------------------\n");
            printf("                            Check Out Kamar Hotel\n");
            printf("-------------------------------------------------------------------------------\n\n");
            printf("Tidak ada kamar yang dipesan !\n");
            system("pause");
            break;
        }
        else //Bila kamar tidak kosong, kamar bisa di Check-out
        {
            curr=(*head);      //memulai dari head
            i=1;
            while(1)
            {
                system("cls");
                printf("-------------------------------------------------------------------------------\n");
                printf("                            Check Out Kamar Hotel\n");
                printf("-------------------------------------------------------------------------------\n");
                printf("Nomor           : %d / %d\n",i,*total);
                printf("Kode Transaksi  : #%d\n",curr->kode);
                printf("Nama            : %s\n",curr->nama);
                printf("Tanggal         : %s\n",curr->tanggal);
                printf("Lama Menginap   : %d\n",curr->hari);
                if(curr->tipe=='S')
                    printf("Tipe Kamar      : Single\n");
                else if(curr->tipe=='D')
                    printf("Tipe Kamar      : Double\n");

                printf("Nomor Kamar     : %d\n",curr->kamar);

                if(curr->status==0)
                    printf("Status          : Booked\n");
                else if(curr->status==1)
                    printf("Status          : Occupied\n");
                printf("Total Biaya     : Rp %d\n",curr->biaya);
                printf("--------------------------------\n");
                printf("Menu :\n");
                printf("1. Next\n");
                printf("2. Prev\n");
                printf("3. Check out\n");
                printf("4. Exit\n");
                printf("Pilihan : ");scanf("%c",&pil);
                fflush(stdin);

                if(pil=='1')           //maju ke data/node selanjutnya
                {
                    curr=curr->next;
                    i++;
                    if(i>*total)
                        i=1;
                }
                else if(pil=='2')       //mundur ke data/node sebelumnya
                {
                    curr=curr->prev;
                    i--;
                    if(i<1)
                        i=*total;
                }
                else if(pil=='3')           //menghapus kamar yang dipilih
                {
                    delprev=(*head);
                    if(curr->next==curr)        //cek apakah hanya ada 1 data/tidak
                    {
                        free(curr);
                        (*head) = (*tail) = curr = NULL;
                        printf("Check Out berhasil\n");
                        system("pause");
                        *total=*total-1;
                        *total_input=0;
                        i=0;
                        break;
                    }
                    else if(curr==(*head))         //cek apakah data pertama/tidak
                    {
                        (*head)=(*head)->next;
                        (*tail)->next=(*head);
                        (*head)->prev=curr->prev;
                        free(curr);
                    }
                    else if(curr==(*tail))        //cek apakah data terakhir/tidak
                    {
                        (*tail)=(*tail)->prev;
                        (*head)->prev=(*tail);
                        (*tail)->next=curr->next;
                        free(curr);
                    }
                    else                       //data ada diantara head dan tail
                    {
                        delnext=curr->next;
                        delprev=curr->prev;
                        delprev->next=delnext;
                        delnext->prev=delprev;
                        free(curr);
                    }
                    printf("Check Out berhasil\n");
                    system("pause");
                    *total=*total-1;
                    i=1;
                    curr=(*head);
                }
                else if(pil=='4')
                    break;
                else                //error handling
                {
                    fflush(stdin);
                    printf("Input Error\n");
                    system("pause");
                }
            }
            break;
        }
    }
}

void insertToChain(int kode,char nama[30],char tanggal[30],char tipe,int hari,int kamar,int status,int biaya,orang **head)      //memasukkan data ke hash function
{
    orang *ptr=*head;
    orang *node=(orang*)malloc(sizeof(orang));
    node->next=NULL;

    copy_data(node,kode,nama,tanggal,tipe,hari,kamar,status,biaya);         //copy data ke node di hashtable

    if(*head==NULL)
    {
        *head=node;
    }
    else
    {
        while(ptr->next!=NULL)
        {
            ptr=ptr->next;
        }
        ptr->next=node;
    }
}

orang* newnode(orang *curr)             //function untuk membuat node baru pada tree
{
    orang *temp = (orang*)malloc(sizeof(orang));
    temp->prev=NULL;
    temp->next=NULL;

    copy_data(temp,curr->kode,curr->nama,curr->tanggal,curr->tipe,curr->hari,curr->kamar,curr->status,curr->biaya);

    return temp;
}

orang* insert(orang* node,orang* curr)          //function untuk menentukan letak node pada tree
{
    if(node==NULL)
        return newnode(curr);

    if(curr->kode < node->kode)
        node->prev = insert(node->prev,curr);
    else if(curr->kode > node->kode)
        node->next = insert(node->next,curr);

    return node;
}

orang* search(orang *root, int kode)                //function untuk mencari data dengan BST
{
    if (root == NULL || root->kode == kode)     //jika kode sama maka return alamat (Root)
       return root;

    if (root->kode < kode)
       return search(root->next, kode);

    return search(root->prev, kode);
}

void cari_data(orang *head)             //function untuk menu mencari data
{
    orang *hasht[500], *curr, *root;
    int pil;
    char cari_nama[30];
    int cari_kamar;
    int cari_kode;
    int i,j;
    int cek;

    while(1)
    {
        system("cls");
        printf("-------------------------------------------------------------------------------\n");
        printf("                           Cari Data Pengunjung Hotel\n");
        printf("-------------------------------------------------------------------------------\n\n");


        if(head==NULL)
        {
            printf("Tidak ada kamar yang dipesan !\n");
            system("pause");
            break;
        }
        else
        {
            printf("1. Cari berdasarkan Nama\n");
            printf("2. Cari berdasarkan Nomor Kamar\n");
            printf("3. Cari berdasarkan Kode Transaksi\n");
            printf("4. Exit\n");
            printf("Pilih : ");scanf("%d",&pil);fflush(stdin);

            if(pil==1)              //cari dengan sequential search
            {
                i=1;
                printf("\nMasukkan nama yang ingin di cari : ");fflush(stdin);gets(cari_nama);fflush(stdin);
                curr = head;
                do          //pengulangan untuk mencari data dari data pertama sampai data terakhir
                {
                    if(strcmpi(curr->nama,cari_nama)==0)         //jika nama yang dicari, maka ditampilkan
                    {
                        tampil_kamar(curr,i,'3');
                        i++;
                    }

                    curr=curr->next;
                }while(curr!=head);

                if(i==1)
                {
                    printf("\nTidak ada kamar yang dipesan atas nama %s !\n",cari_nama);
                }
            }
            else if(pil==2)
            {
                cek=0;

                for(i=0;i<500;i++)          //pengulangan untuk inisialisasi hashtable
                {
                    hasht[i]=NULL;
                }

                curr=head;
                do                      //pengulangan untuk memasukkan data dari linked list utama ke hashtable
                {
                    insertToChain(curr->kode,curr->nama,curr->tanggal,curr->tipe,curr->hari,curr->kamar,curr->status,curr->biaya,&hasht[curr->kamar - 101]);
                    curr=curr->next;
                }while(curr!=head);


                printf("\nMasukkan nomor kamar yang ingin di cari : ");fflush(stdin);scanf("%d",&cari_kamar);fflush(stdin);

                if(cari_kamar<101 || cari_kamar>550)
                {
                    printf("\nMasukkan nomor kamar dengan benar !\n");
                }
                else
                {
                    cek=1;
                }


                if(cek==1)
                {
                    curr = hasht[cari_kamar-101];       //hashtable dimulai dari 0, sementara nomor kamar dari 101. Sehingga dikurangi 101
                    if(curr!=NULL)
                    {
                        i=1;
                        while(curr!=NULL)           //menampilkan data dari alamat hasht[cari_kamar-101]
                        {
                            tampil_kamar(curr,i,'3');
                            i++;
                            curr=curr->next;
                        }
                    }
                    else if(curr==NULL)
                    {
                        printf("\nTidak ada pengunjung yang memesan kamar bernomor %d !\n",cari_kamar);
                    }
                }

            }
            else if(pil==3)
            {
                root=NULL;
                curr=head;
                do          //pengulangan untuk memasukkan data dari linked list utama ke tree
                {
                    if(root==NULL)
                        root=insert(root,curr);         //jika root null maka data yang dimasukkan ke tree dijadikan root
                    else
                        insert(root,curr);              //memasukkan data ke tree
                    curr=curr->next;
                }while(curr!=head);

                printf("\nMasukkan nomor Kode Transaksi yang ingin di cari : ");fflush(stdin);scanf("%d",&cari_kode);fflush(stdin);

                curr=search(root,cari_kode);           //data dari alamat yang dicari dimasukkan ke curr
                if(curr==NULL)
                    printf("\nTidak ada transaksi dengan Kode %d !\n",cari_kode);
                else
                    tampil_kamar(curr,1,'3');           //menampilkan data yang dicari
            }
            else if(pil==4)
                break;
        }

        printf("\n");
        system("pause");
    }

}

int cek_file()              //function untuk mengecek apakah file txt memiliki isi/tidak
{
    FILE *data=fopen("data.txt","r");
    if (data != NULL)                   // cek apakah file ada isi/tidak
    {
        fseek (data, 0, SEEK_END);
        int size = ftell(data);

        if (size == 0)              //jika tidak ada isi
        {
            fclose(data);
            return 1;
        }
        else
        {
            fclose(data);
            return 0;
        }
    }
    fclose(data);
}

//function untuk menu import data dari txt
void masuk_data(orang **head,orang **tail,orang **qhead,orang **qtail,orang **stack,int *total,int *total_input)
{
    char pil;
    int cek=0;

    system("cls");
    printf("-------------------------------------------------------------------------------\n");
    printf("                            Import Data dari File\n");
    printf("-------------------------------------------------------------------------------\n");
    printf("1. Import Data Secara Queue (Data Teratas = Data Pertama)\n");
    printf("2. Import Data Secara Stack (Data Terbawah = Data Pertama)\n\n");
    printf("Pil : ");scanf("%c",&pil);fflush(stdin);
    if(pil=='1')            // Memasukkan data dari txt ke linked list utama secara queue
    {
        cek=cek_file();         //cek apakah file ada isi/tidak
        if(cek==1)          //jika file kosong
        {
            printf("\nNo Data In File ! !\n");
            system("pause");
            return;
        }
        else
        {
            read_data(qhead,qtail,pil);         //memasukkan data dari txt ke queue
            queue_to_linkedlist(head,tail,qhead,qtail,total,total_input);       //memasukkan data dari queue ke linked list utama
            printf("\nData Telah Berhasil Di Import Secara Queue\n");
            system("pause");
        }

    }
    else if(pil=='2')       // Memasukkan data dari txt ke linked list utama secara stack
    {
        cek=cek_file();
        if(cek==1)
        {
            printf("\nNo Data In File ! !\n");
            system("pause");
            return;
        }
        else
        {
            read_data(stack,stack,pil);             //memasukkan data dari txt ke stack
            stack_to_linkedlist(head,tail,stack,total,total_input);         //memasukkan data dari stack ke linked list utama
            printf("\nData Telah Berhasil Di Import Secara Stack\n");
            system("pause");
        }
    }
}

