//---------------------------------------------------------------------------

#define WIN64
#define CPUX64

#include <fmx.h>

#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "Registry.h"
#include <fstream>
#include <string>
#include <ios>
#include <shellapi.h>
#include <vector>
#include <string>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
	TempTreshold = DEFAULT_TEMP_TRESHOLD;
	EverestExecutable = DEFAULT_EVEREST_EXECUTABLE;
	EverestDirectory = DEFAULT_EVEREST_DIRECTORY;
}

void Write(UnicodeString str){
	MainForm->Memo1->Lines->Add(str);
}

void __fastcall TMainForm::FormCreate(TObject *Sender){

    for (unsigned int i = 0;i < MAX_SENSORS;++i){
        TLabel *Label = new TLabel(MainForm);
        Label->Parent = MainForm;
		Label->Position->X = 170;
		Label->Position->Y = 20+i*30;
        Label->Text = "";
        TStyledSettings *s = new TStyledSettings(TStyledSetting::ssOther|TStyledSetting::ssSize);
		Label->StyledSettings = *s;
		Label->Font->Size = 20;
        Label->Height = 25;
        Label->Width = 100;
        Labels.push_back(Label);

        Label = new TLabel(MainForm);
        Label->Parent = MainForm;
        Label->Position->X = 70;
        Label->Position->Y = 20+i*30;
        Label->Text = "";
		Label->StyledSettings = *s;
        Label->Font->Size = 20;
		Label->Height = 25;
        Label->Width = 100;
		NameLabels.push_back(Label);

		Values[i] = 0;
        stale = 0;

    }

	CantOpen = false;

	MainForm->PopupMenu = PopupMenu;

	TRegistryManager RM;

	if (RM.Initialise("Adam's Software Inc.","Temperature Controller")){
		if (RM.DwordExists(L"TempTreshold")) {
			RM.ReadDword(L"TempTreshold",TempTreshold);
		}
		if (RM.StringExists(L"EverestDirectory")) {
			RM.ReadString(L"EverestDirectory",EverestDirectory);
		}
		if (RM.StringExists(L"EverestExecutable")) {
			RM.ReadString(L"EverestExecutable",EverestExecutable);
		}
	}

	Timer1Timer(this);

}

// if tag is <h2> HeaderTag should be "h2"
void FindAllTags(const std::wstring &Website,const std::wstring &HeaderTag,std::vector<unsigned int> &result){
	size_t first,last;
	std::wstring TagBegin,TagEnd;
	TagBegin = L"<" +HeaderTag;
	TagEnd = L"</" + HeaderTag + L">";
	first = Website.find(TagBegin,0);
	while (first != Website.npos){
		last = Website.find(TagEnd,first+1);
		if (last == Website.npos) break;
		result.push_back(first);
		result.push_back(last+TagEnd.size());
		first = Website.find(TagBegin,last+1);
	}
}

std::wstring& DeleteTags(std::wstring &Text){
	size_t first,last;
	last = Text.find('>',0);
	while (last != Text.npos){
		first = Text.find_last_of('<',last);
		if (first == Text.npos) break;
		Text.erase(first,last-first+1);
		last = Text.find('>',first);
	}
	return Text;
}

void __fastcall TMainForm::Timer1Timer(TObject *Sender){

	if (!CONSTANT_SIZES) Width = 295;

	bool found = FindWindow(L"TEVEREST",L"EVEREST Ultimate Edition");

	if (!found && !CantOpen){
		for (unsigned int i = 0;i < MAX_SENSORS;++i){
            Labels[i]->Text = "";
            NameLabels[i]->Text = "";
		}
    	if (OpeningEverest){
			if (OpeningTime < EVEREST_OPENING_TIME){//i tak do czasu znalezienia okna Everesta najczesciej
            	++OpeningTime;
				return;
            } else CantOpen = true;
        } else {
        	if (!CONSTANT_SIZES) Height = 80;
        	ErrorLabel->Text = "Opening Everest ...";
        	ErrorLabel->FontColor = 0xFFA6CAF0;
			ErrorLabel->Visible = true;
        	OpeningEverest = true;
			OpeningTime = 0;
			ValueChangeDuringOpening = 0;
			int seResult;
			if ((seResult = (int)ShellExecute(NULL,L"open",EverestExecutable.c_str(),NULL,EverestDirectory.c_str(),SW_SHOWMINNOACTIVE)) <= 32){
				if (seResult == ERROR_FILE_NOT_FOUND) {
					ErrorLabel->Text = "Can't find Everest executable";
				} else ErrorLabel->Text = "Can't open Everest";
				ErrorLabel->FontColor =  0xFF800000;
				CantOpen = true;
			}
    		return;
        }
    } else if (found && !CantOpen){
    	if (OpeningEverest && (OpeningTime >= EVEREST_OPENING_TIME)){
        	OpeningEverest = false;
            stale = 0;
        }
    }

	HANDLE handle = OpenFileMapping(FILE_MAP_READ,false,L"EVEREST_SensorValues");
	if (handle != INVALID_HANDLE_VALUE){

        if (OpeningEverest){
            ++OpeningTime;
            if ((OpeningTime >= EVEREST_OPENING_TIME) && !stale){
            	OpeningEverest = false;
                CantOpen = true;
            } else if (OpeningTime >= EVEREST_OPENING_TIME){
                CantOpen = true;
            }
            OpeningTime %= 10000;
        }

    	char *Buffer;

        Buffer = (char*)MapViewOfFile(handle,FILE_MAP_READ,0,0,0);

        if (Buffer != NULL){

        	std::wstring Data;
            UnicodeString Temp = Buffer;

            UnmapViewOfFile(Buffer);

            Data = Temp.c_str();

        	std::vector<unsigned int> Tags;
            std::vector<unsigned int> label;
            std::vector<unsigned int> temp;

            FindAllTags(Data,L"temp",Tags);

            unsigned int SensorsC = Tags.size()/2;

            bool ValuesDiffer = false;
            bool TooHot = false;

            for (unsigned int i = 0;i < Tags.size()/2;++i){

            	label.clear();
                temp.clear();

				std::wstring TempData = Data.substr(Tags[i*2],Tags[i*2+1]-Tags[i*2]);

                FindAllTags(TempData,L"label",label);
				FindAllTags(TempData,L"value",temp);

                if (label.size() != 2 || temp.size() != 2){

				} else {
                	std::wstring TempName,TempValue;
                    TempName = TempData.substr(label[0],label[1]-label[0]);
                    TempValue = TempData.substr(temp[0],temp[1]-temp[0]);
                	UnicodeString name,valuestr;
                    float value;

                    name = DeleteTags(TempName).c_str();
                    valuestr = DeleteTags(TempValue).c_str();
                    value = valuestr.ToDouble();

                    if (Values[i] != value){
                    	Values[i] = value;
                        ValuesDiffer = true;
                    }

					if (value > TempTreshold){
                    	TooHot = true;
                    }

                    if (name == L"CPU #1 / Core #1") name = "CPU 1";
					else if (name == L"CPU #1 / Core #2") name = "CPU 2";
                    else if (name == L"GPU Diode") name = "GPU";
                    else if (name == L"WDC WD20EARX-00PASB0") name = "WD Disc";

                    valuestr += " °C";

                    Labels[i]->Text = valuestr;
                    NameLabels[i]->Text = name;

                }

            }

            for (unsigned int i = Tags.size()/2;i < MAX_SENSORS;++i){
                Labels[i]->Text = "";
                NameLabels[i]->Text = "";
            }

            if (!ValuesDiffer){
            	++stale;
            } else {
            	stale = 0;
                if (OpeningEverest){
                    ++ValueChangeDuringOpening;
                    if (ValueChangeDuringOpening > 1){
                        OpeningEverest = false;
                    }
                }
            }

            if ((Height != Height-ClientHeight+40+30*(SensorsC-1)+25+
				  ((stale > 30)?60:0)) && !(OpeningEverest) && !(CONSTANT_SIZES))
            {
				Height = Height-ClientHeight+40+30*(SensorsC-1)+25+
				  ((stale > 30)?60:0);
    		}

			if ((stale > 30) && (!OpeningEverest)){
				UnicodeString staleText = L"Data stopped updating (";
				if (stale < 100) {
					 staleText += stale/2;
				} else {

                }

				staleText += ")";
				StaleLabel->Text = staleText;
				StaleLabel->Visible = true;
				StaleLabel2->Visible = true;
				StaleLabel->Position->Y = 20+SensorsC*30;
                StaleLabel2->Position->Y = 20+(SensorsC+1)*30;
            } else {
                StaleLabel->Visible = false;
                StaleLabel2->Visible = false;
            }

            if (OpeningEverest){
                for (unsigned int i = 0;i < MAX_SENSORS;++i){
                    Labels[i]->Text = "";
                    NameLabels[i]->Text = "";
                }
            } else {
            	ErrorLabel->Visible = false;
            }

            Application->HandleMessage();

			if ((stale != 0 && stale%500 == 0) && !OpeningEverest){
                Beep(700,200);
                Beep(700,200);
                Beep(700,200);
            }

			if (TooHot){
				MessageBeep(0xFFFFFFFF);
            }

        } else if (!OpeningEverest) {
        	if (!CONSTANT_SIZES) Height = 80;
        	ErrorLabel->Visible = true;
            ErrorLabel->Text = "Can't connect to Everest";
            ErrorLabel->FontColor =  0xFF800000;
            for (unsigned int i = 0;i < MAX_SENSORS;++i) Labels[i]->Text = "";
            for (unsigned int i = 0;i < MAX_SENSORS;++i) NameLabels[i]->Text = "";
        }

    }   else if (!OpeningEverest){
        if (!CONSTANT_SIZES) Height = 80;
        ErrorLabel->Visible = true;
        ErrorLabel->Text = "Can't connect to Everest";
		ErrorLabel->FontColor =  0xFF800000;
        for (unsigned int i = 0;i < MAX_SENSORS;++i) Labels[i]->Text = "";
        for (unsigned int i = 0;i < MAX_SENSORS;++i) NameLabels[i]->Text = "";
    }

    CloseHandle(handle);

}
//---------------------------------------------------------------------------





void __fastcall TMainForm::MenuItem1Click(TObject *Sender)
{
	OptionsForm->Left = MainForm->Left;
	OptionsForm->Top = MainForm->Top;
	OptionsForm->Width = MainForm->Width;
	if (MainForm->Height > MIN_OPTIONS_FORM_HEIGHT) OptionsForm->Height = MainForm->Height;
	else OptionsForm->Height = MIN_OPTIONS_FORM_HEIGHT;
	OptionsForm->TempTresholdE->Text = TempTreshold;
	OptionsForm->EverestDirE->Text = EverestDirectory;
	OptionsForm->EverestExeE->Text = EverestExecutable;
	OptionsForm->Show();
	MainForm->Hide();
}
//---------------------------------------------------------------------------


