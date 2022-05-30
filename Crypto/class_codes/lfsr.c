#include <stdio.h>
int main(){
    unsigned char buf[] = {00 ,11 ,22 ,33 ,44 ,55 ,66 ,77 ,88 ,99};
    /* target buffer should be large enough */
    char str[12];

    unsigned char * pin = buf;
    const char * hex = "0123456789ABCDEF";
    char * pout = str;
    for(; pin < buf+sizeof(buf); pout+=3, pin++){
        pout[0] = hex[(*pin>>4) & 0xF];
        pout[1] = hex[ *pin     & 0xF];
        pout[2] = ':';
    }
    pout[-1] = 0;

    printf("%s\n", str);
}