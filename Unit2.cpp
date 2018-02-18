//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "Registry.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TOptionsForm *OptionsForm;

bool saveTempTreshold(){
	int value = _wtoi(OptionsForm->TempTresholdE->Text.c_str());
	if (value == 0 && OptionsForm->TempTresholdE->Text != "0") {
		ShowMessage("Invalid temperature value");
		return false;
	} else if (value <= 200 && value >= 20) {
		TRegistryManager RM;

		if (RM.Initialise("Adam's Software Inc.","Temperature Controller")){
			RM.WriteDword("TempTreshold",value);
		}

		return true;
	} else return false;
}

bool saveEverestDirectory(){

	UnicodeString value = OptionsForm->EverestDirE->Text;

	TRegistryManager RM;

	if (RM.Initialise("Adam's Software Inc.","Temperature Controller")){
		if (RM.WriteString("EverestDirectory",value) == TRegistryManager::WRITE_SUCCESS)
			return true;
		else return false;
	} else return false;

}

bool saveEverestExecutable(){

	UnicodeString value = OptionsForm->EverestExeE->Text;

	TRegistryManager RM;

	if (RM.Initialise("Adam's Software Inc.","Temperature Controller")){
		if (RM.WriteString("EverestExecutable",value) == TRegistryManager::WRITE_SUCCESS)
			return true;
		else return false;
	} else return false;

}

//---------------------------------------------------------------------------
__fastcall TOptionsForm::TOptionsForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TOptionsForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	Application->Terminate();
}

void __fastcall TOptionsForm::Close(){
	MainForm->Top = OptionsForm->Top;
	MainForm->Left = OptionsForm->Left;
	MainForm->Show();
	OptionsForm->Hide();
}

void __fastcall TOptionsForm::ApplyBClick(TObject *Sender)
{
	if (saveTempTreshold()) MainForm->TempTreshold = _wtoi(OptionsForm->TempTresholdE->Text.c_str());
	if (saveEverestDirectory()) MainForm->EverestDirectory = OptionsForm->EverestDirE->Text;
	if (saveEverestExecutable()) MainForm->EverestExecutable = OptionsForm->EverestExeE->Text;

	if (MainForm->CantOpen) {
		MainForm->OpeningEverest = false;
		MainForm->CantOpen = false;
	}

	Close();
}
//---------------------------------------------------------------------------

void __fastcall TOptionsForm::CancelBClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------


