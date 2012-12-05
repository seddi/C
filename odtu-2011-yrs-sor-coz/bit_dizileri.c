/*************************************************************************************************************
 * 							Copyright © Sedat Geldi & Semih Ozkoroglu                                          * 
 * 									Ondokuz Mayis Universitesi                                               *
 * 									Bilgisayar Muhendisligi                                                  *
 *************************************************************************************************************
 *  Kullanicidan alinan N uzunlugundaki bitlerin tum ihtimallerini iceren en kisa bit uzunlugunu hesaplamakta.
 * 
 * Girilen N degerlerine bagli olarak tumevarim'dan yola cikarsak.
 * 
 * 		N = 2 icin 00 , 01 , 11 , 10 --> ihtimallerimiz
 * 					
 * 					00
 * 					 01        ---->     en anlamsiz bitleri yukari kaydirirsak 00110 elde ederiz.Bu da 5 basamaktir.
 * 					  11       ---->
 * 					   10 
 * 		N = 3 icin 000 , 001 , 010 , 011 , 100 , 101 , 110 , 111 --> ihtimallerimiz
 * 			
 * 					000
 * 					 001
 *					  011
 * 					   111     ---->  en anlamsiz bitleri yukari kaydirirsak 0001110100 elde ederiz.Bu da 10 basamaktir.
 * 						110    ---->
 * 						 101
 * 						  010
 * 						   100 					
 * 
 * ( 2^N + N -1 ) formulu buluruz.
 * 
 * fakat N degeri buyudugunde 2^N'in degeri 32 bit veya 64 bitlik bir islemcide herhangi bir veri turunde tutulamayacagi icin ,
 * 2^N 'nin binary karsiligi ile N-1 'in binary karsiligini topladiktan sonra bcd kodlama sistemine cevirip decimal olarak yazdırdık.
 * 
 * Ornegin: N = 11 icin 2^11 = ( 100000000000 ) -> binary karsiligi ve N-1 = 10 => ( 1010 ) binary karsiligini toplayarak 
 * 2^N + N -1 = 2^11 + 11 - 1 => ( 100000001010 ) elde ederiz.Bu binary sayiyi bcd'ye cevirmek icin shift-add_3( kaydir 3 ekle ) 
 * methodunu kullandık.
 * Yani ( 2^N + N -1 ) sonucu asagidaki gibi bir binary sayi oldugunu dusunelim ;
 *
 *  		|Onlar	|Birler	|
 * ------------------------------------------------------			
 *	 	|		|	    | 11010 -> ( 2^N + N-1 ) = 26
 *	Shift-1 |		|	  1 | 1010
 *	Shift-2 |		|	 11 | 010
 *	Shift-3 |		|	110 | 10  ( Her kaydirmada(Shift) her basamak kontrol edilir ve 5 'den buyuk oldugunda 3 eklenir(Add-3))
 *	Add-3 	|	  	|      1001 | 10
 *	Shift-4 |	      1 |      0011 | 0
 *	Shift-5 |	      10|      0110 |
 * --------------------------------------------------
 * Dec      |    2  |   6   | = 26 olarak bulunur.
 *
 * Ancak bu algoritmada N ~= 4700 icin 1 saniye kisiti gecerlidir.Bundan buyuk degerler icin kisit asilmaktadir.
 *
 ********************************************************************************************************************/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

static int lenght = 0;

int *dec2binary(int d){ // Decimal sayiyi binary'e cevir
	int *p, i;
	p = malloc(sizeof(int)*15);
	if( p == NULL ){
		printf( "Yetersiz bellek!" );
		exit(1);
	}
	for ( i = 0; d > 0 ; i++ ){
		p[i] = d%2; d = d/2;
		lenght ++;
	}

	return p;
}
void bcd2dec(int *M, int len,int N){  // Bcd kodu decimal'e cevir
	int i, j, k, top;
	k = len%4;
	for (i = 0; i < len; ){
		top = 0;
		for (j = 0; j < k; j++, i++)
			top += M[i]*pow(2, k-j-1);
		if (k>0) printf("%d",top);
		k = 4;
	}
}

int add_3(int *M , int len){ // 3 ekleme fonksiyonu.

	int tmp = 0, sum = 0;
	int i, j, k, t;

	for (i = len; i > 0;){
		k = (i > 4) ? 4:i; // 4 bit 4 bit grupla en anlamli bitler 4 bitten kucuksede grupla

		if( (k == 3 && M[i-3] && (M[i-2] || M[i-1])) || (k==4 && (M[i-4] || ( M[i-3] && (M[i-2] || M[i-1] ))) ) ){  // Basamak degeri 4'den buyukse 3 ekle
			for (j = 0; j < k; j++, i--){
				if (j < 2){
					sum += M[i-1] + 1;
					M[i-1] = sum%2;
					sum = sum/2; // Eldeyi sakla
				}
				else if (sum){
					sum += M[i-1];
					M[i-1] = sum%2;
					sum = sum/2;
				}
			}
		}
		else i -= k;
	}
	t = 0;
	if (sum){ // En son elde 1 ise diziyi saga kaydir ve eldeyi en basa ekle
		tmp = sum;
		for (i = 0; i < len; i++){
			sum = tmp;
			tmp = M[i];
			M[i] = sum;
		}
		t = 1;
		M[i] = tmp;
	}
	return t;
}
void shift(int *bin_array, int len){ //Kaydirma fonksiyonu
	int k, n, i = 0;

	int *M;
	M = malloc(sizeof(int)*2*len);
	if( M == NULL ){
		printf( "Yetersiz bellek!" );
		exit(1);
	}

	for(k = 0, n = 0; k < len ;n++, k++ ){
		n += i;
		M[n] = bin_array[k]; 						// Bit bit kaydir
		if(n > 1 && k < len - 1) i = add_3(M, n+1); // 3 ekleme icin gonder
	}

	bcd2dec(M,n,len-1);

	free(M);
}

int main(){
	int *P, N, i, *bin ;

	scanf("%d",&N);
	P = malloc(sizeof(int)*(N+1));
	if( P == NULL ){
		printf( "Yetersiz bellek!" );
		exit(1);
	}
	P[0] = 1;
	bin = dec2binary(N-1); 		  // N-1 'in binary hesabini yap

	for ( i = 0; i < N ; i++)
		*(P+i+1) = 0; 			 // 2^N 'in binary karsiligini diziye ata

	for ( i = 0 ; i < lenght ; i++)
		P[N-i] = bin[i]; 		// 2^N + N -1 'in binary karsiligi

	shift(P, N+1); 				// 2^N + N - 1 'in binary karsiligini kaydirmaya gonder
	free(P);
	free(bin);

	return 0;
}
