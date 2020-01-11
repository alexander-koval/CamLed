cmake_minimum_required(VERSION 2.8)

project(CamLed)
add_executable("fifo_camled_server" "fifo_camled_server.c" "error_functions.c" "get_num.c")
add_executable("fifo_camled_client" "fifo_camled_client.c" "error_functions.c" "get_num.c")
