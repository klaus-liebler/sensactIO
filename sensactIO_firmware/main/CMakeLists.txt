# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

idf_component_register(SRCS "aRgbwPwm.cc" "aBlind.cc" "aSinglePwm.cc" "aRgbwPwm.cc" "aOnOff.cc" "connect.c" "http_handlers.cc" "main.cc" "manager.cc" "spiffs.cc"
    EMBED_FILES ../../sensactIO_webui/dist_compressed/index.html.gz
)

spiffs_create_partition_image(storage ../spiffs_filesystem FLASH_IN_PROJECT)
