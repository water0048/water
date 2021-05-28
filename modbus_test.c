#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <modbus/modbus-rtu.h>
 
int main()
{
        modbus_t *ctx;
 
        // libmodbus rtu 컨텍스트 생성
        ctx = modbus_new_rtu("/dev/ttyAMA0", 115200, 'E', 8, 1);
        /*
         * 디바이스 : /dev/ttyXXX
         * 전송속도 : 9600, 19200, 57600, 115200
         * 패리티모드 : N (none) / E (even) / O (odd)
         * 데이터 비트 : 5,6,7 or 8
         * 스톱 비트 : 1, 2
        */
        if (ctx == NULL) {
                fprintf(stderr, "Unable to create the libmodbus context\n");
                return -1;
        }
 
        modbus_set_slave(ctx, 71); // 슬레이브 주소
        modbus_set_debug(ctx, TRUE); // 디버깅 활성
 
        // 연결
        if (modbus_connect(ctx) == -1) {
                fprintf(stderr, "Unable to connect %s\n", modbus_strerror(errno));
                modbus_free(ctx);
                return -1;
        }
 
        uint16_t *read_registers;
        read_registers = (uint16_t *) malloc(256 * sizeof(uint16_t));
 
        // 0x03 기능 코드 (read holding register)
        // 0x012C : 주소 / 2 : 읽을 갯수 (예시)
        modbus_read_registers(ctx, 0x0001, 1, read_registers);
 
        // print temperature / humidity
        printf("%d.%d C\n",  read_registers[0]/100, read_registers[0]%100); // 2545 => 25.45 C
 
 
        modbus_close(ctx);
        modbus_free(ctx);
        return 0;
}