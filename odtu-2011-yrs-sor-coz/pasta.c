/******************************************************************************
 *	Mahmut m = 3 numarali pastadan yerse `1/3` 'luk bir miktarda pasta yemis olur.
 * 	Buna karsilik zeberceddin `1/3 = 1/k + 1/n` esitligini saglayan k ve n numarali pastalarindan birer dilim yemelidir.
 * 	( k != n) Bu durumda k ve n degerlerini su sekilde bulduk.
 * 		
 * 		Eger mahmut 1 numarali pastadan yerse zeberceddin'in 2 numarali pastadan iki dilim yemesi gerekir bu kural disidir.
 * 		m > 1 icin ise 
 *		zeberceedin'in yiyecegi pastalar: mahmut'un yedigi m pastasindan yola cikarak m+1 'inci pastadan yer buna göre buldugumuz
 *	`n` degeri diger yemesi gereken parcasidir.Bundan sonra m degerini artirdigimizda artmis olan m degerine karsilik olan diger parcamiz
 *   buldugumuz n degerinden kucuk bir parca olacagi icin yeni n'inimiz artmis olan m degerine karsilik gelen deger olacaktir.    
 * 	 	
 * 		c artirma miktarimiz ve buna karsilik k degeri (m+c) olur. n = (m+c)*m/c formulu ile n degerini elde ederiz.Ve n degerinin tamsayi 
 * 	oldugu durumlari ele aldik.
 * 	  
 *  Ornegin  m = 3 ise
 * 		c degerini 1 'den baslatip k = m + c 'den k = 4 buna karsilik n = 12 olur.Bir sonraki adimda c = c + 1 olup dongu
 *		en son buldugu n degerine kadardir yani ilk donguye ilk girdiginde c 12'ye kadar articakti ancak dongu icinde buldugu yeni n degeri ile
 *		dongu yeniden sinirlandirilir.Ve zeberceddin 'in yiyecegi pastalar her c 'ye karsilik bulunan k ve tamsayi olan n degeri icin
 *		yiyecegi aralik daraltilir.  
 * 
 * 
 *  
 * */
#include<stdio.h>
#include<stdlib.h>

int main(){
	long int m, l, carpim, c = 0; /* m => Mahmutun pastası , l = zeberceddin'nin yiyebilecegi pastanın biri*/
	int count = 0;
	scanf("%ld", &m);
	if (!m) {
		printf("==>> %ld pastası zaten yok mahmut nasıl yesin :))",m);
		exit(1);
	}
	l = ((m+1)*m)/(c+1); 		/* c+1 zaberceddinin ilk yedigi pasta ona karsılık diger yiyecegi pasta l'dir */
	if (l != m+1){ 				/* zeberceddin ayni pastadan mi yiyor.*/
		c = 1;
		count++;
		for (c = 2; c + m < l ; c++){ 
			carpim = (m+c)*m;
			if (!(carpim % c) && (c+m != carpim/c)){/* zeberceddin tam dilim mi yiyor ve ayni dilimden mi*/   
				count++;
				l = carpim/c; 						/* yeni l degerini uret yiyecegi araligi daralt */
			}
		}
	}
	printf("%d \n",count);
	return 0;
}
