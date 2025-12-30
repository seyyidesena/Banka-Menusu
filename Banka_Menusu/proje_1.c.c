#include <stdio.h>  
struct Banka {  //banka hesabini temsil eden yapi
    int hesap_no;
    float bakiye;
}; //Banka hesabini tek tek degiskenlerle degil, tek bir yapi altinda toplamak icin struct kullandim.
int hesap_sayaci = 1000; 
/* programda kullandigim fonksiyonlar */    
void giris();
void ana_menu(struct Banka *b);//pointer kullanma nedenim: tanimladigim degisikliklerin ana programda da gecerli olmasi icin 
void bakiye_goster(struct Banka *b);
void para_yatir(struct Banka *b);
void para_cek(struct Banka *b);
void burs_krediler(struct Banka *b);
void iban_transfer(struct Banka *b);
/* IBAN kontrol fonksiyonlari */
int iban_uzunluk_kontrol(char iban[]);
int iban_tr_kontrol(char iban[]);
int iban_rakam_kontrol(char iban[]);

int main()
{
    struct Banka b;

    hesap_sayaci++; 
    b.hesap_no = hesap_sayaci; //Hesap numarasi ataniyor
    b.bakiye = 0;
    giris();
    ana_menu(&b); //ustte void ile yazdiklarima adres gonderilir (pointer)
    return 0; 
}
/* ---- GIRIS ---- */ 
void giris()
{
    char isim[30];
    char sifre[20];

    printf("\n---- BANKA GIRISI ----\n");
    printf("Isim: ");
    scanf("%s", isim);
    printf("Sifre: ");
    scanf("%s", sifre);
    printf("Giris basarili. Hosgeldiniz!\n");
}
/* ---- ANA MENU ---- */
void ana_menu(struct Banka *b)
{
    int secim;

    do { 
        printf("\n---- ANA MENU ----\n");
        printf("Hesap No : %d\n", b->hesap_no); //Struct pointer kullandigim icin nokta degil ok operatoru (->)kullandim.
        printf("Bakiye   : %.2f TL\n", b->bakiye);
        printf("\n1 - Bakiye Goruntule");
        printf("\n2 - Para Yatir");
        printf("\n3 - Para Cek");
        printf("\n4 - Burslar / Krediler");
        printf("\n5 - IBAN'a Para Transferi");
        printf("\n6 - Cikis");
        printf("\nSeciminiz: ");
        scanf("%d", &secim);

        switch(secim)
        {
            case 1: bakiye_goster(b);
            break;
            case 2: para_yatir(b);
            break;
            case 3: para_cek(b);
            break;
            case 4: burs_krediler(b);
            break;
            case 5: iban_transfer(b);
            break;
            case 6: printf("Cikis yapiliyor...\n");
            break;
            default: printf("Hatali secim!\n");
        }

    } while(secim != 6);
}
/* ---- BAKIYE ---- */
void bakiye_goster(struct Banka *b)
{
    printf("Mevcut Bakiye: %.2f TL\n", b->bakiye);
}
/* ---- PARA YATIR ---- */
void para_yatir(struct Banka *b)
{
    float tutar;
    printf("Yatirilacak tutar: ");
    scanf("%f", &tutar);

    if(tutar > 0)
    {
        b->bakiye += tutar; //Yanlis para girislerini onlemek icin kosul koydum
        printf("Para yatirildi.\n");
    }
    else
        printf("Gecersiz tutar!\n");
}
/* ---- PARA CEK ---- */
void para_cek(struct Banka *b)
{
    float tutar;
    printf("Cekilecek tutar: ");
    scanf("%f", &tutar);

    if(tutar > 0 && tutar <= b->bakiye)
    {
        b->bakiye -= tutar;
        printf("Para cekildi.\n");
    }
    else
        printf("Yetersiz bakiye veya gecersiz tutar!\n");
}
/* ---- IBAN TRANSFER ---- */
void iban_transfer(struct Banka *b)  {   
    char iban[30];
    float tutar;
    char onay;

    printf("IBAN giriniz (TR ile baslamali): "); 
    scanf("%s", iban);

    if(!iban_uzunluk_kontrol(iban))
    {
        printf("IBAN 26 karakter olmalidir!\n"); 
        return; //hata varsa durdur.
    }

    if(!iban_tr_kontrol(iban))
    {
        printf("IBAN TR ile baslamalidir!\n");
        return;
    }

    if(!iban_rakam_kontrol(iban))
    {
        printf("TR'den sonra sadece rakam olmali!\n"); 
        return;
    }

    printf("Transfer tutari: ");
    scanf("%f", &tutar);

    if(tutar <= 0 || tutar > b->bakiye) //negatif tutar girilmesin diye kosul
    {
        printf("Yetersiz bakiye veya gecersiz tutar!\n");
        return;
    }
    printf("Onayliyor musunuz? (E/H): "); //girilen tutari onay
    scanf(" %c", &onay);

    if(onay == 'E' || onay == 'e') 
    {
        b->bakiye -= tutar; //istenilen tutar bakiyeden eksilmeli
        printf("Transfer basarili.\n");
    }
    else
    {
        printf("Islem iptal edildi.\n");
    }
}
/* ---- BURS ---- */
void burs_krediler(struct Banka *b)
{
    char tc_son;
    int gun, burs_gunu = -1; // -1'in sebebi henuz atanmamis olmasi

    printf("TC Kimlik son rakami (0,2,4,6,8): ");
    scanf(" %c", &tc_son);

    if(tc_son!='0' && tc_son!='2' && tc_son!='4' &&
       tc_son!='6' && tc_son!='8')
    {
        printf("Gecersiz TC son rakami!\n");
        return;
    }
    printf("Ayin gununu giriniz: ");
    scanf("%d", &gun);

    if(tc_son=='0') burs_gunu=6; 
    if(tc_son=='2') burs_gunu=7;
    if(tc_son=='4') burs_gunu=8;
    if(tc_son=='6') burs_gunu=9;
    if(tc_son=='8') burs_gunu=10;

    if(gun == burs_gunu)
    {
        b->bakiye += 3000; 
        printf("BURS YATTI! +3000 TL\n");
    }
    else
    {
        printf("Burs yatmadi. Burs gununuz: %d\n", burs_gunu);
    }
}
/* ---- IBAN KONTROLLERI  ---- */
int iban_uzunluk_kontrol(char iban[])
{
    int i = 0;
    while(iban[i] != '\0') //String(dizi)bitene kadar devam et. ('\0'= string bitti) her karakteri tek tek saymak icin indeks artiriyorum
        i++; 
    return i == 26; //iban in 26 karakter olup olmadigini kontrol edip sonucu geri donduruyorum
}
int iban_tr_kontrol(char iban[])
{
    return iban[0] == 'T' && iban[1] == 'R'; //TR kontrolu
}
int iban_rakam_kontrol(char iban[])
{
    int i;
    for(i = 2; i < 26; i++) // dongunun 2 den baslama sebebi T VE R (2 KARAKTER)+24 SAYI
    {
        if(iban[i] < '0' || iban[i] > '9') //rakam kontrolu harf varsa iptal olur
            return 0;
    }
    return 1;
}

