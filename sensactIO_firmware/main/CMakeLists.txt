# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

idf_component_register(SRCS "aBlind.cc" "aSinglePwm.cc" "aOnOff.cc" "connect.c" "http_handlers.cc" "main.cc" "manager.cc" "spiffs.cc"
    #INCLUDE_DIRS "C:/opt/nanopb"
    EMBED_FILES ../../sensactIO_webui/dist_compressed/index.html.gz
)

spiffs_create_partition_image(storage ../spiffs_filesystem FLASH_IN_PROJECT)
