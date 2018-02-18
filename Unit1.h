//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Types.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Menus.hpp>
#include <vector>

#define MAX_SENSORS 10
#define DEFAULT_EVEREST_DIRECTORY L"C:\\Program Files (x86)\\Lavalys\\EVEREST Ultimate Edition"
#define DEFAULT_EVEREST_EXECUTABLE L"everest.exe"
#define DEFAULT_TEMP_TRESHOLD 100
#define MIN_OPTIONS_FORM_HEIGHT 205
#define EVEREST_OPENING_TIME 30 //1 unit is 0.5 second (timer interval)
#define CONSTANT_SIZES false

class TMainForm : public TForm{
__published:	// IDE-managed Components
	TTimer *Timer1;
	TLabel *ErrorLabel;
	TLabel *StaleLabel;
	TLabel *StaleLabel2;
	TMemo *Memo1;
	TStyleBook *StyleBook2;
	TPopupMenu *PopupMenu;
	TMenuItem *MenuItem1;
	TPanel *Panel1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall MenuItem1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
    float Values[MAX_SENSORS];
    int stale;
    bool OpeningEverest;
    int OpeningTime;
    bool CantOpen;
	int ValueChangeDuringOpening;
	DWORD TempTreshold;
	UnicodeString EverestDirectory;
	UnicodeString EverestExecutable;
    std::vector<TLabel*> Labels;
    std::vector<TLabel*> NameLabels;
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
