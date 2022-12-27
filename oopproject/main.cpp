#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <random>
using namespace std;

class Time
{
private:
    int hour;
    int minute;

public:
    Time(int hour = 0, int minute = 0)
    {
        this->hour = hour;
        this->minute = minute;
    };
    ~Time();
    bool operator==(const Time &other) const
    {
        return hour == other.hour && minute == other.minute;
    }

    bool operator<(const Time &other) const
    {
        if (hour != other.hour)
        {
            return hour < other.hour;
        }
        return minute < other.minute;
    }
    Time operator+(const Time &other) const
    {
        int total_minutes = (hour * 60 + minute) + (other.hour * 60 + other.minute);
        int hours = total_minutes / 60;
        int minutes = total_minutes % 60;
        return Time(hours, minutes);
    }
    bool isCloser(Time other)
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        int currentHour = ltm->tm_hour;
        int currentMinute = ltm->tm_min;

        int distance1 = (hour - currentHour) * 60 + (minute - currentMinute);
        int distance2 = (other.hour - currentHour) * 60 + (other.minute - currentMinute);

        if (distance1 < 0)
            distance1 *= -1;
        if (distance2 < 0)
            distance2 *= -1;

        return distance1 < distance2;
    }
    Time nearest(const vector<Time> times)
    {
        Time nearest_time(0, 0);
        int min_difference = INT_MAX;
        for (int i = 0; i < times.size(); i++)
        {
            Time t = times[i];
            int diff = abs(t.hour - hour) * 60 + abs(t.minute - minute);
            if (diff < min_difference)
            {
                nearest_time = t;
                min_difference = diff;
            }
        }
        return nearest_time;
    }
    int getHour();
    int getMinute();
};

class Person
{
private:
    string namesurname;
    string phone;

public:
    Person(string namesurname, string phone);
    ~Person();
    string getNamesurname();
    string getPhone();
};

class Admin : public Person
{
private:
    string password;
    string passwordline;

public:
    Admin(string namesurname, string phone, string password, string passwordline);
    ~Admin();
    string getPassword();
    void changePassword();
    void addClothes();
    void addCourier();
    void enterCustomerDiscount();
    void readSuggestions();
    void readBills();
    void printAdmin();
};

class Customer : public Person
{
private:
    string username;
    string email;
    string district;
    string discount;
    string birthdate;
    string password;
    string passwordline;

public:
    Customer(string namesurname, string phone, string username, string email, string district, string discount, string birthdate, string password, string passwordline);
    ~Customer();
    string getUsername();
    string getEmail();
    string getDistrict();
    string getDiscount();
    string getBirthdate();
    string getPassword();
    void changePassword();
    void saveInvoice();
    void enterSuggestions();
    void printCustomer();
};

class Courier : public Person
{
private:
    Time distributionFinish;
    int numbers;

public:
    Courier(string namesurname, string phone, Time distributionFinish, int numbers) : Person(namesurname, phone)
    {
        this->distributionFinish = distributionFinish;
        this->numbers = numbers;
    };
    ~Courier();
    Time getDistributionFinish();
    int getNumbers();
    void printCourier();
};

class Clothes
{
private:
    string category;
    int clothesID;
    double price;
    string size;
    string color;

public:
    Clothes(string category, int clothesID, double price, string size, string color);
    ~Clothes();
    string getCategory();
    int getClothesID();
    double getPrice();
    string getSize();
    string getColor();
    void printClothes();
};

class Order : public Clothes
{
private:
    int orderID;
    double orderPrice;
    Time *orderTime;
    Time *deliveryTime;

public:
    Order(string category, int clothesID, double price, string size, string color, int orderID, double orderPrice, Time orderTime, Time deliveryTime) : Clothes(category, clothesID, price, size, color)
    {
        this->orderID = orderID;
        this->orderPrice = orderPrice;
        this->orderTime = &orderTime;
        this->deliveryTime = &deliveryTime;
    };
    ~Order();
    int getOrderID();
    double getOrderPrice();
    Time getOrderTime();
    Time getDeliveryTime();
    void printOrder();
};

class Invoice : public Order
{
private:
    int invoiceID;
    Time *invoiceTime;
};

void clearMenu();
void adminRegister();
void firstMenu();
void registerMenu();
void loginMenu();
void adminLogin();
void adminMenu();
void customerRegister();
void customerLogin();
void customerMenu();
void readOrders();
int password_prompt(string &password);
bool isChar(char c);
bool isDigit(const char c);
bool is_valid(string email);
int getRandomNumber();
void placeOrder();
int generate_random_number();

bool isAdminLoggedIn = false;
bool isCustomerLoggedIn = false;
Admin *admin;
Customer *customer;
vector<Clothes> clothesList;

int main(void)
{
    clearMenu();
    firstMenu();
    return 0;
}

void clearMenu()
{
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
#ifdef __linux__
    system("clear");
#endif
#ifdef __APPLE__
    system("clear");
#endif
}

void adminRegister()
{
    ifstream dosya("kullanicilar.txt");
    int sayac = 0;
    if (dosya.is_open())
    {
        string satir;
        while (getline(dosya, satir))
        {
            sayac++;
        }
        cout << "Dosyada " << sayac << " adet satır var." << endl;
        dosya.close();
    }
    else
    {
        cerr << "Dosya açılamadı!" << endl;
    }
    printf("Yonetici kaydi\n\n");
    printf("İsim Soyisim: ");
    string namesurname;
    cin >> namesurname;
    cout << "Telefon numarasi: ";
    string phone;
    cin >> phone;
    string password;
    password_prompt(password);
    cout << "Kayit basarili!\n";
    ofstream kullanicilar;
    kullanicilar.open("kullanicilar.txt", ios_base::app);
    kullanicilar << "admin:" << namesurname << endl;
    kullanicilar << phone << endl;
    kullanicilar << password << endl;
    kullanicilar << sayac + 3 << endl;
    kullanicilar.close();
    cout << endl;
    clearMenu();
    cout << "Admin girisi\n\n";
    adminLogin();
}

void firstMenu()
{
    cout << "1. Sisteme giris\n";
    cout << "2. Uye kaydi\n\n";
    cout << "3. Cikis\n";
    int choice;
    cout << "Seciminiz: ";
    cin >> choice;
    switch (choice)
    {
    case 1:
        clearMenu();
        loginMenu();
        break;
    case 2:
        clearMenu();
        registerMenu();
        break;
    case 3:
        clearMenu();
        cout << "Cikis yapiliyor...\n";
        exit(0);
        break;
    default:
        clearMenu();
        cout << "Hatali secim!\n";
        firstMenu();
        break;
    }
}

void registerMenu()
{
    cout << "1. Yonetici kaydi\n";
    cout << "2. Musteri kaydi\n\n";
    cout << "3. Geri don\n";
    int choice;
    cout << "Seciminiz: ";
    cin >> choice;
    switch (choice)
    {
    case 1:
        clearMenu();
        adminRegister();
        break;
    case 2:
        clearMenu();
        customerRegister();
        break;
    case 3:
        clearMenu();
        firstMenu();
        break;
    }
}

void loginMenu()
{
    cout << "1. Yonetici girisi\n";
    cout << "2. Musteri girisi\n\n";
    cout << "3. Geri don\n";
    int choice;
    cout << "Seciminiz: ";
    cin >> choice;
    switch (choice)
    {
    case 1:
        clearMenu();
        adminLogin();
        break;
    case 2:
        clearMenu();
        customerLogin();
        break;
    case 3:
        clearMenu();
        firstMenu();
        break;
    }
}

void adminLogin()
{
    cout << "Kullanici adi: ";
    string username;
    cin >> username;
    ifstream kullanicilar;
    kullanicilar.open("kullanicilar.txt");
    string line;
    vector<string> lines;
    for (int i = 0; getline(kullanicilar, line); i++)
    {
        lines.push_back(line);
    }
    kullanicilar.close();
    for (int i = 0; lines.size(); i++)
    {
        if (lines[i] == "admin:" + username)
        {
            string password;
            password_prompt(password);
            if (lines[i + 2] == password)
            {
                clearMenu();
                cout << "\nGiris basarili!\n";
                isAdminLoggedIn = true;
                isCustomerLoggedIn = false;
                Admin yonetici(lines[i], lines[i + 1], lines[i + 2], lines[i + 3]);
                admin = &yonetici;
                admin->printAdmin();
                cout << endl;
                adminMenu();
                break;
            }
            else
            {
                clearMenu();
                cout << "Sifre hatali!\n\n";
                adminLogin();
                break;
            }
        }
    }

    clearMenu();
    cout << "Admin bulunamadi!\n\n";
    adminLogin();
}

void adminMenu()
{

    cout << "1. Kiyafet kategorilerinin yer aldigi dosyalara urun girisi yap\n";
    cout << "2. Sisteme kurye ekle\n";
    cout << "3. Sikayet ve onerileri oku\n";
    cout << "4. Kullaniciya indirim kodu tanimlamasi yap\n";
    cout << "5. Yapilan siparislerin faturalarini goruntule\n\n";
    cout << "9. Şifre değiştir\n\n";

    cout << "0. Cikis\n\n";
    int choice;
    cout << "Seciminiz: ";
    cin >> choice;
    switch (choice)
    {
    case 1:
        clearMenu();
        admin->addClothes();
        clearMenu();
        cout << "Urun girisi basarili!\n\n";
        adminMenu();
        break;
    case 2:
        clearMenu();
        admin->addCourier();
        clearMenu();
        cout << "Kurye ekleme basarili!\n\n";
        adminMenu();
        break;
    case 3:
        clearMenu();
        admin->readSuggestions();
        break;
    case 4:
        clearMenu();
        admin->enterCustomerDiscount();
        break;
    case 5:
        clearMenu();
        admin->readBills();
        break;
    case 0:
        clearMenu();
        isAdminLoggedIn = false;
        isCustomerLoggedIn = false;
        admin->~Admin();
        firstMenu();
        break;
    case 9:
        clearMenu();
        admin->changePassword();
        clearMenu();
        cout << "Sifre degistirme basarili!\n\n";
        adminMenu();
        break;
    }
}

int password_prompt(string &password)
{
    struct termios tty;
    if (tcgetattr(STDIN_FILENO, &tty) != 0)
        return -1;
    tty.c_lflag &= ~ECHO;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) != 0)
        return -1;
    cout << "Sifre: ";
    cin >> password;
    tty.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
    return 0;
}

void customerRegister()
{
    ifstream ikullanicilar;
    ikullanicilar.open("kullanicilar.txt");
    int sayac = 0;
    string satir;
    vector<string> lines;
    if (ikullanicilar.is_open())
    {
        while (getline(ikullanicilar, satir))
        {
            sayac++;
            lines.push_back(satir);
        }
    }
    else
    {
        cerr << "Dosya açilamadi!" << endl;
    }
    ikullanicilar.close();
    cout << "Musteri kaydi\n\n";
    cout << "Isim Soyisim: ";
    string namesurname;
    cin >> namesurname;
    int i = 0;
    while (i < lines.size())
    {
        if (lines[i] == "customer:" + namesurname)
        {
            clearMenu();
            cout << "Bu kullanici adi kullanilmaktadir!\nTekrar kayit olunuz\n\n";
            customerRegister();
            break;
        }
        i++;
    }
    cout << "Ilceyi asagidaki listeden kopyalayip yapistiriniz:\n\n";
    cout << "ortahisar" << endl;
    cout << "akçaabat" << endl;
    cout << "vakfikebir" << endl;
    cout << "besikduzu" << endl;
    cout << "yomra" << endl;
    cout << "arsin" << endl;
    cout << "arakli\n\n\n";
    cout << "Ilce: ";
    string district;
    cin >> district;
    bool isDistrictValid = false;
    if (district == "ortahisar")
    {
        isDistrictValid = true;
    }

    else if (district == "akçaabat")

    {
        isDistrictValid = true;
    }
    else if (district == "vakfikebir")
    {
        isDistrictValid = true;
    }
    else if (district == "besikduzu")
    {
        isDistrictValid = true;
    }
    else if (district == "yomra")
    {
        isDistrictValid = true;
    }
    else if (district == "arsin")
    {
        isDistrictValid = true;
    }
    else if (district == "arakli")
    {
        isDistrictValid = true;
    }
    else
    {
        isDistrictValid = false;
    }
    if (isDistrictValid == false)
    {
        clearMenu();
        cout << "Hatali ilce!\nTekrar kayit olunuz\n\n";
        customerRegister();
    }
    cout << "E-mail adresi: ";
    string email;
    cin >> email;
    email.erase(remove(email.begin(), email.end(), ' '), email.end());
    bool ans = is_valid(email);
    if (ans == false)
    {
        clearMenu();
        cout << "Hatali e-mail adresi!\nTekrar kayit olunuz\n\n";
        customerRegister();
    }
    for (int i = 0; i < lines.size(); i++)
    {
        if (lines[i] == email)
        {
            clearMenu();
            cout << "E-mail adresi kullanilmaktadir!\nTekrar kayit olunuz\n\n";
            customerRegister();
            break;
        }
    }
    cout << "Telefon numarasi: ";
    string phone;
    cin >> phone;
    cout << "Kullanici Adi: ";
    string username;
    cin >> username;

    cout << "Indirim kodu: ";
    string discount;
    cin >> discount;
    cout << "Dogum tarihi: ";
    string birthdate;
    cin >> birthdate;
    string password;
    password_prompt(password);
    cout << "Kayit basarili!\n";
    ofstream okullanicilar;
    okullanicilar.open("kullanicilar.txt", ios_base::app);
    okullanicilar << "customer:" << namesurname << endl;
    okullanicilar << phone << endl;
    okullanicilar << username << endl;
    okullanicilar << email << endl;
    okullanicilar << district << endl;
    okullanicilar << discount << endl;
    okullanicilar << birthdate << endl;
    okullanicilar << password << endl;
    okullanicilar << sayac + 8 << endl;
    okullanicilar.close();
    cout << endl;
    clearMenu();
    customerLogin();
}

bool isChar(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

bool isDigit(const char c)
{
    return (c >= '0' && c <= '9');
}

bool is_valid(string email)
{
    if (!isChar(email[0]))
    {
        return 0;
    }
    int At = -1, Dot = -1;
    for (int i = 0;
         i < email.length(); i++)
    {
        if (email[i] == '@')
        {
            At = i;
        }
        else if (email[i] == '.')
        {
            Dot = i;
        }
    }
    if (At == -1 || Dot == -1)
        return 0;
    if (At > Dot)
        return 0;
    return !(Dot >= (email.length() - 1));
}

void customerLogin()
{
    cout << "Musteri girisi\n\n";
    cout << "Isim Soyisim: ";
    string namesurname;
    cin >> namesurname;
    ifstream kullanicilar;
    kullanicilar.open("kullanicilar.txt");
    string line;
    vector<string> lines;
    while (getline(kullanicilar, line))
    {
        lines.push_back(line);
    }
    kullanicilar.close();
    for (int i = 0; i < lines.size(); i++)
    {
        if (lines[i] == "customer:" + namesurname)
        {
            string password;
            password_prompt(password);
            if (lines[i + 7] == password)
            {
                clearMenu();
                cout << "Giris basarili!\n\n";
                isCustomerLoggedIn = true;
                isAdminLoggedIn = false;
                Customer musteri(lines[i], lines[i + 1], lines[i + 2], lines[i + 3], lines[i + 4], lines[i + 5], lines[i + 6], lines[i + 7], lines[i + 8]);
                customer = &musteri;
                customerMenu();
                break;
            }
            else
            {
                clearMenu();
                cout << "Sifre hatali!\n\n";
                customerLogin();
                break;
            }
            break;
        }
    }
    if (isCustomerLoggedIn == true)
        return;
    clearMenu();
    cout << "Kullanici adi hatali!\n\n";
    customerLogin();
}

void customerMenu()
{
    cout << "1. Siparis ver\n";
    cout << "2. Siparislerim\n";
    cout << "3. Profilim\n";
    cout << "4. Sikayet/Oneri Gir\n\n";
    cout << "9. Şifre değiştir\n\n";
    cout << "0. Cikis\n\n";
    cout << "Secim: ";
    int choice;
    cin >> choice;
    switch (choice)
    {
    case 1:
        clearMenu();
        placeOrder();
        break;
    case 2:
        clearMenu();
        readOrders();
        break;
    case 3:
        clearMenu();
        customer->printCustomer();
        cout << endl;
        customerMenu();
        break;
    case 4:
        clearMenu();
        customer->enterSuggestions();
        break;
    case 9:
        clearMenu();
        customer->changePassword();
        customerMenu();
        break;
    case 0:
        clearMenu();
        isCustomerLoggedIn = false;
        customer->~Customer();
        firstMenu();
        break;
    }
}

void readOrders()
{
    string customerFromTXT = customer->getNamesurname();
    string customerID = customerFromTXT.substr(9, customerFromTXT.length());
    ifstream siparisler;
    siparisler.open("faturalar.txt");
    string line;
    vector<string> lines;
    while (getline(siparisler, line))
    {
        lines.push_back(line);
    }
    siparisler.close();
    for (int i = 0; i < lines.size(); i++)
    {
        if (lines[i].size() > 14)
        {
            string customerIDFromTXT = lines[i].substr(14, lines[i].length());
            if (customerIDFromTXT == customerID)
            {
                cout << lines[i] << endl;
                cout << lines[i + 1] << endl;
                cout << lines[i + 2] << endl;
                cout << lines[i + 3] << endl;
                cout << lines[i + 4] << endl;
                cout << lines[i + 5] << endl;
                cout << lines[i + 6] << endl;
                cout << lines[i + 7] << endl;
                cout << lines[i + 8] << endl;
            }
        }
    }
    cout << "\n\nGeri donmek icin 0'a basiniz: ";
    int choice;
    cin >> choice;
    if (choice == 0)
    {
        clearMenu();
        customerMenu();
    }
    else
    {
        clearMenu();
        cout << "Hatali secim!\n\n";
        readOrders();
    }
}

int getRandomNumber()
{
    srand(time(0));
    return rand() % 1000;
}

void placeOrder()
{

    ifstream kiyafetler("kiyafetler.txt");
    string line;
    vector<string> lines;
    while (getline(kiyafetler, line))
    {
        lines.push_back(line);
    }
    kiyafetler.close();
    cout << "Kiyafet bilgileri:\n\n";
    printf("%-5s%-15s%-7s%-7s%-10s%-12s%-20s\n\n", "ID", "Kategori", "Fiyat", "Boyut", "Renk", "Kalan Adet", "Urun Basligi");
    for (int i = 0; i < lines.size(); i++)
    {
        string *a = NULL;
        a = &lines[i];
        if (a->substr(0, 7) == "clothe:")
        {
            int num = int(lines[i][7]);
            string clotheCategory;
            switch (num - 48)
            {
            case 1:
                clotheCategory = "Erkek Tisort";
                break;
            case 2:
                clotheCategory = "Erkek Pantolon";
                break;
            case 3:
                clotheCategory = "Erkek Ceket";
                break;
            case 4:
                clotheCategory = "Erkek Ayakkabi";
                break;
            case 5:
                clotheCategory = "Kadin Tisort";
                break;
            case 6:
                clotheCategory = "Kadin Pantolon";
                break;
            case 7:
                clotheCategory = "Kadin Ceket";
                break;
            case 8:
                clotheCategory = "Kadin Ayakkabi";
                break;
            default:
                clotheCategory = "Kategori bulunamadi";
                break;
            };
            printf("%-5s%-15s%-7s%-7s%-10s%-12s%-20s\n", lines[i].substr(7, lines[i].length()).c_str(), clotheCategory.c_str(), lines[i + 2].c_str(), lines[i + 3].c_str(), lines[i + 4].c_str(), lines[i + 5].c_str(), lines[i + 1].c_str());
        }
    }
    cout << "\n0. Sepete git\n";
    cout << "9. Ana menuye don\n\n";
    cout << "\nSiparis vermek istediginiz kiyafetin ID'sini girin: ";
    string clotheID;
    do
    {
        string ID;
        cin >> ID;
        clotheID = ID;
        for (int i = 0; i < lines.size(); i++)
        {
            string *a = NULL;
            a = &lines[i];
            if (a->substr(0, 7) == "clothe:" && a->substr(7, a->length()) == clotheID && clotheID != "0")
            {
                Clothes kiyafet(lines[i], stoi(clotheID), stod(lines[i + 2]), lines[i + 3], lines[i + 4]);
                clothesList.push_back(kiyafet);
                clearMenu();
                cout << "Kiyafet basariyla sepete eklendi!\n\n";
                cout << "Sepetinizdeki kiyafet sayisi: " << clothesList.size() << endl
                     << endl;
                placeOrder();
                break;
            }
        }
    } while (clotheID != "0" && clotheID != "9");
    if (clotheID == "0")
    {
        if (clothesList.size() == 0)
        {
            clearMenu();
            cout << "Sepetiniz bos!\n\n";
            placeOrder();
            return;
        }
        clearMenu();
        cout << "Sepetiniz\n\n";
        customer->saveInvoice();
        return;
    }
    if (clotheID == "9")
    {
        clearMenu();
        customerMenu();
        return;
    }
}

int generate_random_number()
{
    // sayı üreteci oluşturun
    std::random_device rd;
    std::mt19937 gen(rd());

    // sayı aralığını belirleyin (1 ile 999999 arasında sayılar üretecek)
    std::uniform_int_distribution<> dis(1, 999999);

    // rastgele sayı üretin
    int random_number = dis(gen);

    return random_number;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Courier::~Courier()
{
}

Time Courier::getDistributionFinish()
{
    return distributionFinish;
}

int Courier::getNumbers()
{
    return numbers;
}

void Courier::printCourier()
{
    cout << "Kurye bilgileri\n\n";
    cout << "Isim Soyisim: " << getNamesurname() << endl;
    cout << "Telefon: " << getPhone() << endl;
    Time a = getDistributionFinish();
    cout << "Teslimat bitis saati: " << a.getHour() << ":" << a.getMinute() << endl;
    getNumbers();
    cout << "Kurye ID: " << getNumbers() << endl;
}

Time::~Time()
{
}

int Time::getHour()
{
    return hour;
}

int Time::getMinute()
{
    return minute;
}

Person::Person(string namesurname, string phone)
{
    this->namesurname = namesurname;
    this->phone = phone;
}

Person::~Person()
{
}

string Person::getNamesurname()
{
    return namesurname;
}

string Person::getPhone()
{
    return phone;
}

Admin::Admin(string namesurname, string phone, string password, string passwordline) : Person(namesurname, phone)
{
    this->password = password;
    this->passwordline = passwordline;
}

Admin::~Admin()
{
    cout << "Admin destructor\n";
}

string Admin::getPassword()
{
    return password;
}

void Admin::readBills()
{
    ifstream file("faturalar.txt");
    string line;
    while (getline(file, line))
    {
        cout << line << endl;
    }
    file.close();
    for (int i = 0; i < 2; i++)
    {
        cout << endl;
    }
    cout << "0. Ana menuye don\n\n";
    cout << "Seciminiz: ";
    string choice;
    cin >> choice;
    if (choice == "0")
    {
        clearMenu();
        adminMenu();
    }
}

void Admin::changePassword()
{
    int lineNumber = stoi(admin->passwordline);
    cout << "Yeni ";
    string newpassword;
    password_prompt(newpassword);
    string newLine = newpassword;
    ifstream inputFile("kullanicilar.txt");
    string line;
    int currentLine = 1;
    ofstream tempFile("temp.txt");
    while (getline(inputFile, line))
    {
        if (currentLine == lineNumber)
        {
            tempFile << newLine << endl;
        }
        else
        {
            tempFile << line << endl;
        }
        currentLine++;
    }
    inputFile.close();
    tempFile.close();
    remove("kullanicilar.txt");
    rename("temp.txt", "kullanicilar.txt");
    clearMenu();
    cout << "Sifre basariyla degistirildi!\n\n";
}

void Admin::addClothes()
{
    cout << "Kiyafet girisi\n\n";
    cout << "Girebileceginiz Kiyafet kategorileri: \n";
    cout << "1. Erkek Tisort\n";
    cout << "2. Erkek Pantolon\n";
    cout << "3. Erkek Ceket\n";
    cout << "4. Erkek Ayakkabi\n";
    cout << "5. Kadin Tisort\n";
    cout << "6. Kadin Pantolon\n";
    cout << "7. Kadin Ceket\n";
    cout << "8. Kadin Ayakkabi\n\n";
    string trashInput;
    getline(cin, trashInput);
    cout << "Secim: ";
    string choice;
    getline(cin, choice);
    stringstream ss(choice);
    int num;
    ss >> num;
    string category;
    switch (num)
    {
    case 1:
        category = "Erkek Tisort";
        break;
    case 2:
        category = "Erkek Pantolon";
        break;
    case 3:
        category = "Erkek Ceket";
        break;
    case 4:
        category = "Erkek Ayakkabi";
        break;
    case 5:
        category = "Kadin Tisort";
        break;
    case 6:
        category = "Kadin Pantolon";
        break;
    case 7:
        category = "Kadin Ceket";
        break;
    case 8:
        category = "Kadin Ayakkabi";
        break;
    };
    cout << "Kategori: " << category << endl;
    cout << "Kiyafet adi: ";
    string id;
    getline(cin, id);
    cout << "Kiyafet ucreti: ";
    string price;
    getline(cin, price);
    cout << "Kiyafet boyu: ";
    string size;
    getline(cin, size);
    cout << "Kiyafet rengi: ";
    string color;
    getline(cin, color);
    cout << "Adet: ";
    string amount;
    getline(cin, amount);
    int randomNumber = getRandomNumber();
    ofstream kiyafetler;
    kiyafetler.open("kiyafetler.txt", ios_base::app);
    kiyafetler << "clothe:" << choice << to_string(randomNumber) << endl;
    kiyafetler << id << endl;
    kiyafetler << price << endl;
    kiyafetler << size << endl;
    kiyafetler << color << endl;
    kiyafetler << amount << endl;
    kiyafetler.close();
}

void Admin::addCourier()
{
    clearMenu();
    ifstream ikuryeler;
    ikuryeler.open("kuryeler.txt");
    string line;
    vector<string> lines;
    while (getline(ikuryeler, line))
    {
        lines.push_back(line);
    }
    ikuryeler.close();
    printf("%-10s%-25s%-15s%-10s\n\n", "ID", "Isim Soyisim", "Telefon", "Saat : Dakika");
    for (int i = 0; i < lines.size(); i += 5)
    {
        int hour = stoi(lines[i + 2]);
        int minute = stoi(lines[i + 3]);
        Time setTime(hour, minute);
        int setNumber = stoi(lines[i + 4]);
        Courier kurye(lines[i], lines[i + 1], setTime, setNumber);
        int getNumber = kurye.getNumbers();
        Time getTime = kurye.getDistributionFinish();
        printf("%-10d%-25s%-15s%02d:%02d\n", getNumber, lines[i].c_str(), lines[i + 1].c_str(), getTime.getHour(), getTime.getMinute());
    }
    cout << "Kurye ekleme\n\n";
    cout << "Isim Soyisim: ";
    string namesurname;
    cin >> namesurname;
    cout << "Telefon: ";
    string phone;
    cin >> phone;
    int randomID = getRandomNumber();
    ofstream okuryeler;
    okuryeler.open("kuryeler.txt", ios_base::app);
    okuryeler << "courier:" << namesurname << endl;
    okuryeler << phone << endl;
    okuryeler << "08" << endl;
    okuryeler << "00" << endl;
    okuryeler << randomID << endl;
    okuryeler.close();
    cout << endl;
    clearMenu();
    cout << "ID: " << randomID << endl;
    cout << "Isim Soyisim: " << namesurname << endl;
    cout << "Telefon: " << phone << endl;
    cout << "Teslimat bitis saati: 08:00 olarak ayarlandi.\n";
    cout << "Kurye eklendi!\n\n";
    adminMenu();
}

void Admin::readSuggestions()
{
    ifstream suggestions;
    suggestions.open("sikayetoneri.txt");
    string line;
    vector<string> lines;
    if (!suggestions.is_open())
    {
        cerr << "Dosya acilamadi!\n\n";
        cout << "Ana menuye donmek icin 0'a basiniz: ";
        string choice;
        cin >> choice;
        if (choice == "0")
        {
            clearMenu();
            adminMenu();
        }
    }
    while (getline(suggestions, line))
    {
        lines.push_back(line);
    }
    suggestions.close();
    for (int i = 0; i < lines.size(); i++)
    {
        cout << lines[i] << endl;
    }
    cout << "\n\n\nAna menuye donmek icin 0'a basiniz: ";
    string choice;
    cin >> choice;
    if (choice == "0")
    {
        clearMenu();
        adminMenu();
    }
    else
    {
        clearMenu();
        readSuggestions();
    }
}

void Admin::enterCustomerDiscount()
{
    clearMenu();
    ifstream icustomers;
    icustomers.open("kullanicilar.txt");
    string line;
    vector<string> lines;
    while (getline(icustomers, line))
    {
        lines.push_back(line);
    }
    icustomers.close();
    printf("%-30s%-25s%-15s%-10s\n\n", "E-Mail", "Isim Soyisim", "Telefon", "Indirim");
    for (int i = 0; i < lines.size(); i++)
    {
        if (lines[i].length() <= 9)
            continue;
        if (lines[i].substr(0, 9) == "customer:")
        {
            int discount;
            if (lines[i + 5] == "-")
            {
                discount = 0;
            }
            else
            {
                string s = lines[i + 5];
                int n = s.length();
                int num;
                stringstream(s.substr(n - 2)) >> num;
                discount = num;
            }
            printf("%-30s%-25s%-15s%-10d\n", lines[i + 3].c_str(), lines[i].substr(9, lines[i].length() - 1).c_str(), lines[i + 1].c_str(), discount);
        }
    }
    cout << "\n\nMusteri indirimi guncelleme\n\n";
    cout << "Musteri E-Mail: ";
    string email;
    cin >> email;
    cout << "Yeni indirim kodu: ";
    string discount;
    cin >> discount;
    string trashInput;
    getline(cin, trashInput);
    ifstream icustomers2;
    icustomers2.open("kullanicilar.txt");
    string line2;
    vector<string> lines2;
    while (getline(icustomers2, line2))
    {
        lines2.push_back(line2);
    }
    icustomers2.close();
    for (int i = 0; i < lines2.size(); i++)
    {
        if (lines2[i].substr(0, 9) == "customer:")
        {
            if (lines2[i + 3] == email)
            {
                int lineNumber = i + 6;
                string newDiscount = "discount:" + discount;
                string newLine = newDiscount;
                ifstream inputFile("kullanicilar.txt");
                string line;
                int currentLine = 1;
                ofstream tempFile("temp.txt");
                while (getline(inputFile, line))
                {
                    if (currentLine == lineNumber)
                    {
                        tempFile << newLine << endl;
                    }
                    else
                    {
                        tempFile << line << endl;
                    }
                    currentLine++;
                }
                inputFile.close();
                tempFile.close();
                remove("kullanicilar.txt");
                rename("temp.txt", "kullanicilar.txt");
                clearMenu();
                cout << "Indirim kuponu basariyla degistirildi!\n\n";
                adminMenu();
            }
        }
    }
}

void Admin::printAdmin()
{
    cout << "\n----------------------------------------\n\n";
    cout << "Admin bilgileri:\n";
    cout << "Isim Soyisim: " << getNamesurname() << endl;
    cout << "Telefon numarasi: " << getPhone() << endl;
    cout << "Sifre: " << getPassword() << endl;
}

Customer::Customer(string namesurname, string phone, string username, string email, string district, string discount, string birthdate, string password, string passwordline) : Person(namesurname, phone)
{
    this->username = username;
    this->email = email;
    this->district = district;
    this->discount = discount;
    this->birthdate = birthdate;
    this->password = password;
    this->passwordline = passwordline;
}

Customer::~Customer()
{
}

string Customer::getUsername()
{
    return username;
}

string Customer::getEmail()
{
    return email;
}

string Customer::getDistrict()
{
    return district;
}

string Customer::getDiscount()
{
    return discount;
}

string Customer::getBirthdate()
{
    return birthdate;
}

string Customer::getPassword()
{
    return password;
}

void Customer::changePassword()
{
    int lineNumber = stoi(customer->passwordline);
    cout << "Yeni ";
    string newpassword;
    password_prompt(newpassword);
    string newLine = newpassword;
    ifstream inputFile("kullanicilar.txt");
    string line;
    int currentLine = 1;
    ofstream tempFile("temp.txt");
    while (getline(inputFile, line))
    {
        if (currentLine == lineNumber)
        {
            tempFile << newLine << endl;
        }
        else
        {
            tempFile << line << endl;
        }
        currentLine++;
    }
    inputFile.close();
    tempFile.close();
    remove("kullanicilar.txt");
    rename("temp.txt", "kullanicilar.txt");
    clearMenu();
    cout << "Sifre başariyla degistirildi!" << endl;
}

void Customer::enterSuggestions()
{
    cout << "Emailiniz: " << email << endl
         << endl;
    string trashInput;
    getline(cin, trashInput);
    cout << "Sikayet ve önerileriniz: ";
    string suggestion;
    getline(cin, suggestion);
    ofstream suggestions;
    suggestions.open("sikayetoneri.txt", ios::app);
    suggestions << "Musteri Email: " << email << endl
                << endl;
    suggestions << suggestion << endl
                << "----------------------------------------" << endl;
    suggestions.close();
    cout << "Sikayet ve öneriniz basariyla alindi!\n\n";
    cout << "Ana menuye donmek icin enter'a basiniz...";
    cin >> trashInput;
    clearMenu();
    customerMenu();
}

void Customer::saveInvoice()
{
    printf("%-7s%-20s%-7s%-7s%-10s\n\n", "ID", "Kategori", "Fiyat", "Boyut", "Renk");
    for (int i = 0; i < clothesList.size(); i++)
    {
        clothesList[i].printClothes();
    }
    cout << "\nSepetinizdeki kiyafet sayisi: " << clothesList.size() << "\n\n";
    cout << "Siparişe devam etmek için 1'e,\nSiparişi tamamlamak için 0'a basiniz: ";
    int choice;
    cin >> choice;
    if (choice == 1)
    {
        clearMenu();
        cout << "Kiyafet basariyla sepete eklendi!\n\n";
        cout << "Sepetinizdeki kiyafet sayisi: " << clothesList.size() << endl
             << endl;
        placeOrder();
    }
    // sipariş tamamlandı kuryeye süre eklenecek ve fatura yazdırılacak
    else if (choice == 0)
    {
        string disc = customer->getDiscount();
        if (disc.length() > 3)
        {
            string discount = disc.substr(disc.size() - 2);
        }
        else
        {
            string discount = "0";
        }
        int discountInt;
        // indirim tutarı alındı
        try
        {
            discountInt = stoi(discount);
        }
        catch (const exception &e)
        {
            discountInt = 0;
        }
        double totalPrice = 0;
        // fstream file("kullanicilar.txt", ios::in | ios::out | ios::ate);
        // indirim kodu kullanıldığı için sıfırlandı
        ifstream ifile("kullanicilar.txt", ios_base::in);
        string line2;
        vector<string> lines2;
        if (ifile.is_open())
        {
            while (getline(ifile, line2))
            {
                lines2.push_back(line2);
            }
        }
        ifile.close();
        // ofstream ofile("kullanicilar.txt", ios_base::out);
        ofstream tempFile("tempFile.txt", ios_base::out);
        for (int i = 0; i < lines2.size(); i++)
        {
            if (lines2[i] == customer->getDiscount())
            {
                lines2[i] = "-";
            }
            else
            {
                continue;
            }
        }
        for (int i = 0; i < lines2.size(); i++)
        {
            tempFile << lines2[i] << endl;
        }
        // ofile.close();
        tempFile.close();
        remove("kullanicilar.txt");
        rename("tempFile.txt", "kullanicilar.txt");
        // if (file.is_open())
        // {
        //     string line;
        //     int lineNumber = 0;
        //     while (getline(file, line))
        //     {
        //         lineNumber++;
        //         if (lineNumber == (stoi(customer->passwordline) - 2))
        //         {
        //             line = "-";
        //             file.seekp(file.tellg());
        //             file << line << endl;
        //         }
        //     }
        // }
        // file.close();
        // aldığı kıyafetlerin ücretleri toplandı
        for (int i = 0; i < clothesList.size(); i++)
        {
            totalPrice = clothesList[i].getPrice() + totalPrice;
        }
        // kuryeler.txt dosyasındaki satırlar çekildi
        ifstream inputFile("kuryeler.txt");
        string line;
        vector<string> lines;
        while (getline(inputFile, line))
        {
            lines.push_back(line);
        }
        inputFile.close();
        vector<Courier> courierList;
        vector<Time> courierDistributionFinishHourList;
        // kuryeler ve kurye dağıtım bitiş saatleri vektörlere eklendi
        for (int i = 0; i < lines.size(); i++)
        {
            if (lines[i].substr(0, 8) == "courier:")
            {
                string courierName = lines[i].substr(8, lines[i].length());
                string courierPhone = lines[i + 1];
                // printf("\nvektore eklenen bitis saati: %2d, dakikasi: %2d", stoi(lines[i + 2]), stoi(lines[i + 3]));
                int courierCode = stoi(lines[i + 4]);
                Courier courier(courierName, courierPhone, Time(stoi(lines[i + 2]), stoi(lines[i + 3])), courierCode);
                courierDistributionFinishHourList.push_back(Time(stoi(lines[i + 2]), stoi(lines[i + 3])));
                courierList.push_back(courier);
            }
        }
        // kurye seçilmesi için bir "seçilen kurye" nesnesi oluşturuldu
        Courier selectedCourier("name", "phone", Time(0, 0), 0);
        // kurye seçilip seçilmediğini kontrol ediyor
        bool isCourierSelectedRandomly = false;
        // şu anki zaman alınıp kurye zamanları ile karşılaştırılacak ve en uygun kurye bulunacak
        time_t t = time(0);
        tm *now = localtime(&t);
        int currentHour = now->tm_hour;
        int currentMinute = now->tm_min;
        // int currentHour = 14;
        // int currentMinute = 0;
        Time thisTime(currentHour, currentMinute);
        // eğer zaman herhangi  bir kuryenin anında gitmesine müsaitse "seçilmiş kurye" nesnesine atanacak
        for (int i = 0; i < courierList.size(); i++)
        {
            Time distributionFinish = courierList[i].getDistributionFinish();
            if (distributionFinish.operator<(thisTime))
            {
                selectedCourier = courierList[i];
                isCourierSelectedRandomly = true;
                break;
            }
        }
        // eğer hiç bir kurye anında çıkacak kadar müsait değilse şu anki zamana en yakın olan kuryeyi bulup "seçilmiş kurye" nesnesine atanacak
        if (isCourierSelectedRandomly == false)
        {
            for (int i = 0; i < courierList.size(); i++)
            {
                Time a = thisTime.nearest(courierDistributionFinishHourList);
                Time x = courierList[i].getDistributionFinish();
                if (x.getHour() == a.getHour() && x.getMinute() == a.getMinute())
                {
                    selectedCourier = courierList[i];
                    break;
                }
            }
        }
        // kurye seçildi şimdi müşterinin hangi ilçede oturduğuna bağlı olarak kuryenin vakti güncellenecek
        string district = customer->getDistrict();
        int hour;
        int minute;
        if (district == "ortahisar")
        {
            hour = 0;
            minute = 35;
        }
        if (district == "akçaabat")

        {
            hour = 0;
            minute = 50;
        }
        if (district == "vakfikebir")
        {
            hour = 1;
            minute = 30;
        }
        if (district == "besikduzu")
        {
            hour = 1;
            minute = 50;
        }
        if (district == "yomra")
        {
            hour = 0;
            minute = 55;
        }
        if (district == "arsin")
        {
            hour = 1;
            minute = 10;
        }
        if (district == "arakli")
        {
            hour = 1;
            minute = 40;
        }
        Time addTimetoCourier(hour, minute);
        // kuryenin vakti güncellendi şimdi kuryenin teslimat saati hesaplanacak
        int deliveryHour;
        int deliveryMinute;
        if (isCourierSelectedRandomly == false)
        {
            Time a = selectedCourier.getDistributionFinish().operator+(addTimetoCourier);
            deliveryHour = a.getHour();
            deliveryMinute = a.getMinute();
        }
        else
        {
            time_t t = time(0);
            tm *now = localtime(&t);
            int currentHour = now->tm_hour;
            int currentMinute = now->tm_min;
            Time thisTime(currentHour, currentMinute);
            Time a = thisTime.operator+(addTimetoCourier);
            deliveryHour = a.getHour();
            deliveryMinute = a.getMinute();
        }
        Time deliveryTime(deliveryHour, deliveryMinute);
        ifstream ikurye("kuryeler.txt");
        string line1;
        vector<string> lines1;
        if (ikurye.is_open())
        {
            while (getline(ikurye, line1))
            {
                lines1.push_back(line1);
            }
        }
        ikurye.close();
        ofstream okurye("kuryeler.txt", ios_base::app);
        ofstream tempFile1("tempFile.txt");
        for (int i = 0; i < lines1.size(); i++)
        {
            if (lines1[i] == to_string(selectedCourier.getNumbers()))
            {
                lines1[i - 2] = to_string(deliveryTime.getHour());
                lines1[i - 1] = to_string(deliveryTime.getMinute());
            }
            else
            {
                continue;
            }
        }
        for (int i = 0; i < lines1.size(); i++)
        {
            tempFile1 << lines1[i] << endl;
        }
        okurye.close();
        tempFile1.close();
        remove("kuryeler.txt");
        rename("tempFile.txt", "kuryeler.txt");
        selectedCourier = Courier("name", "phone", Time(0, 0), 0);
        isCourierSelectedRandomly = false;
        clothesList.clear();
        courierList.clear();
        courierDistributionFinishHourList.clear();
        string trashInput;
        getline(cin, trashInput);
        ofstream fatura("faturalar.txt", ios_base::app);
        int generatedInvoiceID = generate_random_number();
        if (fatura.is_open())
        {
            // müşteri isim
            // customer: formatında yazdırıyor
            string customerName = customer->getNamesurname().substr(9, customer->getNamesurname().size());
            fatura << "Musteri Ismi: " << customerName << endl;
            // fatura id
            fatura << "Fatura Numarasi: " << generatedInvoiceID << endl;
            // müşteri email
            fatura << "Musteri Email: " << customer->getEmail() << endl;
            // indirim tutarı
            fatura << "Indirim Yuzdesi: " << discountInt << endl;
            // toplam fiyat
            fatura << "Toplam Fiyat: " << totalPrice << endl;
            // sipariş teslim saati
            fatura << "Teslimat Tarihi: " << deliveryTime.getHour() << ":" << deliveryTime.getMinute() << endl;
            // sipariş teslim noktası
            fatura << "Siparis Edilecek Bolge: " << customer->getDistrict() << endl
                   << "\n------------------------------\n\n";
        }
        else
        {
            cerr << "faturalar.txt dosyasi acilamadi!" << endl;
            exit(1);
        }
        fatura.close();
        cout << "Siparis teslim saati: " << deliveryTime.getHour() << ":" << deliveryTime.getMinute() << endl;
        cout << "Siparis teslim noktasi: " << customer->getDistrict() << endl;
        cout << "Fatura ID: " << generatedInvoiceID << endl;
        cout << "Faturaniz kaydedildi.\nFaturanizi siparislerim kismindan gorebilirsiniz.\n\n";
        cout << "Bir tusa basarak ana menuye donebilirsiniz...";
        cin.get();
        clearMenu();
        customerMenu();
    }
    else
    {
        clearMenu();
        cout << "Hatali giris yaptiniz!\n\n";
        saveInvoice();
    }
}

void Customer::printCustomer()
{
    cout << "\n----------------------------------------\n\n";
    cout << "Musteri bilgileri:\n";
    cout << "Isim Soyisim: " << getNamesurname() << endl;
    cout << "Telefon numarasi: " << getPhone() << endl;
    cout << "Kullanici adi: " << getUsername() << endl;
    cout << "E-posta: " << getEmail() << endl;
    cout << "Ilce: " << getDistrict() << endl;
    cout << "Indirim kuponu: " << getDiscount() << endl;
    cout << "Dogum tarihi: " << getBirthdate() << endl;
    cout << "Sifre: " << getPassword() << endl;
}

Clothes::Clothes(string category, int clothesID, double price, string size, string color)
{
    this->category = category;
    this->clothesID = clothesID;
    this->price = price;
    this->size = size;
    this->color = color;
}

Clothes::~Clothes()
{
}

string Clothes::getCategory()
{
    return category;
}

int Clothes::getClothesID()
{
    return clothesID;
}

double Clothes::getPrice()
{
    return price;
}

string Clothes::getSize()
{
    return size;
}

string Clothes::getColor()
{
    return color;
}

void Clothes::printClothes()
{
    cout << "worked1" << endl;
    int clotheID = getClothesID();
    int num = int(getCategory()[7]);
    string clotheCategory;
    switch (num - 48)
    {
    case 1:
        clotheCategory = "Erkek Tisort";
        break;
    case 2:
        clotheCategory = "Erkek Pantolon";
        break;
    case 3:
        clotheCategory = "Erkek Ceket";
        break;
    case 4:
        clotheCategory = "Erkek Ayakkabi";
        break;
    case 5:
        clotheCategory = "Kadin Tisort";
        break;
    case 6:
        clotheCategory = "Kadin Pantolon";
        break;
    case 7:
        clotheCategory = "Kadin Ceket";
        break;
    case 8:
        clotheCategory = "Kadin Ayakkabi";
        break;
    default:
        clotheCategory = "Kategori bulunamadi";
        break;
    };
    double price = getPrice();
    string size = getSize();
    string color = getColor();
    printf("%-7s%-20s%-7s%-7s%-10s\n", to_string(clotheID).c_str(), clotheCategory.c_str(), to_string(price).substr(0, 5).c_str(), size.c_str(), color.c_str());
}
