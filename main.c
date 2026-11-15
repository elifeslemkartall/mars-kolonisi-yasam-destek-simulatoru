#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * MARS KOLONISI YASAM DESTEK SIMULATORU
 * Metin tabanli, tek karakterli komutlarla kontrol edilen C oyunu.
 * Komutlar: G, O, T, D, U, K, H, Q
 */

/* Sagllik, enerji, oksijen ve modul hasarini 0-100 araliginda tutar */
void sinirla(int *saglik, int *enerji, int *oksijen, int *modulHasari) {
    if (*saglik > 100) *saglik = 100;
    if (*saglik < 0) *saglik = 0;
    if (*enerji > 100) *enerji = 100;
    if (*enerji < 0) *enerji = 0;
    if (*oksijen > 100) *oksijen = 100;
    if (*oksijen < 0) *oksijen = 0;
    if (*modulHasari > 100) *modulHasari = 100;
    if (*modulHasari < 0) *modulHasari = 0;
}

void tamponuTemizle(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(int argc, char *argv[]) {

    /* ---------- DURUM DEGISKENLERI ---------- */
    int saglik = 100;
    int enerji = 100;
    int su = 10;
    int oksijen = 100;
    int modulHasari = 0;
    int yedekParca = 2;
    int turSayaci = 0;
    int gun = 1;
    int kilitMuhurlu = 0;   /* 0: Kilit muhurlu degil (acik), 1: Muhurlu */

    /* Zorluk seviyesine bagli parametreler */
    int firtinaTur = 3;         /* kum firtinasi tur sayisi */
    int firtinaHasar = 5;       /* firtinada tur basina temel hasar */
    int kesifEsikYuksek = 50;   /* enerji ve oksijen iyiyken su bulma esigi */
    int kesifEsikDusuk = 25;    /* aksi halde su bulma esigi */

    int sonuc = 0;              /* 0: devam, 1: kaybetti, 2: kazandi, 3: cikis */
    int gecerliTur;             /* 1: komut tur harcar, 0: harcamaz */

    srand(time(NULL));

    printf("=====================================\n");
    printf("   MARS KOLONISI YASAM DESTEK SIM.   \n");
    printf("=====================================\n");
    printf("Kurtarma ekibi gelene kadar 15 gun hayatta kalin!\n\n");

    /* ---------- ZORLUK SECIMI ---------- */
    char zorluk;
    do {
        printf("Zorluk seviyesi seciniz. (K: Kolay , N: Normal , Z: Zor): ");
        scanf(" %c", &zorluk);
        tamponuTemizle();

        switch (zorluk) {
            case 'k': case 'K':
                su = 100;
                yedekParca = 3;
                modulHasari = 0;
                firtinaTur = 3;
                firtinaHasar = 5;
                kesifEsikYuksek = 60;
                kesifEsikDusuk = 35;
                printf("Kolay seviye secildi.\n");
                break;
            case 'n': case 'N':
                su = 50;
                yedekParca = 2;
                modulHasari = 10;
                firtinaTur = 5;
                firtinaHasar = 8;
                kesifEsikYuksek = 50;
                kesifEsikDusuk = 25;
                printf("Normal seviye secildi.\n");
                break;
            case 'z': case 'Z':
                su = 30;
                yedekParca = 1;
                modulHasari = 20;
                firtinaTur = 7;
                firtinaHasar = 12;
                kesifEsikYuksek = 40;
                kesifEsikDusuk = 15;
                printf("Zor seviye secildi.\n");
                break;
            default:
                printf("Gecersiz secim! Tekrar deneyin.\n");
                break;
        }
    } while (zorluk != 'k' && zorluk != 'K' &&
             zorluk != 'n' && zorluk != 'N' &&
             zorluk != 'z' && zorluk != 'Z');

    /* ---------- ANA OYUN DONGUSU ---------- */
    do {
        char komut;
        gecerliTur = 1;

        printf("\n[Gun %d | Tur %d] Komutlar: G O T D U K H Q\n", gun, turSayaci);
        printf("Komut girin: ");
        scanf(" %c", &komut);
        tamponuTemizle();

        switch (komut) {

            /* ===== G: KESIF GOREVI ===== */
            case 'g':
            case 'G':
            {
                if (enerji < 15) {
                    printf("Yetersiz enerji! Kesif icin en az 15 enerji gerekir. Komut iptal.\n");
                    gecerliTur = 0;
                    break;
                }
                enerji -= 15;
                oksijen -= 10;
                printf("Kesif gorevine cikildi (Enerji -15, Oksijen -10).\n");

                /* Enerji ve oksijen iyiyse su bulma ihtimali yuksek */
                int esik;
                if (enerji > 40 && oksijen > 30) {
                    esik = kesifEsikYuksek;
                } else {
                    esik = kesifEsikDusuk;
                }

                /* Yaralanma kontrolu */
                int zarYaralanma = rand() % 100;
                if (zarYaralanma < 10 || modulHasari > 70) {
                    printf("Zar: %d / Esik: 10 (veya hasar > 70) -> YARALANDINIZ! Saglik -10\n", zarYaralanma);
                    saglik -= 10;
                } else {
                    printf("Zar: %d / Esik: 10 -> Yaralanma yok.\n", zarYaralanma);
                }

                /* Kesif sonucu: su / yedek parca / eli bos */
                int zar = rand() % 100;
                if (zar < esik) {
                    int bulunan = 5 + rand() % 11;   /* 5-15 birim su */
                    su += bulunan;
                    printf("Zar: %d / Esik: %d -> BASARILI: %d birim su buldunuz!\n", zar, esik, bulunan);
                } else if (zar < esik + 20) {
                    yedekParca++;
                    printf("Zar: %d / Esik: %d -> Su bulamadiniz ama 1 yedek parca buldunuz!\n", zar, esik);
                } else {
                    printf("Zar: %d / Esik: %d -> BASARISIZ: Eli bos dondunuz.\n", zar, esik);
                }
                break;
            }

            /* ===== K: KUM FIRTINASI ===== */
            case 'k':
            case 'K':
            {
                int hasarliTur = 0, toplamSaglikKaybi = 0, toplamOksijenKaybi = 0;
                int i;
                int zararEsigi = 60;

                /* Oyuncunun durumuna gore firtina daha tehlikeli olur */
                if (modulHasari > 70) {
                    zararEsigi = 80;
                } else if (modulHasari > 40) {
                    zararEsigi = 70;
                }

                printf("\n*** KUM FIRTINASI BASLADI (%d tur) ***\n", firtinaTur);

                for (i = 1; i <= firtinaTur; i++) {
                    int zar = rand() % 100;
                    int carpan = 1;
                    int hasar;

                    if (i % 2 == 0) {
                        carpan = 2;
                        printf("Tur %d: Ruzgar siddetlendi! (hasar x2)\n", i);
                    } else {
                        printf("Tur %d:\n", i);
                    }
                    hasar = firtinaHasar * carpan;

                    if (zar < zararEsigi) {
                        int olay = zar % 3;
                        if (olay == 0) {
                            saglik -= hasar;
                            toplamSaglikKaybi += hasar;
                            hasarliTur++;
                            printf("  Zar: %d / Esik: %d -> YARALANMA! Saglik -%d\n", zar, zararEsigi, hasar);
                        } else if (olay == 1) {
                            if (kilitMuhurlu == 1) {
                                printf("  Zar: %d / Esik: %d -> Oksijen sizintisi, ama hava kilidi muhurlu: atlandi.\n", zar, zararEsigi);
                                continue;
                            }
                            oksijen -= hasar;
                            toplamOksijenKaybi += hasar;
                            hasarliTur++;
                            printf("  Zar: %d / Esik: %d -> OKSIJEN SIZINTISI! Oksijen -%d\n", zar, zararEsigi, hasar);
                        } else {
                            modulHasari += hasar;
                            hasarliTur++;
                            printf("  Zar: %d / Esik: %d -> MODUL HASARI! Hasar +%d\n", zar, zararEsigi, hasar);
                        }
                    } else {
                        printf("  Zar: %d / Esik: %d -> Firtinadan zararsiz korundunuz.\n", zar, zararEsigi);
                    }

                    /* Degerleri sinirla ve olum kontrolu */
                    sinirla(&saglik, &enerji, &oksijen, &modulHasari);
                    if (saglik <= 0 || oksijen <= 0) {
                        printf("  !!! Kritik seviye, firtina simulasyonu kesildi !!!\n");
                        break;
                    }
                }

                printf("--- FIRTINA OZETI ---\n");
                printf("Hasar alinan tur: %d\n", hasarliTur);
                printf("Toplam kaybedilen saglik: %d\n", toplamSaglikKaybi);
                printf("Toplam kaybedilen oksijen: %d\n", toplamOksijenKaybi);
                break;
            }

            /* ===== O: OKSIJEN URETIMI ===== */
            case 'o':
            case 'O':
            {
                int miktar, artik, kullanilan, uretilen;

                if (enerji < 10) {
                    printf("Yetersiz enerji! Oksijen uretimi icin en az 10 enerji gerekir. Komut iptal.\n");
                    gecerliTur = 0;
                    break;
                }
                if (su < 2) {
                    printf("Yetersiz su! En az 2 birim su gerekir. Komut iptal.\n");
                    gecerliTur = 0;
                    break;
                }

                printf("Mevcut su: %d. Kac birim su kullanilsin? ", su);
                if (scanf("%d", &miktar) != 1) {
                    tamponuTemizle();
                    printf("Gecersiz giris! Komut iptal.\n");
                    gecerliTur = 0;
                    break;
                }
                tamponuTemizle();
                if (miktar < 2 || miktar > su) {
                    printf("Gecersiz miktar (2 ile %d arasinda olmali). Komut iptal.\n", su);
                    gecerliTur = 0;
                    break;
                }

                artik = miktar % 2;               /* tek ise 1 birim artik */
                kullanilan = miktar - artik;
                uretilen = (kullanilan / 2) * 3;  /* 2 su -> 3 oksijen */

                if (oksijen + uretilen > 100) {
                    printf("Oksijen 100'u asamaz, fazlasi bosa gidiyor.\n");
                }

                su -= kullanilan;
                enerji -= 10;
                oksijen += uretilen;
                printf("%d su harcandi, %d oksijen uretildi (Enerji -10).\n", kullanilan, uretilen);
                if (artik == 1) {
                    printf("Girilen su tek sayi: 1 birim artik olarak depoda kaldi.\n");
                }
                break;
            }

            /* ===== T: TAMIR ===== */
            case 't':
            case 'T':
            {
                int gerekenParca, onarilan;

                if (yedekParca <= 0) {
                    printf("Yedek parca yok, tamir yapilamaz! Komut iptal.\n");
                    gecerliTur = 0;
                    break;
                }
                if (enerji < 10) {
                    printf("Yetersiz enerji! Tamir icin en az 10 enerji gerekir. Komut iptal.\n");
                    gecerliTur = 0;
                    break;
                }
                if (modulHasari == 0) {
                    printf("Modulde hasar yok, tamire gerek yok. Komut iptal.\n");
                    gecerliTur = 0;
                    break;
                }

                if (modulHasari <= 30) {
                    printf("Hasar seviyesi: HAFIF\n");
                    gerekenParca = 1;
                    onarilan = 20;
                } else if (modulHasari <= 70) {
                    printf("Hasar seviyesi: ORTA\n");
                    gerekenParca = 2;
                    onarilan = 35;
                } else {
                    printf("Hasar seviyesi: AGIR\n");
                    gerekenParca = 3;
                    onarilan = 60;
                }

                if (yedekParca < gerekenParca) {
                    printf("Bu hasar icin %d yedek parca gerekir, sizde %d var. Komut iptal.\n", gerekenParca, yedekParca);
                    gecerliTur = 0;
                    break;
                }

                yedekParca -= gerekenParca;
                enerji -= 10;
                modulHasari -= onarilan;
                printf("%d parca harcandi, %d hasar onarildi (Enerji -10).\n", gerekenParca, onarilan);
                break;
            }

            /* ===== D: DURUM RAPORU ===== */
            case 'd':
            case 'D':
            {
                gecerliTur = 0;   /* tur harcamaz */
                printf("\n========= DURUM RAPORU =========\n");
                printf("Gun: %d | Tur: %d\n", gun, turSayaci);
                printf("Saglik       : %d\n", saglik);
                printf("Enerji       : %d\n", enerji);
                printf("Oksijen      : %d\n", oksijen);
                printf("Su           : %d\n", su);
                printf("Yedek parca  : %d\n", yedekParca);
                printf("Modul hasari : %d\n", modulHasari);
                printf("Hava kilidi  : %s\n", kilitMuhurlu == 1 ? "MUHURLU" : "ACIK");

                if (oksijen < 25) printf("UYARI: Oksijen kritik seviyede!\n");
                if (saglik < 30) printf("UYARI: Saglik kritik seviyede!\n");
                if (modulHasari > 70) printf("UYARI: Modul hasari kritik seviyede!\n");
                if (enerji < 20) printf("UYARI: Enerji kritik seviyede!\n");
                if (su < 5) printf("UYARI: Su kritik seviyede!\n");

                if (oksijen >= 25 && saglik >= 30 && modulHasari <= 70 && enerji >= 20 && su >= 5) {
                    printf("TUM SISTEMLER NORMAL\n");
                }
                printf("================================\n");
                break;
            }

            /* ===== U: UYU / DINLEN ===== */
            case 'u':
            case 'U':
            {
                int enerjiArtis = 20 + (100 - enerji) / 5;
                enerji += enerjiArtis;
                oksijen -= 3;   /* oksijen tuketimi devam eder */
                printf("Dinlendiniz: Enerji +%d, Oksijen -3.\n", enerjiArtis);

                if (oksijen < 40) {
                    if (oksijen < 20) {
                        saglik -= 10;
                        printf("Oksijen 20'nin altinda! Dinlenmek yerine saglik -10.\n");
                    } else {
                        int saglikArtis = enerjiArtis / 4;
                        saglik += saglikArtis;
                        printf("Oksijen dusuk, iyilesme yavas: Saglik +%d.\n", saglikArtis);
                    }
                } else {
                    int saglikArtis = 10 + enerjiArtis / 4;
                    saglik += saglikArtis;
                    printf("Saglik +%d.\n", saglikArtis);
                }
                break;
            }

            /* ===== H: HAVA KILIDI SIFRESI ===== */
            case 'h':
            case 'H':
            {
                char sifre, tahmin;
                int deneme = 0;

                if (kilitMuhurlu == 1) {
                    printf("Hava kilidi zaten muhurlu. Komut iptal.\n");
                    gecerliTur = 0;
                    break;
                }
                if (enerji < 5) {
                    printf("Yetersiz enerji! Sifre denemesi icin en az 5 enerji gerekir. Komut iptal.\n");
                    gecerliTur = 0;
                    break;
                }

                sifre = 'A' + (gun % 26);
                printf("IPUCU: Sifre, alfabede A harfinden bugunun gun sayisi kadar ilerideki harftir.\n");
                printf("3 deneme hakkiniz var.\n");

                do {
                    printf("Sifre (tek harf): ");
                    scanf(" %c", &tahmin);
                    tamponuTemizle();

                    /* Kucuk harf girisini buyuk harfe cevir */
                    if (tahmin >= 'a' && tahmin <= 'z') {
                        tahmin = tahmin - 'a' + 'A';
                    }

                    if (tahmin != sifre) {
                        deneme++;
                        enerji -= 5;
                        if (enerji < 0) enerji = 0;
                        if (tahmin < 'A' || tahmin > 'Z') {
                            printf("Gecersiz karakter! Enerji -5 (kalan hak: %d)\n", 3 - deneme);
                        } else if (tahmin < sifre) {
                            printf("Yanlis! Ileri git. Enerji -5 (kalan hak: %d)\n", 3 - deneme);
                        } else {
                            printf("Yanlis! Geri git. Enerji -5 (kalan hak: %d)\n", 3 - deneme);
                        }
                    }
                } while (tahmin != sifre && deneme < 3);

                if (tahmin == sifre) {
                    kilitMuhurlu = 1;
                    printf("Sifre dogru! Hava kilidi MUHURLENDI.\n");
                } else {
                    modulHasari += 20;
                    printf("3 deneme basarisiz! Modul hasari +20.\n");
                }
                break;
            }

            /* ===== Q: CIKIS ===== */
            case 'q':
            case 'Q':
                printf("Simulasyondan cikiliyor...\n");
                gecerliTur = 0;
                sonuc = 3;
                break;

            default:
                printf("Tanimsiz komut girdiniz! Lutfen gecerli bir harf girin.\n");
                gecerliTur = 0;
                break;
        }

        /* ---------- KOMUT SONRASI SINIRLAMA ---------- */
        sinirla(&saglik, &enerji, &oksijen, &modulHasari);

        /* ---------- ZAMAN AKISI ---------- */
        if (gecerliTur == 1) {
            turSayaci++;
            if (turSayaci % 3 == 0) {
                gun++;
                oksijen -= 8;
                printf("\n============================\n");
                printf("--- %d. GUN BASLADI! (Oksijen -8) ---\n", gun);
                if (modulHasari > 70) {
                    saglik -= 5;
                    printf("Agir hasar sebebiyle 5 saglik kaybettiniz!\n");
                }
                printf("============================\n");
            }
            sinirla(&saglik, &enerji, &oksijen, &modulHasari);
        }

        /* ---------- OYUN SONU KONTROLU ---------- */
        if (sonuc != 3) {
            if (saglik <= 0 || oksijen <= 0) {
                sonuc = 1;
            } else if (gun > 15) {
                sonuc = 2;
            }
        }

    } while (sonuc == 0);

    /* ---------- SONUC RAPORU ---------- */
    int hayattaKalinanGun = (gun > 15) ? 15 : gun;
    int puan = (hayattaKalinanGun * 100) + saglik + oksijen - modulHasari;

    printf("\n=====================================\n");
    if (sonuc == 1) {
        if (saglik <= 0) printf("OYUN BITTI: Sagliginiz tukendi. Kaybettiniz!\n");
        else printf("OYUN BITTI: Oksijeniniz tukendi. Kaybettiniz!\n");
    } else if (sonuc == 2) {
        printf("TEBRIKLER! 15 gun hayatta kaldiniz, kurtarma ekibi geldi!\n");
    } else {
        printf("Simulasyondan ayrildiniz.\n");
    }
    printf("-------------- SONUC RAPORU --------------\n");
    printf("Hayatta kalinan gun : %d\n", hayattaKalinanGun);
    printf("Toplam tur          : %d\n", turSayaci);
    printf("Saglik              : %d\n", saglik);
    printf("Enerji              : %d\n", enerji);
    printf("Oksijen             : %d\n", oksijen);
    printf("Su                  : %d\n", su);
    printf("Yedek parca         : %d\n", yedekParca);
    printf("Modul hasari        : %d\n", modulHasari);
    printf("PUAN                : %d\n", puan);
    printf("(Puan = gun*100 + saglik + oksijen - modulHasari)\n");
    printf("=====================================\n");

    return 0;
}


    
