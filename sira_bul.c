#include <stdio.h>
#include <stdlib.h>

#define basamak_sayisi N
#define GIRIS "bg.gir"
#define CIKIS "bg.cik"


/* Ondokuz Mayis Universitesi 
 * Bilgisayar Muhendisligi Bolumu
 * 2. sinif ogrencisiyim
 * 
 * Sedat Geldi
 * 
 * sedat.geldi@bil.omu.edu.tr */ 

int sirami_bul( int n, int sayi, int sira );
int fakt( int sayi );
int power( int x, int y );
int *rakam_dizi( int sayi, int N );

int main()
{	
	int N, sayi, num[2];
	int i = 0, sira = 0;
	FILE *gir, *cik;
	
	if ((gir = fopen(GIRIS, "r")) != NULL)
	{
			while(! feof(gir) ) 
			{ 
				fscanf(gir, "%d", &num[i]);
				i++;
			}
		printf("okuma işlemi basarili..\n");
	}
	else 
	{
		fprintf(stderr, "%s dosyasi okumak icin acilamadi..", GIRIS);
		perror("cunku");
		return 1;
	}
	fclose(gir);
	
	N = num[0];
	sayi = num[1];
	sira = sirami_bul( N, sayi, sira);
	
	if ((cik = fopen(CIKIS, "w")) != NULL)
	{
		fprintf(cik, "%d", sira);
		printf("yazma işlemi basarili..\n");
	}
	else
	{ 
		fprintf(stderr, "%s dosyasi yazmak icin acilamadi..", CIKIS);
		perror("cunku");
		return 1;
	}
	fclose(cik);	

return 0;
}
int sirami_bul(int N, int sayi, int sira)
{	
	int r, i, j, k, boy;
	int *v;
	
	v = rakam_dizi(sayi, N);
	boy = N;
	for (i = 0; N > 0; i++, N--) 
	{
		k = 0;
		r = fakt(N)/N;  /* her rakamin bir basamaktaki kullanilma sayisi */
        for (j = i+1; j < boy ; j++)
			if (v[i] > v[j]) k++;
		sira = sira + r*k;	
    }
    
	free(v);
	return sira+1;
}
int fakt(int sayi)
{
	int i;
	int fakt;
	fakt = sayi;
	for (i = sayi-1 ; i >= 1 ; --i)
		fakt = fakt*i;
		
	return fakt;
}
int power(int x, int y)
{
	int p = 1;
	for ( ; y > 0 ; y--)
		p = p*x;
	return p;	
}

int *rakam_dizi(int sayi, int N)
{
	int i, p, pow;
	int *v;
	v = malloc (sizeof(int) * N);
	for (i = 0, p = N-1 ; i < N ; i++ , p--)
	{	
		pow = power(10, p);
		v[i] = sayi / pow; /* sayinin en sol basamaktaki rakamamını dızye eleman olarak atama */
		sayi = sayi % pow; /* sayiyi soldan bir basamak eksiltiyoruz */
	} 
	return v;
}
