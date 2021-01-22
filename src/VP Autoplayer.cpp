#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>

#define spaces "                    "

using namespace std;

int currentNote = 0;
int notePreview = 10;

bool pressedKey[4] = { false };

HKL layout = GetKeyboardLayout(0);

string music;

string tempNotes;

vector<char> hotkeys = { '-', '=', '[', ']' };
vector<char> specialCharacters = { '!', '@', '#', '$', '%', '^', '&', '*', '(', ')' };
vector<string> dividedMusic;
vector<string> previewMusic;

char checkSpecial(char note)
{
	switch (note){
		case '!':
			return '1';
		case '@':
			return '2';
		case '#':
			return '3';
		case '$': 
			return '4';
		case '%': 
			return '5';
		case '^':
			return '6';
		case '&':
			return '7';
		case '*':
			return '8';
		case '(':
			return '9';
		case ')':
			return '0';
		default: 
			exit(0);
	}
}

void groupNotes(string music)
{
	for (int i = 0; i < music.size(); i++)
	{

		if (music[i] == ' ') {
			continue;
		}

		if (music[i] == '[')
		{
			i++; 

			while (music[i] != ']')
			{
				tempNotes.push_back(music[i]);
				cout << "Looping on " << music[i] << endl;
				i++;

			}
			
			dividedMusic.push_back(tempNotes);
			cout << "Pushing back tempNotes" << endl;
			tempNotes = "";
			continue;

		}

		else
		{
			dividedMusic.push_back(string(1, music[i]));
			cout << "Adding " << music[i] << endl;
		}

	}
}

void start()
{
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	system("cls");
	cout << "Enter a sheet to play: " << endl;
	dividedMusic.clear();
	getline(std::cin >> noskipws, music);

	groupNotes(music);

	previewMusic = dividedMusic;

	for (int i = 0; i < notePreview; i++)
	{
		previewMusic.push_back(string(1, ' '));
	}

	if (dividedMusic.size() < notePreview)
	{
		cout << endl << "Please enter a sheet longer than " << notePreview << " notes";
		Sleep(1000);
		start();
	}

	while (currentNote < dividedMusic.size())
	{
		system("cls");

		for (int i = currentNote; i <= currentNote + notePreview; i++)
		{
			cout << previewMusic[i] << " ";
		}

		cout << endl << "Hotkeys - ";
		for (int i = 0; i < hotkeys.size(); i++)
		{
			cout << "'" << hotkeys[i] << "', ";
		}
		cout << " Right ALT to stop; END for new sheet";

		if (GetAsyncKeyState(VK_RMENU))
		{
			exit(10);
		}

		if (GetAsyncKeyState(VK_END))
		{
			currentNote = 0;
			start();
		}

		for (int i = 0; i < hotkeys.size(); i++)
		{
			if (GetAsyncKeyState(VkKeyScanExA(hotkeys[i], layout)) && !pressedKey[i])
			{
				pressedKey[i] = true;

				for (char& note : dividedMusic[currentNote])
				{

					if (find(specialCharacters.begin(), specialCharacters.end(), note) != specialCharacters.end())
					{

						note = checkSpecial(note);

						//Press down shift
						ip.ki.dwFlags = KEYEVENTF_SCANCODE; // 0 for key press
						ip.ki.wScan = MapVirtualKeyA(VK_SHIFT, MAPVK_VK_TO_VSC);
						SendInput(1, &ip, sizeof(INPUT)); //Send shift down

						//Play key
						ip.ki.dwFlags = KEYEVENTF_SCANCODE; // 0 for key press
						ip.ki.wScan = MapVirtualKeyA(VkKeyScanExA(note, layout), MAPVK_VK_TO_VSC);
						SendInput(1, &ip, sizeof(INPUT)); //Send key down

						//Release key
						ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;; // 0 for key press
						ip.ki.wScan = MapVirtualKeyA(VkKeyScanExA(note, layout), MAPVK_VK_TO_VSC);
						SendInput(1, &ip, sizeof(INPUT)); //Send key up

						//Release shift
						ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;; // 0 for key press
						ip.ki.wScan = MapVirtualKeyA(VK_SHIFT, MAPVK_VK_TO_VSC);
						SendInput(1, &ip, sizeof(INPUT)); //Send shift up

					}

					else
					{
						if (isupper(note))
						{
							//Press down shift
							ip.ki.dwFlags = KEYEVENTF_SCANCODE; // 0 for key press
							ip.ki.wScan = MapVirtualKeyA(VK_SHIFT, MAPVK_VK_TO_VSC);
							SendInput(1, &ip, sizeof(INPUT)); //Send shift down
						}

						//Play key
						ip.ki.dwFlags = KEYEVENTF_SCANCODE; // 0 for key press
						ip.ki.wScan = MapVirtualKeyA(VkKeyScanExA(tolower(note), layout), MAPVK_VK_TO_VSC);
						SendInput(1, &ip, sizeof(INPUT)); //Send key down

						//Release key
						ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;; // 0 for key press
						ip.ki.wScan = MapVirtualKeyA(VkKeyScanExA(tolower(note), layout), MAPVK_VK_TO_VSC);
						SendInput(1, &ip, sizeof(INPUT)); //Send key up

						if (isupper(note))
						{
							//Release shift
							ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;; // 0 for key press
							ip.ki.wScan = MapVirtualKeyA(VK_SHIFT, MAPVK_VK_TO_VSC);
							SendInput(1, &ip, sizeof(INPUT)); //Send shift up

						}
					}
				}
				currentNote++;
			}
			else if (!GetAsyncKeyState(VkKeyScanExA(hotkeys[i], layout)) && pressedKey[i]) { pressedKey[i] = false; }
		}

	}

	start();

}

int main()
{
	start();
}