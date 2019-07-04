#ifndef COURTROOM_H
#define COURTROOM_H

#include "aoimage.h"
#include "aobutton.h"
#include "aocharbutton.h"
#include "aoemotebutton.h"
#include "aopacket.h"
#include "aoscene.h"
#include "aomovie.h"
#include "aocharmovie.h"
#include "aomusicplayer.h"
#include "aosfxplayer.h"
#include "aoblipplayer.h"
#include "aoevidencebutton.h"
#include "aolineedit.h"
#include "aotextedit.h"
#include "aoevidencedisplay.h"
#include "datatypes.h"
#include "aoapplication.h"
#include "lobby.h"
#include "hardware_functions.h"
#include "file_functions.h"
#include "datatypes.h"
#include "debug_functions.h"
#include "aoiclog.h"
#include "aojukebox.h"
#include "aorooms.h"
#include "aomixer.h"
#include "aochat.h"
#include "aoroomcontrols.h"

#include <QMainWindow>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QListWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QSlider>
#include <QVector>
#include <QCloseEvent>
#include <QSignalMapper>
#include <QMap>
#include <QTextBrowser>
#include <QSpinBox>

#include <QDebug>
#include <QScrollBar>
#include <QRegExp>
#include <QBrush>
#include <QTextCharFormat>
#include <QFont>
#include <QInputDialog>
#include <QFileDialog>

class AOApplication;
class AOViewport;

// This is for inline message-colouring.
enum INLINE_COLOURS {
    INLINE_BLUE,
    INLINE_GREEN,
    INLINE_ORANGE,
    INLINE_GREY
};

class Courtroom : public QMainWindow
{
  Q_OBJECT
public:
  explicit Courtroom(AOApplication *p_ao_app);

  void append_char(char_type p_char) { char_list.append(p_char); }
  void append_evidence(evi_type p_evi) { evidence_list.append(p_evi); }
  void set_music(QVector<track_type> &f_music) { ui_music_list->set_tracks(f_music); }
  void set_areas(QVector<area_type> &f_areas) { area_list = f_areas; ui_area_list->set_areas(f_areas); }

  void arup_append(int players, QString status, QString cm, QString locked);
  void arup_modify(int type, int place, QString value);

  void character_loading_finished();

  //sets position of widgets based on theme ini files
  void set_widgets();
  //sets font size based on theme ini files
  void set_font(QWidget *widget, QString p_identifier);
  //helper function that calls above function on the relevant widgets
  void set_fonts();

  void set_window_title(QString p_title);

  //reads theme inis and sets size and pos based on the identifier
  void set_size_and_pos(QWidget *p_widget, QString p_identifier);

  //sets status as taken on character with cid n_char and places proper shading on charselect
  void set_taken(int n_char, bool p_taken);

  //sets the evidence list member variable to argument
  void set_evidence_list(QVector<evi_type> &p_evi_list);

  //called when a DONE#% from the server was received
  void done_received();

  //sets the local mute list based on characters available on the server
  void set_mute_list();

  // Sets the local pair list based on the characters available on the server.
  void set_pair_list();

  //takes in serverD-formatted IP list as prints a converted version to server OOC
  //admittedly poorly named
  void set_ip_list(QString p_list);

  //send a message that the player is banned and quits the server
  void set_ban(int p_cid);

  //cid = character id, returns the cid of the currently selected character
  int get_cid() {return m_cid;}
  QString get_current_char() {return current_char;}

  //properly sets up some varibles: resets user state
  void enter_courtroom(int p_cid);

  void list_areas();

  //these are for OOC chat
  void append_ms_chatmessage(QString f_name, QString f_message);
  void append_server_chatmessage(QString p_name, QString p_message, QString p_colour);

  void handle_chatmessage(QStringList *p_contents);
  void handle_background(QString background);
  void handle_wtce(QString wtce, int variant);

  //prints who played the song to IC chat and plays said song(if found on local filesystem)
  //takes in a list where the first element is the song name and the second is the char id of who played it
  void handle_song(QStringList *p_contents);

  //sets the hp bar of defense(p_bar 1) or pro(p_bar 2)
  //state is an number between 0 and 10 inclusive
  void set_hp_bar(int p_bar, int p_state);

  void announce_case(QString title, bool def, bool pro, bool jud, bool jur, bool steno);

  void check_connection_received();

  ~Courtroom();

  static constexpr int chatmessage_size = 23;
private:
  AOApplication *ao_app;

  int m_courtroom_width = 714;
  int m_courtroom_height = 668;

  int m_viewport_x = 0;
  int m_viewport_y = 0;

  int m_viewport_width = 256;
  int m_viewport_height = 192;

  int maximumMessages = 0;

  // The character ID of the character this user wants to appear alongside with.
  int other_charid = -1;

  // The offset this user has given if they want to appear alongside someone.
  int offset_with_pair = 0;

  QVector<char_type> char_list;
  QVector<evi_type> evidence_list;
  QVector<area_type> area_list;

  QSignalMapper *char_button_mapper;

  //triggers ping_server() every 60 seconds
  QTimer *keepalive_timer;

  QString previous_ic_message = "";

  //char id, muted or not
  QMap<int, bool> mute_map;

  //character id, which index of the char_list the player is
  int m_cid = -1;
  //cid and this may differ in cases of ini-editing
  QString current_char = "";

  int objection_state = 0;
  int realization_state = 0;
  int text_color = 0;
  bool is_presenting_evidence = false;

  int defense_bar_state = 0;
  int prosecution_bar_state = 0;

  int current_char_page = 0;
  int char_columns = 10;
  int char_rows = 9;
  int max_chars_on_page = 90;

  const int button_width = 60;
  const int button_height = 60;

  int current_emote = 0;
  QVector<evi_type> local_evidence_list;

  int current_evidence_page = 0;
  int current_evidence = 0;
  int evidence_columns = 6;
  int evidence_rows = 3;
  int max_evidence_on_page = 18;

  AOMusicPlayer *music_player;
  AOSfxPlayer *modcall_player;

  AOImage *ui_background;

  AOViewport *ui_viewport;

  AOICLog *ui_ic_chatlog;

  QListWidget *ui_mute_list;
  AORoomChooser *ui_area_list;
  AOJukebox *ui_music_list;
  AOMixer *ui_mixer;
  AOChat *ui_ic_chat;
  AORoomControls *ui_room_controls;

  AOButton *ui_pair_button;
  QListWidget *ui_pair_list;
  QSpinBox *ui_pair_offset_spinbox;

  AOServerChat *ui_ms_chat;
  AOServerChat *ui_server_chat;

  QVector<AOEmoteButton*> ui_emote_list;

  AOButton *ui_change_character;
  AOButton *ui_reload_theme;
  AOButton *ui_call_mod;
  AOButton *ui_settings;
  AOButton *ui_announce_casing;
  AOButton *ui_switch_area_music;

  QCheckBox *ui_guard;
  QCheckBox *ui_casing;

  QCheckBox *ui_showname_enable;

  AOButton *ui_mute;

  QComboBox *ui_text_color;

  AOButton *ui_evidence_button;
  AOImage *ui_evidence;
  AOLineEdit *ui_evidence_name;
  QWidget *ui_evidence_buttons;
  QVector<AOEvidenceButton*> ui_evidence_list;
  AOButton *ui_evidence_left;
  AOButton *ui_evidence_right;
  AOButton *ui_evidence_present;
  AOImage *ui_evidence_overlay;
  AOButton *ui_evidence_delete;
  AOLineEdit *ui_evidence_image_name;
  AOButton *ui_evidence_image_button;
  AOButton *ui_evidence_x;
  AOTextEdit *ui_evidence_description;

  AOImage *ui_char_select_background;

  //abstract widget to hold char buttons
  QWidget *ui_char_buttons;

  QVector<AOCharButton*> ui_char_button_list;
  QVector<AOCharButton*> ui_char_button_list_filtered;
  AOImage *ui_selector;

  AOButton *ui_back_to_lobby;

  QLineEdit *ui_char_password;

  AOButton *ui_char_select_left;
  AOButton *ui_char_select_right;

  AOButton *ui_spectator;

  QLineEdit *ui_char_search;
  QCheckBox *ui_char_passworded;
  QCheckBox *ui_char_taken;

  void construct_char_select();
  void set_char_select();
  void set_char_select_page();
  void put_button_in_place(int starting, int chars_on_this_page);
  void filter_character_list();

  void set_evidence_page();
public slots:
  void mod_called(QString p_ip);

  void case_called(QString msg, bool def, bool pro, bool jud, bool jur, bool steno);

private slots:
  void on_mute_list_clicked(QModelIndex p_index);
  void on_pair_list_clicked(QModelIndex p_index);

  void on_chat_return_pressed();

  void on_ooc_return_pressed(QString name, QString message);
  void on_ms_return_pressed(QString name, QString message);
  void on_icChat_positionChanged(QString pos);
  void on_jukebox_trackSelected(QString track);
  void on_rooms_roomSelected(QString room);

  void on_hold_it_clicked();
  void on_objection_clicked();
  void on_take_that_clicked();
  void on_custom_objection_clicked();

  void on_mute_clicked();
  void on_pair_clicked();

  void on_defense_minus_clicked();
  void on_defense_plus_clicked();
  void on_prosecution_minus_clicked();
  void on_prosecution_plus_clicked();

  void on_text_color_changed(int p_color);

  void on_music_slider_moved(int p_value);
  void on_sfx_slider_moved(int p_value);
  void on_blip_slider_moved(int p_value);

  void on_pair_offset_changed(int value);

  void on_roomControls_wtce(WTCE_TYPE type);

  void on_change_character_clicked();
  void on_reload_theme_clicked();
  void on_call_mod_clicked();
  void on_settings_clicked();
  void on_announce_casing_clicked();

  void on_pre_clicked();
  void on_flip_clicked();
  void on_guard_clicked();

  void on_showname_enable_clicked();

  void on_back_to_lobby_clicked();

  void on_char_select_left_clicked();
  void on_char_select_right_clicked();
  void on_char_search_changed(const QString& newtext);
  void on_char_taken_clicked(int newstate);
  void on_char_passworded_clicked(int newstate);

  void on_spectator_clicked();

  void char_clicked(int n_char);

  void on_switch_area_music_clicked();

  void on_casing_clicked();

  void ping_server();

  void load_bass_opus_plugin();
};

#endif // COURTROOM_H