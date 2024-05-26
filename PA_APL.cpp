#include <iostream>     // Library untuk input dan output standar
#include <fstream>      // Library untuk operasi file input//output
#include <sstream>      // Library untuk manipulasi string stream
#include <vector>       // Library untuk menggunakan container vector
#include <limits>       // Library untuk mendapatkan batas tipe data primitif
#include <cstdlib>      // Library untuk fungsi umum seperti rand() dan srand()
#include <ctime>        // Library untuk fungsi waktu seperti time()
#include <iomanip>      // Library untuk manipulasi format output
#include <algorithm>    // Library untuk fungsi algoritma standar seperti sort() dan lainnya

using namespace std;    // agar tidak perlu menulis 'std::' sebelum setiap elemen dari standar C++ library


struct User {           // Struct User digunakan untuk merepresentasikan informasi tentang seorang user
    int id;
    string username;
    string password;
};

struct Transaction {    // Struct Transaction digunakan untuk merepresentasikan informasi tentang transaksi tabungan
    int id;
    int tabunganId;
    int userId;
    string type;
    long int jumlah;
    string tanggal;
};

struct Tabungan {       // Struct Tabungan digunakan untuk merepresentasikan informasi tentang sebuah tabungan
    int id;
    int userId;
    string namaTabungan;
    string pin;
    long int jumlah;
    int maximal_tabungan;
    string target_tabungan;
    vector<Transaction> transaksi;  //Vector untuk menyimpan transaksi terkait dengan tabungan
};


// Fungsi start menu
void showStartMenu();
void registerUser();
void loginUser();

// Fungsi setelah login
void showMainMenu(const string& username, int userId);
void createSaving(int userId, const string& username);
void seeSaving(int userId, const string& username);
void seeSavingAchieveTarget(int userId, const string& username);

// Fungsi pada menu transaksi
void showTransactionMenu(int userId, const string& username);
void seeSavingTransaction(int userId);
void deposit(int userId);
void withdraw(int userId);

// Fungsi untuk mencatat riwayat transaksi
void recordTransaction(int tabunganId, int userId, const string& type, long int jumlah);

// Fungsi metode sorting = quick sort
void swap(Tabungan& a, Tabungan& b);
void quickSort(vector<Tabungan>& tabungans, int low, int high);
int partition(vector<Tabungan>& tabungans, int low, int high);

// Fungsi metode searching = binary search
int binarySearchById(const vector<Tabungan>& tabungans, int left, int right, int target);

// Fungsi untuk menghasilkan nilai kembalian true atau false
bool isUsernameTaken(const string& username);
bool checkMaximalTabungan(int userId);


// Utama
int main() {
    system("cls");
    showStartMenu();
    return 0;
}

void showStartMenu() {
    int choice; // Variabel menyimpan pilihan user

    // Tampilan menu awal
    cout << "=== SELAMAT DATANG ===" << endl;
    cout << "1. Register" << endl;
    cout << "2. Login" << endl;
    cout << "3. Exit" << endl;
    cout << "Masukkan Pilihan : ";
    cin >> choice;  // Input

    // Memastikan input valid
    while (cin.fail() || choice < 1 || choice > 3) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input tidak valid. Masukkan angka antara 1 dan 3 : ";
        cin >> choice;
    }

    // Memproses pilihan user menggunakan switch statement
    switch (choice) {
        case 1:
            system("cls");
            registerUser(); // Memanggil fungsi register
            break;
        case 2:
            system("cls");
            loginUser();    // Memanggil fungsi login
            break;
        case 3:
            system("cls");
            cout << "Terima Kasih." << endl;
            return;
        default:
            cout << "Pilihan tidak valid.\n";
            break;
    }

    showStartMenu();    // Rekursif, Memanggil fungsinya sendiri untuk menampilkan fungsinya lagi
}

void registerUser() {
    User newUser;   // Membuat objek User baru untuk menyimpan user yang akan register

    cout << "=== Registrasi ===" << endl;
    cout << "Masukkan username: ";
    cin >> newUser.username;    // Input

    // Periksa apakah username sudah digunakan sebelumnya
    if (isUsernameTaken(newUser.username)) {
        system("cls");
        cout << "Username sudah digunakan. Silakan coba username lain." << endl;
        return;
    }

    cout << "Masukkan password: ";
    cin >> newUser.password;    // Input

    ifstream infile("users.csv");   // Membuka file untuk membaca data user yang sudah terdaftar
    string line;
    newUser.id = 1;     // ID user baru dimulai dari 1

    // Memeriksa apakah file kosong
    bool fileIsEmpty = infile.peek() == ifstream::traits_type::eof();
    infile.close();

    ofstream outfile("users.csv", ios::app);    // Membuka file untuk menulis data user baru
    if (outfile.is_open()) {
        // Jika file kosong, tulis header kolom
        if (fileIsEmpty) {
            outfile << "ID,Username,Password\n";
        }

        infile.open("users.csv");   // Membuka kembali file untuk membaca ID user terakhir
        while (getline(infile, line)) {
            stringstream ss(line);
            int currentId;
            ss >> currentId;
            newUser.id = currentId + 1; // Menggunakan ID user terakhir yang ada di file dan menambahkan 1
        }
        infile.close();

        // Menulis informasi user baru ke dalam file
        outfile << newUser.id << "," 
                << newUser.username << "," 
                << newUser.password << "\n";
        outfile.close();

        system("cls");
        cout << "Registrasi berhasil! User ID Anda adalah " << newUser.id << endl;

    } else {
        cerr << "Gagal membuka file untuk menyimpan data user." << endl;
    }

    showStartMenu();    // Kembali ke menu awal setelah registrasi berhasil
}

void loginUser() {
    string username, password;  // Variabel untuk menyimpan usn dan pass yang dimasukkan user
    cout << "=== Login ===" << endl;    
    cout << "Masukkan username: ";
    cin >> username;    // Input
    cout << "Masukkan password: ";
    cin >> password;    // Input

    ifstream file("users.csv"); // Membuka file untuk membaca data user yang sudah terdaftar
    string line, fileUsername, filePassword;
    int fileId = -1;    // Variabel untuk menyimpan ID user yang ditemukan, nilai -1 untuk menandakan bahwa tidak ada ID yang ditemukan

    if (file.is_open()) {
        // Membaca setiap baris dalam file dan memeriksa apakah usn dan pass cocok
        while (getline(file, line)) {
            stringstream ss(line);
            ss >> fileId;
            ss.ignore(1, ',');
            getline(ss, fileUsername, ',');
            getline(ss, filePassword, ',');

            // Jika usn dan pass cocok, berhasil login
            if (fileUsername == username && filePassword == password) {
                system("cls");
                file.close();   // Menutup file 

                // Panggil fungsi showMainMenu dengan usn dan ID user yang ditemukan
                if (fileId != -1) { 
                    showMainMenu(username, fileId); 
                } else {
                    cout << "Gagal mendapatkan userId." << endl;
                    showStartMenu();
                }
                return;
            }
        }
        file.close();   // Menutup file setekah selesai di baca
    }

    system("cls");
    cout << "Login gagal. Username atau password salah." << endl;
    showStartMenu();    // Kembali ke menu awal jika login gagal
}

bool isUsernameTaken(const string& username) {
    ifstream userFile("users.csv"); // Membuka file untuk membaca data user yang sudah terdaftar
    string userLine, fileUsername, filePassword;
    int fileId;

    if (userFile.is_open()) {
        // Baca setiap baris dalam file dan memeriksa apakah usn sudah digunakan
        while (getline(userFile, userLine)) {
            stringstream ss(userLine);
            ss >> fileId;
            ss.ignore(1, ',');
            getline(ss, fileUsername, ',');
            getline(ss, filePassword, ',');

            // Jika usn sudah digunakan, maka kembalikan nilai true
            if (fileUsername == username) {
                userFile.close();   // Menutup file
                return true;
            }
        }
        userFile.close();
    }
    return false;   // Kembalikan nilai false jika usn belum digunakan
}

void showMainMenu(const string& username, int userId) {
    system("cls");

    // Periksa apakah user memiliki tabungan yang mencapai batas maksimal
    bool hasMaximalTabungan = checkMaximalTabungan(userId);

    cout << "=== SELAMAT DATANG " << username << " ===" << endl;

    // Menampilkan pesan tambahan jika user memiliki tabungan yang mencapai batas maksimal
    if (hasMaximalTabungan) {
        cout << "Kamu Memiliki Tabungan Yang Sudah Mencapai Maximal, Mari Kita Cek Di Menu 4" << endl << endl;
    }

    cout << "1. Buat Tabungan Baru" << endl;
    cout << "2. Lihat Tabungan" << endl;
    cout << "3. Transaksi" << endl;
    
    // Menampilkan opsi tambahan jika user memiliki tabungan yang mencapai batas maksimal
    if (hasMaximalTabungan) {
        cout << "4. Tabungan Yang Mencapai Target" << endl;
        cout << "5. Exit" << endl;
        cout << endl;
    } else {
        cout << "4. Exit" << endl;
        cout << endl;
    }
    
    int choice;
    cout << "Masukkan Pilihan : ";
    cin >> choice;

    // Menentukan maksimal pilihan berdasarkan apakah user memiliki tabungan yang mencapai batas maksimal
    int maxChoice = hasMaximalTabungan ? 5 : 4;

    // Memastikan input yang valid
    while (cin.fail() || choice < 1 || choice > maxChoice) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input tidak valid. Masukkan angka antara 1 dan " << maxChoice << " : ";
        cin >> choice;
    }

    // Memproses pilihan user menggunakan switch statement
    switch (choice) {
        case 1:
            system("cls");
            createSaving(userId, username);  // Memanggil fungsi createSaving untuk membuat tabungan baru
            break;
        case 2:
            system("cls");
            seeSaving(userId, username);    // Memanggil fungsi seeSaving untuk melihat tabungan
            break;
        case 3:
            system("cls");
            showTransactionMenu(userId, username);  // Memanggil fungsi showTransactionMenu untuk menampilkan menu transaksi
            break;
        case 4:
            if (hasMaximalTabungan) {
                system("cls");
                seeSavingAchieveTarget(userId, username);   // Memanggil fungsi seeSavingAchieveTarget untuk melihat tabungan yang mencapai target
            } else {
                system("cls");
                cout << "Terima Kasih" << endl;
                return;
            }
            break;
        case 5:
            system("cls");
            cout << "Terima Kasih" << endl;
            return;
        default:
            cout << "Pilihan tidak valid.\n";
            break;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Tekan ENTER untuk kembali ke menu utama...";
    cin.get();
    system("cls");

    showMainMenu(username, userId); // Rekursif, Memanggil fungsinya sendiri untuk menampilkan fungsinya lagi
}


void showTransactionMenu(int userId, const string& username) {
    cout << "=== MENU TRANSAKSI ===" << endl;
    cout << "1. Lihat Transaksi Tabungan" << endl;
    cout << "2. Setor" << endl;
    cout << "3. Tarik" << endl;
    cout << "4. Exit" << endl;
    
    int choice;
    cout << "Masukkan Pilihan : ";
    cin >> choice;

    // Memastikan input yang valid
    while (cin.fail() || choice < 1 || choice > 4) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input tidak valid. Masukkan angka antara 1 dan 4 : ";
        cin >> choice;
    }

    switch (choice) {
        case 1:
            system("cls");
            seeSavingTransaction(userId);   // Memanggil fungsi seeSavingTransaction untuk melihat transaksi tabungan
            break;
        case 2:
            system("cls");
            deposit(userId);    // Memanggil fungsi deposit untuk melakukan setoran uang ke tabungan
            break;
        case 3:
            system("cls");
            withdraw(userId);   // Memanggil fungsi withdraw untuk melakukan penarikan uang dari tabungan
            break;
        case 4:
            system("cls");
            return;
        default:
            cout << "Pilihan tidak valid.\n";
    }
}

void createSaving(int userId, const string& username) {
    Tabungan newSaving; // Membuat objek Tabungan baru untuk menyimpan informasi tabungan yang akan dibuat
    newSaving.userId = userId;  // Mengatur ID user untuk tabungan baru

    cout << "=== Buat Tabungan ===" << endl;
    cout << "Masukkan Nama Tabungan: ";
    cin.ignore();
    getline(cin, newSaving.namaTabungan);   // Input

    // Meminta user untuk memasukkan PIN tabungan yang terdiri dari 4 digit angka
    string pin;
    while (true) {
        cout << "Masukkan PIN Tabungan (4 digit angka): ";
        cin >> pin;

        // Memeriksa apakah PIN yang dimasukkan valid
        if (pin.length() == 4 && pin.find_first_not_of("0123456789") == string::npos) {
            break;
        } else {
            cout << "PIN harus terdiri dari 4 digit angka. Silakan coba lagi." << endl;
        }
    }

    newSaving.pin = pin;    // Mengatur PIN untuk tabungan baru
    newSaving.jumlah = 0;   // Mengatur jumlah tabungan awal menjadi 0

    string hasMaxTabungan;
    // Meminta user untuk memasukkan apakah ada batas maksimal jumlah tabungan
    while (true) {
        cout << "Apakah ada maksimal jumlah tabungan? [y//n] : ";
        cin >> hasMaxTabungan;

        if (hasMaxTabungan == "y" || hasMaxTabungan == "Y") {
            int maxTabungan;
            cout << "Masukkan jumlah maksimal tabungan: ";
            while (!(cin >> maxTabungan) || maxTabungan < 0) {
                cout << "Jumlah maksimal tabungan tidak valid. Silakan masukkan angka positif: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            newSaving.maximal_tabungan = maxTabungan;
            break;
        } else if (hasMaxTabungan == "n" || hasMaxTabungan == "N") {
            newSaving.maximal_tabungan = -1;
            break;
        } else {
            cout << "Pilihan tidak valid, hanya input y atau n saja." << endl;
        }
    }

    string hasTargetTabungan;
    // Meminta user untuk memasukkan apakah ada target waktu tabungan
    while (true) {
        cout << "Apakah ada target waktu tabungan? [y//n] : ";
        cin >> hasTargetTabungan;

        if (hasTargetTabungan == "y" || hasTargetTabungan == "Y") {
            int targetDay, targetMonth, targetYear;

            // Mendapatkan tanggal dan waktu saat ini
            time_t now = time(0);
            tm *ltm = localtime(&now);
            int currentYear = 1900 + ltm->tm_year;
            int currentMonth = 1 + ltm->tm_mon;
            int currentDay = ltm->tm_mday;

            while (true) {
                // Meminta user untuk memasukkan tanggal target
                cout << "Masukkan tanggal (1-31): ";
                while (!(cin >> targetDay) || targetDay < 1 || targetDay > 31) {
                    cout << "Tanggal tidak valid. Harap masukkan angka antara 1 dan 31: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                 // Meminta user untuk memasukkan bulan target
                cout << "Masukkan bulan (1-12): ";
                while (!(cin >> targetMonth) || targetMonth < 1 || targetMonth > 12) {
                    cout << "Bulan tidak valid. Harap masukkan angka antara 1 dan 12: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                 // Meminta user untuk memasukkan tahun target
                cout << "Masukkan tahun (minimal " << currentYear << ", maksimal 2100): ";
                while (!(cin >> targetYear) || targetYear < currentYear || targetYear > 2100) {
                    cout << "Tahun tidak valid. Harap masukkan angka antara " << currentYear << " dan 2100: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                if (targetYear == currentYear && targetMonth < currentMonth) {
                    cout << "Bulan tidak valid. Harap masukkan bulan yang lebih besar atau sama dengan bulan sekarang." << endl;
                    continue;
                }

                if (targetYear == currentYear && targetMonth == currentMonth && targetDay < currentDay) {
                    cout << "Tanggal tidak valid. Harap masukkan tanggal yang lebih besar atau sama dengan tanggal sekarang." << endl;
                    continue;
                }

                // Mengatur target tabungan dengan format tanggal yang sesuai
                newSaving.target_tabungan = to_string(targetDay) + "//" + to_string(targetMonth) + "//" + to_string(targetYear);
                break;
            }
            break; 
        } else if (hasTargetTabungan == "n" || hasTargetTabungan == "N") {
            newSaving.target_tabungan = "";
            break; 
        } else {
            cout << "Pilihan tidak valid, hanya input y atau n saja." << endl;
        }
    }

    // Meminta user untuk memasukkan jumlah setoran pertama
    long int initialDeposit;
    cout << "Masukkan jumlah setoran pertama (minimal 10000): ";
    while (true) {
        while (!(cin >> initialDeposit) || initialDeposit < 10000) {
            cout << "Jumlah setoran pertama tidak valid. Harap masukkan angka minimal 10000: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // Memeriksa apakah jumlah setoran pertama valid untuk tidak melebihi batas dari nominal maximal tabungan
        if (newSaving.maximal_tabungan != -1 && initialDeposit > newSaving.maximal_tabungan) {
            cout << "Jumlah setoran pertama tidak boleh lebih dari maksimal jumlah tabungan: ";
            continue;
        }

        break;
    }
    newSaving.jumlah = initialDeposit;  // Mengatur jumlah setoran pertama untuk tabungan baru

    int newSavingId = 1;
    string line;
    ifstream readfile("tabungan.csv");
    // Membaca file tabungan.csv untuk mendapatkan ID tabungan baru
    if (readfile.is_open()) {
        getline(readfile, line);
        while (getline(readfile, line)) {
            stringstream ss(line);
            string field;
            getline(ss, field, ',');
            int currentId = stoi(field);
            if (currentId >= newSavingId) {
                newSavingId = currentId + 1;
            }
        }
        readfile.close();   // Menutup file
    }
    newSaving.id = newSavingId; // Atur ID tabungan baru

    ofstream outfile("tabungan.csv", ios::app);
    // Menyimpan informasi tabungan baru ke dalam file tabungan.csv
    if (outfile.is_open()) {
        if (newSavingId == 1) {
            outfile << "id,user_id,nama_tabungan,pin_tabungan,jumlah,maximal_tabungan,target_tabungan\n";
        }

        // Menulis informasi tabungan baru ke dalam file
        outfile << newSaving.id << ","
                << newSaving.userId << ","
                << newSaving.namaTabungan << ","
                << newSaving.pin << ","
                << initialDeposit << ","
                << newSaving.maximal_tabungan << ","
                << newSaving.target_tabungan << "\n";
        outfile.close();    // Menutup file setelah selesai

        // Merekam transaksi setoran pertama ke dalam file transaksi.csv
        recordTransaction(newSaving.id, userId, "Setoran Pertama", initialDeposit);

        cout << "Tabungan berhasil dibuat untuk user " << username << endl;
    } else {
        cerr << "Gagal membuka file untuk menyimpan data tabungan." << endl;
    }
}

void recordTransaction(int tabunganId, int userId, const string& type, long int jumlah) {
    ofstream outfile("transaksi.csv", ios::app);    // Membuka file transaksi.csv untuk ditambahkan data transaksi baru

    if (outfile.is_open()) {    // Memeriksa apakah file transaksi.csv berhasil dibuka
        int transactionId = 1;
        string line;
        ifstream infile("transaksi.csv");   // Membuka file transaksi.csv untuk mendapatkan ID transaksi baru 
        if (infile.is_open()) {
            getline(infile, line);
            while (getline(infile, line)) {
                stringstream ss(line);
                int currentId;
                ss >> currentId;
                if (currentId >= transactionId) {
                    transactionId = currentId + 1;   // Mendapatkan ID transaksi baru
                }
            }
            infile.close(); // Menutup file
        }

        // Mendapatkan tanggal dan waktu saat ini
        time_t now = time(0);
        tm *ltm = localtime(&now);
        stringstream ss;
        ss << 1900 + ltm->tm_year << "-"
           << 1 + ltm->tm_mon << "-"
           << ltm->tm_mday;

        string tanggalTransaksi = ss.str();     // Mengonversi tanggal dan waktu menjadi string

        if (transactionId == 1) {
            outfile << "ID,ID Tabungan,ID User,Jenis Transaksi,Jumlah Transaksi,Tanggal Transaksi\n";   // Menulis header jika file kosong
        }

        // Menulis informasi transaksi baru ke dalam file transaksi.csv
        outfile << transactionId << "," << tabunganId << "," << userId << ","
                << type << "," << jumlah << "," << tanggalTransaksi << "\n";
        outfile.close();
    } else {
        cerr << "Gagal membuka file untuk menyimpan data transaksi." << endl;
    }
}

void seeSaving(int userId, const string& username) {
    ifstream infile("tabungan.csv");    // Membuka file tabungan.csv
    string line;
    vector<Tabungan> tabungans;         // Membuat vektor untuk menyimpan informasi tabungan

    // Memeriksa apakah file berhasil dibuka
    if (infile.is_open()) {

        // Membaca header file tabungan.csv
        getline(infile, line);

        // Membaca baris - baris data dari file dan menyimpannya ke dalam vektor tabungans
        while (getline(infile, line)) {
            stringstream ss(line);
            string field;
            Tabungan tabungan;

            getline(ss, field, ',');
            tabungan.id = stoi(field);

            getline(ss, field, ',');
            tabungan.userId = stoi(field);

            getline(ss, tabungan.namaTabungan, ',');

            getline(ss, field, ',');

            getline(ss, field, ',');
            tabungan.jumlah = stol(field);

            getline(ss, field, ',');
            tabungan.maximal_tabungan = stoi(field);

            getline(ss, tabungan.target_tabungan, ',');
            
            // Menambahkan tabungan ke dalam vektor jika userId sesuai dengan userId yang dimasukkan
            if (tabungan.userId == userId) {
                tabungans.push_back(tabungan);
            }
        }
        infile.close();     // Menutup file

        // Mengurutkan tabungans berdasarkan jumlah tabungan menggunakan algoritma quick sort
        quickSort(tabungans, 0, tabungans.size() - 1);

        // Menampilkan informasi tabungan ke layar
        cout << "Tabungan untuk " << username << " (User ID " << userId << ") yang telah diurutkan berdasarkan jumlah:" << endl;
        cout << "----------------------------------------------------------------" << endl;
        cout << left << setw(6) << "ID"
             << setw(18) << "Nama Tabungan"
             << setw(16) << "Jumlah"
             << setw(15) << "Maximal"
             << setw(15) << "Target"
             << endl;
        cout << "----------------------------------------------------------------" << endl;

        // Menampilkan informasi setiap tabungan dalam vektor tabungans
        for (const Tabungan& tabungan : tabungans) {
            cout << left << setw(6) << tabungan.id
                 << setw(18) << tabungan.namaTabungan
                 << setw(16) << tabungan.jumlah
                 << setw(15) << (tabungan.maximal_tabungan == -1 ? "Tidak Ada" : to_string(tabungan.maximal_tabungan))
                 << setw(15) << (tabungan.target_tabungan.empty() ? "Tidak Ada" : tabungan.target_tabungan)
                 << endl;
        }

        cout << "----------------------------------------------------------------" << endl;

        string response;
        cout << "Apakah Anda ingin melihat informasi jelas dari salah satu tabungan anda? [y//n]: ";
        cin >> response;    // Input

        // Memvalidasi input respons dari user
        while (response != "y" && response != "Y" && response != "n" && response != "N") {
            cout << "Input tidak valid: ";
            cin >> response;
        }

        if (response == "y" || response == "Y") {
            // Mengurutkan tabungans berdasarkan ID tabungan menggunakan algoritma pengurutan binner
            sort(tabungans.begin(), tabungans.end(), [](const Tabungan& a, const Tabungan& b) {
                return a.id < b.id;
            });
            
            int searchTarget;
            cout << "Masukkan ID tabungan untuk mencari: ";
            cin >> searchTarget; // Input

            // Mencari ID tabungan dalam vektor tabungans menggunakan algoritma pencarian biner
            int foundIndex = binarySearchById(tabungans, 0, tabungans.size() - 1, searchTarget);
            if (foundIndex != -1) {
                // Menampilkan informasi tabungan yang ditemukan ke layar
                cout << "Tabungan dengan ID " << searchTarget << " ditemukan:" << endl;
                cout << left << setw(6) << "ID"
                     << setw(18) << "Nama Tabungan"
                     << setw(16) << "Jumlah"
                     << setw(15) << "Maximal"
                     << setw(15) << "Target"
                     << endl;
                cout << "----------------------------------------------------------------" << endl;
                cout << left << setw(6) << tabungans[foundIndex].id
                     << setw(18) << tabungans[foundIndex].namaTabungan
                     << setw(16) << tabungans[foundIndex].jumlah
                     << setw(15) << (tabungans[foundIndex].maximal_tabungan == -1 ? "Tidak Ada" : to_string(tabungans[foundIndex].maximal_tabungan))
                     << setw(15) << (tabungans[foundIndex].target_tabungan.empty() ? "Tidak Ada" : tabungans[foundIndex].target_tabungan)
                     << endl;
                cout << "----------------------------------------------------------------" << endl;
            } else {
                cout << "Tabungan dengan ID " << searchTarget << " tidak ditemukan." << endl;
            }
        }
    } else {
        cerr << "Gagal membuka file tabungan.csv" << endl;
    }
}

void seeSavingTransaction(int userId) {
    ifstream tabunganFile("tabungan.csv");  // Membuka file tabungan.csv
    string tabunganLine;
    vector<Tabungan> userTabungans;         // Membuat vektor untuk menyimpan informasi tabungan user

    // Memeriksa apakah file berhasil dibuka
    if (tabunganFile.is_open()) {

        // Membaca header file tabungan.csv
        getline(tabunganFile, tabunganLine);

        // Membaca baris - baris data dari file dan menyimpannya dalam vektor userTabungans
        while (getline(tabunganFile, tabunganLine)) {
            stringstream ss(tabunganLine);
            Tabungan tabungan;
            string field;

            getline(ss, field, ',');
            tabungan.id = stoi(field);

            getline(ss, field, ',');
            tabungan.userId = stoi(field);

            getline(ss, tabungan.namaTabungan, ',');

            userTabungans.push_back(tabungan);
        }
        tabunganFile.close();   // Menutup file setelah selesai

        // Memeriksa apakah vektor userTabungans kosong
        if (userTabungans.empty()) {
            cout << "Anda belum memiliki tabungan." << endl;
            return;
        }

        // Menampilkan daftar tabungan user yang dapat dipilih untuk dilihat riwayat transaksinya
        cout << "Pilih Tabungan:" << endl;
        cout << "---------------------------" << endl;
        cout << left << setw(6) << "ID"
             << setw(18) << "Nama Tabungan" << endl;
        cout << "---------------------------" << endl;

        for (const Tabungan& tabungan : userTabungans) {
            if (tabungan.userId == userId) {
                cout << left << setw(6) << tabungan.id << setw(18) << tabungan.namaTabungan << endl;
            }
        }
        cout << "---------------------------" << endl;

        int selectTabunganId;
        cout << "Masukkan ID Tabungan: ";
        cin >> selectTabunganId;

        Tabungan selectTabungan;
        bool foundTabungan = false;
        // Mencari tabungan yang dipilih oleh user dalam vektor userTabungans
        for (const Tabungan& tabungan : userTabungans) {
            if (tabungan.userId == userId && tabungan.id == selectTabunganId) {
                selectTabungan = tabungan;
                foundTabungan = true;
                break;
            }
        }

        // Memeriksa apakah tabungan ditemukan
        if (!foundTabungan) {
            cout << "Tabungan tidak ditemukan." << endl;
            return;
        }

        ifstream transaksiFile("transaksi.csv");    // Membuka file transaksi.csv 
        string transaksiLine;

        // Memeriksa apakah file transaksi berhasil dibuka
        if (transaksiFile.is_open()) {
            getline(transaksiFile, transaksiLine);

            // Menampilkan riwayat transaksi untuk tabungan yang dipilih
            cout << "Transaksi untuk Tabungan \"" << selectTabungan.namaTabungan << "\":" << endl;
            cout << "----------------------------------------------------------------" << endl;
            cout << left << setw(6) << "ID" << setw(20) << "Jenis Transaksi" << setw(20) << "Jumlah Transaksi" << setw(15) << "Tanggal Transaksi" << endl;
            cout << "----------------------------------------------------------------" << endl;

            // Membaca baris - baris data dari file transaksi.csv dan menampilkan transaksi yang sesuai dengan tabungan dan ID user
            while (getline(transaksiFile, transaksiLine)) {
                stringstream ss(transaksiLine);
                string field;
                int transaksiTabunganId, transaksiUserId;
                string jenisTransaksi, tanggalTransaksi;
                long int jumlahTransaksi;

                getline(ss, field, ',');
                int transactionId = stoi(field);

                getline(ss, field, ',');
                transaksiTabunganId = stoi(field);

                getline(ss, field, ',');
                transaksiUserId = stoi(field);

                getline(ss, jenisTransaksi, ',');

                getline(ss, field, ',');
                jumlahTransaksi = stol(field);

                getline(ss, tanggalTransaksi, ',');

                 // Menampilkan transaksi yang sesuai dengan tabungan dan ID user
                if (transaksiTabunganId == selectTabunganId && transaksiUserId == userId) {
                    cout << left << setw(6) << transactionId
                         << setw(20) << jenisTransaksi
                         << setw(20) << jumlahTransaksi
                         << setw(15) << tanggalTransaksi
                         << endl;
                }
            }
            transaksiFile.close();  // Menutup file
            cout << "----------------------------------------------------------------" << endl;
        } else {
            cerr << "Gagal membuka file transaksi.csv" << endl;
        }
    } else {
        cerr << "Gagal membuka file tabungan.csv" << endl;
    }
}

void deposit(int userId) {
    ifstream tabunganFile("tabungan.csv"); // Membuaka file tabungan.csv
    vector<Tabungan> tabungans;
    string line;

    // Memeriksa apakah file berhasil dibuka
    if (!tabunganFile.is_open()) {
        cerr << "Gagal membuka file tabungan.csv" << endl;
        return;
    }

    // Membaca header file tabungan.csv
    getline(tabunganFile, line);

    // Membaca data tabungan dari file dan menyimpannya dalam vector tabungans
    while (getline(tabunganFile, line)) {
        stringstream ss(line);
        Tabungan tabungan;
        string field;

        getline(ss, field, ',');
        tabungan.id = stoi(field);

        getline(ss, field, ',');
        tabungan.userId = stoi(field);

        getline(ss, tabungan.namaTabungan, ',');

        getline(ss, tabungan.pin, ',');

        getline(ss, field, ',');
        tabungan.jumlah = stol(field);

        getline(ss, field, ',');
        tabungan.maximal_tabungan = stoi(field);

        getline(ss, tabungan.target_tabungan, ',');

        tabungans.push_back(tabungan);
    }
    tabunganFile.close();   // Menutup file

    // Memeriksa apakah user memiliki tabungan
    if (tabungans.empty()) {
        cout << "Anda belum memiliki tabungan." << endl;
        return;
    }

    // Menampilkan daftar tabungan yang dimiliki oleh user
    cout << "Pilih Tabungan:" << endl;
    cout << "---------------------------" << endl;
    cout << left << setw(6) << "ID" << setw(18) << "Nama Tabungan" << endl;
    cout << "---------------------------" << endl;

    for (const Tabungan& tabungan : tabungans) {
        if (tabungan.userId == userId) {
            cout << left << setw(6) << tabungan.id << setw(18) << tabungan.namaTabungan << endl;
        }
    }
    cout << "---------------------------" << endl;

    // Meminta user memasukkan ID tabungan untuk melakukan setoran
    int selectTabunganId;
    cout << "Masukkan ID Tabungan: ";
    while (!(cin >> selectTabunganId)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input tidak valid. Masukkan ID Tabungan: ";
    }

    // Mencari tabungan yang sesuai dengan ID yang dimasukkan user
    Tabungan* selectTabungan = nullptr;
    for (Tabungan& tabungan : tabungans) {
        if (tabungan.id == selectTabunganId && tabungan.userId == userId) {
            selectTabungan = &tabungan;
            break;
        }
    }

    // Memeriksa apakah tabungan yang diminta ditemukan
    if (!selectTabungan) {
        cout << "Tabungan tidak ditemukan." << endl;
        return;
    }

    // Meminta user memasukkan PIN tabungan untuk otorisasi
    string pin;
    cout << "Masukkan PIN Tabungan: ";
    cin >> pin;

    // Memverifikasi PIN yang dimasukkan user
    if (pin != selectTabungan->pin) {
        cout << "PIN salah." << endl;
        return;
    }

    // Meminta user memasukkan jumlah setoran
    long int jumlahDeposit;
    cout << "Masukkan jumlah setoran (minimal 1000): ";
    while (!(cin >> jumlahDeposit) || jumlahDeposit < 1000) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input tidak valid. Masukkan jumlah setoran (minimal 1000): ";
    }

    // Memperbarui saldo tabungan setelah setoran
    long int saldoBaru = selectTabungan->jumlah + jumlahDeposit;
    long int sisaSaldo = 0;

    // Pointer ke tabungan
    Tabungan* selectTabunganLain = nullptr;
    
    // Memeriksa apakah setoran melebihi batas maksimal tabungan
    if (selectTabungan->maximal_tabungan != -1 && saldoBaru > selectTabungan->maximal_tabungan) {
        // Menghitung sisa saldo setelah batas maksimal tercapai
        sisaSaldo = saldoBaru - selectTabungan->maximal_tabungan;
        saldoBaru = selectTabungan->maximal_tabungan;
        selectTabungan->jumlah = saldoBaru;

        // Merekam transaksi setoran ke tabungan
        recordTransaction(selectTabungan->id, userId, "Setor", jumlahDeposit - sisaSaldo);

        // Menampilkan pesan dan sisa saldo setelah batas tercapai
        cout << "Tabungan telah terpenuhi dan terdapat " << sisaSaldo << " uang yang tersisa." << endl;

        while (true) {
            cout << "Apakah anda ingin?" << endl;
            cout << "1. Tarik" << endl;

            // Menampilkan daftar tabungan lain yang dimiliki user
            vector<Tabungan> tabunganLain;
            for (const Tabungan& tabungan : tabungans) {
                if (tabungan.userId == userId && tabungan.id != selectTabungan->id) {
                    tabunganLain.push_back(tabungan);
                }
            }

            if (!tabunganLain.empty()) {
                cout << "2. Masukan ke dalam tabungan yang ada" << endl;
            }

            // Meminta user memilih opsi
            cout << "Pilih: ";
            string pilihan;
            cin >> pilihan;

            if (pilihan == "1") {
                // Menampilkan saldo tabungan dan jumlah uang yang ditarik
                cout << "Saldo tabungan: " << selectTabungan->jumlah << endl;
                cout << "Uang ditarik: " << sisaSaldo << endl;
                break;
            } else if (pilihan == "2" && !tabunganLain.empty()) {
                // Memilih tabungan lain untuk menampung sisa saldo
                cout << "Pilih Tabungan:" << endl;
                cout << "---------------------------" << endl;
                cout << left << setw(6) << "ID" << setw(18) << "Nama Tabungan" << endl;
                cout << "---------------------------" << endl;

                for (const Tabungan& tabungan : tabunganLain) {
                    cout << left << setw(6) << tabungan.id << setw(18) << tabungan.namaTabungan << endl;
                }
                cout << "---------------------------" << endl;

                int selectTabunganLainId;
                cout << "Masukkan ID Tabungan: ";
                while (!(cin >> selectTabunganLainId)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Input tidak valid. Masukkan ID Tabungan: ";
                }

                // Mencari tabungan lain yang dipilih oleh user
                for (Tabungan& tabungan : tabungans) {
                    if (tabungan.id == selectTabunganLainId && tabungan.userId == userId) {
                        selectTabunganLain = &tabungan;
                        break;
                    }
                }

                // Memeriksa apakah tabungan lain ditemukan
                if (!selectTabunganLain) {
                    cout << "Tabungan tidak ditemukan." << endl;
                } else if (sisaSaldo > 0) {
                    selectTabunganLain->jumlah += sisaSaldo;    // Menambahkan sisa saldo ke tabungan lain yang dipilih
                    recordTransaction(selectTabunganLain->id, userId, "Setor", sisaSaldo);  // Merekam transaksi setoran ke tabungan lain
                    break;
                } else {
                    cout << "Tidak ada sisa saldo untuk dipindahkan." << endl;
                    break;
                }
            } else {
                cout << "Pilihan tidak valid. Coba lagi." << endl;
            }
        }
    } else {
        selectTabungan->jumlah = saldoBaru;     // Memperbarui saldo tabungan jika setoran tidak melebihi batas maksimal
        recordTransaction(selectTabungan->id, userId, "Setor", jumlahDeposit);   // Merekam transaksi setoran ke tabungan
        cout << "Saldo tabungan: " << selectTabungan->jumlah << endl;   // Menampilkan saldo tabungan setelah setoran
    }

    ofstream tabunganOutFile("tabungan.csv");   // Membuka file tabungan.csv untuk menulis data tabungan yang diperbarui

    // Memeriksa apakah file tabungan.csv berhasil dibuka
    if (!tabunganOutFile.is_open()) {
        cerr << "Gagal membuka file tabungan.csv" << endl;
        return;
    }

    tabunganOutFile << "id,user_id,nama_tabungan,pin_tabungan,jumlah,maximal_tabungan,target_tabungan\n";

    // Menulis data tabungan yang diperbarui ke file
    for (const Tabungan& tabungan : tabungans) {
        tabunganOutFile << tabungan.id << "," 
                        << tabungan.userId << "," 
                        << tabungan.namaTabungan << ","
                        << tabungan.pin << "," 
                        << tabungan.jumlah << "," 
                        << tabungan.maximal_tabungan << ","
                        << tabungan.target_tabungan << "\n";
    }


    
    tabunganOutFile.close();    // Menutup file tabungan.csv setelah selesai menulis    

    cout << "Data tabungan telah diperbarui." << endl;
}

void withdraw(int userId) {
    vector<Tabungan> tabungans;     // Vector untuk menyimpan tabungan dari file CSV
    ifstream tabunganFile("tabungan.csv");  // Membuka file tabungan.csv
    string tabunganLine;

    // Memeriksa apakah file tabungan.csv berhasil dibuka
    if (tabunganFile.is_open()) {
        
        getline(tabunganFile, tabunganLine);    // Membaca header file tabungan.csv

        // Membaca data tabungan dari file tabungan.csv
        while (getline(tabunganFile, tabunganLine)) {
            stringstream ss(tabunganLine);
            Tabungan tabungan;
            string field;

            getline(ss, field, ',');
            tabungan.id = stoi(field);

            getline(ss, field, ',');
            tabungan.userId = stoi(field);

            getline(ss, tabungan.namaTabungan, ',');

            getline(ss, tabungan.pin, ',');

            getline(ss, field, ',');
            tabungan.jumlah = stol(field);

            getline(ss, field, ',');
            tabungan.maximal_tabungan = stoi(field);

            getline(ss, tabungan.target_tabungan, ',');

            tabungans.push_back(tabungan);  // Menambahkan tabungan ke vector
        }
        tabunganFile.close();   // Menutup file

        // Menampilkan daftar tabungan yang dimiliki user
        cout << "Pilih Tabungan:" << endl;
        cout << "---------------------------" << endl;
        cout << left << setw(6) << "ID"
             << setw(18) << "Nama Tabungan" << endl;
        cout << "---------------------------" << endl;

        for (const Tabungan& tabungan : tabungans) {
            if (tabungan.userId == userId) {
                cout << left << setw(6) << tabungan.id
                     << setw(18) << tabungan.namaTabungan << endl;
            }
        }
        cout << "---------------------------" << endl;

        // Meminta user memilih tabungan untuk penarikan
        int selectTabunganId;
        cout << "Masukkan ID Tabungan: ";
        cin >> selectTabunganId;    // Input

        Tabungan* selectedTabunganPtr = nullptr;    // Pointer ke tabungan yang dipilih oleh user
        
        // Mencari tabungan yang dipilih dalam vector
        for (Tabungan& tabungan : tabungans) {
            if (tabungan.id == selectTabunganId && tabungan.userId == userId) {
                selectedTabunganPtr = &tabungan;
                break;
            }
        }

        // Memeriksa apakah tabungan dipilih ditemukan
        if (selectedTabunganPtr != nullptr) {
            // Meminta konfirmasi dari user untuk melakukan penarikan
            string confirm;
            cout << "Apakah kamu yakin ingin menarik uang dari tabungan ini? [y//n]: ";
            cin >> confirm;

            if (confirm == "y" || confirm == "Y") {
                // Meminta jumlah penarikan dari user
                long int jumlahPenarikan;
                cout << "Masukkan jumlah penarikan (minimal 1000): ";
                while (!(cin >> jumlahPenarikan) || jumlahPenarikan < 1000) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Input tidak valid. Masukkan jumlah penarikan (minimal 1000): ";
                }

                // Memeriksa pin tabungan
                string pinInput;
                cout << "Masukkan PIN Tabungan: ";
                cin >> pinInput;

                // Memeriksa apakah PIN yang dimasukkan benar
                if (pinInput == selectedTabunganPtr->pin) {
                    // Memeriksa apakah jumlah penarikan tidak melebihi saldo tabungan
                    if (jumlahPenarikan <= selectedTabunganPtr->jumlah) {
                        // Mengurangi saldo tabungan setelah penarikan
                        selectedTabunganPtr->jumlah -= jumlahPenarikan;

                        // Merekam transaksi penarikan
                        recordTransaction(selectedTabunganPtr->id, userId, "Tarik", jumlahPenarikan);

                        // Membuka file tabungan.csv untuk menulis perubahan saldo
                        ofstream tabunganOutfile("tabungan.csv");
                        if (tabunganOutfile.is_open()) {
                            tabunganOutfile << "id,user_id,nama_tabungan,pin_tabungan,jumlah,maximal_tabungan,target_tabungan\n";
                            // Menulis data tabungan yang diperbarui ke file
                            for (const Tabungan& tabungan : tabungans) {
                                tabunganOutfile << tabungan.id << ","
                                                << tabungan.userId << ","
                                                << tabungan.namaTabungan << ","
                                                << tabungan.pin << ","
                                                << tabungan.jumlah << ","
                                                << tabungan.maximal_tabungan << ","
                                                << tabungan.target_tabungan << "\n";
                            }
                            tabunganOutfile.close();    // Menutup file
                        } else {
                            cerr << "Gagal membuka file tabungan.csv untuk menyimpan perubahan." << endl;
                        }

                        cout << "Penarikan berhasil. Saldo tabungan: " << selectedTabunganPtr->jumlah << endl;
                    } else {
                        cout << "Jumlah penarikan melebihi saldo tabungan." << endl;
                    }
                } else {
                    cout << "PIN yang dimasukkan salah." << endl;
                }
            } else {
                cout << "Operasi penarikan dibatalkan." << endl;
            }
        } else {
            cout << "Tabungan tidak ditemukan atau bukan milik user ini." << endl;
        }
    } else {
        cerr << "Gagal membuka file tabungan.csv." << endl;
    }
}

void swap(Tabungan& a, Tabungan& b) {
    // Menukar dua objek Tabungan menggunakan variabel sementara
    Tabungan temp = a;
    a = b;
    b = temp;
}

int partition(vector<Tabungan>& tabungans, int low, int high) {
    long int pivot = tabungans[high].jumlah;    // Menggunakan jumlah tabungan sebagai pivot
    int i = low - 1;

    // Melakukan partisi terhadap vector tabungan
    for (int j = low; j < high; j++) {
        if (tabungans[j].jumlah > pivot) {
            i++;
            swap(tabungans[i], tabungans[j]);    // Menukar elemen jika jumlah tabungan lebih besar dari pivot
        }
    }
    // Menukar elemen pivot dengan elemen di indeks i+1
    swap(tabungans[i + 1], tabungans[high]);

    // Mengembalikan indeks pivot yang baru
    return i + 1;
}

void quickSort(vector<Tabungan>& tabungans, int low, int high) {
    // Algoritma quicksort untuk mengurutkan vector tabungan
    if (low < high) {
        int pi = partition(tabungans, low, high);

        // Rekursif untuk mengurutkan subarray sebelum dan setelah pivot
        quickSort(tabungans, low, pi - 1);
        quickSort(tabungans, pi + 1, high);
    }
}

int binarySearchById(const vector<Tabungan>& tabungans, int left, int right, int target) {
    // Algoritma pencarian biner untuk mencari ID target dalam vector tabungan
    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (tabungans[mid].id == target) {
            return mid;     // Mengembalikan indeks jika ID ditemukan
        } else if (tabungans[mid].id < target) {
            left = mid + 1;     // Pindah ke bagian kanan setengah array jika ID di tengah lebih kecil dari target
        } else {
            right = mid - 1;    // Pindah ke bagian kiri setengah array jika ID di tengah lebih besar dari target
        }
    }

    return -1;  // Mengembalikan -1 jika ID tidak ditemukan
}

bool checkMaximalTabungan(int userId) {
    ifstream infile("tabungan.csv");    // Membuka file tabungan.csv
    
    // Memeriksa apakah file tabungan.csv berhasil dibuka
    if (!infile.is_open()) {
        cerr << "Gagal membuka file tabungan." << endl;
        return false;
    }


    string line;
    getline(infile, line);
    while (getline(infile, line)) {
        stringstream ss(line);
        string field;
        vector<string> fields;

        while (getline(ss, field, ',')) {
            fields.push_back(field);
        }

        // Memeriksa apakah baris tabungan terkait dengan user yang sesuai
        if (stoi(fields[1]) == userId) {
            long int jumlah = stol(fields[4]);
            int maximal_tabungan = stoi(fields[5]);

            // Memeriksa apakah jumlah tabungan telah mencapai atau melebihi batas maksimum
            if (maximal_tabungan != -1 && jumlah >= maximal_tabungan) {
                infile.close();
                return true;
            }
        }
    }

    infile.close();
    return false;
}

void seeSavingAchieveTarget(int userId, const string& username) {
    ifstream tabunganFile("tabungan.csv");  // Membuka file tabungan.csv
    vector<Tabungan> tabungans;
    string line;

    // Memeriksa apakah file tabungan berhasil dibuka
    if (!tabunganFile.is_open()) {
        cerr << "Gagal membuka file tabungan.csv" << endl;
        return;
    }

    getline(tabunganFile, line);    // Membaca header file tabungan.csv

    // Memuat data tabungan dari file CSV
    while (getline(tabunganFile, line)) {
        stringstream ss(line);
        Tabungan tabungan;
        string field;

        getline(ss, field, ',');
        tabungan.id = stoi(field);

        getline(ss, field, ',');
        tabungan.userId = stoi(field);

        getline(ss, tabungan.namaTabungan, ',');

        getline(ss, tabungan.pin, ',');

        getline(ss, field, ',');
        tabungan.jumlah = stol(field);

        getline(ss, field, ',');
        tabungan.maximal_tabungan = stoi(field);

        getline(ss, tabungan.target_tabungan, ',');

        tabungans.push_back(tabungan);  // Menambahkan tabungan ke dalam vektor tabungans
    }
    tabunganFile.close();   // Menutup file

    // Membuat vektor untuk menyimpan tabungan yang mencapai target
    vector<Tabungan> tabunganMencapaiTarget;
    for (const Tabungan& tabungan : tabungans) {
        if (tabungan.userId == userId && tabungan.jumlah >= tabungan.maximal_tabungan) {
            tabunganMencapaiTarget.push_back(tabungan);
        }
    }

    // Menampilkan response jika tidak ada tabungan yang mencapai target
    if (tabunganMencapaiTarget.empty()) {
        cout << "Tidak ada tabungan yang mencapai target." << endl;
        return;
    }

    // Menampilkan tabungan yang mencapai target
    cout << "Tabungan yang mencapai target:" << endl;
    cout << "------------------------------------------------------------------------" << endl;
    cout << left << setw(6) << "ID" << setw(18) << "Nama Tabungan" << setw(15) << "Jumlah" << setw(20) << "Maximal Jumlah" << endl;
    cout << "------------------------------------------------------------------------" << endl;

    for (const Tabungan& tabungan : tabunganMencapaiTarget) {
        cout << left << setw(6) << tabungan.id << setw(18) << tabungan.namaTabungan << setw(15) << tabungan.jumlah << setw(20) << tabungan.maximal_tabungan << endl;
    }
    cout << "------------------------------------------------------------------------" << endl;


    char pilihan;
    cout << "Apakah Anda ingin menarik semua saldo pada tabungan yang mencapai target? [y//n]: ";
    cin >> pilihan; // Input

    // Memproses pilihan user
    if (pilihan == 'n' || pilihan == 'N') {
        cout << "Kembali ke menu utama." << endl;
        return;
    } else if (pilihan == 'y' || pilihan == 'Y') {
        int idTabungan;
        cout << "Masukkan ID tabungan yang ingin Anda tarik: ";
        
        // Memvalidasi input ID tabungan
        while (!(cin >> idTabungan)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid. Masukkan ID tabungan: ";
        }

        bool found = false;
        for (auto it = tabungans.begin(); it != tabungans.end(); ++it) {
            if (it->id == idTabungan && it->userId == userId && it->jumlah >= it->maximal_tabungan) {
                tabungans.erase(it);
                found = true;
                break;
            }
        }

        // Jika tabungan yang valid ditemukan
        if (found) {
            ifstream transaksiFile("transaksi.csv");
            vector<Transaction> transactions;
            string line;

            // Membuka file transaksi
            if (transaksiFile.is_open()) {
                getline(transaksiFile, line); 
                while (getline(transaksiFile, line)) {
                    stringstream ss(line);
                    Transaction transaction;
                    string field;

                    getline(ss, field, ',');
                    transaction.id = stoi(field);

                    getline(ss, field, ',');
                    transaction.tabunganId = stoi(field);

                    getline(ss, field, ',');
                    transaction.userId = stoi(field); 

                    getline(ss, transaction.type, ','); 

                    getline(ss, field, ',');
                    transaction.jumlah = stol(field);

                    getline(ss, transaction.tanggal, ',');

                    transactions.push_back(transaction);
                }
                transaksiFile.close();

                // Menghapus transaksi yang terkait dengan tabungan yang akan ditarik
                transactions.erase(remove_if(transactions.begin(), transactions.end(),
                    [idTabungan](const Transaction& trans) { return trans.tabunganId == idTabungan; }),
                    transactions.end());

                ofstream transaksiOutFile("transaksi.csv");
                if (transaksiOutFile.is_open()) {
                    transaksiOutFile << "id,tabungan_id,user_id,type,jumlah,tanggal\n";
                    for (const Transaction& transaction : transactions) {
                        transaksiOutFile << transaction.id << ","
                                         << transaction.tabunganId << ","
                                         << transaction.userId << ","
                                         << transaction.type << ","
                                         << transaction.jumlah << ","
                                         << transaction.tanggal << "\n";
                    }
                    transaksiOutFile.close();
                } else {
                    cerr << "Gagal membuka file transaksi.csv" << endl;
                    return;
                }

                // Menulis kembali tabungan ke file
                ofstream tabunganOutFile("tabungan.csv");

                if (!tabunganOutFile.is_open()) {
                    cerr << "Gagal membuka file tabungan.csv" << endl;
                    return;
                }

                tabunganOutFile << "id,user_id,nama_tabungan,pin_tabungan,jumlah,maximal_tabungan,target_tabungan\n";

                for (const Tabungan& tabungan : tabungans) {
                    tabunganOutFile << tabungan.id << ","
                                    << tabungan.userId << ","
                                    << tabungan.namaTabungan << ","
                                    << tabungan.pin << ","
                                    << tabungan.jumlah << ","
                                    << tabungan.maximal_tabungan << ","
                                    << tabungan.target_tabungan << "\n";
                }

                tabunganOutFile.close();

                cout << "Tabungan berhasil ditarik." << endl;
            } else {
                cerr << "Gagal membuka file transaksi.csv" << endl;
                return;
            }
        } else {
            cout << "Tabungan tidak ditemukan atau tidak memenuhi syarat untuk ditarik." << endl;
        }
    } else {
        cout << "Pilihan tidak valid. Kembali ke menu utama." << endl;
        return;
    }
}
