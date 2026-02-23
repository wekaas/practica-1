#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;

int main() {

    int tc_fd;

    uint8_t read_byte;
    uint16_t tc_packet_id;
    uint16_t tc_packet_seq_ctrl;
    uint16_t tc_packet_len;
    uint32_t tc_df_header;
    uint16_t tc_packet_err_ctrl;

    tc_fd = open("single-tc.bin", O_RDONLY);

    // Read Packet ID and store it into tc_packet_id

    read(tc_fd, &read_byte, 1);        // Read MSB
    tc_packet_id = read_byte;
    tc_packet_id = tc_packet_id << 8;  // Shift it 8 bits to the left

    read(tc_fd, &read_byte, 1);               // Read LSB
    tc_packet_id = tc_packet_id | read_byte;  // OR the MSB and the LSB

    printf("APID: 0x%X\n", tc_packet_id & 0x07FF);

    //PACKET SEQUENCE CONTROL:

    read(tc_fd, &read_byte, 1);        // Read MSB
    tc_packet_seq_ctrl = read_byte;
    tc_packet_seq_ctrl = tc_packet_seq_ctrl << 8;  // Shift it 8 bits to the left

    read(tc_fd, &read_byte, 1);               // Read LSB
    tc_packet_seq_ctrl = tc_packet_seq_ctrl | read_byte;  // OR the MSB and the LSB

    printf("Secuence flags: 0x%X\n", (tc_packet_seq_ctrl & 0xC000) >> 14);
    printf("Secuence count: %d\n", (tc_packet_seq_ctrl & 0x3FFF));

    //PACKET LENGTH

    read(tc_fd, &read_byte, 1);        // Read MSB
    tc_packet_len = read_byte;
    tc_packet_len = tc_packet_len << 8;  // Shift it 8 bits to the left

    read(tc_fd, &read_byte, 1);               // Read LSB
    tc_packet_len = tc_packet_len | read_byte;  // OR the MSB and the LSB

    printf("Packet Length: 0x%X\n", tc_packet_len);

    //DATA FIELD HEADER

    read(tc_fd, &read_byte, 1);        // Read MSB
    tc_df_header = read_byte;
    tc_df_header = tc_df_header << 8;  // Shift it 24 bits to the left

    read(tc_fd, &read_byte, 1);               // Read LSB
    tc_df_header = tc_df_header | read_byte;  // OR the MSB and the LSB
    tc_df_header = tc_df_header <<8;

    read(tc_fd, &read_byte, 1);
    tc_df_header = tc_df_header | read_byte;
    tc_df_header = tc_df_header <<8;

    read(tc_fd, &read_byte, 1);
    tc_df_header = tc_df_header | read_byte;

    printf("Ack: 0x%X\n", (tc_df_header & 0x0F000000) >> 24);
    printf("Service type: %d\n", (tc_df_header & 0x00FF0000) >> 16);
    printf("Service Subtype: %d\n", (tc_df_header & 0x0000FF00) >> 8);
    printf("Source ID: 0x%X\n", tc_df_header & 0x000000FF);

    //PACKET ERROR CONTROL

    read(tc_fd, &read_byte, 1);        // Read MSB
    tc_packet_err_ctrl = read_byte;
    tc_packet_err_ctrl = tc_packet_err_ctrl << 8;  // Shift it 8 bits to the left

    read(tc_fd, &read_byte, 1);               // Read LSB
    tc_packet_err_ctrl = tc_packet_err_ctrl | read_byte;  // OR the MSB and the LSB

    printf("Packet Error Control: 0x%X\n", tc_packet_err_ctrl);

    close(tc_fd);

    int tm_fd;

    uint8_t byte_to_write;

    //CREO LAS VARIABLES DE LOS QUE VOY A USAR:

    uint16_t tm_packet_id = 0;
    uint16_t tm_packet_seq_ctrl;
    uint16_t tm_packet_len;
    uint32_t tm_df_header = 0;
    uint32_t tm_source_data = 0;

    tm_fd = open("single-tm.bin", O_WRONLY | O_CREAT | O_TRUNC, 0664);

    //PACKET ID:

    tm_packet_id = tm_packet_id | (1 << 11);

    tm_packet_id = tm_packet_id | (0x32C); //primero creo el bit(te dice el enunciado el valor de cada parte

    byte_to_write = (tm_packet_id & 0xFF00) >> 8;
    write(tm_fd, &byte_to_write, 1);

    byte_to_write = (tm_packet_id & 0x00FF);
    write(tm_fd, &byte_to_write, 1);

    //PACKET SEQUENCE CONTROL:

    tm_packet_seq_ctrl = 0xC000; //lo pongo de forma bruta

    byte_to_write = (tm_packet_seq_ctrl & 0xFF00) >> 8;
    write(tm_fd, &byte_to_write, 1);

    byte_to_write = (tm_packet_seq_ctrl & 0x00FF);
    write(tm_fd, &byte_to_write, 1);

    //PACKET LENGTH:

    tm_packet_len = 0x0007;

    byte_to_write = (tm_packet_len & 0xFF00) >> 8;
    write(tm_fd, &byte_to_write, 1);

    byte_to_write = (tm_packet_len & 0x00FF);
    write(tm_fd, &byte_to_write, 1);

    //PACKET DATA FIELD HEADER:

    tm_df_header = tm_df_header | (1 << 28) | (1 << 16) | (1 << 8) | 0x78;

    byte_to_write = (tm_df_header & 0xFF000000) >> 24;
    write(tm_fd, &byte_to_write, 1);

    byte_to_write = (tm_df_header & 0x00FF0000)  >> 16;
    write(tm_fd, &byte_to_write, 1);

    byte_to_write = (tm_df_header & 0x0000FF00) >> 8;
    write(tm_fd, &byte_to_write, 1);

    byte_to_write = (tm_df_header & 0x000000FF);
    write(tm_fd, &byte_to_write, 1);

    //SOURCE DATA:

    tm_source_data = tm_source_data | (1 << 28) | (1 << 27) | (0x32C << 16) | (0x3 <<14);

    byte_to_write = (tm_source_data & 0xFF000000) >> 24;
    write(tm_fd, &byte_to_write, 1);

    byte_to_write = (tm_source_data & 0x00FF0000)  >> 16;
    write(tm_fd, &byte_to_write, 1);

    byte_to_write = (tm_source_data & 0x0000FF00) >> 8;
    write(tm_fd, &byte_to_write, 1);

    byte_to_write = (tm_source_data & 0x000000FF);
    write(tm_fd, &byte_to_write, 1);

    close(tm_fd);


    return 0;

}
