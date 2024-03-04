#include <iostream>
#include <fstream>
#include "Tasks.h"
#include "Examples.h"
#include <string>
using namespace std;

//для task2
const int ROWS = 4;
const int COLUMNS = 16;
const int BYTES_PER_ELEMENT = 2;
char s[ROWS][COLUMNS + 1];
char so[ROWS][COLUMNS];
unsigned short sh_dat[ROWS * COLUMNS];
unsigned short sd2[ROWS * COLUMNS];
char st[9];
string filename_in = "input.txt";
string filename_out = "output.dat";
string filename_bin = "encrypted.dat";



void MenuTask()
{
    setlocale(LC_CTYPE, "Ukr");
    cout << "     Menu Task   \n";
    cout << "    1.  Calculation of expressions using bitwise operations  \n";
    cout << "    2.  Data encryption using bitwise operations \n";
    cout << "    3.  Data encryption using structures with bit fields \n";
    cout << "    4.  The problem of using bitwise operations \n";
    cout << "    5.  Examples of problems using bitwise operations \n";
    cout << "    6.  Exit \n";
}

void task1() {
    int a, b, c, d;
    cout << "Введіть значення a: ";
    cin >> a;
    cout << "Введіть значення b: ";
    cin >> b;
    cout << "Введіть значення c: ";
    cin >> c;
    cout << "Введіть значення d: ";
    cin >> d;
    int result = (1024 * b) + (((d << 4) + (a << 2)) >> 9) - ((c << 12) | (d << 3));
    int result2 = 1024 * b + ((d * 15 + 12 * a) / 512) - 4097 * c + d * 13;
    cout << "Результат(бін): " << result << endl;
    cout << "Результат(звичайні): " << result << endl;
    system("pause");
}

void task2() {
    // ввід інф
    cout << "Введіть текст для шифрування: "<<endl;
    for (int i = 0; i < ROWS; i++) {
        cin.getline(s[i], COLUMNS + 1, '\n');
        for (int j = strlen(s[i]); j < COLUMNS; j++) {
            s[i][j] = ' ';
        }
    }
    // перетв тексту в масив даних
    int j = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int l = 0; l < COLUMNS; l++) {
            unsigned short r = s[i][l] & 0x0F;  // молодша частина аскі коду
            r |= (i << 4) & 0x30;               // номер рядка
            r |= (l << 6) & 0xC0;               // позиція в рядку
            r |= ((s[i][l] >> 4) & 0x0F) << 10; // старша частина аскі коду

            // обчисл біта парності
            unsigned short b1 = 0, b2 = 0;
            for (int k = 0; k < 4; k++) {
                if (r & (1 << k)) {
                    b1++;
                }
                if (r & (1 << (k + 4))) {
                    b2++;
                }
            }
            r |= (b1 % 2) << 14;
            r |= (b2 % 2) << 15;
            sh_dat[j] = r;
            j++;
        }
    }
    // запис даних у бін файл
    ofstream ofsb(filename_bin, ios::out | ios::binary);
    ofsb.write((char*)sh_dat, ROWS * COLUMNS * sizeof(unsigned short));
    ofsb.close();
    // читання даних з бін файлу
    ifstream ifs(filename_bin, ios::in | ios::binary);
    ifs.read((char*)sd2, ROWS * COLUMNS * sizeof(unsigned short));
    // розшифр даних
    for (int i = 0; i < ROWS * COLUMNS; i++) {
        unsigned short r = sd2[i];
        unsigned short b1 = (r >> 14) & 1;
        unsigned short b2 = (r >> 15) & 1;
        unsigned short c1 = r & 0x0F;
        unsigned short c2 = (r >> 10) & 0x0F;
        unsigned short n = (r >> 4) & 0x03;
        unsigned short m = (r >> 6) & 0x0F;
        // перевірка бітів парності
        bool valid = true;
        for (int k = 0; k < 4; k++) {
            if ((c1 & (1 << k)) != b1) {
                valid = false;
                break;
            }
            if ((c2 & (1 << k)) != b2) {
                valid = false;
                break;
            }
        }   
    }
    // вивід розшифр тексту
    for (int i = 0; i < ROWS; i++) {
        for (int l = 0; l < COLUMNS; l++) {
            cout << so[i][l];
        }
        cout << endl;
    }
    // запис розшифр тексту у файл
    ofstream ofst(filename_out);
    for (int i = 0; i < ROWS; i++) {
        ofst << s[i] << endl;
    }
    ofst.close();
    cout << "Дані розшифровано" << endl;
} 



void task3()
{
    
}
// ф-цыя для перевірки правильності передачі
bool checkTransmission(const double data[8], unsigned char crcByte) {
    // Ініціалізація CRC-байту
    unsigned char calculatedCrc = 0;
    // обчисл CRC-байту
    for (int i = 0; i < 8; ++i) {
        // отримання біта для обробки
        bool bit = (data[i] >= 0) ? 1 : 0;
        // додавання біта до CRC-байту
        calculatedCrc |= (bit << i);
    }
    // порівняння обчисленого CRC-байту з переданим
    return calculatedCrc == crcByte;
}
void task4()
{ 
    // введення даних та CRC-байту
    double inputData[8];
    unsigned char receivedCrc;
    cout << "Введіть 8 дійсних чисел:\n";
    for (int i = 0; i < 8; ++i) {
        cout << "a[" << i << "]: ";
        cin >> inputData[i];
    }   
    cout << "Введіть CRC-байт: ";
    cin >> receivedCrc;
    // Перевірка правильності переданої послідовності
    if (checkTransmission(inputData, receivedCrc)) {
        cout << "Помилка у переданій послідовності.\n";
    }
    else {
        cout << "Правильна передана послідовність.\n";
    }
    system("pause");
}


/*
Введіть 8 дійсних чисел:
a[0]: -1.5
a[1]: 2.7
a[2]: -0.8
a[3]: 4.2
a[4]: -3.0
a[5]: 1.6
a[6]: 0.9
a[7]: -2.4
Введіть CRC-байт: 10101010
*/