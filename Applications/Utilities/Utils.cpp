char* itoa(int value, char* str, int base) {
    if (base < 2 || base > 36) {
        *str = '\0'; // Invalid base
        return str;
    }
    char* ptr = str, *ptr1 = str, tmp_char;
    int tmp_value;

    if (value < 0 && base == 10) {
        *ptr++ = '-';
        value = -value;
    }

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[tmp_value - value * base];
    } while (value);

    *ptr-- = '\0';

    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }

    return str;
}


// char getClock(int totalSecondes) {
//     int minutes = totalSecondes / 60;
// 	int secondes = totalSecondes % 60;

// 	char buffer[6];

// 	buffer[0] = '0' + (minutes / 10);
// 	buffer[1] = '0' + (minutes % 10);
// 	buffer[2] = ':';
// 	buffer[3] = '0' + (secondes / 10);
// 	buffer[4] = '0' + (secondes % 10);
// 	buffer[5] = '\0';

//     return buffer;
// }