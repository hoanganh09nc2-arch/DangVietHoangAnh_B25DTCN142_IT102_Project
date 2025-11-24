#include <stdio.h>
#include <string.h>

#define MAX_TRIPS 100

struct Location {
    char name[50];
    char address[100];
};

struct Trip {
    char tripId[20];
    char datetime[30];
    struct Location from;
    struct Location to;

    int totalSeats;
    int bookedSeats;
};

struct Trip tripList[MAX_TRIPS];
int tripCount = 0;

int findTripIndexById(char tripId[]) {
    for (int i = 0; i < tripCount; i++) {
        if (strcmp(tripList[i].tripId, tripId) == 0) {
            return i;
        }
    }
    return -1;
}

void addTrip() {
    struct Trip newTrip;
    char tempId[20];

    printf("\n=== THEM CHUYEN XE MOI ===\n");

    do {
        printf("Nhap ma chuyen xe: ");
        scanf("%s", tempId);

        if (findTripIndexById(tempId) != -1) {
            printf("? Ma chuyen xe da ton tai! Vui long nhap lai.\n");
        }
    } while (findTripIndexById(tempId) != -1);

    strcpy(newTrip.tripId, tempId);

    printf("Nhap thoi gian (VD: 2025-12-10 09:00): ");
    getchar();
    fgets(newTrip.datetime, sizeof(newTrip.datetime), stdin);
    newTrip.datetime[strcspn(newTrip.datetime, "\n")] = '\0';

    printf("Nhap ten diem di: ");
    fgets(newTrip.from.name, sizeof(newTrip.from.name), stdin);
    newTrip.from.name[strcspn(newTrip.from.name, "\n")] = '\0';

    printf("Nhap dia chi diem di: ");
    fgets(newTrip.from.address, sizeof(newTrip.from.address), stdin);
    newTrip.from.address[strcspn(newTrip.from.address, "\n")] = '\0';

    printf("Nhap ten diem den: ");
    fgets(newTrip.to.name, sizeof(newTrip.to.name), stdin);
    newTrip.to.name[strcspn(newTrip.to.name, "\n")] = '\0';

    printf("Nhap dia chi diem den: ");
    fgets(newTrip.to.address, sizeof(newTrip.to.address), stdin);
    newTrip.to.address[strcspn(newTrip.to.address, "\n")] = '\0';

    do {
        printf("Nhap tong so ghe: ");
        scanf("%d", &newTrip.totalSeats);

        if (newTrip.totalSeats <= 0) {
            printf("Tong so ghe phai > 0!\n");
        }
    } while (newTrip.totalSeats <= 0);

    newTrip.bookedSeats = 0;

    tripList[tripCount++] = newTrip;

    printf("Them chuyen xe thanh cong!\n");
}


void updateTrip() {
    char id[20];
    printf("\n=== CAP NHAT CHUYEN XE ===\n");

    printf("Nhap ma chuyen xe can cap nhat: ");
    scanf("%s", id);

    int index = findTripIndexById(id);

    if (index == -1) {
        printf("? Khong tim thay chuyen xe!\n");
        return;
    }

    struct Trip *trip = &tripList[index];

    printf("Nhap thoi gian moi: ");
    getchar();
    fgets(trip->datetime, sizeof(trip->datetime), stdin);
    trip->datetime[strcspn(trip->datetime, "\n")] = '\0';

    printf("Nhap ten diem di moi: ");
    fgets(trip->from.name, sizeof(trip->from.name), stdin);
    trip->from.name[strcspn(trip->from.name, "\n")] = '\0';

    printf("Nhap dia chi diem di moi: ");
    fgets(trip->from.address, sizeof(trip->from.address), stdin);
    trip->from.address[strcspn(trip->from.address, "\n")] = '\0';

    printf("Nhap ten diem den moi: ");
    fgets(trip->to.name, sizeof(trip->to.name), stdin);
    trip->to.name[strcspn(trip->to.name, "\n")] = '\0';

    printf("Nhap dia chi diem den moi: ");
    fgets(trip->to.address, sizeof(trip->to.address), stdin);
    trip->to.address[strcspn(trip->to.address, "\n")] = '\0';

    int newSeats;
    do {
        printf("Nhap tong so ghe moi: ");
        scanf("%d", &newSeats);

        if (newSeats < trip->bookedSeats) {
            printf("? Tong so ghe moi khong duoc nho hon %d ghe da dat!\n", trip->bookedSeats);
        }
    } while (newSeats < trip->bookedSeats);

    trip->totalSeats = newSeats;

    printf("? Cap nhat chuyen xe thanh cong!\n");
}

int main() {
    int choice;

    do {
        printf("\n===== MENU QUAN LY CHUYEN XE =====\n");
        printf("1. Them chuyen xe (F01)\n");
        printf("2. Cap nhat chuyen xe (F02)\n");
        printf("0. Thoat\n");
        printf("Moi nhap lua chon: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addTrip();
                break;
            case 2:
                updateTrip();
                break;
            case 0:
                printf("Thoat chuong trinh.\n");
                break;
            default:
                printf("Lua chon khong hop le!\n");
        }

    } while (choice != 0);

    return 0;
}

