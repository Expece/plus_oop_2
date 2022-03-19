#include<iostream>
#include<windows.h>
#include<string>
#include<vector>
#include<fstream>
using namespace std;

class Date
{
	int day;
	int month;
	int year;

	friend class Supplier;
};

class Supplier
{
	Date date;
	string firma;		// название фирмы поставщика
	string adr;			// юридический адрес 
public:
	int type;           // 1- поставщик сырья, 2- поставщик оборудования	
	double balance;     // сальдо (положительное - должны поставщику, отрицательное-наоборот)
	static int count;   // счетчик существующих объектов класса
	int ID;
	friend bool searchByName(Supplier& supplier, string* name);
	friend void save(Supplier& supplier, vector <Supplier> arr, int* n);
	friend int load(Supplier& supplier, vector <Supplier>& arr);
	//--------------------------------------
	Supplier()
	{
		count++;
		firma = "";
		adr = "";
		type = 0;
		balance = 0;
		ID = count;
	}
	//--------------------------------------
	void set_info()
	{
		cout << "============== Поставщик №" << ID << " ==============" << endl << endl;
		cout << "Название фирмы поставщика:";
		cin >> firma;
		cout << "Юридический адрес:";
		cin.ignore();
		getline(cin, adr);
		do
		{
			cout << "Тип:";
			cin >> type;
		} while (type != 1 && type != 2);
		cout << "Баланс:";
		cin >> balance;
	}
	//--------------------------------------
	void get_info()
	{
		cout << "============== Поставщик №" << ID << " ==============" << endl << endl;
		cout << "Название фирмы поставщика:" << firma << endl;
		cout << "Юридический адрес:" << adr << endl;
		cout << "Тип:" << type << endl;
		cout << "Баланс:" << balance << endl;
	}
	//--------------------------------------
	bool searchType(int* nomber)
	{
		return (*nomber == type);
	}
	//--------------------------------------
	void balanceSort(int* ID)
	{
		this->ID = *ID + 1;
	}
};
int Supplier::count = 0;
//==========================================
bool searchByName(Supplier& supplier, string* name)
{
	return (supplier.firma == *name);
}
//==========================================
void save(Supplier& supplier, vector <Supplier> arr, int* n)
{
	ofstream file_txt;
	file_txt.open("file.txt");

	for (int i = 0; i < *n; i++)
	{
		file_txt << arr[i].firma << endl;
		file_txt << arr[i].adr << endl;
	}
	file_txt.close();

	ofstream file_bin;
	file_bin.open("file.bin");

	file_bin << *n << endl;
	for (int i = 0; i < *n; i++)
	{
		file_bin << arr[i].type << endl;
		file_bin << arr[i].balance << endl;
	}
	file_bin.close();
}
//==========================================
int load(Supplier& supplier, vector <Supplier>& arr)
{
	ifstream file_bin;
	file_bin.open("file.bin");
	if (!(file_bin.is_open()))
	{
		return 0;
	}
	else
	{
		int size = 0;
		file_bin >> size;
		arr.resize(size);

		for (int i = 0; i < size; i++)
		{
			file_bin >> arr[i].type;
			file_bin >> arr[i].balance;
		}
		file_bin.close();


		ifstream file_txt;
		file_txt.open("file.txt");

		for (int i = 0; i < size; i++)
		{
			file_txt >> arr[i].firma;
			file_txt.ignore();
			getline(file_txt, arr[i].adr);
		}
		file_txt.close();

		return size;
	}
}
//==========================================
int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "ru");
	int n = 1;
	vector <Supplier> arr(n);
	n = load(arr[0], arr);
	if (n != 0)
	{
		cout << "База данных загружена" << endl;
	}
	else
	{
		cout << "Введите кол-во поставщиков:" << endl;
		cin >> n;
		arr.resize(n);
		for (int i = 0; i < n; i++)
		{
			arr[i].set_info();
		}
	}

C:
	int menu = 0;

	cout << "==========================================" << endl << endl;
	cout << "Вывести данные в табличном виде..........1" << endl;
	cout << "Добавить новый элемент в базу............2" << endl;
	cout << "Фильтр по типу...........................3" << endl;
	cout << "Сортировать по уменьшению сальдо.........4" << endl;
	cout << "Поиск поставщика по названию фирмы ......5" << endl;
	cout << "Выйти из программы.......................6" << endl;

	cin >> menu;

	switch (menu)
	{
	case 1:
	{
		int number = 0;
		do
		{
			cout << "Введите номер поставщика (чтобы вывести всех поставщиков введите 0):" << endl;
			cin >> number;
		} while (number > n || number < 0);
		for (int i = 0; i < n; i++)
		{
			if (number == 0)
			{
				arr[i].get_info();
			}
			else
			{
				arr[number - 1].get_info();
				break;
			}
		}
		break;
	}
	case 2: {n++; arr.resize(n); arr[n - 1].set_info(); break; }
	case 3:
	{
		int nomber = 0;
		do
		{
			cout << "Тип:";
			cin >> nomber;
		} while (nomber != 1 && nomber != 2);
		for (int i = 0; i < n; i++)
		{
			if (arr[i].searchType(&nomber))
				arr[i].get_info();
		}
		break;
	}
	case 4:
	{
		for (int i = 0; i < n; i++)
		{

			int SmallestIndex = i;
			for (int j = i + 1; j < n; j++)
			{
				if (arr[SmallestIndex].balance > arr[j].balance)
					SmallestIndex = j;
			}
			arr[i].balanceSort(&SmallestIndex);
			arr[SmallestIndex].balanceSort(&i);
			swap(arr[i], arr[SmallestIndex]);
		}
		cout << "Отсортировано" << endl;
		break;
	}
	case 5:
	{
		cout << "Введите название фирмы:";
		string name = "";
		cin >> name;
		for (int i = 0; i < n; i++)
		{
			if (searchByName(arr[i], &name))
				arr[i].get_info();
		}
		break;
	}
	case 6: {save(arr[0], arr, &n); goto F; }
	}
	goto C;
F:
	return 0;
}