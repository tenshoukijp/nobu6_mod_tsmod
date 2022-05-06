#include "WinTarget.h"
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

#include "CommonGlobalHandle.h"
#include "GameDataStruct.h"
#include "SetHelpFileName.h"
#include "PluginMod.h"
#include "ScenarioMod.h"
#include "Menu.h"

using namespace std;

#define SAVEDATAEDITOR_FILENAME  "SaveDataEditor.exe"
#define MODDEBUGGER_FILENAME  "ModDebugger.exe"
#define SAKURAEDITOR_FILENAME  "Sakura.exe"
#define MEMORYEDITOR_FILENAME  "MemoryEditorMod.dll"

extern LRESULT OnMouseEventHandler(	HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
extern LRESULT OnKeyEventHandler( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
extern LRESULT OnViewingApplicationPopUpMenu( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
extern LRESULT OnSelectingApplicationTopMenu( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, int nPos );
HRESULT OnSelectingApplicationSystemMenu(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/*
WM_ACTIVATE                 �E�C���h�E���A�N�e�B�u�E��A�N�e�B�u�ɂȂ�Ƃ� 
WM_ACTIVATEAPP              �ʂ̃A�v���P�[�V�����̃E�C���h�E���A�N�e�B�u�E��A�N�e�B�u�ɂȂ�Ƃ� 
WM_ASKCBFORMATNAME          �N���b�v�{�[�h�̃t�H�[�}�b�g���𕶎���o�b�t�@�ɃR�s�[��v�� 
WM_CANCELMODEJOURNAL        �W���[�i���������L�����Z�����ꂽ 
WM_CANCELMODE               ���b�Z�[�W�{�b�N�X���̃_�C�A���O�{�b�N�X���\������悤�Ƃ��Ă��� 
WM_CAPTURECHAINGED          �}�E�X�̃L���v�`�����ω����� 
WM_CHANGECBCHAIN            �N���b�v�{�|�h�r���[���`�F�[�����ύX����� 
WM_CHAR                     �L�[�{�[�h���當�����͂��󂯎���� 
WM_CHARTOITEM               WM_CHAR�ɉ����ăI�[�i�[�`�惊�X�g�{�b�N�X���󂯎�� 
WM_CHILDACTIVATE            MDI�R���g���[���̎q�E�C���h�E���A�N�e�B�u�� 
WM_CHOOSEFONT_GETLOGFONT    [�t�H���g�̎w��]�_�C�A���O�{�b�N�X�ɑ��郁�b�Z�[�W 
WM_CLEAR                    �e�L�X�g�{�b�N�X�E�R���{�{�N�X�̑I���e�L�X�g���폜 
WM_CLOSE                    �E�C���h�E���邢�̓A�v���P�[�V�������N���[�Y���ꂽ 
WM_COMMAND                  ���j���[���I�����ꂽ���邢�̓R���g���[���ɃC�x���g���������� 
WM_COMPAREITEM              ���ёւ����w�肳�ꂽ�V�������ڂ̃��X�g�{�b�N�X�Ȃǂő��ΓI�Ȉʒu�����肷��B 
WM_COMPACTING               �V�X�e�����������s���̏�Ԃɂ��� 
WM_COPY                     �e�L�X�g�{�b�N�X�E�R���{�{�b�N�X�̑I���e�L�X�g���N���b�v�{�[�h�ɃR�s�[�����Ƃ� 
WM_COPYDATA                 �ʂ̃v���Z�X��̃E�B���h�E�Ƀf�[�^�𑗐M���� 
WM_CREATE                   �E�C���h�E���쐬�� 
WM_CTLCOLORBTN              �R�}���h�{�^�����`�悳��悤�Ƃ��Ă��� 
WM_CTLCOLORDLG              �_�C�A���O���`�悳��悤�Ƃ��Ă��� 
WM_CTLCOLOREDIT             �e�L�X�g�{�b�N�X���`�悳��悤�Ƃ��Ă��� 
WM_CTLCOLORLISTBOX          ���X�g�{�b�N�X���`�悳��悤�Ƃ��Ă��� 
WM_CTLCOLORMSGBOX           ���b�Z�[�W�{�b�N�X���`�悳��悤�Ƃ��Ă��� 
WM_CTLCOLORSCROLLBAR        �X�N���[���o�[���`�悳��悤�Ƃ��Ă��� 
WM_CTLCOLORSTATIC           ���x�����`�悳��悤�Ƃ��Ă��� 
WM_CUT                      �e�L�X�g�{�b�N�X�E�R���{�{�b�N�X�̑I���e�L�X�g���N���b�v�{�[�h�ɃR�s�[ 
WM_DEADCHAR                 �A�N�Z���g�L���̂悤�ȕ����Ƒg�������������󂯎���� 
WM_DELETEITEM               �I�[�i�`�惊�X�g�{�b�N�X�̍��ڂ��폜���ꂽ 
WM_DESTROY                  �E�C���h�E���j�����ꂽ 
WM_DESTROYCLIPBOARD         EmptyClipboard()�ɂ���ăN���b�v�{�[�h����ɂȂ��� 
WM_DEVICECHANGE             �n�[�h�E�F�A�̍\�����ύX���ꂽ 
WM_DEVMODECHANGE            WIN.INI �̃f�t�H���g�̃f�o�C�X�����ύX���ꂽ 
WM_DISPLAYCHANGE            �f�B�X�v���C�̉𑜓x���ύX���ꂽ 
WM_DRAWCLIPBOARD            �N���b�v�{�[�h�̓��e���ς���� 
WM_DRAWITEM                 �I�[�i�[�`��R���g���[���̊O�ς��ς���� 
WM_DROPFILES                �t�@�C���}�l�[�W������� Drag & Drop 
WM_ENABLE                   �E�C���h�E���L���E�����ɂȂ��� 
WM_ENDSESSION               Windows ���I������ 
WM_ENTERIDLE                �_�C�A���O�{�b�N�X�E���j���[�����͑҂� 
WM_ENTERMENULOOP            ���[�_���ȃ��j���[���[�v�ɓ������Ƃ� 
WM_ENTERSIZEMOVE            �E�B���h�E���ړ��E�T�C�Y�ύX���� 
WM_ERASEBKGND               �o�b�N�O���E���h������(�ăy�C���g) 
WM_EXITMENULOOP             ���[�_���ȃ��j���[���甲���o���� 
WM_EXITSIZEMOVE             �E�B���h�E�̈ړ��E�T�C�Y�ύX�̏������I�������Ƃ� 
WM_FONTCHANGE               �V�X�e���t�H���g���ύX���ꂽ 
WM_GETDLGCODE               �R���g���[���Ɋ֘A�t����ꂽ�E�C���h�E�ɑ����郁�b�Z�[�W 
WM_GETFONT                  �e�L�X�g�{�b�N�X���x���������ݎg���Ă���t�H���g�̃n���h����ݒ�E�擾���� 
WM_GETHOTKEY                �E�B���h�E�Ɋ֘A�t�����Ă���z�b�g�L�[���擾���� 
WM_GETICON                  �E�C���h�E�Ɋ֘A�t�����Ă���A�C�R���̃n���h�����擾 
WM_GETMINMAXINFO            �t�H�[���̍ő�E�ŏ������ɉ\�ȃT�C�Y���擾 
WM_GETTEXT                  �R���g���[���̃L���v�V�����E�e�L�X�g�{�b�N�X�̃e�L�X�g���o�b�t�@�ɃR�s�[ 
WM_GETTEXTLENGTH            �E�B���h�E�Ɋ֘A�t�����Ă���e�L�X�g�̃T�C�Y(�o�C�g��)���擾 
WM_HELP                     [F1] or [?]�̉����ꂽ�ꍇ 
WM_HOTKEY                   RegisterHotKey�֐��œo�^���ꂽ�z�b�g�L�[�������ꂽ�Ƃ� 
WM_HSCROLL                  �����X�N���[���o�[�𒲐����Ă��� 
WM_HSCROLLCLIPBOARD         �N���b�v�{�[�h�r���[���Ő����X���N�[���o�[���g��ꂽ 
WM_ICONERASEBKGND           �N���X�A�C�R�������鎞�A�A�C�R���̕`��O�ɒʒm 
WM_IME_CHAR                 IME���ϊ���̕����������� 
WM_IME_COMPOSITION          �L�[���͂ɂ����IME���m���̏�Ԃ�ύX���� 
WM_IME_COMPOSITIONFULL      �m���̕�����\������E�C���h�E�ɃX�y�[�X���Ȃ� 
WM_IME_CONTROL              IME�ɃR�}���h�𑗂� 
WM_IME_ENDCOMPOSITION       �������m�肵�� 
WM_IME_KEYDOWN              �L�[�������� 
WM_IME_KEYUP                �L�[�𗣂��� 
WM_IME_NOTIFY               IME�E�C���h�E�̕ω����A�v���P�[�V�����ɒm�点�� 
WM_IME_REQUEST              �V�X�e����IME��ύX���悤�Ƃ��Ă��� 
WM_IME_SELECT               �V�X�e����IME��ύX���悤�Ƃ��Ă��� 
WM_IME_SETCONTEXT           IME�̓��̓E�C���h�E���A�N�e�B�u�ɂȂ������A�v���P�[�V�����ɒʒm���� 
WM_IME_STARTCOMPOSITION     �m�肷��O�ɒʒm���� 
WM_INITDIALOG               �_�C�A���O�{�b�N�X���\������� 
WM_INITMENU                 ���j���[���\������钼�O�ɒʒm 
WM_INITMENUPOPUP            �|�b�v�A�b�v���j���[���\������钼�O�ɒʒm 
WM_INPUTLANGCHANGE          ���ꃍ�P�[�����ύX���ꂽ�Ƃ��g�b�v���x���E�B���h�E�ɒʒm 
WM_INPUTLANGCHANGEREQUEST   ���ꃍ�P�[�����ύX���ꂽ�Ƃ��A���̓t�H�[�J�X�����E�B���h�E�ɒʒm 
WM_KEYDOWN                  �L�[�{�[�h�̃L�[�������ꂽ 
WM_KEYUP                    �L�[��������ꂽ 
WM_KILLFOCUS                �t�H�[�J�X���Ȃ��Ȃ钼�O�ɒʒm 
WM_LBUTTONDBLCLK            ���̃}�E�X�{�^�����_�u���N���b�N���� 
WM_LBUTTONDOWN              ���̃}�E�X�{�^���������� 
WM_LBUTTONUP                ���̃}�E�X�{�^����������ꂽ 
WM_MBUTTONDBLCLK            �����̃}�E�X�{�^�����_�u���N���b�N���ꂽ 
WM_MBUTTONDOWN              �����̃}�E�X�{�^���������ꂽ 
WM_MBUTTONUP                �����̃}�E�X�{�^����������ꂽ 
WM_MDIACTIVATE              MDI�Ŏw�肵���q�E�C���h�E���A�N�e�B�u�ɂ��� 
WM_MDICASCADE               MDI�q�E�C���h�E���J�X�P�[�h�\���ɂ��� 
WM_MDICREATE                MDI�q�E�C���h�E���쐬���� 
WM_MDIDESTROY               MDI�q�E�C���h�E��j������ 
WM_MDIGETACTIVE             ���݃A�N�e�B�u��MDI�q�E�C���h�E�̃n���h�����擾 
WM_MDIICONARRANGE           MDI�q�E�C���h�E�̃A�C�R���𐮗� 
WM_MDIMAXIMIZE              MDI�q�E�C���h�E���ő剻 
WM_MDINEXT                  ���̎q�E�C���h�E���A�N�e�B�u�ɂ��� 
WM_MDIRESTORE               �ő剻�E�ŏ�������Ă���q�E�C���h�E�����̃T�C�Y�ɖ߂� 
WM_MDISETMENU               ���j���[��MDI�t���[���E�C���h�E�Ƀ����N 
WM_MDITILE                  MDI�q�E�C���h�E���^�C������ 
WM_MEASUREITEM              �I�[�i�[�`��R���g���[�����쐬���ꂽ�Ƃ� 
WM_MENUCHAR                 ���j���[�̃A�N�Z�X�L�[�̉p�����ƈ�v���Ȃ��������󂯎���� 
WM_MENUSELECT               ���j���[�������\���E�I�����ꂽ 
WM_MOUSEACTIVATE            �}�E�X�N���b�N�ɂ��A�N�e�B�u�E�C���h�E���ڂ��� 
WM_MOUSEMOVE                �}�E�X���ړ����� 
WM_MOUSEWHELL               �}�E�X�z�C�[������]���� 
WM_MOVE                     �E�C���h�E���ړ����� 
WM_MOVING                   ���T�C�Y���ɃE�B���h�E�ɑ����� 
WM_NCACTIVATE               ��N���C�A���g�̈悪�A�N�e�B�u�E��A�N�e�B�u�ł��� 
WM_NCCALCSIZE               �E�C���h�E�̃T�C�Y���Čv�Z���� 
WM_NCCREATE                 �E�C���h�E�̃X�^�C�����ύX���ꂽ���ɍŏ��ɑ����郁�b�Z�[�W 
WM_NCDESTROY                ��N���C�A���g�̈悪�j�� 
WM_NCHITTEST                ��N���C�A���g�̈�̂ǂ̕����Ń}�E�X�����삳�ꂽ�����ׂ� 
WM_NCLBUTTONDBCLK           ��N���C�A���g�̈�ō��{�^�����_�u���N���b�N 
WM_NCLBUTTONDOWN            ��N���C�A���g�̈�ō��{�^�������� 
WM_NCLBUTTONUP              ��N���C�A���g�̈�ō��{�^������� 
WM_NCMBUTTONDBCLK           ��N���C�A���g�̈�Œ����{�^�����_�u���N���b�N 
WM_NCMBUTTONDOWN            ��N���C�A���g�̈�Œ����{�^�������� 
WM_NCMBUTTONUP              ��N���C�A���g�̈�Œ����{�^������� 
WM_NCMOUSEMOVE              ��N���C�A���g�̈�Ń}�E�X���ړ� 
WM_NCPAINT                  ��N���C�A���g�̈�ōăy�C���g���K�v 
WM_NCRBUTTONDBCLK           ��N���C�A���g�̈�ŉE�{�^�����_�u���N���b�N 
WM_NCRBUTTONDOWN            ��N���C�A���g�̈�ŉE�{�^�������� 
WM_NCRBUTTONUP              ��N���C�A���g�̈�ŉE�{�^������� 
WM_NEXTDLGCTL               ���̓t�H�[�J�X��ʂ̎q�E�C���h�E�Ɉړ� 
WM_NEXTMENU                 ���j���[�o�[�܂��̓V�X�e�����j���[�������E�̖��L�[���g�p���Ĉړ������Ƃ� 
WM_NOTIFY                   �R���g���[���ɃC�x���g�������������A�܂��̓R���g���[�����f�[�^��v�����Ă��� 
WM_NULL                     ���ꎩ�͉̂������Ȃ������b�Z�[�W�𑀍삷��ꍇ�Ɏg�� 
WM_PAINT                    �E�C���h�E�̃N���C�A���g�̈���ĕ`�悵�Ȃ���΂Ȃ�Ȃ� 
WM_PAINTCLIPBOARD           �E�C���h�E�̃N���C�A���g�̈���ĕ`�悵�Ȃ���΂Ȃ�Ȃ� 
WM_PAINTICON                �E�C���h�E�̃N���C�A���g�̈���ĕ`�悵�Ȃ���΂Ȃ�Ȃ� 
WM_PALETTECHANGED           �V�X�e���p���b�g���ύX���ꂽ 
WM_PALETTEISCHANGING        �V�X�e���p���b�g���ʂ̃E�C���h�E�ɂ���ĕύX����悤�Ƃ��Ă��� 
WM_PASTE                    �e�L�X�g�{�b�N�X�A�R���{�{�b�N�X�ɂ����ăN���b�v�{�[�h����R�s�[����Ƃ����� 
WM_PARENTNOTIFY             ����C�x���g���q�E�C���h�E�Ŕ����������Ƃ�e�E�C���h�E�ɒʒm���� 
WM_POWER                    �V�X�e�����p���[�ߖ��Ԃɓ���E�o�邱�ƒʒm���� 
WM_POWERBROADCAST           �p���[�Ǘ��C�x���g�����o���邽�߂ɑ����Ă��� 
WM_PRINT                    �w��̃f�o�C�X�R���e�L�X�g�̍ĕ`���v�� 
WM_PRINTCLIENT              �w��̃f�o�C�X�R���e�L�X�g���̃E�B���h�E�̃N���C�A���g�̈�̕`���v�� 
WM_QUERYDRAGICON            �A�C�R����\�����悤�Ƃ��鎞�ɃN���X�A�C�R���������Ȃ��E�C���h�E�ɑ��� 
WM_QUERYENDSESSION          Windows���I�����鎞�ɑS�ẴE�C���h�E�ɖ⍇�킹�� 
WM_QUERYNEWPALETTE          ���̓t�H�[�J�X�����鎞�A�_���p���b�g�Œu�����邩�ǂ����⍇�킹�� 
WM_QUERYOPEN                �A�C�R�������ꂽ�E�C���h�E���������鎞�A���O�̈ʒu�ƃT�C�Y��v������Ă��邩�ǂ����⍇�킹�� 
WM_QUEUESYNC                ���[�U�[���̓��b�Z�[�W�ƃt�b�N�v���V�[�W������̃��b�Z�[�W����ʂ��� 
WM_QUIT                     �A�v���P�[�V��������������Ō�̃��b�Z�[�W 
WM_RBUTTONDBCLK             �E�̃}�E�X�{�^�����_�u���N���b�N���� 
WM_RBUTTONDOWN              �E�}�E�X�{�^���������� 
WM_RBUTTONUP                �E�}�E�X�{�^����������� 
WM_RENDERALLFORMATS         �f�B���C�h�����_�����O���Ă���Ƃ��A�N���b�v�{�[�h�̃I�[�i�[���j�������O�ɑ����� 
WM_RENDERFORMAT             �f�B���C�h�����_�����O�����Ă���f�[�^���A�v���P�[�V�������v�������Ƃ� 
WM_SETCURSOR                �}�E�X���L���v�`�����Ă���E�C���h�E���Ȃ����A�}�E�X������E�C���h�E�ɑ����� 
WM_SETFOCUS                 �E�C���h�E�����̓t�H�[�J�X�𓾂� 
WM_SETFONT                  �_���t�H���g��ݒ肷�� 
WM_SETHOTKEY                �z�b�g�L�[���E�B���h�E�Ɋ֘A�t���� 
WM_SETICON                  �A�C�R�����E�C���h�E�Ɋ֘A�t���� 
WM_SETREDRAW                �R���g���[���̍ĕ`�摀��𐧌䂷�� 
WM_SETTEXT                  �E�C���h�E(�R���g���[��)�̃^�C�g���E�e�L�X�g��ύX 
WM_SETTINGCHANGE            �V�X�e���S�̂ɉe������ݒ�ύX�������Ƃ� 
WM_SHOWWINDOW               �E�C���h�E���\���E��\���ɂȂ��� 
WM_SIZE                     �E�C���h�E�̃T�C�Y���ύX 
WM_SIZECLIPBOARD            �N���b�v�{�[�h�r���[���̃N���C�A���g�̈�̃T�C�Y���ύX�����Ƃ� 
WM_SIZING                   �E�B���h�E�̃T�C�Y���ύX�� 
WM_SPOOLERSTATUS            ����L���[�Ɉ���W���u���ǉ��E�폜���ꂽ 
WM_STYLECHANGED             SetWindowLong�֐��ɂ���ăE�C���h�E�X�^�C����ύX���� 
WM_STYLECHANGING            SetWindowLong�֐��ɂ���ăE�C���h�E�X�^�C����ύX���ꂻ�� 
WM_SYNCPAINT                �ʂ̃X���b�h�ō쐬���ꂽ�E�B���h�E����ɏd�Ȃ����Ƃ� 
WM_SYSCHAR                  [Alt]�L�[�Ƃ̑g�����ȊO��WM_CHAR�Ɠ��� 
WM_SYSCOLORCHANGE           �V�X�e���J���[���ύX���ꂽ 
WM_SYSCOMMAND               �V�X�e�����j���[�����삳�ꂽ 
WM_SYSDEADCHAR              �A�N�Z���g�L���̂悤�ȕʂ̕����Ƒg�������������͂��󂯎���� 
WM_SYSKEYDOWN               �V�X�e���������Ӗ�����L�[�������� 
WM_SYSKEYUP                 �V�X�e���������Ӗ�����L�[��������ꂽ 
WM_TIMECHANGE               �V�X�e���^�C�}���ύX���ꂽ 
WM_TIMER                    SetTimer()�Őݒ肵�����Ԃ��o�� 
WM_UNDO                     �e�L�X�g�{�b�N�X�� Undo ��������s���� 
WM_USER                     ���[�U�[����`�ł��郁�b�Z�[�W�̎g�p�̈��\�������ł��ꎩ�̂ɈӖ��͂Ȃ� 
WM_USERCHANGED              ���O�I���E���O�I�t���ɑS�ẴE�B���h�E�ɑ����� 
WM_VKEYTOITEM               ���X�g�{�b�N�X��WM_KEYDOWN�ɉ��������Ƃ� 
WM_VSCROLL                  ���[�U�[�������X�N���[���o�[�𒲐����� 
WM_VSCROLLCLIPBOARD         �N���b�v�{�[�h�r���[�����ŃX�N���[���C�x���g���� 
WM_WINDOWPOSCHANGED         SetWindowPos()��Ɉʒu�ƃT�C�Y��ύX�����E�C���h�E�ɑ����� 
WM_WINDOWPOSCHANGING        SetWindowPos()�ňʒu�ƃT�C�Y��ύX���Ă���E�C���h�E�ɑ����� 
WM_WININICHANGE             WIN.INI���ύX���ꂽ 
*/

/*
���b�Z�[�W�͈̔� 

���b�Z�[�W��ʂ��ĉ��炩�̒ʒm��v�����s�������ꍇ�A WM_COPYDATA�𗘗p����̂��K���������ʓI�Ƃ͌���܂���B
�ǂ̂悤�ȃ��b�Z�[�W�ł���A����𓖎��ҊԂ��ʐM�̍��}�ƔF���ł���΂悢�̂ł�����A
�Ǝ��ɒ�`�������l�����b�Z�[�W�Ƃ��ė��p����̂��܂��ꋻ�ł��B

�������A���̂悤�ȏꍇ�A���̒�`�������l�������̃��b�Z�[�W�Əd�����Ă͂Ȃ�܂��񂩂�A
�\�ߗ��p�\�ȃ��b�Z�[�W�͈̔͂�m���Ă����Ȃ���΂Ȃ�܂���B 


���b�Z�[�W�͈̔�

�Ӗ�

0����WM_USER - 1    �V�X�e���ɂ���Ē�`����Ă��郁�b�Z�[�W�B 
WM_USER����0x7FFF   �X�̃E�C���h�E�N���X�̂��߂̃��b�Z�[�W�B ���Ƃ��΁A�c�[���`�b�v�R���g���[�����A�N�e�B�u�ɂ���TTM_ACTIVATE�́A WM_USER + 1�Ƃ��Ē�`����Ă���B 
WM_APP����0xBFFF    �A�v���P�[�V���������R�Ɏg���Ă悢�Ƃ����͈́B 
0xC000����0xFFFF    RegisterWindowMessage���Ԃ��l�B 
0xFFFF���傫���l  �����̂��߂ɗ\�񂳂�Ă���B 

���ڂ��ׂ��Ƃ����WM_APP�ł��B ���̃��b�Z�[�W����0xBFFF�܂ł̒l�̓A�v���P�[�V���������R�Ɏg�����Ƃ�������Ă��邽�߁A
�I���W�i���ȃ��b�Z�[�W�Ƃ��ăA�v���P�[�V�����Ԃ̒ʐM�ɗ��p�ł��܂��B
WM_COPYDATA�̓f�[�^�̑��M��ړI�Ƃ������b�Z�[�W�ł����A�P���ɒʒm��v���Ȃǂ��s�������ꍇ��WM_APP�̂ق����K�؂ł��傤�B 

#define WM_LAYOUTCHANGE WM_APP
�E
�E
PostMessage(hwndTarget, WM_LAYOUTCHANGE, 0, 0); // SendMessage�ł��悢


���̃R�[�h�ł́AWM_APP���Ӑ}�I��WM_LAYOUTCHANGE�Ƃ����`�Œ�`���Ă��܂����A
����̓��b�Z�[�W�̖����𖼑O���琄���ł���悤�ɂ��邽�߂ł��B
���Ƃ��΁A�ΏۂƂȂ�E�C���h�E�Ƀ��C�A�E�g�̕ύX�𑣂��̂ł���΁A
����ɑ��������O���g�����ق����R�[�h�͕�����₷���Ȃ�܂��B
wParam��lParam�͎��R�Ɏg�����Ƃ��ł��܂����A
�����܂ő����̂�4�o�C�g�̃f�[�^��2�܂łƂȂ�܂��B
�����̃I���W�i�����b�Z�[�W���K�v�ȏꍇ�́AWM_APP+n(n��0����0xBFFF)�Ƃ����`�Œ�`���܂��B 
*/ 

// �摜�n�̃n�b�N
LRESULT Hook_DefWindowProcACustom(
	HWND hWnd,      // �E�B���h�E�̃n���h��
	UINT Msg,       // ���b�Z�[�W�̎��ʎq
	WPARAM wParam,  // ���b�Z�[�W�̍ŏ��̃p�����[�^
	LPARAM lParam   // ���b�Z�[�W�� 2 �Ԗڂ̃p�����[�^
)
{
	if ( Msg == WM_CREATE ) {
		// DefWindowProc�ɗ���Ă��邱�Ɩ������^�[������Ă���B

	} else if (Msg == WM_COMMAND ) {
		// �����͗��Ȃ��BDDraw�̕����g�����ƁB

	// �}�E�X�n�̏����S��
	} else if ( WM_MOUSEFIRST <= Msg && Msg <= WM_MOUSELAST ) {
		// �����͗L��
		return OnMouseEventHandler( hWnd, Msg, wParam, lParam );

	// �A�v������̃A�C�R�����E�N���������ɏo�Ă���V�X�e���R�}���h��I�񂾂Ƃ�
	} else if ( Msg == WM_SYSCOMMAND ) {
		// �����͗�����̂�����BDDraw�̃t�b�N�̕��͗��Ȃ��B
		return OnSelectingApplicationSystemMenu( hWnd, Msg, wParam, lParam );


	// �|�b�v�A�b�v���j���[(���C���E�B���h�E�ɂ������Ă郁�j���[[=�t�@�C���E�����E�Ȃ�]�̃��C�����j���[]�ȊO�̃��j���[�̂���)�\�����͂������L���ƂȂ�B*/
	} else if ( Msg == WM_NCMOUSEMOVE ) {

		/*WM_MENUSELECT�́A���C�����j���[�̍��ڂ̑I�����ɂ͑����Ă��邱�Ƃ͂���܂���B
		  �N���b�N�����ꍇ�͑�����̂ł����A�ȒP�ȊT�v���Ɏ����Ă������߂ɂ��A ���ڂ̑I�����[���I�ɍČ����鉿�l�͂���ł��傤�B
		  �}�E�X�J�[�\�������C�����j���[��E�C���h�E�^�C�g���Ȃǂ̔�N���C�A���g�̈�� �ʉ߂���Ƃ��ɂ�WM_NCMOUSEMOVE�Ƃ������b�Z�[�W�������邽�߁A
		  ����𗘗p�������ڂ̑I����@���l���Ă݂܂��B*/

		int   nPos;
		POINT pt;
		HMENU hmenu;

		hmenu = GetMenu(CommonGlobalHandle::tenshouWND);

		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		// ���̍��W�Ƀg�b�v���j���[�̍��ڂ͂��邩�H
		nPos = MenuItemFromPoint(CommonGlobalHandle::tenshouWND, hmenu, pt);
		if (nPos != -1) { // ���݂���Ȃ��
			 OnSelectingApplicationTopMenu( hWnd, Msg, wParam, lParam, nPos );
		}

		/*
		MenuItemFromPoint�́A��3�����ŕ\�����ʒu�ɍ��ڂ����邩�ǂ����𒲂ׁA 
		���ڂ��Ȃ����-1��Ԃ��A����΍��ڂ̈ʒu��Ԃ��܂��B
		���̑�3������lParam�ŕ\�����J�[�\���̈ʒu���w�肷��΁A
		�J�[�\�������ڂ�I�����Ă��邩�ǂ������m�F�ł��邽�߁A
		��̏�����WM_MENUSELECT�Ɠ����悤�ɏ������Ƃ��ł��܂��B
		MenuItemFromPoint���Ԃ��͍̂��ڂ̎��ʎq�ł͂Ȃ��ʒu�ł�����A
		GetItemData�̑�3������TRUE�ɂ��܂��B 
		*/
	} else if ( Msg == WM_MENUSELECT ) {
		// �����͗��Ȃ��A��q�̏ꏊ�ŕʓr�t�b�N���Ă���B

	// �|�b�v�A�b�v���j���[(���C�����j���[�̃T�u�Ƃ��ĂԂ牺�����Ă���̂�A����ɂ��̃T�u�̂���)���\������钼�O�Ɏ��s�����B
	} else if ( Msg == WM_INITMENUPOPUP ) {
		return OnViewingApplicationPopUpMenu( hWnd, Msg, wParam, lParam );

	} else if ( WM_KEYFIRST <= Msg && Msg <= WM_KEYLAST  ) {
		// �����͗L���B�������AWM_KEYDOWN�͓V�ċL�ɂ���ĒD���Ă���B
		return OnKeyEventHandler( hWnd, Msg, wParam, lParam );

	// �E�N���b�N�Ƃقړ��`�Blparam�Ȃǂɓ���l���Ⴄ�����B
	} else if ( Msg == WM_CONTEXTMENU ) {
		/*
		POINT pt;

		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		// OutputDebugStream("%d%d", pt.x, pt.y);	

		return 0;
		*/


	// �ŏ�����Ԃ��猳�ւƖ߂��Ƃ�
	} else if ( Msg == WM_QUERYOPEN ) {
		// �����͗��Ȃ��A�ǂ����Ń��^�[������Ă��܂��Ă���B

		// WM_QUERYOPEN��0��Ԃ����ꍇ�A�E�C���h�E�͍ŏ����̏�Ԃ��猳�ɖ߂�܂���B
		// ���̂��߁A���̃��b�Z�[�W�Ń��b�Z�[�W�{�b�N�X��\������΁A �E�C���h�E�����ɖ߂��Ă悢���̊m�F���\�ƂȂ�܂��B
		// 0�ȊO�̒l��Ԃ����ꍇ�́A�E�C���h�E�͌��̃T�C�Y�ɖ߂�܂��B DefWindowProc�́A���̃��b�Z�[�W�ł͏��TRUE��Ԃ��܂��B 

		// return 0;

		// �Q�[���N�����ƁA�I���������A����B����DDraw.dll��WM_PAINT���܂߂��������t�b�N���Ă���̂ŗ��Ȃ��B
	} else if ( Msg == WM_PAINT ) {
		// �����͗��Ȃ��BDDraw�Ńt�b�N���Ă���ł�邱��

	// �t�H�[�J�X���Ȃ��Ȃ钼�O�ɒʒm 
	} else if ( Msg == WM_KILLFOCUS ) {
		// �����͂����Ɨ���

	// �E�C���h�E�����̓t�H�[�J�X�𓾂�
	} else if ( Msg == WM_SETFOCUS ) {
		// �����͂����Ɨ���

	// PCOPYDATASTRUCT�^�f�[�^�̓]���𓾂�
	} else if ( Msg == WM_COPYDATA ) {
		// OutputDebugString("CPYDTA");
		/*
		��M���̗�:

			PCOPYDATASTRUCT pData = (PCOPYDATASTRUCT)lParam;

			if (pData->dwData == 1) dwData�͑��M�`�����l���Ƃł������ׂ�����
				SetWindowText((HWND)wParam, (LPTSTR)pData->lpData);
			}
		*/


		/*
		���M���̐���:

			WM_COPYDATA�́A�K��SendMessage�ő���܂��B

				SendMessage(hwndTarget, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)&data);

			wParam�́A���M���̃E�C���h�E�n���h�����w�肵�܂��B
			lParam�́ACOPYDATASTRUCT�\���̂̃A�h���X�ł��B
			COPYDATASTRUCT�\���̂́A�ȉ��̂悤�ɒ�`����Ă��܂��B

				typedef struct tagCOPYDATASTRUCT {
					ULONG_PTR dwData;	// ���M�`�����l���Ƃł���������
					DWORD cbData;		// lpData�̃f�[�^�T�C�Y(sizeof)
					PVOID lpData;       // ���肽���f�[�^�̃A�h���X
				} COPYDATASTRUCT, *PCOPYDATASTRUCT;


			dwData�́A����ׂ��f�[�^�̎��ʎq���w�肵�܂��B
			WM_COPYDATA���󂯎�������͂��̒l���Q�Ƃ��āA �ǂ̌n���̃f�[�^�������Ă����̂����m�F�ł��܂��B
			cbData�́AlpData���w���������̃T�C�Y�ł��B
			COPYDATASTRUCT�\���̂̃T�C�Y���w�肷��̂ł͂���܂���B
			lpData�́A���肽���f�[�^���i�[���郁�����ւ̃A�h���X���w�肵�܂��B
			���̃����o�ɂ́ANULL���w�肷�邱�Ƃ��ł��܂��B(�f�[�^�𑗂�Ȃ��ĉ����̍��}��������ꍇ)


		���M���̗�:
			TCHAR szData[] = TEXT("hello");
			COPYDATASTRUCT data;

			data.dwData = 1; // ���M�`�����l���ԍ��Ƃł���������
			data.cbData = sizeof(szData);
			data.lpData = szData;
			
			SendMessage(hwndTarget, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)&data);
		*/

	// �A�N�e�B�u���A��A�N�e�B�u���̐؂�ւ��̎�
	} else if ( WM_ACTIVATE ) {
		int fActive =  wParam & 0xFFFF;         // �A�N�e�B�u���t���O 
		/*
		 0 (WA_INACTIVE) ��A�N�e�B�u������܂��B
		 1 (WA_ACTIVE) �}�E�X�N���b�N�ȊO�̕��@�i�Ⴆ�΁A�L�[�{�[�h�C���^�[�t�F�[�X�ɂ��I����A SetActiveWindow �֐��ɂ����́j�ŃA�N�e�B�u������܂��B
		 2 (WA_CLICKACTIVE) �}�E�X�N���b�N
		 */
		// �}�E�X�ɂ���ăA�N�e�B�u�ɂȂ���
		if (fActive == WA_CLICKACTIVE ) {
			if ( IsIconic(CommonGlobalHandle::tenshouWND) ) {
				OpenIcon(CommonGlobalHandle::tenshouWND);
			}
		}

	}


	return 0;
}


// �|�b�v�A�b�v���j���[(���C���E�B���h�E�ɂ������Ă鍀�ڈȊO�̃��j���[�̂���)�\�����͂������L���ƂȂ�B
HRESULT OnViewingApplicationPopUpMenu( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam ) {
	return 0;
}

// �g�b�v���j���[(�V�ċL�̏ꍇ�u�I���v�Ȃ�)��I�񂾍ۂɎ��s�����B
HRESULT OnSelectingApplicationTopMenu(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam, int nPos ) {
	// OutputDebugStream("%d", nPos);
	return 0;
}


// �A�v���̍���A�C�R�����E�N���������ɂł�u�V�X�e�����j���[�v�����s�������B
HRESULT OnSelectingApplicationSystemMenu(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam ) {

	int nId = LOWORD(wParam);

	// �V�X�e�����j���[�ɒǉ����Ă���u�����̈ʒu�ɖ߂��v�̍���
	if (nId == RESOURCE_MENU_ID_CENTER_POS_IN_SYSTEM_MENU ) {

		SetCenterWindow(CommonGlobalHandle::tenshouWND);

	// �X�N���[���Z�C�o�[���N�����悤�Ƃ��Ă���
	} else if ( wParam == SC_SCREENSAVE ) {
		// �����͗L���B
		
	// �ő剻���悤�Ƃ��Ă���
	} else if ( wParam == SC_MAXIMIZE ) {
		// �����͂��Ȃ��B�V�ċL�����^�[�����Ă��܂��Ă���B

	// �ŏ������悤�Ƃ��Ă���
	} else if ( wParam == SC_MAXIMIZE ) {
		// �����͂��Ȃ��B�V�ċL�����^�[�����Ă��܂��Ă���B

	// ���̃T�C�Y�ւƖ߂����Ƃ��Ă���B
	} else if ( wParam == SC_RESTORE ) {
		// �����͗L���B

	}

	return 0;
}





void setMenuEnableOrDisable() {

	struct stat  st;

	// �������G�f�B�^�̏ꍇ�ɁA�����Ƀ��j���[�������݁BID��t����B
	if ( stat( MEMORYEDITOR_FILENAME , &st) == 0 ) {
		AddMenuItem( "�������G�f�B�^", RESOURCE_MENU_ID_MODDEBUGGER_EXECUTE, RESOURCE_MENU_ID_MEMORY_EDITOR_MOD);
	}

	// �Z�[�u�f�[�^�G�f�B�^������
	if ( stat( SAVEDATAEDITOR_FILENAME , &st) != 0 ) {
		// �Z�[�u�f�[�^�G�f�B�^�N���̃��j���[���g���Ȃ��悤�ɂ���B
		EnableMenuItem(CommonGlobalHandle::hMenu, RESOURCE_MENU_ID_SAVEDATA_EXECUTE, MF_GRAYED);
	}

	if ( stat( MODDEBUGGER_FILENAME, &st) != 0 ) {
		// �l�n�c�f�o�b�K�[�N���̃��j���[���폜����B
		DeleteMenu(CommonGlobalHandle::hMenu, RESOURCE_MENU_ID_MODDEBUGGER_EXECUTE, MF_BYCOMMAND);
	}

	// �T�N���G�f�B�^�[�����݂��Ȃ�������c
	if ( stat( SAKURAEDITOR_FILENAME, &st) != 0 ) {
		// �T�N���G�f�B�^�N���̃��j���[���폜����B
		DeleteMenu(CommonGlobalHandle::hMenu, RESOURCE_MENU_ID_SAKURA_EXECUTE, MF_BYCOMMAND);
	}

}


int idSelectMenu = -1;
// �Z�[�u�f�[�^�G�f�B�^���N��
void WINAPI OnTenshouExeWmMessageSaveDataEditor() {
	ShellExecute(CommonGlobalHandle::tenshouWND, NULL, SAVEDATAEDITOR_FILENAME, psznb6savedatafilename, NULL, SW_SHOWNORMAL);
}

// �l�n�c�f�o�b�K�[���N��
void WINAPI OnTenshouExeWmMessageModDebugger() {
	ShellExecute(CommonGlobalHandle::tenshouWND, NULL, MODDEBUGGER_FILENAME, NULL, NULL, SW_SHOWNORMAL);
}

// �T�N���G�f�B�^���N��
void WINAPI OnTenshouExeWmMessageSakuraEditor() {
	ShellExecute(CommonGlobalHandle::tenshouWND, NULL, SAKURAEDITOR_FILENAME, NULL, NULL, SW_SHOWNORMAL);
}

// �c�u�c�ł̃w���v���N��
void WINAPI OnTenshouExeWmMessageDvdVerHelp() {
	ShellExecute(CommonGlobalHandle::tenshouWND, NULL, DVDVER_HELP_FILENAME, NULL, NULL, SW_SHOWNORMAL);
}

// �b�g�l�ł̃w���v���N��
void WINAPI OnTenshouExeWmMessageChmVerHelp() {
	ShellExecute(CommonGlobalHandle::tenshouWND, NULL, CHMVER_HELP_FILENAME, NULL, NULL, SW_SHOWNORMAL);
}

// �w�o�ł�WinHlp32.exe���N��
void WINAPI OnTenshouExeWmMessageXpVerExeHelp() {
	ShellExecute(CommonGlobalHandle::tenshouWND, "open", XPVER_HELPEXE_FILENAME, CDVER_HELP_FILENAME, NULL, SW_SHOWNORMAL);
}


// ���j���[�I��
void SwitchSelectingMenuExecute() {

	// �ȉ������̍��ڂɒǉ��������m
	if (idSelectMenu == RESOURCE_MENU_ID_SAVEDATA_EXECUTE) {
		OnTenshouExeWmMessageSaveDataEditor();	
	} else if (idSelectMenu == RESOURCE_MENU_ID_MODDEBUGGER_EXECUTE) {
		OnTenshouExeWmMessageModDebugger();	
	} else if (idSelectMenu == RESOURCE_MENU_ID_SAKURA_EXECUTE) {
		OnTenshouExeWmMessageSakuraEditor();	
	} else if (idSelectMenu == RESOURCE_MENU_ID_DVD_VER_HELP_EXECUTE) {
		OnTenshouExeWmMessageDvdVerHelp();	
	} else if (idSelectMenu == RESOURCE_MENU_ID_CHM_VER_HELP_EXECUTE) {
		OnTenshouExeWmMessageChmVerHelp();	
	} else if (idSelectMenu == RESOURCE_MENU_ID_XP_VER_HELPEXE_EXECUTE) {
		OnTenshouExeWmMessageXpVerExeHelp();	
	}

	// PluginMod��ApplicationSelectMenuItem���\�b�h
	// �ǂݍ���DLL�Q�ɑ΂��āAOnApplicationSelectMenuItem������΁A��������s�Ȃ���Ή��
	for ( vector<HMODULE>::iterator it = listOfTargetDLL.begin(); it != listOfTargetDLL.end(); it++ ) {

		// DLL���ǂ߂��̂ł���΁AOnFinalize�֐������݂��邩���`�F�b�N�B
		PFNTARGETMODAPPLICATIONSELECTMENU p_OnApplicationSelectMenuItem = (PFNTARGETMODAPPLICATIONSELECTMENU)GetProcAddress(*it, "OnApplicationSelectMenuItem");

		// OnFinalize�����݂���Ύ��s
		if ( !p_OnApplicationSelectMenuItem ) { continue; }

		((PFNTARGETMODAPPLICATIONSELECTMENU) p_OnApplicationSelectMenuItem)(idSelectMenu);
	}

	// ScenarioMod�ő��₷�����̌ďo��
	if (p_snOnApplicationSelectMenuItem) {
		p_snOnApplicationSelectMenuItem(idSelectMenu);
	}


}


/*
WM_COMMAND����ID�̔���
00417013  |. 0FB74424 10    MOVZX   EAX, WORD PTR SS:[ESP+10]
00417018  |. 8BEC           MOV     EBP, ESP
0041701A  |. 83F8 64        CMP     EAX, 64
0041701D  |. 74 1F          JE      SHORT TENSHOU.0041703E
0041701F  |. 83F8 6F        CMP     EAX, 6F
00417022  |. 74 31          JE      SHORT TENSHOU.00417055
00417024  |. 83F8 70        CMP     EAX, 70
00417027  |. 74 49          JE      SHORT TENSHOU.00417072
00417029  |. 83F8 79        CMP     EAX, 79
0041702C  |. 74 6B          JE      SHORT TENSHOU.00417099
0041702E  |. 3D 81000000    CMP     EAX, 81
00417033  |. 0F84 85000000  JE      TENSHOU.004170BE
00417039  |. E9 9B000000    JMP     TENSHOU.004170D9
*/
/*
���ID����̊e�X�ŏ������ׂ����e�����s������́A�ȉ��̍s�ɃW�����v���Ă���B����ăv���O�����ł����̗���ɏK���B
004170D9  |> 33C0           XOR     EAX, EAX
004170DB  |. 5D             POP     EBP
004170DC  \. C3             RETN
*/

int pTenshouExeJumpFromToOnTenshouExeWmMessage			=0x41702E;	// �֐��͂��̃A�h���X����AOnTenshouExeCallWmMessage�ւƃW�����v���Ă���B
int pTenshouExeJumpVersionFromToOnTenshouExeWmMessage	=0x4170BE;  // �u�V�ċL�ɂ��āv�̃o�[�W�����\���p
int pTenshouExeReturnLblFromOnTenshouExeWmMessage		=0x4170D9;	// �֐����Ō�܂ł����ƁA����TENSHOU.EXE���ɒ��ڃW�����v����Ƃ����Ȃ�Ƃ��댯�ȍs��w


// �w���v�̃o�[�W�����\���������ꍇ�ɂ����Ɉ�[�W�����v���āA����ɂ����W�����v
// __asm �� je�R�}���h�Ń|�C���^�łȂ��ƃ_���Ȃ��߁B
__declspec( naked ) void WINAPI OnTenshouExeWmMessageVersionInfo() {
	__asm {
		jmp pTenshouExeJumpVersionFromToOnTenshouExeWmMessage
	}
}
#pragma warning(push)
#pragma warning(disable:4414)
// �ȉ��uC4414�v�̃��[�j���O�̔�������R�[�h�������Ă��A�R���p�C�����ɂ͏o�͂���Ȃ��Ȃ�B

// ���̊֐���TENSHOU.EXE�����b�Z�[�W��ǂ݂��I���邽�тɁA���ڎ��s�����B
// TENSHOU.EXE���Ń��b�Z�[�W���\�z�����^�C�~���O�ł���naked�֐����Ă΂��B
__declspec( naked ) void WINAPI OnTenshouExeWmMessage() {
	// �X�^�b�N�ɂ��߂Ă���
	__asm {
		// ���XTENSHOU.EXE���ɂ��������̂�������
		// �o�[�W������񂾂����ꍇ�́AOnTenshouExeWmMessageVersionInfo �o�R�ł��̂܂܃o�[�W�����\���_�C�A���O�̃A�h���X��
		cmp eax, 0x81							// ���j���[���\�[�X��129 == 0x81 �̓o�[�W�������
		je OnTenshouExeWmMessageVersionInfo		// JE  TENSHOU.004170BE 

		push eax
		push ebx
		push ecx
		push edx
		push esp
		push ebp
		push esi
		push edi
	}

	// eax�ɂ� CallWindowProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )�̂�����wParam�̒l�������Ă���
	// �����ۑ����Ă���
	__asm {
		mov idSelectMenu, eax		// eax���R�s�[���Ă���
	}

	// �ǂ̃��j���[���I�΂ꂽ�̂�����BTENSHOU.EXE�Ƀf�t�H�ł��������̂́ATENSHOU.EXE�� __asm�̐擪�Q�s�ł���Ă�B
	SwitchSelectingMenuExecute();


	// �X�^�b�N�Ɉ����o��
	__asm {
		pop edi
		pop esi
		pop ebp
		pop esp
		pop ecx
		pop edx
		pop ebx
		pop eax

		jmp pTenshouExeReturnLblFromOnTenshouExeWmMessage
	}
}

// �ȏ�uC4414�v�̃��[�j���O�̔�������R�[�h�������Ă��A�R���p�C�����ɂ͏o�͂���Ȃ��Ȃ�B
#pragma warning(pop)

/*
0041702E  |. 3D 81000000    CMP     EAX, 81							; �� OnTenshouExeWmMessage��
							�� ���������s���ɏ��������Ă��K�v������B
00417033  |. 0F84 85000000  JE      TENSHOU.004170BE				; �� nop(=90) ���T��
*/

char cmdOnTenshouExeJumpFromWmMessage[6]="\xE9";


void WriteAsmJumperOnTenshouExeWmMessage() {
	// �܂��A�h���X�𐔎��Ƃ��Ĉ���
	int iAddress = (int)OnTenshouExeWmMessage;
	int SubAddress = iAddress - ( pTenshouExeJumpFromToOnTenshouExeWmMessage + 5 )	;

	// �T�Ƃ����̂́A0041702E  -E9 ????????  JMP TSMod.JmpOnTenshouExeWmMessage  �̖��߂ɕK�v�ȃo�C�g���B�v����ɂT�o�C�g�����Ǝ��̃j�[���j�b�N���ߌQ�Ɉړ�����̂��B�����Ă�������̍������W�����v����ۂ̖ړI�i�Ƃ��ė��p�����B
	memcpy(cmdOnTenshouExeJumpFromWmMessage+1, &SubAddress, 4); // +1 ��E9�̎�����4�o�C�g�����������邩��B


	// �\�z�����j�[���j�b�N���߂�TENSHOU.EXE�̃������ɏ���������
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(pTenshouExeJumpFromToOnTenshouExeWmMessage), cmdOnTenshouExeJumpFromWmMessage, 5, NULL); // 5�o�C�g�̂ݏ�������
}
