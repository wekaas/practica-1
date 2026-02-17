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

    return 0;

}
