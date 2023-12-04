#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

string tahta[11][11];
string* path[40];
string* piyonlar[4][4];
int alinan_yol[4][4] = { {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0} };
int tahtadaki_piyon_sayisi[4] = {0,0,0,0};
vector <int> kale;
vector <int> home;
string* baslangic_noktasi[4];
string* bitis_noktasi[4];
static int oyuncu_sayisi = 0;
vector<string> oyuncu_isimleri;
string* home_adres[4][4];
string* kale_adres[4][4];
vector < vector <string*>> tahtadaki_piyonlar;
bool oynayabildi_mi = true;
bool piyon_kirdi_mi = false;
string kirilan_piyon;

void tahta_olustur();
void tahta_goster();
void kale_olustur();
void home_olustur();
void yer_degisStr(string&, string&);
void yer_degisInt(int&, int&);
void path_olustur();
void tahta_guncelle();
void sira_duzenle(vector <int>&);
bool zar_kontrol(vector<int>, int);
string* ilerle(string*, int, int);
int zar_at(string);
void oyna(int,int);
void tahta_dosya_yaz();
void Start();
void piyon_adres_duzenle(int, int, int);

int main()
{
	tahta_olustur();
	tahta_goster();
	Start();
	path_olustur();
	kale_olustur();
	home_olustur();
	tahta_guncelle();
	tahta_goster();
	
	ofstream dosyaYaz("sira.txt");
	string token = ";";
	bool oyun_bitti = false;
	if (dosyaYaz.is_open()) {
		for (int oyuncu = 0; oyuncu < oyuncu_sayisi && !oyun_bitti; oyuncu++)
		{
			int zar = zar_at(oyuncu_isimleri[oyuncu]);
			oyna(oyuncu, zar);
			string oynadi_mi = oynayabildi_mi ? "oynadi" : "oynayamadi";
			string kirdi_mi = piyon_kirdi_mi ? "piyon kirdi" : "piyon kiramadi";
			dosyaYaz << endl<< oyuncu_isimleri[oyuncu] << token << zar << token << oynadi_mi;
			dosyaYaz << token << kirdi_mi << token;
			 oynayabildi_mi = true;
			 piyon_kirdi_mi = false;
			if (piyon_kirdi_mi) {
				dosyaYaz  << kirilan_piyon << token;
			}
			if (oyuncu == oyuncu_sayisi - 1) {
				oyuncu = -1;
			}
			for (int i = 0; i < oyuncu_sayisi; i++)
			{
				if (home[i] == 4) {
					cout << "OYUN BITTI KAZANAN: " << oyuncu_isimleri[i] << " TEBRIKLER" << endl;
					oyun_bitti = true;
				}
			}
			string bitti_mi = oyun_bitti ? "bitirdi" : "bitiremedi";
			dosyaYaz << bitti_mi;
			tahta_dosya_yaz();
		}
		dosyaYaz.close();
	}
}

void tahta_olustur()
{
	for (int i = 0; i < 11; i++)
	{
		for (int k = 0; k < 11; k++)
		{
			if (i == 4 || i == 6 || k == 4 || k == 6) {
				tahta[i][k] = " 00 ";
			}
			else {
				tahta[i][k] = " -  ";
			}
		}
	}
	tahta[0][0] = " K1 "; tahta[0][1] = " K2 "; tahta[1][0] = " K3 "; tahta[1][1] = " K4 ";
	piyonlar[0][0] = &tahta[0][0]; piyonlar[0][1] = &tahta[0][1]; piyonlar[0][2] = &tahta[1][0]; piyonlar[0][3] = &tahta[1][1];

	tahta[0][9] = " Y1 "; tahta[0][10] = " Y2 "; tahta[1][9] = " Y3 "; tahta[1][10] = " Y4 ";
	piyonlar[1][0] = &tahta[0][9]; piyonlar[1][1] = &tahta[0][10]; piyonlar[1][2] = &tahta[1][9]; piyonlar[1][3] = &tahta[1][10];

	tahta[9][0] = " M1 "; tahta[9][1] = " M2 "; tahta[10][0] = " M3 "; tahta[10][1] = " M4 "; 
	piyonlar[2][0] = &tahta[9][0]; piyonlar[2][1] = &tahta[9][1]; piyonlar[2][2] = &tahta[10][0]; piyonlar[2][3] = &tahta[10][1];

	tahta[9][9] = " S1 "; tahta[9][10] = " S2 "; tahta[10][9] = " S3 "; tahta[10][10] = " S4 ";
	piyonlar[3][0] = &tahta[9][9]; piyonlar[3][1] = &tahta[9][10]; piyonlar[3][2] = &tahta[10][9];  piyonlar[3][3] = &tahta[10][10];

	yer_degisStr(tahta[0][5], tahta[4][5]); yer_degisStr(tahta[5][0], tahta[5][4]);
	yer_degisStr(tahta[10][5], tahta[6][5]); yer_degisStr(tahta[5][10], tahta[5][6]);
	tahta[5][5] = " F  ";

	baslangic_noktasi[0] = &tahta[4][0];baslangic_noktasi[1] = &tahta[0][6];  
	baslangic_noktasi[2] = &tahta[10][4]; baslangic_noktasi[3] = &tahta[6][10];

	bitis_noktasi[0] = &tahta[5][0];  bitis_noktasi[1] = &tahta[0][5];
	bitis_noktasi[2] = &tahta[10][5]; bitis_noktasi[3] = &tahta[5][10];

	home_adres[0][0] = &tahta[5][4]; home_adres[0][1] = &tahta[5][3];
	home_adres[0][2] = &tahta[5][2]; home_adres[0][3] = &tahta[5][1];

	home_adres[1][0] = &tahta[4][5]; home_adres[1][1] = &tahta[3][5];
	home_adres[1][2] = &tahta[2][5]; home_adres[1][3] = &tahta[1][5];

	home_adres[2][0] = &tahta[6][5]; home_adres[2][1] = &tahta[7][5];
	home_adres[2][2] = &tahta[8][5]; home_adres[2][3] = &tahta[9][5];

	home_adres[3][0] = &tahta[5][6]; home_adres[3][1] = &tahta[5][7];
	home_adres[3][2] = &tahta[5][8]; home_adres[3][3] = &tahta[5][9];



	kale_adres[0][0] = &tahta[0][0]; kale_adres[0][1] = &tahta[0][1];
	kale_adres[0][2] = &tahta[1][0]; kale_adres[0][3] = &tahta[1][1];


	kale_adres[1][0] = &tahta[0][9]; kale_adres[1][1] = &tahta[0][10]; 
	kale_adres[1][2] = &tahta[1][9]; kale_adres[1][3] = &tahta[1][10];

	kale_adres[2][0] = &tahta[9][0]; kale_adres[2][1] = &tahta[9][1];
	kale_adres[2][2] = &tahta[10][0]; kale_adres[2][3] = &tahta[10][1];

	kale_adres[3][0] = &tahta[9][9]; kale_adres[3][1] = &tahta[9][10];
	kale_adres[3][2] = &tahta[10][9]; kale_adres[3][3] = &tahta[10][10];

	for (int i = 0; i < 4; i++)
	{
		vector <string*> vec;
		tahtadaki_piyonlar.push_back(vec);
	}
}

void tahta_goster()
{
		cout << "===========================================" << endl;
		for (int i = 0; i < 11; i++)
		{
			for (int k = 0; k < 11; k++)
			{
				cout << tahta[i][k];
			}
			cout << "\n" << endl;
		}
		cout << "===========================================" << endl;
}

void tahta_guncelle() {
	if (oyuncu_sayisi == 2) {
		tahta[9][0] = " -  "; tahta[9][1] = " -  ";
		tahta[10][0] = " -  "; tahta[10][1] = " -  ";
		tahta[9][9] = " -  "; tahta[9][10] = " -  ";
		tahta[10][9] = " -  "; tahta[10][10] = " -  ";
	}
	else if (oyuncu_sayisi == 3) {
		tahta[9][9] = " -  "; tahta[9][10] = " -  ";
		tahta[10][9] = " -  "; tahta[10][10] = " -  ";
	}
}

void yer_degisStr(string& isim1, string& isim2)
{
	string isim = isim1;
	isim1 = isim2;
	isim2 = isim;
}

void yer_degisInt(int& sayi1, int& sayi2)
{
	int sayi = sayi1;
	sayi1 = sayi2;
	sayi2 = sayi;
}

void path_olustur() {
	int iter = 0;
	for (int i = 10; i > 5; i--)
	{
		path[iter++] = &tahta[i][4];
	}
	for (int i = 3; i >= 0; i--) {
		path[iter++] = &tahta[6][i];
	}
	for (int i = 5; i > 3; i--)
	{
		path[iter++] = &tahta[i][0];
	}
	for (int i = 1; i < 5; i++)
	{
		path[iter++] = &tahta[4][i];
	}
	for (int i = 3; i >= 0; i--) {
		path[iter++] = &tahta[i][4];
	}
	for (int i = 5; i < 7; i++)
	{
		path[iter++] = &tahta[0][i];
	}
	for (int i = 1; i < 5; i++)
	{
		path[iter++] = &tahta[i][6];
	}
	for (int i = 7; i < 11; i++)
	{
		path[iter++] = &tahta[4][i];
	}
	for (int i = 5; i < 7; i++)
	{
		path[iter++] = &tahta[i][10];
	}
	for (int i = 9; i > 5; i--)
	{
		path[iter++] = &tahta[6][i];
	}
	for (int i = 7; i < 11; i++)
	{
		path[iter++] = &tahta[i][6];
	}
	path[iter++] = &tahta[10][5];
}

void Start()
{
	char zar_at = 'z';
	vector<char> renkler = { 'K', 'Y', 'M', 'S' };
	vector<int> zarlar;
	cout << "Oyunu kac kisi oynamak istiyorsunuz ? (2-4)" << endl;
	cin >> oyuncu_sayisi;

	for (int i = 0; i < 4 - oyuncu_sayisi; i++)
	{
		renkler.pop_back();
	}

	for (int i = 0; i < oyuncu_sayisi; i++) {
		string isim;
		char secim;
		cout << i + 1 << ". oyuncunun ismini giriniz." << endl;
		cin >> isim;
		cout << "Oyun siralamasi icin zar atiniz. (z ile zar atabilirsiniz)" << endl;
		cin >> secim;
		srand(time(0));
		if (secim == zar_at) {
			bool zar_ktrl;
			int zar = rand() % 6 + 1;
			zar_ktrl = zar_kontrol(zarlar, zar);
			cout << "Attiginiz zar " << zar << endl;
			while (zar_ktrl)
			{
				cout << "Baska bir oyuncu ayni zari atmis. Lutfen tekrar zar atiniz. (z)" << endl;
				cin >> secim;
				if (secim == zar_at) {
					zar = rand() % 6 + 1;
					zar_ktrl = zar_kontrol(zarlar, zar);
					cout << "Attiginiz zar " << zar << endl;
				}
			}
			zarlar.push_back(zar);
		}
		oyuncu_isimleri.push_back(isim);
	}
	sira_duzenle(zarlar);
}

bool zar_kontrol(vector<int> zarlar, int zar)
{
	for (auto& i : zarlar)
	{
		if (i == zar) {
			return true;
		}
	}
	return false;
}

void sira_duzenle(vector <int>& zarlar) {
	for (int i = oyuncu_sayisi; i > 1; i--)
	{
		for (int k = 0; k < i - 1; k++)
		{
			if (zarlar.at(k) < zarlar.at(k + 1)) {
				yer_degisStr(oyuncu_isimleri.at(k), oyuncu_isimleri.at(k + 1));
				yer_degisInt(zarlar.at(k), zarlar.at(k + 1));
			}
		}
	}
}

int zar_at(string isim)
{
	int zar;
	char secim, zar_at = 'z';
	srand(time(0));
	cout << isim << " sira sende. Zar at.(z ile zar atabilirsin)" << endl;
	cin >> secim;
	if (secim == zar_at)
	{
		zar = rand() % 6 + 1;
		cout << "Attiginiz zar " << zar << endl;
	}
	else {
		return 0;
	}
	return zar;
}

void kale_olustur() {
	for (int i = 0; i < oyuncu_sayisi; i++)
	{
		kale.push_back(4);
	}
}

void home_olustur() {
	for (int i = 0; i < oyuncu_sayisi; i++)
	{
		home.push_back(0);
	}
}

void oyna(int oyuncu, int zar) {
	if (zar == 6) {
		if (tahtadaki_piyon_sayisi[oyuncu] == 0) {
			if (*baslangic_noktasi[oyuncu] == " 00 ") {
				for (int i = 0; i < 4; i++)
				{
					if (*kale_adres[oyuncu][i] != " 00 ") {
						*baslangic_noktasi[oyuncu] = *piyonlar[oyuncu][i];
						tahtadaki_piyonlar[oyuncu].push_back(baslangic_noktasi[oyuncu]);
						*piyonlar[oyuncu][i] = " 00 ";
						piyonlar[oyuncu][i] = baslangic_noktasi[oyuncu];
						tahtadaki_piyon_sayisi[oyuncu]++;
						kale[oyuncu]--;
						tahta_goster();
						break;
					}
				}
			}
			else {
				cout << "Baslangic noktasi dolu piyon cikaramazsiniz!" << endl;
			}
		}
		else if(tahtadaki_piyon_sayisi[oyuncu] > 0) {
			if (kale[oyuncu] > 0) {
				char secim = ' ';
				if (*baslangic_noktasi[oyuncu] == " 00 ") {
					cout << "Kaleden piyon cikarmak ister misiniz ? (e/h)" << endl;
					cin >> secim;
					if (secim == 'e') {
						for (int j = 0; j < 4; j++)
						{
							if (*kale_adres[oyuncu][j] != " 00 ") {
								*baslangic_noktasi[oyuncu] = *piyonlar[oyuncu][j]; 
								tahtadaki_piyonlar[oyuncu].push_back(baslangic_noktasi[oyuncu]);
								*piyonlar[oyuncu][j] = " 00 ";
								piyonlar[oyuncu][j] = baslangic_noktasi[oyuncu];
								tahtadaki_piyon_sayisi[oyuncu]++;
								kale[oyuncu]--;
								tahta_goster();
								break;
							}
						}
					}
					else if (secim == 'h') {
						char secim2 = ' ';
						for (int i = 0; i < tahtadaki_piyon_sayisi[oyuncu]; i++)
						{
							cout << *tahtadaki_piyonlar[oyuncu][i] << " piyonunu oynamak ister misiniz? (e/h)" << endl;
							cin >> secim2;
							if (secim2 == 'e') {
								string* ptr = tahtadaki_piyonlar[oyuncu][i];
								tahtadaki_piyonlar[oyuncu][i] = ilerle(tahtadaki_piyonlar[oyuncu][i], zar, oyuncu);
								if (ptr != tahtadaki_piyonlar[oyuncu][i]) {
									piyon_adres_duzenle(i, zar, oyuncu);
									tahta_goster();
								}
								break;
							}
						}
					}
				}
				else {
					cout << "Baslangic noktasi dolu kaleden piyon cikaramazsiniz!" << endl;
					for (int i = 0; i < tahtadaki_piyon_sayisi[oyuncu]; i++)
					{
						cout << *tahtadaki_piyonlar[oyuncu][i] << " piyonunu oynamak ister misiniz? (e/h)" << endl;
						cin >> secim;
						if (secim == 'e') {
							string* ptr = tahtadaki_piyonlar[oyuncu][i];
							tahtadaki_piyonlar[oyuncu][i] = ilerle(tahtadaki_piyonlar[oyuncu][i], zar, oyuncu);
							if (ptr != tahtadaki_piyonlar[oyuncu][i]) {
								piyon_adres_duzenle(i, zar, oyuncu);
								tahta_goster();
							}
							break;
						}
					}
				}
			}
			else if (kale[oyuncu] == 0) {
				char secim2 = ' ';

				for (int i = 0; i < tahtadaki_piyon_sayisi[oyuncu]; i++)
				{
					cout << *tahtadaki_piyonlar[oyuncu][i] << " piyonunu oynamak ister misiniz? (e/h)" << endl;
					cin >> secim2;
					if (secim2 == 'e') {
						string* ptr = tahtadaki_piyonlar[oyuncu][i];
						tahtadaki_piyonlar[oyuncu][i] = ilerle(tahtadaki_piyonlar[oyuncu][i], zar, oyuncu);
						if (ptr != tahtadaki_piyonlar[oyuncu][i]) {
							piyon_adres_duzenle(i, zar, oyuncu);
							tahta_goster();
						}
						break;
					}
				}
				/*for (int i = 0 + home[oyuncu]; i < tahtadaki_piyon_sayisi[oyuncu] + home[oyuncu]; i++)
				{
					cout << *piyonlar[oyuncu][i] << " piyonunu oynamak ister misiniz? (e/h)" << endl;
					cin >> secim2;
					if (secim2 == 'e') {
						piyonlar[oyuncu][i] = ilerle(piyonlar[oyuncu][i], zar, oyuncu);

						alinan_yol[oyuncu][i] += zar;
						cout << "alinanyol: " << alinan_yol[oyuncu][i] << endl;
						if (alinan_yol[oyuncu][i] >= 39) {
							*home_adres[oyuncu][i] = *piyonlar[oyuncu][i];
							*piyonlar[oyuncu][i] = " 00 ";
							piyonlar[oyuncu][i] = home_adres[oyuncu][i];
							home[oyuncu]++;
							tahtadaki_piyon_sayisi[oyuncu]--;
						}
						tahta_goster();
						break;
					}
				}*/
			}
		}
	}
	else {
		char secim = ' ';
		if (tahtadaki_piyon_sayisi[oyuncu] == 1) {
			for (int i = 0; i < tahtadaki_piyon_sayisi[oyuncu]; i++)
			{
				string* ptr = tahtadaki_piyonlar[oyuncu][i];
				tahtadaki_piyonlar[oyuncu][i] = ilerle(tahtadaki_piyonlar[oyuncu][i], zar, oyuncu);
				if (ptr != tahtadaki_piyonlar[oyuncu][i]) {
					piyon_adres_duzenle(i, zar, oyuncu);
					tahta_goster();
				}
				break;
			}
		}
		else if (tahtadaki_piyon_sayisi[oyuncu] > 1) {
			for (int i = 0; i < tahtadaki_piyon_sayisi[oyuncu]; i++)
			{
				cout << *tahtadaki_piyonlar[oyuncu][i] << " piyonunu oynamak ister misiniz? (e/h)" << endl;
				cin >> secim;
				if (secim == 'e') {
					string* ptr = tahtadaki_piyonlar[oyuncu][i];
					tahtadaki_piyonlar[oyuncu][i] = ilerle(tahtadaki_piyonlar[oyuncu][i], zar, oyuncu);
					if (ptr != tahtadaki_piyonlar[oyuncu][i]) {
						piyon_adres_duzenle(i, zar, oyuncu);
						tahta_goster();
					}
					break;
				}
			}
		}
	}
}

string* ilerle(string* piyon, int zar, int oyuncu)
{
	for (int i = 0; i < 40; i++)
	{
		if (path[i] == piyon) {
			i += zar;
			if (i >= 40) {
				i = i % 40;
				
			}
			if (*path[i] != " 00 ") {
				for (int k = 0 ; k < tahtadaki_piyon_sayisi[oyuncu]; k++)
				{
					if (*path[i] == *tahtadaki_piyonlar[oyuncu][k]) {
						cout << "Buraya oynayamazsiniz baska bir piyonunuz var!" << endl;
						oynayabildi_mi = false;
						return piyon;
					}
				}

				for (int player = 0; player < oyuncu_sayisi; player++)
				{
					for (int j = 0; j < 4; j++)
					{
						if (*path[i] == *piyonlar[player][j]) {
							cout << *piyonlar[player][j] << " piyonunu kalesine gonderdiniz!" << endl;
							piyon_kirdi_mi = true;
							for (int iter = 0; iter < tahtadaki_piyon_sayisi[player]; iter++)
							{
								if (*piyonlar[player][j] == *tahtadaki_piyonlar[player][iter]) {
									tahtadaki_piyonlar[player].erase(tahtadaki_piyonlar[player].begin() + iter);
									break;
								}
							}
							kirilan_piyon = *piyonlar[player][j];
							*kale_adres[player][j] = *piyonlar[player][j];
							alinan_yol[player][j] = 0;
							*piyonlar[player][j] = " 00 ";
							piyonlar[player][j] = kale_adres[player][j];
							tahtadaki_piyon_sayisi[player]--;
							kale[player]++;
							*path[i] = *piyon;
							return path[i];
						}
					}
				}
			}
			else {
				*path[i] = *piyon;
			}
			return path[i];
		}
	}
	return nullptr;
}

void tahta_dosya_yaz() {
	ofstream dosyaYaz("tahta.txt");

	if (dosyaYaz.is_open()) {

		dosyaYaz << "===========================================" << endl;
		for (int i = 0; i < 11; i++)
		{
			for (int k = 0; k < 11; k++)
			{
				dosyaYaz << tahta[i][k];
			}
			dosyaYaz << "\n" << endl;
		}
		dosyaYaz << "===========================================" << endl;
		dosyaYaz.close();
	}
}

void piyon_adres_duzenle(int k, int zar, int oyuncu) {
	for (int i = 0; i < 4; i++)
	{
		if (*piyonlar[oyuncu][i] == *tahtadaki_piyonlar[oyuncu][k]) {
			*piyonlar[oyuncu][i] = " 00 ";
			piyonlar[oyuncu][i] = tahtadaki_piyonlar[oyuncu][k];
			alinan_yol[oyuncu][i] += zar;

			if (alinan_yol[oyuncu][i] >= 39) {
				*home_adres[oyuncu][i] = *piyonlar[oyuncu][i];
				tahtadaki_piyonlar[oyuncu].erase(tahtadaki_piyonlar[oyuncu].begin()+k);
				*piyonlar[oyuncu][i] = " 00 ";
				piyonlar[oyuncu][i] = home_adres[oyuncu][i];
				home[oyuncu]++;
				tahtadaki_piyon_sayisi[oyuncu]--;
			}
			break;
		}
	}
}


