#include <iostream>
#include <string>
#include <iomanip>
#include<stdlib.h>
using namespace std;

class phuong_tien {
private:
    int ma_xe;
    string loai_xe;
    int gio_vao;
    int tg_do;

public:
    phuong_tien();
    phuong_tien(int ma, string loai, int gio, int tg);

    int lay_ma_xe() const;
    string lay_loai_xe() const;
    double tinh_phi() const;
    void hien_thi() const;
};

phuong_tien::phuong_tien() : ma_xe(0), loai_xe(""), gio_vao(0), tg_do(0) {}

phuong_tien::phuong_tien(int ma, string loai, int gio, int tg)
    : ma_xe(ma), loai_xe(loai), gio_vao(gio), tg_do(tg) {
}

int phuong_tien::lay_ma_xe() const { return ma_xe; }
string phuong_tien::lay_loai_xe() const { return loai_xe; }

double phuong_tien::tinh_phi() const {
    double gia = (loai_xe == "Car") ? 6.0 : (loai_xe == "Motorcycle") ? 4.0 : 2.0;
    bool cao_diem = (gio_vao >= 6 && gio_vao <= 9) || (gio_vao >= 15 && gio_vao <= 18);
    double tong = 0;
    for (int h = 0; h < tg_do; ++h) {
        if (h < 2) tong += gia * (cao_diem ? 1.30 : 0.95);
        else       tong += gia * 0.90;
    }
    return tong;
}

void phuong_tien::hien_thi() const {
    cout << "  ID: " << ma_xe << " | " << loai_xe
        << " | " << gio_vao << "h | " << tg_do << "h"
        << " | Phi: $" << fixed << setprecision(2) << tinh_phi() << "\n";
}

class bai_xe {
private:
    int ma_bai;
    char kich_thuoc;
    int suc_chua;
    static const int max_xe = 3;
	int so_xe_hien_tai;

public:
    phuong_tien** phuong_tiens;
    bool* co;
    bai_xe();
    bai_xe(int ma, char kt) ;


    int lay_ma() const;
    bool xoa(int ma_xe);
    bool chua(int ma_xe) const;
    void hien_thi() const;
    bool co_loai(string loai, int& dem, double& tong) const;
    bool bai_day() const;
	bool them_xe_vao(const phuong_tien& v);
    int tim_chi_so_xe(int id_xe) const;
    void doi_kich_thuoc(int suc_chua_moi);
	~bai_xe() {
		for (int i = 0; i < suc_chua; ++i) {
			delete phuong_tiens[i];
		}
        delete[] phuong_tiens;
		delete[] co;
	}
    bai_xe operator = (const bai_xe& a) {
        for (int i = 0; i < max_xe; ++i) {
            this->co[i] = a.co[i];
            this->phuong_tiens[i] = a.phuong_tiens[i];
        }
    }
};

bai_xe::bai_xe() : so_xe_hien_tai(0), ma_bai(0), kich_thuoc('S'), suc_chua(0), phuong_tiens(nullptr), co(nullptr) {}

bai_xe::bai_xe(int ma, char kt) : so_xe_hien_tai(0), ma_bai(ma), kich_thuoc(kt),
suc_chua(kt == 'S' ? 1 : kt == 'M' ? 2 : 3), phuong_tiens(new phuong_tien*[suc_chua]), co(new bool[suc_chua]) {
	for (int i = 0; i < suc_chua; ++i) {
		phuong_tiens[i] = nullptr;
        co[i] = false;
	}
}

int bai_xe::lay_ma() const { return ma_bai; }

bool bai_xe::xoa(int ma_xe) {
    for (int i = 0; i < suc_chua; ++i) {
		if (co[i] && phuong_tiens[i]->lay_ma_xe() == ma_xe) {
			delete phuong_tiens[i];
            for (int j = i; j < suc_chua - 1; ++j) {
                phuong_tiens[j] = phuong_tiens[j + 1];
                co[j] = co[j + 1];
            }
            phuong_tiens[so_xe_hien_tai - 1] = nullptr;
            so_xe_hien_tai--;
            return true;
		}
    }


    return false;
}

bool bai_xe::chua(int ma_xe) const {
	for (int i = 0; i < suc_chua; ++i) {
		if (co[i] && phuong_tiens[i]->lay_ma_xe() == ma_xe) {
			return true;
		}
	}
    return false;
}

void bai_xe::hien_thi() const {
    int cap = (kich_thuoc == 'S') ? 1 : (kich_thuoc == 'M') ? 2 : 3;
    int so_xe = 0;
    for (int i = 0; i < suc_chua; ++i) {
        if (co[i]) {
            ++so_xe;
        }
    }
    cout << "Bai " << ma_bai << " [" << kich_thuoc << "] "
        << so_xe << "/" << cap << " xe\n";
    for(int i = 0 ; i < suc_chua ; ++i) {
        if (co[i]) {
            phuong_tiens[i]->hien_thi();
        }
	}
}

bool bai_xe::co_loai(string loai, int& dem, double& tong) const {
    dem = 0; tong = 0;
    for (int i = 0; i < suc_chua; ++i) {
        if (co[i] && phuong_tiens[i]->lay_loai_xe() == loai) {
            ++dem;
            tong += phuong_tiens[i]->tinh_phi();
        }
    }
    return dem > 0;
}

bool bai_xe::bai_day() const {
	for (int i = 0; i < suc_chua; ++i) {
		if (!co[i]) {
			return false;
		}
	}
	return true;
}

bool bai_xe::them_xe_vao(const phuong_tien& v) {
	for (int i = 0; i < suc_chua; ++i) {
		if (!co[i]) {
			phuong_tiens[i] = new phuong_tien(v);
			co[i] = true;
			so_xe_hien_tai++;
			return true;
		}
	}
    so_xe_hien_tai = suc_chua;
	doi_kich_thuoc(suc_chua * 2);
    for (int i = 0; i < suc_chua; ++i) {
        if (!co[i]) {
            phuong_tiens[i] = new phuong_tien(v);
            co[i] = true;
            so_xe_hien_tai++;
            return true;
        }
    }
	return false;
}

int bai_xe::tim_chi_so_xe(int id_xe) const {
	for (int i = 0; i < max_xe; ++i) {
		if (co[i] && phuong_tiens[i]->lay_ma_xe() == id_xe) {
			return i;
		}
	}
	return -1;
}


void bai_xe::doi_kich_thuoc(int suc_chua_moi) {
	phuong_tien** phuong_tiens_moi = new phuong_tien * [suc_chua_moi];
	for (int i = 0; i < suc_chua; ++i) {
		phuong_tiens_moi[i] = phuong_tiens[i];
	}
    for (int i = 0; i < suc_chua; ++i) {
		delete phuong_tiens[i];
	}
	phuong_tiens = phuong_tiens_moi;


    suc_chua = suc_chua_moi;
    if (suc_chua == 1) {
        kich_thuoc = 'S';
    } else if (suc_chua == 2) {
        kich_thuoc = 'M';
    } else {
        kich_thuoc = 'L';
    }
}

class SystemInterface {
private:
    int suc_chua_bai;
    bool* co;
	bai_xe **bai_xes;

   
	int so_bai_hien_tai;

public:
    SystemInterface();
	bool ton_tai_bai(int ma) const;
	bool ton_tai_xe(int ma) const;
	int sinh_id() const;
    void runMenu();
	void thay_doi_kich_thuoc_bai(int suc_chua_moi);
	~SystemInterface() {
		for (int i = 0; i < suc_chua_bai; ++i) {
			delete bai_xes[i];
		}
		delete[] bai_xes;
		delete[] co;
	}
};

SystemInterface::SystemInterface() : suc_chua_bai(5), so_bai_hien_tai(0), co(new bool[suc_chua_bai]), bai_xes(new bai_xe*[suc_chua_bai]) {
    for (int i = 0; i < suc_chua_bai; ++i) {
        co[i] = false;
		bai_xes[i] = nullptr;
    }
}

bool SystemInterface::ton_tai_bai(int ma) const {
	for (int i = 0; i < suc_chua_bai; ++i) {
		if (co[i] && bai_xes[i]->lay_ma() == ma) {
			return true;
		}
	}
	return false;
}
bool SystemInterface::ton_tai_xe(int ma) const {
	for (int i = 0; i < suc_chua_bai; ++i) {
		if (co[i] && bai_xes[i]->tim_chi_so_xe(ma) != -1) {
			return true;
		}
	}
	return false;
}
int SystemInterface::sinh_id() const {
    srand(time(0));
    bool sinhma = true;
    int ma;
    while (sinhma == true) {
		ma = rand() % 10000;
        if(!ton_tai_bai(ma)) {
            sinhma = false;
        }
    }
    return ma;
}

void SystemInterface::thay_doi_kich_thuoc_bai(int suc_chua_moi) {
	bai_xe** bai_xes_moi = new bai_xe * [suc_chua_moi];
	bool* co_moi = new bool[suc_chua_moi];
    for (int i = 0; i < suc_chua_bai; i++) {
        bai_xes_moi[i] = bai_xes[i];    
		co_moi[i] = co[i];
		delete bai_xes[i];
    }
	bai_xes = bai_xes_moi;
	co = co_moi;
}

void SystemInterface::runMenu() {
    int chon;
    while (true) {
        cout << "\n===== QUAN LY BAI DO XE =====\n";
        cout << "1. Tao bai do\n";
        cout << "2. Them xe\n";
        cout << "3. Xoa bai do\n";
        cout << "4. Lay xe ra\n";
        cout << "5. Hien thi tat ca bai do\n";
        cout << "6. Hien thi xe theo bai\n";
        cout << "7. Liet ke xe theo loai\n";
        cout << "8. Bao cao phi trung binh\n";
        cout << "9. Thoat\n";
        cout << "Chon: ";
        cin >> chon;
        //các e chuyển sang lệnh switch cho thầy
        switch (chon) {
            case 1: {
                int ma; char kt;
				if (so_bai_hien_tai >= suc_chua_bai) {
					thay_doi_kich_thuoc_bai(suc_chua_bai * 2);
					break;
				}
                cout << "Ma bai: "; cin >> ma;
				if (ton_tai_bai(ma)) {
					cout << "Bai da ton tai\n";
					ma = sinh_id();
				}
                cout << "Kich thuoc (S/M/L): "; cin >> kt;
				for (int i = 0; i < suc_chua_bai; ++i) {
					if (!co[i]) {
						bai_xes[i] = new bai_xe(ma, kt);
						co[i] = true;
						so_bai_hien_tai++;
						cout << "Da tao bai " << ma << "\n";
						break;
					}
				}
                break;
            }
            case 2: {
                int ma, gio, tg; string loai;
                cout << "Ma xe: "; cin >> ma;
                if (ton_tai_xe(ma)) {
					cout << "Xe da ton tai\n";
					ma = sinh_id();
                }
                cout << "Loai (Car/Motorcycle/Bicycle): "; cin >> loai;
                cout << "Gio vao (0-23): "; cin >> gio;
                cout << "Thoi luong (1-23): "; cin >> tg;
                phuong_tien v(ma, loai, gio, tg);

				for (int i = 0; i < suc_chua_bai; ++i) {
					if (!co[i]) {
						bai_xes[i]->them_xe_vao(v);
						cout << "Da them xe " << ma << " vao bai " << bai_xes[i]->lay_ma() << "\n";
                        break;
					}
				}
                cout << "Het bai r";
                break;
            }
            case 3: {
                int ma;
                cout << "Ma bai can xoa: "; cin >> ma;
				for (int i = 0; i < suc_chua_bai; ++i) {
                    if (co[i] && bai_xes[i].lay_ma() == ma) {
						for (int j = i; j < max_bai; ++j) {
							bai_xes[j] = bai_xes[j + 1];
							co[j] = co[j + 1];
						}
						cout << "Da xoa bai " << ma << "\n";
                        break;
                    }
                }
				cout << "Ko tim thay bai " << ma << "\n";
                break;
            }
            case 4: {
                int ma;
                cout << "Ma xe can lay ra: "; cin >> ma;
                for (int i = 0; i < max_bai; ++i) {
                    for (int j = 0; j < 3; ++j) {
						if (bai_xes[i].phuong_tiens[j]->lay_ma_xe() == ma) {
							bai_xes[i].xoa(ma);
							cout << "Da lay xe ra\n";
							break;
						}
                    }
                }
				cout << "Khong tim thay xe";
                break;
            }
            case 5: {
				for (int i = 0; i < so_bai_hien_tai; ++i) {
                    if (co[i]) {
                        bai_xes[i].hien_thi();
                    }
				}
                break;
            }
            case 6: {
                int ma;
                cout << "Ma bai: "; cin >> ma;
				for (int i = 0; i < max_bai; ++i) {
					if (co[i] && bai_xes[i].lay_ma() == ma) {
						bai_xes[i].hien_thi();
						break;
					}
				}
                cout << "Ko tim thay bai";
                break;
            }
            case 7: {
                string loai;
                cout << "Loai xe: "; cin >> loai;
                int dem; double tong;
				for (int i = 0; i < max_bai; ++i) {
					if (co[i] && bai_xes[i].co_loai(loai, dem, tong)) {
						bai_xes[i].hien_thi();
					}
				}
                break;
            }
            case 8: {
                string loai_arr[3] = { "Car", "Motorcycle", "Bicycle" };

                for (int i = 0; i < 3; ++i) {
					int dem = 0; double tong = 0;
                    for (int j = 0; j < max_bai; ++j) {
                        if(co[j]){
                            int d, double s;
							bai_xes[j].co_loai(loai_arr[i], d, s);
                            dem += d; tong += s;
                        }
                    }
					cout << loai_arr[i] << ": ";
					if (dem == 0) cout << "khong co xe\n";
					else cout << dem << " xe | TB: $" << fixed << setprecision(2) << tong / dem << "\n";
                }
                break;
            }
            case 9: {
                return;
            }
        }
    }
}

int main() {
    SystemInterface sys;
    sys.runMenu();
    return 0;
}