#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h> 
#include <time.h> 
#define MAXTICKETS 100 
#define MAX_TRIPS 100 

struct Location {
    char name[50];       // ten dia diem
    char address[100];   // dia chi cua dia diem
};
struct Trip {
    char tripId[20]; // ma chuyen xe
    struct Location departure; // diem khoi hanh
    struct Location destination; // diem den
    char date[20]; // ngay gio chuyen xe
    int totalSeats; // tong so ghe
    int bookedSeats; // so ghe da dat
};
struct Trip trips[MAX_TRIPS] = {
    {
        "AA001",
        {"Ha Noi", "So 1 Trang Tien, Hoan Kiem, Ha Noi"},
        {"Hai Phong", "So 2 Le Hong Phong, Hai Phong"},
        "2024-07-01 08:00",
        40,
        10
    },
    {
        "AA002",
        {"Da Nang", "So 3 Nguyen Van Linh, Da Nang"},
        {"Hue", "So 4 Hung Vuong, Hue"},
        "2024-07-02 09:30",
        45,
        20
    },
    {
        "AA003",
        {"TP HCM", "So 5 Ly Tu Trong, Quan 1, TP HCM"},
        {"Nha Trang", "So 6 Tran Phu, Nha Trang"},
        "2024-07-03 07:45",
        50,
        15
    },
    {
        "AA004",
        {"Can Tho", "So 7 Cach Mang Thang 8, Can Tho"},
        {"Bac Lieu", "So 8 Tran Huynh, Bac Lieu"},
        "2024-07-04 10:00",
        35,
        12
    },
    {
        "AA005",
        {"Vinh", "So 9 Nguyen Du, TP Vinh"},
        {"Thanh Hoa", "So 10 Hac Thanh, Thanh Hoa"},
        "2024-07-05 13:15",
        30,
        5
    }
};

struct Passenger {
    char name[50]; // ten hanh khach
    char phone[20]; // so dien thoai hanh khach
};
struct Ticket {
    char ticketId[20]; // ma ve (sinh ngau nhien hoac tu dong)
    char tripId[20]; // ma chuyen xe
    struct Passenger passenger; // thong tin hanh khach
    int seatNumber; // so ghe da dat
    double price; // gia ve
    int paymentStatus; // trang thai thanh toan (0: chua thanh toan, 1: da thanh toan)
    char date[32]; // ngay gio dat ve
    int action; //trang thai ve (1: Khoa ve, 2: Huy ve)
};
struct Ticket tickets[MAXTICKETS] = {
    {
        "V001",
        "TX001",
        {"Nguyen Van A", "0912345678"},
        1,
        250000.0,
        1,
        "2024-06-28 07:30"
    },
    {
        "V002",
        "TX002",
        {"Tran Thi B", "0922233344"},
        5,
        300000.0,
        0,
        "2024-07-01 09:00"
    },
    {
        "V003",
        "TX003",
        {"Le Van C", "0933456789"},
        10,
        350000.0,
        1,
        "2024-07-02 09:30"
    },
    {
        "V004",
        "TX004",
        {"Pham Thi D", "0944556677"},
        2,
        200000.0,
        1,
        "2024-07-03 07:45"
    },
    {
        "V005",
        "TX005",
        {"Hoang Van E", "0955667788"},
        8,
        270000.0,
        0,
        "2024-07-04 10:00"
    }
};
int ticketCount = 5; // bien dem so luong ve
int tripCount = 5; // bien dem so luong chuyen xe

// khai bao ham
void inputString(char *prompt, char *str, int size); // nhap xau ky tu
int isTripIdUnique(const char *tripId); // kiem tra ma chuyen xe co duy nhat khong
int isEmptyString(const char *str); // kiem tra xau co trong khong
void showMenu(); // hien thi menu
void addTrip(); // them chuyen xe
void updateTripInfo(); // cap nhat thong tin chuyen xe
void bookTicket(); // dat ve
int isValidPhone(const char* phone); // kiem tra so dien thoai hop le khong
int findTripIndexById(const char* tripId); // tim chi so chuyen xe theo ma
int isSeatBooked(const char* tripId, int seatNumber, struct Ticket* tickets, int ticketCount); // kiem tra ghe da dat
void checkTicketStatus(struct Ticket tickets[], int ticketCount, struct Trip trips[], int tripCount); // kiem tra tinh trang ve
void listTrip(); // liet ke chuyen xe
int findTicketIndexById(const char *ticketId); // kiem tra ve da dat
void payTicket(); // Thanh toan ve xe
void statusTicketManagement(); //Quan li trang thai ve


int main() { 
    int choice; // bien lua chon
    while (1) { // vong lap menu
        showMenu(); // goi ham hien thi menu
        printf("Moi ban nhap lua chon: "); // thong bao nhap lua chon
        scanf("%d", &choice);
        getchar(); // doc ky tu enter con lai
        switch(choice){
            case 1:
                addTrip(); // goi ham them chuyen xe
                break;
            case 2:
                updateTripInfo(); // goi ham cap nhat chuyen xe
                break;
            case 3:
                bookTicket(); // goi ham dat ve
                break;
            case 4:
                checkTicketStatus(tickets, ticketCount, trips, tripCount); // kiem tra ve
                break;
            case 5:
                listTrip(); // liet ke chuyen xe
                break;
            case 6:
            	payTicket(); // thanh toan ve xe
            	break;
            case 7: 
            	statusTicketManagement(); // quan li trang thai ve
            	break;
            case 9:
                printf("Thoat chuong trinh...!\n"); // ket thuc
                return 0;
            default:
                printf("Loi! Moi ban nhap lai\n"); // lua chon khac
        }
    }
    return 0;
}

void inputString(char *prompt, char *str, int size) { // ham nhap xau
    while (1) {
        printf("%s", prompt);
        if (fgets(str, size, stdin) == NULL) {
            str[0] = '\0';
        } else {
            str[strcspn(str, "\n")] = '\0';
        }
        if (strlen(str) == 0) {
            printf("Khong duoc bo trong.\n");
            continue;
        }
        break;
    }
}

int isTripIdUnique(const char *tripId) { // kiem tra ma chuyen xe co duy nhat khong
    for (int i = 0; i < tripCount; i++) {
        if (strcmp(trips[i].tripId, tripId) == 0) {
            return 0;
        }
    }
    return 1;
}

int isEmptyString(const char *str) { // kiem tra xau rong
    return (str == NULL) || (strlen(str) == 0);
}

int isValidDateTime(const char *str) { // Ham check nhap ngay gio duoi dinh dang chuan : YYYY-MM-DD HH:MM:SS
    if (strlen(str) != 19) {  // Kiem tra tong so ki tu co trong mang
		return 0;
	}
    if (str[4] != '-' || str[7] != '-') { // Kiem tra vi tri dau '-'
		return 0;
	}
    if (str[10] != ' ') {  // Kiem tra vi tri dau cach
		return 0;
	}
    if (str[13] != ':' || str[16] != ':') { // Kiem tra vi tri dau ':'
		return 0;
	}
    for (int i = 0; i < 19; i++) {  // Kiem tra tat ca cac ki tu so
        if (i == 4 || i == 7 || i == 10 || i == 13 || i == 16) {
			continue;
		}
        if (!isdigit(str[i])) {
			return 0;
		}
    }

    return 1;
}


void showMenu() { // ham hien thi menu
    const char *menuLines[] = {
        "========== QUAN LY VE XE KHACH ==========",
        "| 1. Them chuyen xe moi                  |",
        "| 2. Cap nhat thong tin chuyen xe        |",
        "| 3. Dat ve                              |",
        "| 4. Kiem tra tinh trang ve              |",
        "| 5. Liet ke chuyen xe                   |",
        "| 6. Thanh toan ve                       |",
        "| 7. Quan ly trang thai ve (Lock, Cancel)|",
        "| 8. Bao cao thong ke va doanh thu       |",
        "| 9. Thoat                               |",
        "=========================================="
    };
    int n = sizeof(menuLines) / sizeof(menuLines[0]);
    for (int i = 0; i < n; i++) {
        printf("%s\n", menuLines[i]);
    }
}

void addTrip() { // ham them chuyen xe
    if (tripCount >= MAX_TRIPS) { // Kiem tra danh sach da day chua
        printf("Them chuyen xe that bai: Danh sach chuyen xe da day.\n");
        return;
    }

    struct Trip newTrip;
    char temp[100];

    // Nhap ma chuyen xe hop le (khong rong, khong trung)
    while (1) {
        inputString("Nhap ma chuyen xe (duy nhat): ", temp, sizeof(temp));
        if (isEmptyString(temp)) {
            printf("Ma chuyen xe khong duoc de trong. Vui long nhap lai.\n");
            continue;
        }
        if (!isTripIdUnique(temp)) {
            printf("Ma chuyen xe da ton tai. Vui long nhap lai.\n");
            continue;
        }
        strncpy(newTrip.tripId, temp, sizeof(newTrip.tripId) - 1);
        newTrip.tripId[sizeof(newTrip.tripId) - 1] = '\0';
        break;
    }

    // Nhap ten diem di (khong rong)
    while (1) {
        inputString("Nhap ten diem khoi hanh: ", newTrip.departure.name, sizeof(newTrip.departure.name));
        if (isEmptyString(newTrip.departure.name)) {
            printf("Ten diem khoi hanh khong duoc de trong. Vui long nhap lai.\n");
        } else {
            break;
        }
    }

    inputString("Nhap dia chi diem khoi hanh: ", newTrip.departure.address, sizeof(newTrip.departure.address));

    // Nhap ten diem den (khong rong)
    while (1) {
        inputString("Nhap ten diem den: ", newTrip.destination.name, sizeof(newTrip.destination.name));
        if (isEmptyString(newTrip.destination.name)) {
            printf("Ten diem den khong duoc de trong. Vui long nhap lai.\n");
        } else {
            break;
        }
    }

    inputString("Nhap dia chi diem den: ", newTrip.destination.address, sizeof(newTrip.destination.address));

    // Nhap ngay gio chuyen xe 
    while (1) {
    inputString("Nhap ngay gio chuyen xe (YYYY-MM-DD HH:MM:SS): ", 
                newTrip.date, sizeof(newTrip.date));

    if (isEmptyString(newTrip.date)) {
        printf("Ngay gio khong duoc de trong!\n");
        continue;
    }

    if (!isValidDateTime(newTrip.date)) {
        printf("Sai dinh dang! Vui long nhap dung dinh dang YYYY-MM-DD HH:MM:SS\n");
        continue;
    }

    break;
}


    // Nhap tong so ghe (phai > 0)
    while (1) {
        inputString("Nhap tong so ghe: ", temp, sizeof(temp));
        if (isEmptyString(temp)) {
            printf("Tong so ghe khong duoc de trong. Vui long nhap lai.\n");
            continue;
        }
        newTrip.totalSeats = atoi(temp);
        if (newTrip.totalSeats <= 0) {
            printf("Tong so ghe phai nhap dung dinh dang 1 so lon hon 0. Vui long nhap lai.\n");
            continue;
        }
        break;
    }

    newTrip.bookedSeats = 0; // Khoi tao so ghe da dat la 0

    // Kiem tra danh sach day lan cuoi (co the co race condition)
    if (tripCount < MAX_TRIPS) {
        trips[tripCount++] = newTrip; // Luu vao danh sach
        printf("Them chuyen xe thanh cong!\n");
    } else {
        printf("Them chuyen xe that bai: Danh sach chuyen xe da day.\n");
    }
}

void updateTripInfo() { // ham cap nhat thong tin chuyen xe
    char tripId[20]; // bien ma chuyen xe
    inputString("Nhap ma chuyen xe can cap nhat: ", tripId, sizeof(tripId));

    if (isEmptyString(tripId)) {
        printf("Ma chuyen xe khong duoc de trong.\n");
        return;
    }

    for (int i = 0; i < tripCount; ++i) {
        if (strcmp(trips[i].tripId, tripId) == 0) {
            char newDepartureName[50]; // khoi hanh moi
            char newDepartureAddr[100]; // dia chi khoi hanh moi
            char newDestinationName[50]; // diem den moi
            char newDestinationAddr[100]; // dia chi diem den moi
            char newDate[32]; // ngay gio moi
            char temp[32]; // bien tam
            int newTotalSeats; // tong so ghe moi

            inputString("Nhap ten diem khoi hanh moi: ", newDepartureName, sizeof(newDepartureName));
            if (isEmptyString(newDepartureName)) {
                printf("Ten diem khoi hanh khong duoc de trong.\n");
                return;
            }
            inputString("Nhap dia chi diem khoi hanh moi: ", newDepartureAddr, sizeof(newDepartureAddr));

            inputString("Nhap ten diem den moi: ", newDestinationName, sizeof(newDestinationName));
            if (isEmptyString(newDestinationName)) {
                printf("Ten diem den khong duoc de trong.\n");
                return;
            }
            inputString("Nhap dia chi diem den moi: ", newDestinationAddr, sizeof(newDestinationAddr));

            inputString("Nhap ngay gio moi cho chuyen xe: ", newDate, sizeof(newDate));
            if (isEmptyString(newDate)) {
                printf("Ngay gio chuyen xe khong duoc de trong.\n");
                return;
            }

            while (1) {
                inputString("Nhap tong so ghe moi: ", temp, sizeof(temp));
                if (isEmptyString(temp)) {
                    printf("Tong so ghe khong duoc de trong.\n");
                    continue;
                }
                newTotalSeats = atoi(temp); // chuyen chuoi sang so
                if (newTotalSeats < trips[i].bookedSeats) {
                    printf("Tong so ghe khong duoc nho hon so ghe da dat (%d).\n", trips[i].bookedSeats);
                } else if (newTotalSeats <= 0) {
                    printf("Tong so ghe phai lon hon 0.\n");
                } else {
                    break;
                }
            }

            strncpy(trips[i].departure.name, newDepartureName, sizeof(trips[i].departure.name)-1);
            trips[i].departure.name[sizeof(trips[i].departure.name)-1] = '\0';
            strncpy(trips[i].departure.address, newDepartureAddr, sizeof(trips[i].departure.address)-1);
            trips[i].departure.address[sizeof(trips[i].departure.address)-1] = '\0';
            strncpy(trips[i].destination.name, newDestinationName, sizeof(trips[i].destination.name)-1);
            trips[i].destination.name[sizeof(trips[i].destination.name)-1] = '\0';
            strncpy(trips[i].destination.address, newDestinationAddr, sizeof(trips[i].destination.address)-1);
            trips[i].destination.address[sizeof(trips[i].destination.address)-1] = '\0';
            strncpy(trips[i].date, newDate, sizeof(trips[i].date)-1);
            trips[i].date[sizeof(trips[i].date)-1] = '\0';
            trips[i].totalSeats = newTotalSeats; // cap nhat tong so ghe

            printf("Cap nhat chuyen xe thanh cong\n");
            return;
        }
    }
    printf("Khong tim thay chuyen xe\n");
}

int isValidPhone(const char* phone) { // kiem tra so dien thoai hop le
    int len = strlen(phone);
    if (len < 8 || len > 15)
        return 0;
    for (int i = 0; i < len; ++i)
        if (!isdigit((unsigned char)phone[i]))
            return 0;
    return 1;
}

int findTripIndexById(const char* tripId) { // tim vi tri chuyen xe theo id
    for (int i = 0; i < tripCount; ++i)
        if (strcmp(trips[i].tripId, tripId) == 0)
            return i;
    return -1;
}

int isSeatBooked(const char* tripId, int seatNumber, struct Ticket* tickets, int ticketCount) { // kiem tra so ghe da dat ve chua
    for (int i = 0; i < ticketCount; ++i)
        if (strcmp(tickets[i].tripId, tripId) == 0 && tickets[i].seatNumber == seatNumber)
            return 1;
    return 0;
}


// Chuc nang dat ve
void bookTicket() { 
    extern struct Ticket tickets[];
    extern int ticketCount;
	int seatNumber = 0;
	
	
	
    if (tripCount == 0) {
        printf("Dat ve that bai: Khong co chuyen xe nao!\n");
        return;
    }

    printf("----- Danh sach cac chuyen xe -----\n");
    for (int i = 0; i < tripCount; ++i) {
        printf("%d. Ma chuyen xe: %s | Khoi hanh: %s | Den: %s | Ghe trong: %d\n",
               i + 1, trips[i].tripId,
               trips[i].departure.name, trips[i].destination.name,
               trips[i].totalSeats - trips[i].bookedSeats);
    }

    char inputTripId[32];
    int tripIdx = -1;

    // Vong lap nhap ma chuyen xe hop le
    while (1) {
        inputString("Nhap ma chuyen xe muon dat ve: ", inputTripId, sizeof(inputTripId));
        if (isEmptyString(inputTripId)) {
            printf("Nhap khong dung ma chuyen xe!\n");
            printf("Vui long nhap lai!\n");
            continue;
        }
        tripIdx = findTripIndexById(inputTripId);
        if (tripIdx == -1) {
            printf("Khong tim thay chuyen xe.\n");
            printf("Vui long nhap lai!\n");
            continue;
        }
        if (trips[tripIdx].bookedSeats >= trips[tripIdx].totalSeats) {
            printf("Chuyen xe da het ghe!\n");
            printf("Dat ve that bai!\n");
            return;
        }
        break;
    }

    struct Ticket newTicket;
    memset(&newTicket, 0, sizeof(struct Ticket));
    strncpy(newTicket.tripId, trips[tripIdx].tripId, sizeof(newTicket.tripId) - 1);
    newTicket.tripId[sizeof(newTicket.tripId) - 1] = '\0';

    // Vong lap nhap ten khach hang
    while (1) {
        inputString("Nhap ten hanh khach: ", newTicket.passenger.name, sizeof(newTicket.passenger.name));
        if (isEmptyString(newTicket.passenger.name)) {
            printf("Ten hanh khach khong duoc de trong!\n");
            continue;
        }
        break;
    }
    
    // Vong lap nhap Sdt
	while (1) {    
        inputString("Nhap so dien thoai hanh khach: ", newTicket.passenger.phone, sizeof(newTicket.passenger.phone));
        if (isEmptyString(newTicket.passenger.phone)) {
            printf("So dien thoai khong duoc de trong!\n");
            continue;
        }
        if (!isValidPhone(newTicket.passenger.phone)) {
            printf("So dien thoai khong dung dinh dang hoac khong hop le!\n");
            continue;
        }
		break;  
	} 
	while(1){
        char tempSeat[16];
        inputString("Nhap so ghe: ", tempSeat, sizeof(tempSeat));
        seatNumber = atoi(tempSeat);

        if (seatNumber < 1 || seatNumber > trips[tripIdx].totalSeats) {
            printf("So ghe phai nhap duoi dinh dang so > 0 va <= tong so ghe!\n");
            continue;
           
        }
        if (isSeatBooked(trips[tripIdx].tripId, seatNumber, tickets, ticketCount)) {
            printf("So ghe nay da duoc dat trong chuyen xe nay!\n");
            continue;
            
        }
        break;
	}
	while(1){
        newTicket.seatNumber = seatNumber;

        newTicket.paymentStatus = 0;

        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        strftime(newTicket.date, sizeof(newTicket.date), "%Y-%m-%d %H:%M:%S", tm_info);

        char tempPrice[32];
        inputString("Nhap gia ve: ", tempPrice, sizeof(tempPrice));
        newTicket.price = atof(tempPrice);
        if (newTicket.price <= 0) {
            printf("Gia ve phai lon hon 0!\n");
            continue;
        }
        break;
    }

    snprintf(newTicket.ticketId, sizeof(newTicket.ticketId), "V%03d", ticketCount + 1);
    tickets[ticketCount++] = newTicket;
    trips[tripIdx].bookedSeats++;

    printf("Dat ve thanh cong\n");

    printf("----- Danh sach ve da dat -----\n");
    for (int i = 0; i < ticketCount; ++i) {
        printf("Ma ve: %s | Ma chuyen xe: %s | Ten khach: %s | SDT: %s | Ghe: %d | Gia: %.2f | Ngay dat: %s\n",
            tickets[i].ticketId, tickets[i].tripId, tickets[i].passenger.name,
            tickets[i].passenger.phone, tickets[i].seatNumber, tickets[i].price, tickets[i].date);
    }
}

void checkTicketStatus(struct Ticket tickets[], int ticketCount, struct Trip trips[], int tripCount) {
    char inputId[32];
    inputString("Nhap ma ve can kiem tra: ", inputId, sizeof(inputId));
    if (isEmptyString(inputId)) {
        printf("Ma ve khong duoc de trong!\n");
        return;
    }
    int found = 0;
    for (int i = 0; i < ticketCount; ++i) {
        if (strcmp(tickets[i].ticketId, inputId) == 0) {
            found = 1;
            printf("\n===== THONG TIN VE =====\n");
            printf("Ma ve        : %s\n", tickets[i].ticketId);
            printf("Ten khach    : %s\n", tickets[i].passenger.name);
            printf("SDT          : %s\n", tickets[i].passenger.phone);
            printf("Chuyen xe    : %s\n", tickets[i].tripId);

            for (int t = 0; t < tripCount; ++t) {
                if (strcmp(trips[t].tripId, tickets[i].tripId) == 0) {
                    printf("Tuyen xe     : %s - %s\n", trips[t].departure.name, trips[t].destination.name);
                    printf("Dia chi khoi hanh : %s\n", trips[t].departure.address);
                    printf("Dia chi diem den  : %s\n", trips[t].destination.address);
                    printf("Ngay gio chuyen xe: %s\n", trips[t].date);
                    break;
                }
            }

            printf("So ghe       : %d\n", tickets[i].seatNumber);
            printf("Gia ve       : %.2f\n", tickets[i].price);
            printf("Ngay dat     : %s\n", tickets[i].date);
            printf("Trang thai   : %s\n", tickets[i].paymentStatus == 1 ? "Da thanh toan" : "Chua thanh toan");
            printf("========================\n");
            return;
        }
    }
    printf("Khong tim thay ve!\n");
}


// Ham hien thi danh sach
void listTrip() {
    if (tripCount == 0) {
        printf("Khong co chuyen xe nao trong he thong.\n");
        return;
    }

    int pageSize = 5; // s chuyen hien thi moi trang
    int page = 0;
    int totalPages = (tripCount + pageSize - 1) / pageSize;

    while (1) {
        int start = page * pageSize;
        int end = start + pageSize;
        if (end > tripCount) end = tripCount;

        printf("\n===== Danh sach chuyen xe (Trang %d/%d) ====\n", page + 1, totalPages);
        for (int i = start; i < end; i++) {
            printf("TripId: %s | %s -> %s | Thoi gian: %s | Tong ghe: %d | Da dat: %d | Gia: %.2f\n",
                   trips[i].tripId,
                   trips[i].departure,
                   trips[i].destination,   
                   trips[i].date,
                   trips[i].totalSeats,
                   trips[i].bookedSeats,
                   tickets[i].price);
        }

        // Ðieu khien phan trang
        printf("\nNhap n (next), p (prev), q (quit): ");
        char cmd;
        scanf(" %c", &cmd);
        if (cmd == 'n' || cmd == 'N') {
            if (page + 1 < totalPages) page++;
        } else if (cmd == 'p' || cmd == 'P') {
            if (page > 0) page--;        } else if (cmd == 'q' || cmd == 'Q') {
            break;
        }
    }
}

// Thanh toan ve
int findTicketIndexById(const char *ticketId) {
    for (int i = 0; i < ticketCount; i++) {
        if (strcmp(tickets[i].ticketId, ticketId) == 0) {
            return i; // tr? v? v? trí vé trong m?ng
        }
    }
    return -1; // không tìm th?y
}

void payTicket() {
    char ticketId[20];
    printf("Nhap ma ve can thanh toan: ");
    scanf("%s", ticketId);

    int idx = findTicketIndexById(ticketId);  // Tim vi tri ve bang chi so index

    if (idx == -1) {
        printf("Khong tim thay ve.\n");
        return;
    }

    // Lay ve ra de xu ly
    struct Ticket tk = tickets[idx]; // Tao 1 bien de sao chep thong tin ve tu mang tickets tai vi tri idx.

    // Kiem tra trang thai ve
    if (tk.paymentStatus == 1) {
        printf("Ve nay da duoc thanh toan truoc do.\n");
        return;
    }

    // Hien thi thông tin ve truoc khi thanh toan
    printf("\n=== THONG TIN VE ===\n");
    printf("Ma ve: %s\n", tk.ticketId);
    printf("Chuyen: %s\n", tk.tripId);
    printf("Hanh khach: %s | SDT: %s\n", tk.passenger.name, tk.passenger.phone);
    printf("So ghe: %d | Gia: %.2f\n", tk.seatNumber, tk.price);
    printf("Ngay dat: %s\n", tk.date);

    // Xac nhan thanh toan
    char confirm;
    printf("\nBan co muon thanh toan ve nay? (y/n): ");
    scanf(" %c", &confirm);

    if (confirm == 'y' || confirm == 'Y') {
        tickets[idx].paymentStatus = 1; // cap nhat trang thai thanh toan
        printf("Thanh toan thanh cong cho ve %s.\n", tk.ticketId);
    } else {
        printf("Thanh toan da bi huy.\n");
    }
}

// Ham quan li trang thai ve
void statusTicketManagement() {
    char ticketId[20];

    while (1) {
        printf("Nhap ma ve can huy: ");
        fgets(ticketId, sizeof(ticketId), stdin);
        ticketId[strcspn(ticketId, "\n")] = '\0'; 

        if (strlen(ticketId) == 0) {
            printf("Ma ve khong duoc de trong. Nhap lai!\n");
            continue;
        }
        break;
    }

    int idx = findTicketIndexById(ticketId);

    if (idx == -1) {
        printf("Khong tim thay ve.\n");
        return;
    }

    struct Ticket tk = tickets[idx];

    if (tk.paymentStatus == 1) {
        printf("Ve nay da duoc thanh toan, khong the huy.\n");
        return;
    }

    printf("\n=== THONG TIN VE ===\n");
    printf("Ma ve: %s\n", tk.ticketId);
    printf("Chuyen: %s\n", tk.tripId);
    printf("Hanh khach: %s | SDT: %s\n", tk.passenger.name, tk.passenger.phone);
    printf("So ghe: %d | Gia: %.2f\n", tk.seatNumber, tk.price);
    printf("Ngay dat: %s\n", tk.date);

    char confirm;

    while (1) {
        printf("\nBan co muon huy ve nay? (y/n): ");
        scanf(" %c", &confirm);

        if (confirm == 'y' || confirm == 'Y' ||
            confirm == 'n' || confirm == 'N') {
            break;
        }

        printf("Chi duoc nhap y/n. Vui long thu lai!\n");
    }

    if (confirm == 'y' || confirm == 'Y') {
        tickets[idx].action = 1; 

        int tripIdx = findTripIndexById(tk.tripId);
        if (tripIdx != -1 && trips[tripIdx].bookedSeats > 0) {
            trips[tripIdx].bookedSeats--;
        }

        printf("Huy ve thanh cong cho ma ve %s.\n", tk.ticketId);
    } else {
        printf("Huy ve da bi huy thao tac.\n");
    }
}

