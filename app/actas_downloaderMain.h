#ifndef ACTAS_DOWNLOADERMAIN_H
#define ACTAS_DOWNLOADERMAIN_H

//(*Headers(actas_downloaderDialog)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/gauge.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/slider.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include <UR_V16X/UR_V16X.h>
#include <curl/curl.h>

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
} actas_config_t;

class actas_downloaderDialog: public wxDialog
{
    public:

        actas_downloaderDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~actas_downloaderDialog();
        void configure();
        void OnInternalIdle() override;
        UR_V16X *v16x;
        static char *msg;

    private:

        static actas_config_t actas_config;
        void fire_process(void);
        static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
        void add_transfer(CURLM *cm, int i, FILE *pagefile, char *urls);
        void generate_filetest(int cnt);
        void curl_main(uint32_t base_num);
        void curl_main();
        uint32_t _get_base_dpto_prefix(int index);
        uint32_t _read_file_to_buff(const char file_conf[], uint8_t **cbuff);
        void _read_url(char *url);

        void OnKeyDown(wxKeyEvent& event);

        //(*Handlers(actas_downloaderDialog)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void Onstart_curl_bClick(wxCommandEvent& event);
        void Ondpto_radioctrlSelect(wxCommandEvent& event);
        void Onbase_url_txtctrlText(wxCommandEvent& event);
        void Onindex_slider_ctrlCmdScroll(wxScrollEvent& event);
        void Onindex_slider_ctrlCmdScrollThumbRelease(wxScrollEvent& event);
        void OnPaint(wxPaintEvent& event);
        void OnKeyUp(wxKeyEvent& event);
        void Onserver_mode_chkboxClick(wxCommandEvent& event);
        //*)

        //(*Identifiers(actas_downloaderDialog)
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_RADIOBOX1;
        static const long ID_SLIDER1;
        static const long ID_STATICTEXT2;
        static const long ID_STATICTEXT3;
        static const long ID_GAUGE1;
        static const long ID_STATICTEXT4;
        static const long ID_STATICTEXT5;
        static const long ID_STATICTEXT6;
        static const long ID_STATICTEXT7;
        static const long ID_PANEL1;
        static const long ID_TEXTCTRL1;
        static const long ID_STATICTEXT1;
        static const long ID_CHECKBOX1;
        static const long ID_PANEL2;
        static const long ID_TEXTCTRL2;
        static const long ID_PANEL3;
        static const long ID_NOTEBOOK1;
        //*)

        //(*Declarations(actas_downloaderDialog)
        wxButton* quit_b;
        wxButton* start_curl_b;
        wxCheckBox* server_mode_chkbox;
        wxGauge* cnt_download_gauge;
        wxNotebook* Notebook1;
        wxPanel* Panel1;
        wxPanel* Panel2;
        wxPanel* Panel3;
        wxRadioBox* dpto_radioctrl;
        wxSlider* index_slider_ctrl;
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxStaticText* base_url_label;
        wxStaticText* cnt_download_lblctrl;
        wxStaticText* index_slider_name_txtctrl;
        wxStaticText* index_slider_value_lblctrl;
        wxStaticText* total_lblctrl;
        wxTextCtrl* about_txtctrl;
        wxTextCtrl* base_url_txtctrl;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // ACTAS_DOWNLOADERMAIN_H
