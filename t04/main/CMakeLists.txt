idf_component_register(SRCS "main.c"
                       INCLUDE_DIRS "include"
                        REQUIRES defines console commands uart_console uart_utils_funcs spiffs
                      )

spiffs_create_partition_image(storage ../spiffs_dir FLASH_IN_PROJECT)                      