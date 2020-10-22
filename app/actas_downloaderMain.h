#ifndef ACTAS_DOWNLOADERMAIN_H
#define ACTAS_DOWNLOADERMAIN_H

//(*Headers(actas_downloaderDialog)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/panel.h>
//*)

#include <UR_V16X/UR_V16X.h>
#include <curl/curl.h>

typedef struct __actas_config_s {
    uint32_t base_dpto_prefix;
    uint32_t cnt_upto;
} actas_config_t;

class actas_downloaderDialog: public wxDialog
{
    public:

        actas_downloaderDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~actas_downloaderDialog();
        void configure();

    private:

        actas_config_t actas_config;
        void fire_process(void);
        static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
        void add_transfer(CURLM *cm, int i, FILE *pagefile, char *urls);
        void generate_filetest(int cnt);
        void curl_main(uint32_t base_num);
        void curl_main();

        //(*Handlers(actas_downloaderDialog)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void Onstart_curl_bClick(wxCommandEvent& event);
        //*)

        //(*Identifiers(actas_downloaderDialog)
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_PANEL1;
        //*)

        //(*Declarations(actas_downloaderDialog)
        wxButton* quit_b;
        wxButton* start_curl_b;
        wxPanel* Panel1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // ACTAS_DOWNLOADERMAIN_H
