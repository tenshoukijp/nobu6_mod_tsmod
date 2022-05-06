#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
 
#define MSG(m) {\
    MessageBoxA(NULL,m,NULL,MB_OK);}
 
//�E�B���h�E�n���h��
HWND hwnd;
//�C���X�^���X�n���h��
HINSTANCE hinst;
 
//�E�B���h�E����
#define WIDTH 500
#define HEIGHT 300
 
 
 
LRESULT CALLBACK WinProc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp ) {
    static MCI_OPEN_PARMS open2;
    static MCI_PLAY_PARMS play2;
    static OPENFILENAME ofn = {0};
    static TCHAR strFile[MAX_PATH] , strCustom[256] = TEXT( "Before files\0*.*\0\0" );
 
    int result;
 
    char buf[1000];
 
    switch( msg ) {
    case WM_DESTROY:
        mciSendCommand( open2.wDeviceID, MCI_CLOSE, 0, 0 );
 
        PostQuitMessage( 0 );
        return 0;
 
    case WM_CREATE:
        ofn.lStructSize = sizeof ( OPENFILENAME );
        ofn.hwndOwner = hwnd;
        ofn.lpstrFilter =   TEXT( "mp3 files {*.mp3}\0*.mp3\0" )
                            TEXT( "All files {*.*}\0*.*\0\0" );
        ofn.lpstrCustomFilter = strCustom;
        ofn.nMaxCustFilter = 256;
        ofn.nFilterIndex = 0;
        ofn.lpstrFile = strFile;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_FILEMUSTEXIST;
 
        return 0;
 
    case WM_RBUTTONUP:
 
        GetOpenFileName( &ofn );
        SetWindowText( hwnd , strFile );
 
        //mp3�Đ��̏ꍇ
        open2.lpstrDeviceType = "MPEGVideo";
        open2.lpstrElementName = strFile;
        result = mciSendCommand( 0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
                                 ( DWORD_PTR )&open2 );
 
        //�G���[�Ȃ�O�ȊO���Ԃ�
        if( result ) {
            //�G���[�擾
            mciGetErrorString( result, buf, sizeof( buf ) );
            MSG( buf );
            PostQuitMessage( 0 );
            return -1;
        }
        
        play2.dwCallback = ( DWORD )hwnd;
        result = mciSendCommand( open2.wDeviceID, MCI_PLAY, MCI_NOTIFY, ( DWORD_PTR )&play2 );
        //�G���[�Ȃ�O�ȊO���Ԃ�
        if( result ) {
            //�G���[�擾
            mciGetErrorString( result, buf, sizeof( buf ) );
            MSG( buf );
            PostQuitMessage( 0 );
            return -1;
        }
        
 
        return 0;
 
 
    case MM_MCINOTIFY:
        if( lp == open2.wDeviceID ) {
 
            if( wp == MCI_NOTIFY_SUCCESSFUL ) {
                MSG( "�Đ�����" );
                //�V�[�N�o�[��擪�ɖ߂�
                mciSendCommand( open2.wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, 0 );
            }
            return 0;
        }
 
    }
    return DefWindowProc( hwnd, msg, wp, lp );
}
 
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd ) {
    MSG msg;
    WNDCLASS wc;
 
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WinProc;
    wc.cbClsExtra = wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hCursor = wc.hIcon = NULL;
    wc.hbrBackground = ( HBRUSH )GetStockObject( BLACK_BRUSH );
    wc.lpszClassName = "test";
    wc.lpszMenuName = NULL;
 
    if( !RegisterClass( &wc ) ) {
        MSG( "�N���X�̓o�^���s" );
        return -1;
    }
 
    //�C���X�^���X�n���h��
    hinst = hInstance;
 
    hwnd = CreateWindowA( "test", "�e�X�g�E�B���h�E", WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                          0, 0, 400, 400, NULL, NULL, hInstance, NULL );
 
 
    if( hwnd == NULL ) {
        MSG( "�E�B���h�E�쐬���s" );
        return -1;
    }
 
 
    //�G���[�`�F�b�N�p�ϐ�
    int check;
 
    while( check = GetMessage( &msg, NULL, 0, 0 ) ) {
        if( check == -1 ) {
            break;
        }
        DispatchMessage( &msg );
    }
 
    //�N���X���
    UnregisterClass( "test", hinst );
 
    return 0;
 
}
