#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include <regex>

using namespace std;

// ==================== TEMPLATE CLASSES ====================
template<typename T>
class DanhSach {
private:
    vector<T> data;
public:
    void them(const T& item) { data.push_back(item); }
    void xoa(int index) { if (index >= 0 && index < data.size()) data.erase(data.begin() + index); }
    T& lay(int index) { return data[index]; }
    int kichThuoc() const { return data.size(); }
    vector<T>& layDuLieu() { return data; }
    const vector<T>& layDuLieu() const { return data; }
    void xoaTatCa() { data.clear(); }
    bool rong() const { return data.empty(); }
};

// ==================== UTILITY FUNCTIONS ====================
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void clearScreen() {
    system("cls");
}

void veKhung(int x, int y, int w, int h) {
    COORD coord;
    coord.X = x; coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << char(201);
    for (int i = 0; i < w - 2; i++) cout << char(205);
    cout << char(187);

    for (int i = 1; i < h - 1; i++) {
        coord.Y = y + i;
        coord.X = x;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        cout << char(186);
        coord.X = x + w - 1;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        cout << char(186);
    }

    coord.X = x; coord.Y = y + h - 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << char(200);
    for (int i = 0; i < w - 2; i++) cout << char(205);
    cout << char(188);
}

string nhapMatKhau() {
    string pass = "";
    char ch;
    while (true) {
        ch = _getch();
        if (ch == 13) break; // Enter
        if (ch == 27) return "ESC"; // ESC
        if (ch == 8) { // Backspace
            if (!pass.empty()) {
                pass.pop_back();
                cout << "\b \b";
            }
        }
        else {
            pass += ch;
            cout << "*";
        }
    }
    return pass;
}

bool kiemTraChuoiRong(const string& str) {
    return str.empty() || str.find_first_not_of(' ') == string::npos;
}

bool kiemTraKyTuDacBiet(const string& str) {
    regex pattern("^[a-zA-Z0-9 ]+$");
    return !regex_match(str, pattern);
}

bool kiemTraNgayHopLe(const string& ngay) {
    // Format: dd/mm/yyyy
    regex pattern("^(0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[0-2])/([0-9]{4})$");
    if (!regex_match(ngay, pattern)) return false;

    stringstream ss(ngay);
    int day, month, year;
    char sep;
    ss >> day >> sep >> month >> sep >> year;

    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) daysInMonth[1] = 29;

    if (day > daysInMonth[month - 1]) return false;
    return true;
}

// ==================== CLASS DEFINITIONS ====================
class KhachHang {
private:
    int soThuTu;
    string cmnd;
    string hoTen;
public:
    KhachHang() : soThuTu(0), cmnd(""), hoTen("") {}
    KhachHang(int stt, string id, string ten) : soThuTu(stt), cmnd(id), hoTen(ten) {}

    int laySoThuTu() const { return soThuTu; }
    string layCMND() const { return cmnd; }
    string layHoTen() const { return hoTen; }

    void datSoThuTu(int stt) { soThuTu = stt; }
    void datCMND(const string& id) { cmnd = id; }
    void datHoTen(const string& ten) { hoTen = ten; }

    string toString() const {
        return to_string(soThuTu) + "|" + cmnd + "|" + hoTen;
    }

    static KhachHang fromString(const string& str) {
        stringstream ss(str);
        string token;
        vector<string> tokens;
        while (getline(ss, token, '|')) tokens.push_back(token);
        if (tokens.size() >= 3)
            return KhachHang(stoi(tokens[0]), tokens[1], tokens[2]);
        return KhachHang();
    }
};

class Ve {
private:
    string maVe;
    string maChuyenBay;
    KhachHang khachHang;
    int soGhe;
public:
    Ve() : maVe(""), maChuyenBay(""), soGhe(0) {}
    Ve(string maCB, KhachHang kh, int ghe) : maChuyenBay(maCB), khachHang(kh), soGhe(ghe) {
        maVe = maCB + "-" + to_string(ghe);
    }

    string layMaVe() const { return maVe; }
    string layMaChuyenBay() const { return maChuyenBay; }
    KhachHang layKhachHang() const { return khachHang; }
    int laySoGhe() const { return soGhe; }

    bool luuFile() const {
        try {
            ofstream file(maVe + ".txt");
            if (!file.is_open()) return false;

            file << "Ma ve: " << maVe << endl;
            file << "Ma chuyen bay: " << maChuyenBay << endl;
            file << "CMND: " << khachHang.layCMND() << endl;
            file << "Ho ten: " << khachHang.layHoTen() << endl;
            file << "So ghe: " << soGhe << endl;
            file.close();
            return true;
        }
        catch (...) {
            return false;
        }
    }

    string toString() const {
        return maVe + "|" + maChuyenBay + "|" + khachHang.layCMND() + "|" +
            khachHang.layHoTen() + "|" + to_string(soGhe);
    }

    static Ve fromString(const string& str) {
        stringstream ss(str);
        string token;
        vector<string> tokens;
        while (getline(ss, token, '|')) tokens.push_back(token);
        if (tokens.size() >= 5) {
            KhachHang kh(0, tokens[2], tokens[3]);
            Ve v(tokens[1], kh, stoi(tokens[4]));
            return v;
        }
        return Ve();
    }
};

class MayBay {
private:
    string soHieu;
    int soCho;
public:
    MayBay() : soHieu(""), soCho(0) {}
    MayBay(string sh, int sc) : soHieu(sh), soCho(sc) {}

    string laySoHieu() const { return soHieu; }
    int laySoCho() const { return soCho; }

    void datSoHieu(const string& sh) { soHieu = sh; }
    void datSoCho(int sc) { soCho = sc; }

    string toString() const {
        return soHieu + "|" + to_string(soCho);
    }

    static MayBay fromString(const string& str) {
        stringstream ss(str);
        string token;
        vector<string> tokens;
        while (getline(ss, token, '|')) tokens.push_back(token);
        if (tokens.size() >= 2)
            return MayBay(tokens[0], stoi(tokens[1]));
        return MayBay();
    }
};

class ChuyenBay {
private:
    string maChuyenBay;
    string soHieuMayBay;
    string ngayKhoiHanh;
    string sanBayDen;
    int trangThai;
    DanhSach<Ve> danhSachVe;
    DanhSach<int> danhSachGheTrong;
public:
    ChuyenBay() : maChuyenBay(""), soHieuMayBay(""), ngayKhoiHanh(""), sanBayDen(""), trangThai(1) {}
    ChuyenBay(string ma, string mayBay, string ngay, string sanBay, int soCho)
        : maChuyenBay(ma), soHieuMayBay(mayBay), ngayKhoiHanh(ngay), sanBayDen(sanBay), trangThai(1) {
        for (int i = 1; i <= soCho; i++) {
            danhSachGheTrong.them(i);
        }
    }

    string layMaChuyenBay() const { return maChuyenBay; }
    string laySoHieuMayBay() const { return soHieuMayBay; }
    string layNgayKhoiHanh() const { return ngayKhoiHanh; }
    string laySanBayDen() const { return sanBayDen; }
    int layTrangThai() const { return trangThai; }

    DanhSach<Ve>& layDanhSachVe() { return danhSachVe; }
    const DanhSach<Ve>& layDanhSachVe() const { return danhSachVe; }
    DanhSach<int>& layDanhSachGheTrong() { return danhSachGheTrong; }
    const DanhSach<int>& layDanhSachGheTrong() const { return danhSachGheTrong; }

    void datTrangThai(int tt) { trangThai = tt; }

    string layTenTrangThai() const {
        switch (trangThai) {
        case 0: return "Huy chuyen";
        case 1: return "Con ve";
        case 2: return "Het ve";
        case 3: return "Hoan tat";
        default: return "Khong xac dinh";
        }
    }

    bool themVe(const Ve& ve) {
        int soGhe = ve.laySoGhe();
        bool gheHopLe = false;
        int viTriGhe = -1;

        for (int i = 0; i < danhSachGheTrong.kichThuoc(); i++) {
            if (danhSachGheTrong.lay(i) == soGhe) {
                gheHopLe = true;
                viTriGhe = i;
                break;
            }
        }

        if (!gheHopLe) return false;

        if (!ve.luuFile()) return false;

        danhSachVe.them(ve);
        danhSachGheTrong.xoa(viTriGhe);

        if (danhSachGheTrong.rong()) trangThai = 2;

        return true;
    }

    bool xoaVe(const string& maVe) {
        for (int i = 0; i < danhSachVe.kichThuoc(); i++) {
            if (danhSachVe.lay(i).layMaVe() == maVe) {
                int soGhe = danhSachVe.lay(i).laySoGhe();
                danhSachGheTrong.them(soGhe);
                danhSachVe.xoa(i);
                if (trangThai == 2) trangThai = 1;
                return true;
            }
        }
        return false;
    }

    string toString() const {
        stringstream ss;
        ss << maChuyenBay << "|" << soHieuMayBay << "|" << ngayKhoiHanh << "|"
            << sanBayDen << "|" << trangThai;
        return ss.str();
    }

    static ChuyenBay fromString(const string& str, int soCho) {
        stringstream ss(str);
        string token;
        vector<string> tokens;
        while (getline(ss, token, '|')) tokens.push_back(token);
        if (tokens.size() >= 5) {
            ChuyenBay cb(tokens[0], tokens[1], tokens[2], tokens[3], soCho);
            cb.datTrangThai(stoi(tokens[4]));
            return cb;
        }
        return ChuyenBay();
    }
};

// ==================== FILE MANAGER ====================
class QuanLyFile {
public:
    static bool luuMayBay(const DanhSach<MayBay>& ds) {
        try {
            ofstream file("MayBay.txt");
            if (!file.is_open()) return false;

            for (int i = 0; i < ds.kichThuoc(); i++) {
                file << ds.layDuLieu()[i].toString() << endl;
            }
            file.close();
            return true;
        }
        catch (...) {
            return false;
        }
    }

    static bool docMayBay(DanhSach<MayBay>& ds) {
        ifstream file("MayBay.txt");
        if (!file.is_open()) return false;

        string line;
        ds.xoaTatCa();

        while (getline(file, line)) {
            if (!line.empty()) ds.them(MayBay::fromString(line));
        }
        file.close();
        return true;
    }

    static bool luuChuyenBay(const DanhSach<ChuyenBay>& ds) {
        try {
            ofstream file("ChuyenBay.txt");
            if (!file.is_open()) return false;

            for (int i = 0; i < ds.kichThuoc(); i++) {
                const ChuyenBay& cb = ds.layDuLieu()[i];
                file << cb.toString() << endl;

                for (int j = 0; j < cb.layDanhSachVe().kichThuoc(); j++) {
                    file << "VE:" << cb.layDanhSachVe().layDuLieu()[j].toString() << endl;
                }

                file << "GHE:";
                for (int j = 0; j < cb.layDanhSachGheTrong().kichThuoc(); j++) {
                    file << cb.layDanhSachGheTrong().layDuLieu()[j];
                    if (j < cb.layDanhSachGheTrong().kichThuoc() - 1) file << ",";
                }
                file << endl;
            }
            file.close();
            return true;
        }
        catch (...) {
            return false;
        }
    }

    static bool docChuyenBay(DanhSach<ChuyenBay>& ds, const DanhSach<MayBay>& dsMB) {
        ifstream file("ChuyenBay.txt");
        if (!file.is_open()) return false;

        string line;
        ds.xoaTatCa();
        ChuyenBay* cbHienTai = nullptr;

        while (getline(file, line)) {
            if (line.empty()) continue;

            if (line.substr(0, 3) == "VE:") {
                if (cbHienTai) {
                    Ve ve = Ve::fromString(line.substr(3));
                    cbHienTai->layDanhSachVe().them(ve);
                }
            }
            else if (line.substr(0, 4) == "GHE:") {
                if (cbHienTai) {
                    cbHienTai->layDanhSachGheTrong().xoaTatCa();
                    string gheStr = line.substr(4);
                    if (!gheStr.empty()) {
                        stringstream ss(gheStr);
                        string ghe;
                        while (getline(ss, ghe, ',')) {
                            if (!ghe.empty()) cbHienTai->layDanhSachGheTrong().them(stoi(ghe));
                        }
                    }
                }
            }
            else {
                int soCho = 0;
                stringstream ss(line);
                string token;
                vector<string> tokens;
                while (getline(ss, token, '|')) tokens.push_back(token);

                if (tokens.size() >= 2) {
                    string soHieu = tokens[1];
                    for (int i = 0; i < dsMB.kichThuoc(); i++) {
                        if (dsMB.layDuLieu()[i].laySoHieu() == soHieu) {
                            soCho = dsMB.layDuLieu()[i].laySoCho();
                            break;
                        }
                    }
                }
                ChuyenBay cb = ChuyenBay::fromString(line, soCho);
                ds.them(cb);
                cbHienTai = &ds.lay(ds.kichThuoc() - 1);
            }
        }
        file.close();
        return true;
    }

    static bool luuKhachHang(const DanhSach<KhachHang>& ds) {
        try {
            ofstream file("KhachHang.txt");
            if (!file.is_open()) return false;

            for (int i = 0; i < ds.kichThuoc(); i++) {
                file << ds.layDuLieu()[i].toString() << endl;
            }
            file.close();
            return true;
        }
        catch (...) {
            return false;
        }
    }

    static bool docKhachHang(DanhSach<KhachHang>& ds) {
        ifstream file("KhachHang.txt");
        if (!file.is_open()) return false;

        string line;
        ds.xoaTatCa();

        while (getline(file, line)) {
            if (!line.empty()) ds.them(KhachHang::fromString(line));
        }
        file.close();
        return true;
    }
};

// ==================== SYSTEM MANAGER ====================
class HeThong {
private:
    DanhSach<MayBay> danhSachMayBay;
    DanhSach<ChuyenBay> danhSachChuyenBay;
    DanhSach<KhachHang> danhSachKhachHang;
    map<string, string> taiKhoan;

    bool dangNhap() {
        clearScreen();
        setColor(11);
        veKhung(25, 5, 50, 10);

        COORD coord;
        coord.X = 30; coord.Y = 6;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        setColor(14);
        cout << "* * * DANG NHAP HE THONG * * *";

        int demSai = 0;
        while (demSai < 3) {
            coord.X = 30; coord.Y = 8;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            setColor(15);
            cout << "User: ";
            string user;
            getline(cin, user);

            coord.X = 30; coord.Y = 9;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            cout << "Password: ";
            string pass = nhapMatKhau();

            if (pass == "ESC") return false;

            // TC4: User không tồn tại
            if (taiKhoan.find(user) == taiKhoan.end()) {
                demSai++;
                setColor(12);
                coord.X = 30; coord.Y = 11;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
                cout << "Tai khoan khong ton tai! Con " << (3 - demSai) << " lan     ";
                Sleep(1500);
            }
            // TC1, TC2: Kiểm tra password
            else if (taiKhoan[user] == pass) {
                setColor(10);
                coord.X = 30; coord.Y = 11;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
                cout << "Dang nhap thanh cong!";
                Sleep(1000);
                return true;
            }
            else {
                demSai++;
                setColor(12);
                coord.X = 30; coord.Y = 11;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
                cout << "Sai mat khau! Con " << (3 - demSai) << " lan     ";
                Sleep(1500);
            }

            // Xóa dòng nhập
            coord.X = 30; coord.Y = 8;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            cout << "                                    ";
            coord.X = 30; coord.Y = 9;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            cout << "                                    ";
            coord.X = 30; coord.Y = 11;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            cout << "                                    ";
        }

        // TC3: Sai 3 lần
        setColor(12);
        coord.X = 30; coord.Y = 11;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        cout << "Sai 3 lan, thoat he thong!";
        Sleep(2000);
        return false;
    }

    // TC7-TC11: Quản lý máy bay
    void themMayBay() {
        clearScreen();
        setColor(14);
        cout << "\n\n================= THEM MAY BAY =================\n\n";

        setColor(15);
        cout << "Nhap so hieu may bay: ";
        string soHieu;
        getline(cin, soHieu);

        if (kiemTraChuoiRong(soHieu)) {
            setColor(12);
            cout << "Loi: Khong duoc de trong!\n";
            Sleep(2000);
            return;
        }

        // TC8: Kiểm tra trùng số hiệu
        for (int i = 0; i < danhSachMayBay.kichThuoc(); i++) {
            if (danhSachMayBay.lay(i).laySoHieu() == soHieu) {
                setColor(12);
                cout << "Loi: So hieu da ton tai!\n";
                Sleep(2000);
                return;
            }
        }

        cout << "Nhap so cho: ";
        int soCho;
        cin >> soCho;
        cin.ignore();

        // TC9: Kiểm tra số chỗ
        if (soCho <= 0) {
            setColor(12);
            cout << "Loi: So cho khong hop le!\n";
            Sleep(2000);
            return;
        }

        MayBay mb(soHieu, soCho);
        danhSachMayBay.them(mb);

        if (QuanLyFile::luuMayBay(danhSachMayBay)) {
            setColor(10);
            cout << "\nThem may bay thanh cong!\n";
        }
        else {
            setColor(12);
            cout << "\nLoi: Loi I/O khi luu file!\n";
        }
        Sleep(2000);
    }

    void hienThiDanhSachMayBay() {
        clearScreen();
        setColor(14);
        cout << "\n\n================= DANH SACH MAY BAY =================\n\n";

        // TC10: File trống
        if (danhSachMayBay.rong()) {
            setColor(12);
            cout << "Khong co may bay nao!\n";
        }
        else {
            // TC11: Hiển thị đầy đủ
            setColor(11);
            cout << left << setw(20) << "So hieu" << setw(15) << "So cho" << endl;
            cout << "----------------------------------------\n";
            setColor(15);
            for (int i = 0; i < danhSachMayBay.kichThuoc(); i++) {
                MayBay& mb = danhSachMayBay.lay(i);
                cout << left << setw(20) << mb.laySoHieu() << setw(15) << mb.laySoCho() << endl;
            }
        }

        setColor(14);
        cout << "\nNhan phim bat ky de tiep tuc...";
        _getch();
    }

    // TC12-TC18: Quản lý chuyến bay
    void themChuyenBay() {
        clearScreen();
        setColor(14);
        cout << "\n\n================= THEM CHUYEN BAY =================\n\n";

        setColor(15);
        cout << "Nhap ma chuyen bay: ";
        string maCB;
        getline(cin, maCB);

        if (kiemTraChuoiRong(maCB)) {
            setColor(12);
            cout << "Loi: Khong duoc de trong!\n";
            Sleep(2000);
            return;
        }

        // TC13: Kiểm tra trùng mã
        for (int i = 0; i < danhSachChuyenBay.kichThuoc(); i++) {
            if (danhSachChuyenBay.lay(i).layMaChuyenBay() == maCB) {
                setColor(12);
                cout << "Loi: Ma chuyen bay da ton tai!\n";
                Sleep(2000);
                return;
            }
        }

        cout << "Nhap so hieu may bay: ";
        string soHieu;
        getline(cin, soHieu);

        // TC14: Kiểm tra máy bay tồn tại
        MayBay* mb = nullptr;
        for (int i = 0; i < danhSachMayBay.kichThuoc(); i++) {
            if (danhSachMayBay.lay(i).laySoHieu() == soHieu) {
                mb = &danhSachMayBay.lay(i);
                break;
            }
        }

        if (!mb) {
            setColor(12);
            cout << "Loi: May bay khong ton tai!\n";
            Sleep(2000);
            return;
        }

        cout << "Nhap ngay khoi hanh (dd/mm/yyyy): ";
        string ngay;
        getline(cin, ngay);

        // TC15: Kiểm tra định dạng ngày
        if (!kiemTraNgayHopLe(ngay)) {
            setColor(12);
            cout << "Loi: Ngay khong hop le!\n";
            Sleep(2000);
            return;
        }

        cout << "Nhap san bay den: ";
        string sanBay;
        getline(cin, sanBay);

        if (kiemTraChuoiRong(sanBay)) {
            setColor(12);
            cout << "Loi: Khong duoc de trong!\n";
            Sleep(2000);
            return;
        }

        ChuyenBay cb(maCB, soHieu, ngay, sanBay, mb->laySoCho());
        danhSachChuyenBay.them(cb);

        if (QuanLyFile::luuChuyenBay(danhSachChuyenBay)) {
            setColor(10);
            cout << "\nThem chuyen bay thanh cong!\n";
        }
        else {
            setColor(12);
            cout << "\nLoi: Loi I/O khi luu file!\n";
        }
        Sleep(2000);
    }

    void hienThiDanhSachChuyenBay() {
        clearScreen();
        setColor(14);
        cout << "\n\n";
        cout << "========================================================================================================\n";
        cout << "|                              DANH SACH CHUYEN BAY                                                  |\n";
        cout << "========================================================================================================\n";

        // TC38: Danh sách rỗng
        if (danhSachChuyenBay.rong()) {
            setColor(12);
            cout << "Danh sach rong!\n";
        }
        else {
            // TC18: Hiển thị bảng đầy đủ
            setColor(11);
            cout << left << setw(15) << "Ma CB" << setw(15) << "May bay" << setw(15) << "Ngay KH"
                << setw(20) << "San bay den" << setw(15) << "Trang thai" << setw(10) << "So ve" << endl;
            cout << "--------------------------------------------------------------------------------------------------------\n";

            setColor(15);
            for (int i = 0; i < danhSachChuyenBay.kichThuoc(); i++) {
                ChuyenBay& cb = danhSachChuyenBay.lay(i);
                cout << left << setw(15) << cb.layMaChuyenBay()
                    << setw(15) << cb.laySoHieuMayBay()
                    << setw(15) << cb.layNgayKhoiHanh()
                    << setw(20) << cb.laySanBayDen()
                    << setw(15) << cb.layTenTrangThai()
                    << setw(10) << cb.layDanhSachVe().kichThuoc() << endl;
            }
        }
        cout << "========================================================================================================\n";

        setColor(14);
        cout << "\nNhan phim bat ky de tiep tuc...";
        _getch();
    }

    // TC19-TC24: Đặt vé
    void datVe() {
        clearScreen();
        setColor(14);
        cout << "\n\n================= DAT VE =================\n\n";

        setColor(15);
        cout << "Nhap ma chuyen bay: ";
        string maCB;
        getline(cin, maCB);

        if (kiemTraChuoiRong(maCB)) {
            setColor(12);
            cout << "Loi: Khong duoc de trong!\n";
            Sleep(2000);
            return;
        }

        ChuyenBay* cb = nullptr;
        for (int i = 0; i < danhSachChuyenBay.kichThuoc(); i++) {
            if (danhSachChuyenBay.lay(i).layMaChuyenBay() == maCB) {
                cb = &danhSachChuyenBay.lay(i);
                break;
            }
        }

        if (!cb) {
            setColor(12);
            cout << "Loi: Khong tim thay chuyen bay!\n";
            Sleep(2000);
            return;
        }

        // TC17: Chuyến bay hoàn tất
        if (cb->layTrangThai() == 3) {
            setColor(12);
            cout << "Loi: Khong the dat ve cho chuyen bay da hoan tat!\n";
            Sleep(2000);
            return;
        }

        if (cb->layTrangThai() == 0) {
            setColor(12);
            cout << "Loi: Chuyen bay da huy!\n";
            Sleep(2000);
            return;
        }

        // TC21: Chuyến bay hết vé
        if (cb->layTrangThai() == 2) {
            setColor(12);
            cout << "Loi: Het cho!\n";
            Sleep(2000);
            return;
        }

        cout << "\nDanh sach ghe trong: ";
        for (int i = 0; i < cb->layDanhSachGheTrong().kichThuoc(); i++) {
            cout << cb->layDanhSachGheTrong().lay(i);
            if (i < cb->layDanhSachGheTrong().kichThuoc() - 1) cout << ", ";
        }
        cout << endl;

        int soGhe;
        cout << "\nNhap so ghe: ";
        cin >> soGhe;
        cin.ignore();

        // TC20: Ghế đã được đặt
        bool gheHopLe = false;
        for (int i = 0; i < cb->layDanhSachGheTrong().kichThuoc(); i++) {
            if (cb->layDanhSachGheTrong().lay(i) == soGhe) {
                gheHopLe = true;
                break;
            }
        }

        if (!gheHopLe) {
            setColor(12);
            cout << "Loi: Ghe da duoc dat!\n";
            Sleep(2000);
            return;
        }

        // TC23: Nhập trống CMND
        cout << "Nhap CMND: ";
        string cmnd;
        getline(cin, cmnd);

        if (kiemTraChuoiRong(cmnd)) {
            setColor(12);
            cout << "Loi: Khong duoc de trong thong tin!\n";
            Sleep(2000);
            return;
        }

        // TC23: Nhập trống tên
        cout << "Nhap ho ten: ";
        string hoTen;
        getline(cin, hoTen);

        if (kiemTraChuoiRong(hoTen)) {
            setColor(12);
            cout << "Loi: Khong duoc de trong thong tin!\n";
            Sleep(2000);
            return;
        }

        // TC40: Tên có ký tự đặc biệt
        if (kiemTraKyTuDacBiet(hoTen)) {
            setColor(12);
            cout << "Loi: Ten khong hop le (chua ky tu dac biet)!\n";
            Sleep(2000);
            return;
        }

        KhachHang kh(danhSachKhachHang.kichThuoc() + 1, cmnd, hoTen);
        danhSachKhachHang.them(kh);
        QuanLyFile::luuKhachHang(danhSachKhachHang);

        setColor(10);
        cout << "\nDat ve thanh cong! Vui long den quay quan ly de xu ly thanh toan.\n";
        Sleep(2000);
    }

    // Xử lý đặt vé (từ danh sách chờ)
    void xuLyDatVe() {
        clearScreen();
        setColor(14);
        cout << "\n\n================= XU LY DAT VE =================\n\n";

        // TC32: Danh sách rỗng
        if (danhSachKhachHang.rong()) {
            setColor(12);
            cout << "Chua co khach hang nao!\n";
            Sleep(2000);
            return;
        }

        setColor(11);
        cout << left << setw(10) << "STT" << setw(15) << "CMND" << setw(30) << "Ho ten" << endl;
        cout << "---------------------------------------------------------------\n";
        setColor(15);
        for (int i = 0; i < danhSachKhachHang.kichThuoc(); i++) {
            KhachHang& kh = danhSachKhachHang.lay(i);
            cout << left << setw(10) << kh.laySoThuTu() << setw(15) << kh.layCMND() << setw(30) << kh.layHoTen() << endl;
        }

        cout << "\nNhap so thu tu khach hang: ";
        int stt;
        cin >> stt;
        cin.ignore();

        KhachHang* kh = nullptr;
        int viTri = -1;
        for (int i = 0; i < danhSachKhachHang.kichThuoc(); i++) {
            if (danhSachKhachHang.lay(i).laySoThuTu() == stt) {
                kh = &danhSachKhachHang.lay(i);
                viTri = i;
                break;
            }
        }

        if (!kh) {
            setColor(12);
            cout << "Loi: Khong tim thay khach hang!\n";
            Sleep(2000);
            return;
        }

        cout << "Nhap ma chuyen bay: ";
        string maCB;
        getline(cin, maCB);

        ChuyenBay* cb = nullptr;
        for (int i = 0; i < danhSachChuyenBay.kichThuoc(); i++) {
            if (danhSachChuyenBay.lay(i).layMaChuyenBay() == maCB) {
                cb = &danhSachChuyenBay.lay(i);
                break;
            }
        }

        if (!cb) {
            setColor(12);
            cout << "Loi: Khong tim thay chuyen bay!\n";
            Sleep(2000);
            return;
        }

        cout << "\nDanh sach ghe trong: ";
        for (int i = 0; i < cb->layDanhSachGheTrong().kichThuoc(); i++) {
            cout << cb->layDanhSachGheTrong().lay(i);
            if (i < cb->layDanhSachGheTrong().kichThuoc() - 1) cout << ", ";
        }
        cout << endl;

        cout << "\nNhap so ghe: ";
        int soGhe;
        cin >> soGhe;
        cin.ignore();

        // TC19: Đặt vé hợp lệ
        Ve ve(maCB, *kh, soGhe);
        if (cb->themVe(ve)) {
            danhSachKhachHang.xoa(viTri);
            QuanLyFile::luuKhachHang(danhSachKhachHang);

            // TC24: Lỗi I/O
            if (!QuanLyFile::luuChuyenBay(danhSachChuyenBay)) {
                setColor(12);
                cout << "\nLoi: Loi I/O khi luu file!\n";
            }
            else {
                setColor(10);
                cout << "\nXu ly dat ve thanh cong! Ma ve: " << ve.layMaVe() << endl;
                // TC16: Trạng thái tự động cập nhật khi hết vé
            }
            Sleep(2000);
        }
        else {
            setColor(12);
            cout << "\nLoi: Khong the dat ve! Ghe khong hop le hoac loi luu file ve.\n";
            Sleep(2000);
        }
    }

    // TC25-TC28: Trả vé
    void xuLyTraVe() {
        clearScreen();
        setColor(14);
        cout << "\n\n================= XU LY TRA VE =================\n\n";

        setColor(15);
        cout << "Nhap ma ve: ";
        string maVe;
        getline(cin, maVe);

        if (kiemTraChuoiRong(maVe)) {
            setColor(12);
            cout << "Loi: Khong duoc de trong!\n";
            Sleep(2000);
            return;
        }

        bool timThay = false;
        for (int i = 0; i < danhSachChuyenBay.kichThuoc(); i++) {
            ChuyenBay& cb = danhSachChuyenBay.lay(i);

            // TC26: Chuyến bay hoàn tất
            if (cb.layTrangThai() == 3) {
                // Kiểm tra xem vé có thuộc chuyến bay này không
                for (int j = 0; j < cb.layDanhSachVe().kichThuoc(); j++) {
                    if (cb.layDanhSachVe().lay(j).layMaVe() == maVe) {
                        setColor(12);
                        cout << "Loi: Khong the tra ve, chuyen bay da hoan tat!\n";
                        Sleep(2000);
                        return;
                    }
                }
                continue;
            }

            // TC25: Trả vé hợp lệ
            if (cb.xoaVe(maVe)) {
                if (!QuanLyFile::luuChuyenBay(danhSachChuyenBay)) {
                    setColor(12);
                    cout << "\nLoi: Loi I/O khi luu file!\n";
                }
                else {
                    setColor(10);
                    cout << "\nTra ve thanh cong!\n";
                }
                Sleep(2000);
                return;
            }
        }

        // TC27, TC28: Vé không tồn tại hoặc đã bị xóa
        setColor(12);
        cout << "Loi: Khong tim thay ve hoac ve khong hop le!\n";
        Sleep(2000);
    }

    // TC29-TC32: Quản lý khách hàng
    void themKhachHang() {
        clearScreen();
        setColor(14);
        cout << "\n\n================= THEM KHACH HANG =================\n\n";

        setColor(15);
        cout << "Nhap CMND: ";
        string cmnd;
        getline(cin, cmnd);

        if (kiemTraChuoiRong(cmnd)) {
            setColor(12);
            cout << "Loi: Khong duoc de trong!\n";
            Sleep(2000);
            return;
        }

        // TC30: CMND trùng
        for (int i = 0; i < danhSachKhachHang.kichThuoc(); i++) {
            if (danhSachKhachHang.lay(i).layCMND() == cmnd) {
                setColor(12);
                cout << "Loi: Khach hang da ton tai!\n";
                Sleep(2000);
                return;
            }
        }

        cout << "Nhap ho ten: ";
        string hoTen;
        getline(cin, hoTen);

        if (kiemTraChuoiRong(hoTen)) {
            setColor(12);
            cout << "Loi: Khong duoc de trong!\n";
            Sleep(2000);
            return;
        }

        if (kiemTraKyTuDacBiet(hoTen)) {
            setColor(12);
            cout << "Loi: Ten khong hop le!\n";
            Sleep(2000);
            return;
        }

        // TC29: Thêm khách hàng mới
        KhachHang kh(danhSachKhachHang.kichThuoc() + 1, cmnd, hoTen);
        danhSachKhachHang.them(kh);

        if (QuanLyFile::luuKhachHang(danhSachKhachHang)) {
            setColor(10);
            cout << "\nThem khach hang thanh cong!\n";
        }
        else {
            setColor(12);
            cout << "\nLoi: Loi I/O khi luu file!\n";
        }
        Sleep(2000);
    }

    void timKhachHangTheoCMND() {
        clearScreen();
        setColor(14);
        cout << "\n\n================= TIM KHACH HANG =================\n\n";

        setColor(15);
        cout << "Nhap CMND: ";
        string cmnd;
        getline(cin, cmnd);

        // TC31: Tìm khách hàng theo CMND
        for (int i = 0; i < danhSachKhachHang.kichThuoc(); i++) {
            if (danhSachKhachHang.lay(i).layCMND() == cmnd) {
                KhachHang& kh = danhSachKhachHang.lay(i);
                setColor(11);
                cout << "\nThong tin khach hang:\n";
                cout << "STT: " << kh.laySoThuTu() << endl;
                cout << "CMND: " << kh.layCMND() << endl;
                cout << "Ho ten: " << kh.layHoTen() << endl;

                setColor(14);
                cout << "\nNhan phim bat ky de tiep tuc...";
                _getch();
                return;
            }
        }

        setColor(12);
        cout << "Khong tim thay khach hang!\n";
        Sleep(2000);
    }

    // TC33-TC36: Thống kê
    void hienThiKhachTheoChuyenBay() {
        clearScreen();
        setColor(14);
        cout << "\n\n================= DANH SACH KHACH THEO CHUYEN BAY =================\n\n";

        setColor(15);
        cout << "Nhap ma chuyen bay: ";
        string maCB;
        getline(cin, maCB);

        ChuyenBay* cb = nullptr;
        for (int i = 0; i < danhSachChuyenBay.kichThuoc(); i++) {
            if (danhSachChuyenBay.lay(i).layMaChuyenBay() == maCB) {
                cb = &danhSachChuyenBay.lay(i);
                break;
            }
        }

        // TC36: Không tìm thấy mã chuyến bay
        if (!cb) {
            setColor(12);
            cout << "Loi: Khong ton tai chuyen bay!\n";
            Sleep(2000);
            return;
        }

        // TC33: Hiển thị danh sách vé
        cout << "\n";
        setColor(11);
        cout << left << setw(20) << "Ma ve" << setw(15) << "CMND" << setw(30) << "Ho ten" << setw(10) << "So ghe" << endl;
        cout << "-------------------------------------------------------------------------------\n";
        setColor(15);

        if (cb->layDanhSachVe().rong()) {
            cout << "Chua co khach hang nao!\n";
        }
        else {
            for (int i = 0; i < cb->layDanhSachVe().kichThuoc(); i++) {
                const Ve& ve = cb->layDanhSachVe().layDuLieu()[i];
                cout << left << setw(20) << ve.layMaVe()
                    << setw(15) << ve.layKhachHang().layCMND()
                    << setw(30) << ve.layKhachHang().layHoTen()
                    << setw(10) << ve.laySoGhe() << endl;
            }
        }

        setColor(14);
        cout << "\nNhan phim bat ky de tiep tuc...";
        _getch();
    }

    void hienThiGheTrongTheoChuyenBay() {
        clearScreen();
        setColor(14);
        cout << "\n\n================= DANH SACH GHE TRONG =================\n\n";

        setColor(15);
        cout << "Nhap ma chuyen bay: ";
        string maCB;
        getline(cin, maCB);

        ChuyenBay* cb = nullptr;
        for (int i = 0; i < danhSachChuyenBay.kichThuoc(); i++) {
            if (danhSachChuyenBay.lay(i).layMaChuyenBay() == maCB) {
                cb = &danhSachChuyenBay.lay(i);
                break;
            }
        }

        if (!cb) {
            setColor(12);
            cout << "Loi: Khong ton tai chuyen bay!\n";
            Sleep(2000);
            return;
        }

        // TC34: Hiển thị ghế trống
        cout << "\nTrang thai: " << cb->layTenTrangThai() << endl;
        cout << "So ghe trong: " << cb->layDanhSachGheTrong().kichThuoc() << endl;
        cout << "\nDanh sach ghe trong: ";

        if (cb->layDanhSachGheTrong().rong()) {
            cout << "Khong con ghe trong!\n";
        }
        else {
            for (int i = 0; i < cb->layDanhSachGheTrong().kichThuoc(); i++) {
                cout << cb->layDanhSachGheTrong().lay(i);
                if (i < cb->layDanhSachGheTrong().kichThuoc() - 1) cout << ", ";
            }
            cout << endl;
        }

        setColor(14);
        cout << "\nNhan phim bat ky de tiep tuc...";
        _getch();
    }

    void thongKeSoChuyenBayTheoMayBay() {
        clearScreen();
        setColor(14);
        cout << "\n\n================= THONG KE CHUYEN BAY THEO MAY BAY =================\n\n";

        setColor(15);
        cout << "Nhap so hieu may bay: ";
        string soHieu;
        getline(cin, soHieu);

        // TC35: Thống kê số chuyến bay
        int dem = 0;
        for (int i = 0; i < danhSachChuyenBay.kichThuoc(); i++) {
            if (danhSachChuyenBay.lay(i).laySoHieuMayBay() == soHieu) {
                dem++;
            }
        }

        setColor(11);
        cout << "\nMay bay " << soHieu << " da thuc hien: " << dem << " chuyen bay\n";

        setColor(14);
        cout << "\nNhan phim bat ky de tiep tuc...";
        _getch();
    }

    void menuQuanLyMayBay() {
        while (true) {
            clearScreen();
            setColor(14);
            cout << "\n\n";
            cout << "===================================\n";
            cout << "|      QUAN LY MAY BAY          |\n";
            cout << "===================================\n";
            setColor(15);
            cout << "1. Them may bay\n";
            cout << "2. Hien thi danh sach may bay\n";
            cout << "0. Quay lai\n";
            cout << "===================================\n";
            setColor(11);
            cout << "Chon chuc nang: ";

            int luaChon;
            cin >> luaChon;
            cin.ignore();

            switch (luaChon) {
            case 1:
                themMayBay();
                break;
            case 2:
                hienThiDanhSachMayBay();
                break;
            case 0:
                return;
            default:
                setColor(12);
                cout << "Lua chon khong hop le!\n";
                Sleep(1000);
            }
        }
    }

    void menuQuanLyChuyenBay() {
        while (true) {
            clearScreen();
            setColor(14);
            cout << "\n\n";
            cout << "===================================\n";
            cout << "|    QUAN LY CHUYEN BAY         |\n";
            cout << "===================================\n";
            setColor(15);
            cout << "1. Them chuyen bay\n";
            cout << "2. Hien thi danh sach chuyen bay\n";
            cout << "0. Quay lai\n";
            cout << "===================================\n";
            setColor(11);
            cout << "Chon chuc nang: ";

            int luaChon;
            cin >> luaChon;
            cin.ignore();

            switch (luaChon) {
            case 1:
                themChuyenBay();
                break;
            case 2:
                hienThiDanhSachChuyenBay();
                break;
            case 0:
                return;
            default:
                setColor(12);
                cout << "Lua chon khong hop le!\n";
                Sleep(1000);
            }
        }
    }

    void menuQuanLyKhachHang() {
        while (true) {
            clearScreen();
            setColor(14);
            cout << "\n\n";
            cout << "===================================\n";
            cout << "|    QUAN LY KHACH HANG         |\n";
            cout << "===================================\n";
            setColor(15);
            cout << "1. Them khach hang\n";
            cout << "2. Tim khach hang theo CMND\n";
            cout << "0. Quay lai\n";
            cout << "===================================\n";
            setColor(11);
            cout << "Chon chuc nang: ";

            int luaChon;
            cin >> luaChon;
            cin.ignore();

            switch (luaChon) {
            case 1:
                themKhachHang();
                break;
            case 2:
                timKhachHangTheoCMND();
                break;
            case 0:
                return;
            default:
                setColor(12);
                cout << "Lua chon khong hop le!\n";
                Sleep(1000);
            }
        }
    }

    void menuQuanLy() {
        while (true) {
            clearScreen();
            setColor(14);
            cout << "\n\n";
            cout << "===================================\n";
            cout << "|         MENU QUAN LY           |\n";
            cout << "===================================\n";
            setColor(15);
            cout << "1. Quan ly may bay\n";
            cout << "2. Quan ly chuyen bay\n";
            cout << "3. Quan ly khach hang\n";
            cout << "4. Xu ly dat ve\n";
            cout << "5. Xu ly tra ve\n";
            cout << "6. Thong ke\n";
            cout << "0. Thoat\n";
            cout << "===================================\n";
            setColor(11);
            cout << "Chon chuc nang: ";

            int luaChon;
            cin >> luaChon;
            cin.ignore();

            switch (luaChon) {
            case 1:
                menuQuanLyMayBay();
                break;
            case 2:
                menuQuanLyChuyenBay();
                break;
            case 3:
                menuQuanLyKhachHang();
                break;
            case 4:
                xuLyDatVe();
                break;
            case 5:
                xuLyTraVe();
                break;
            case 6:
                menuThongKe();
                break;
            case 0:
                return;
            default:
                setColor(12);
                cout << "Lua chon khong hop le!\n";
                Sleep(1000);
            }
        }
    }

    void menuThongKe() {
        while (true) {
            clearScreen();
            setColor(14);
            cout << "\n\n";
            cout << "===================================\n";
            cout << "|         MENU THONG KE          |\n";
            cout << "===================================\n";
            setColor(15);
            cout << "1. Hien thi danh sach khach theo chuyen bay\n";
            cout << "2. Hien thi danh sach ghe trong theo chuyen bay\n";
            cout << "3. Thong ke so chuyen bay theo may bay\n";
            cout << "0. Quay lai\n";
            cout << "===================================\n";
            setColor(11);
            cout << "Chon chuc nang: ";

            int luaChon;
            cin >> luaChon;
            cin.ignore();

            switch (luaChon) {
            case 1:
                hienThiKhachTheoChuyenBay();
                break;
            case 2:
                hienThiGheTrongTheoChuyenBay();
                break;
            case 3:
                thongKeSoChuyenBayTheoMayBay();
                break;
            case 0:
                return;
            default:
                setColor(12);
                cout << "Lua chon khong hop le!\n";
                Sleep(1000);
            }
        }
    }

public:
    HeThong() {
        // TC37: File bị thiếu - Kiểm tra và tạo file nếu cần
        if (!QuanLyFile::docMayBay(danhSachMayBay)) {
            setColor(12);
            cout << "Canh bao: Khong tim thay file MayBay.txt. Tao file moi.\n";
            QuanLyFile::luuMayBay(danhSachMayBay);
            Sleep(1500);
        }

        if (!QuanLyFile::docChuyenBay(danhSachChuyenBay, danhSachMayBay)) {
            setColor(12);
            cout << "Canh bao: Khong tim thay file ChuyenBay.txt. Tao file moi.\n";
            QuanLyFile::luuChuyenBay(danhSachChuyenBay);
            Sleep(1500);
        }

        if (!QuanLyFile::docKhachHang(danhSachKhachHang)) {
            setColor(12);
            cout << "Canh bao: Khong tim thay file KhachHang.txt. Tao file moi.\n";
            QuanLyFile::luuKhachHang(danhSachKhachHang);
            Sleep(1500);
        }

        // Đọc tài khoản
        ifstream file("Admin.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                string user, pass;
                if (getline(ss, user, '|') && getline(ss, pass)) {
                    taiKhoan[user] = pass;
                }
            }
            file.close();
        }

        // Tạo tài khoản mặc định
        if (taiKhoan.empty()) {
            taiKhoan["admin"] = "admin123";
            taiKhoan["user1"] = "123456";
            ofstream outFile("Admin.txt");
            if (outFile.is_open()) {
                for (auto& tk : taiKhoan) {
                    outFile << tk.first << "|" << tk.second << endl;
                }
                outFile.close();
            }
        }
    }

    void chay() {
        while (true) {
            clearScreen();
            setColor(14);
            cout << "\n\n";
            cout << "============================================\n";
            cout << "|   HE THONG QUAN LY CHUYEN BAY NOI DIA   |\n";
            cout << "============================================\n";
            setColor(15);
            cout << "1. Hien thi danh sach chuyen bay\n";
            cout << "2. Dat ve\n";
            cout << "3. Quan ly (can dang nhap)\n";
            cout << "0. Thoat\n";
            cout << "============================================\n";
            setColor(11);
            cout << "Chon chuc nang: ";

            int luaChon;
            cin >> luaChon;
            cin.ignore();

            switch (luaChon) {
            case 1:
                hienThiDanhSachChuyenBay();
                break;
            case 2:
                datVe();
                break;
            case 3:
                if (dangNhap()) {
                    menuQuanLy();
                }
                break;
            case 0:
                // TC41: Lưu dữ liệu và thoát an toàn
                setColor(10);
                cout << "\nDang luu du lieu...\n";
                QuanLyFile::luuMayBay(danhSachMayBay);
                QuanLyFile::luuChuyenBay(danhSachChuyenBay);
                QuanLyFile::luuKhachHang(danhSachKhachHang);
                cout << "Cam on ban da su dung he thong!\n";
                Sleep(1500);
                return;
            default:
                setColor(12);
                cout << "Lua chon khong hop le!\n";
                Sleep(1000);
            }
        }
    }
};

// ==================== MAIN ====================
int main() {
    // Set console to UTF-8
    SetConsoleOutputCP(CP_UTF8);

    HeThong heThong;
    heThong.chay();

    return 0;
}