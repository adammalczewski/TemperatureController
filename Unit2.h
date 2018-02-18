//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Types.hpp>
//---------------------------------------------------------------------------
class TOptionsForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *TempTresholdE;
	TButton *ApplyB;
	TButton *CancelB;
	TLabel *Label2;
	TEdit *EverestDirE;
	TLabel *EverestExecutableL;
	TEdit *EverestExeE;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ApplyBClick(TObject *Sender);
	void __fastcall CancelBClick(TObject *Sender);
	void __fastcall Close();
private:	// User declarations
public:		// User declarations
	__fastcall TOptionsForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TOptionsForm *OptionsForm;
//---------------------------------------------------------------------------
#endif
