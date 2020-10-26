#pragma once

typedef struct __actas_config_s {
    uint32_t base_dpto_prefix;
    uint16_t cnt_upto;
    char dpto[20];
    char base_url[256];
    uint32_t cnt_completed;
    uint32_t cnt_download;
    uint32_t key_timeout;
    uint8_t show_url;
    bool is_show_url;
    bool is_server_running;
    bool stop_download;
    bool server_enabled;
} actas_config_t;

typedef struct __xfrprogress_s {
    FILE *pagefile;
    char filename[50];
} xfr_progress_t;
