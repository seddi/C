/***********************************************************************************************
 * Copyright © Sedat Geldi & Semih Ozkoroglu                                                              *
 * *********************************************************************************************
 * Birden fazla olan kelimeleri alfabetik siraya gore listeler.Ornegin girdimiz su sekilde ise ;
 * 
 * "Adem istanbul'dan geldi. Ama yarin istanbul'a geri giden yine adem olacak."
 * 
 * Cikti : adem , istanbul ,
 * 
 * Buyuk kucuk karakter ayrimi yapilmamaktadir.Ciktimiz kucuk karakterdedir.
 * 
 * 
 ************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000    /* Hash tablosu icin sabit bir boyut belirle */
#define MAXBUF 101  /*Her satirdan okunacak en fazla karakter sayisi kural geregi 100 + \0 karakteri*/ 

typedef struct new {
	char *kelime;
	int sayi;
	struct new *sonra;
	struct new *once;
} WORD;

WORD *word = NULL;				/* kelimeleri tutacak olan struct yapısı */
WORD *Hword[MAX]; 				/* kelimelerin hash degerinde kelimelerin bilgilerinin tutuldugu struct dizisi*/
WORD *bas = NULL, *son = NULL;  /* sıralama sırasında yer kelimelere yer degisikligi icin struct'ların basını ve sonunu belirle */
WORD *q;         				/* max_count sayıda olan kelimeleri yazdırmak icin sıralamada kullanılan struct pointer */

static int state;  				/* equal fonsiyonunda aynı slota denk gelen diger kelime ile o slotta blunan kelimenin esitlik durumunu global tut*/

/* hash tablosu boyutuna gore slot numarsı uret */
int hash(char s[]){
	int i, top = 0;
	for (i = 0; s[i] != '\0' && (s[i]=(s[i] >= 65 && s[i] <= 90) ? (s[i]+32):s[i]); i++) /* eger buyuk harf ise kucuk yap ve hashle*/
		top += (i+1) * s[i];     /* kendi belirledigimiz bir hash uretme ifadesi cakısma olsada kontrol yapılmakta */
	return top % MAX;
}

/* max_count sayıdaki kelimeleri alfabetik sıralayarak struct zincir fonksiyonu */
void add_sort(WORD *p){ 
	WORD *ilk, *w;
	int i;
	if(bas == NULL){
		bas = p;
		p->sonra = NULL;
		p->once = NULL;
		return ;
	}
	for (q = bas; q;q = q->sonra){
		i = 1;
		if(strcmp(p->kelime, q->kelime) == -1){ /* gelen kelime kucukse gir*/
			ilk = q->once;
			i = 0;
				if (!q->once){ 					/* oncesi bossa basa koy*/
					q->once = p;
					p->once = NULL;
					p->sonra = q;
					bas = p;
					return ;
				}
				if (q->once ){					/* doluysa araya koy*/
					p->once = ilk;
					p->sonra = q;
					q->once = p;
					ilk->sonra = p;
					return ;
				}
			break;
		}
		w = q;
	}	
	if(i) q = w;
	if (!q->sonra){								/*gelen kelime en buyuk oldugundan sona ekle*/
		q->sonra = p;
		p->sonra = NULL;
		p->once = q;
		return ;
	}
}

/* max_count sayıda olan kelimeleri yaz */
void print(FILE *fp,int max_count){
	int i;
	WORD *p;
	for (i = 0; i < MAX; i++)
		if ((word = Hword[i]) != NULL)
			for (; word; word = word->sonra)
				if (word->sayi == max_count){ 
					p = (WORD *)malloc(sizeof(WORD));
					p->kelime = strdup(word->kelime);
					add_sort(p);
				}
	for (q = bas; q; q = q->sonra){
		printf("%s, ",q->kelime);
	}
	for (q = bas; q; q = q->sonra){
		free(q);
	}
}

/* alfabeden bir harf ise true don */
int word_end(char c){
	return (c >= 97 && c <= 122) || (c >= 65 && c <= 90);
}

/* gelen 2 kelime ayni mi kontrol et */
WORD *equal(WORD *w, char *b){
	int len, i;
	for (; w; w = w->sonra){
		if ((len = strlen(w->kelime)) == strlen(b)){
			state = 1;
			for (i = 0; i < len; i++){
				if (w->kelime[i] != b[i]){
					state = 0; break;        /* kelimeler icinde ilk farklı karakter icin donguden cık*/
				}
			}
			if (state) return w; 			/* donguden cıktıgında state = 1 ise (yani kelimeler esit ise) o anki struct'ın adresini don*/
		}
	}
	state = 0;
	return w; 								/* kelime boyları esit degil veya kelimeler esit degilse o anki struct'ın adresini don */
}

/* kelimeleri h slotuna ekle */
void load(char *b, int h){
	word = malloc(sizeof(WORD));
	word->kelime = strdup(b);
	word->sayi = 1;         		/* farklı bir kelime geldiginde yukleme yapılır ve sayısını 1 ile ilkle */
	word->sonra = Hword[h];
	Hword[h] = word;
}

/* bellekleri geri ver */
void freee(){
	int i;
	for (i = 0; i < MAX; i++)
		if ((word = Hword[i]) != NULL)
			for (; word; word = word->sonra)
				free(word);
}

int main(){

	int h, i, durum;
	int max_count = 2;								/* Birden fazla kelime varsa yazdir */
	char *buf, c;
	
	buf = malloc(sizeof(char)*MAXBUF);
	for (i = 0; (c = getchar()) != EOF; ){         /* "Stdin" dosyasından karakter oku ve EOF ile sonlandır */
		if (word_end(c)){ 
			*(buf+(i++)*sizeof(char)) = c;
			durum =1;
			}                    

		else if( durum ){  				   		 /* harf dısında karakter bulana kadar buf'a karakter ekle (kelimeleri ayıkla) */
			*(buf+i*sizeof(char)) = '\0';        /* kelime sonu karakteri ekle */
			h = hash(buf);	                     /* kelimeyi hash fonk gonder ve slot numarasını don */
			if ((Hword[h]) != NULL){             /* Hash tablosunda h slotu dolu mu? */
				word = Hword[h];
				word = equal(word, buf);         /* esitlik fonksıyonundan donen struct adresi */
				if (state){
					word->sayi += 1;  			 /* Hash'deki h slotu dolu ve ayni kelime ise sadece sayisini artır */
					max_count = (word->sayi > max_count) ? word->sayi:max_count; 	/* kelimenin sayısı max_count den buyukse maxlen'i 
																				    	kelimenin sayisi ile degistir */
				}
				else
					load(buf, h);                 	/* Hash'deki h slotu dolu ve farklı kelime ise yeni kelimeyi yukle */
			}
			else if(Hword[h] == NULL)
				load(buf, h);            			/* Hash'deki h slotu bossa ilk kelimeyi yukle */
			free(buf);
			buf = malloc(sizeof(char)*MAX);	
			i = 0;
			durum = 0;
		}
	}
	print(stdout,max_count);    /* "Stdout" dosyasına cıktı uret */
	freee();
	return 0;
}
