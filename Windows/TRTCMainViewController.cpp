/*
* Module:   TRTCMainViewController
*
* Function: 使用TRTC SDK完成 1v1 和 1vn 的视频通话功能
*
*    1. 支持九宫格平铺和前后叠加两种不同的视频画面布局方式，该部分由 TRTCVideoViewLayout 
*       来计算每个视频画面的位置排布和大小尺寸(window暂未实现九宫格布局)
*
*    2. 支持对视频通话的分辨率、帧率和流畅模式进行调整，该部分由 TRTCSettingViewController 来实现
*
*    3. 创建或者加入某一个通话房间，需要先指定 roomid 和 userid，这部分由 TRTCNewViewController 来实现
*/

#include "stdafx.h"
#include "afxdialogex.h"
#include "TRTCDemo.h"
#include "Base.h"
#include "StorageConfigMgr.h"
#include "TRTCMainViewController.h"
#include "TRTCGetUserIDAndUserSig.h"
#include "TRTCSettingViewController.h"

#include <ctime>
#include <cstdio>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 推荐用户自定义消息至少是WM_USER+100，因为很多新控件也要使用WM_USER消息。
#define WM_CUSTOM_MESSAGE (WM_USER + 100)

TRTCCloud* getTRTCCloud()
{
    if (TRTCMainViewController::g_cloud == nullptr)
    {
        TRTCMainViewController::g_cloud = new TRTCCloud();
    }
    return TRTCMainViewController::g_cloud;
}

void destroyTRTCCloud()
{
    if (TRTCMainViewController::g_cloud != nullptr)
        delete TRTCMainViewController::g_cloud;
    TRTCMainViewController::g_cloud = nullptr;
}

TRTCCloud* TRTCMainViewController::g_cloud = nullptr;
class CCustomMessageWrapper
{
public:
    CCustomMessageWrapper(std::function<void(void)> func) : m_func(func) {}
    ~CCustomMessageWrapper() {}

    void exec()
    {
        if (m_func)
        {
            m_func();
        }
    }
private:
    std::function<void(void)> m_func;
};


// CTRTCDemoDlg 对话框

TRTCMainViewController::TRTCMainViewController(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_TESTTRTCAPP_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void TRTCMainViewController::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(TRTCMainViewController, CDialogEx)
    ON_MESSAGE(WM_CUSTOM_MESSAGE, OnCustomMessage)
    ON_MESSAGE(WM_CUSTOM_CLOSE_SETTINGVIEW, OnMsgSettingViewClose)
    ON_BN_CLICKED(IDC_EXIT_ROOM, &TRTCMainViewController::OnBnClickedExitRoom)
    ON_BN_CLICKED(IDC_BTN_SETTING, &TRTCMainViewController::OnBnClickedSetting)
    ON_BN_CLICKED(IDC_BTN_LOG, &TRTCMainViewController::OnBnClickedLog)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
/*
* 初始化界面控件，包括主要的视频显示View，以及底部的一排功能按钮
*/
BOOL TRTCMainViewController::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    newFont.CreatePointFont(120, L"微软雅黑");

    CWnd *pBtnSetting = GetDlgItem(IDC_BTN_SETTING);
    pBtnSetting->SetFont(&newFont);

    CWnd *pBtnExit = GetDlgItem(IDC_EXIT_ROOM);
    pBtnExit->SetFont(&newFont);

    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    // 执行此操作
    SetIcon(m_hIcon, TRUE);         // 设置大图标
    SetIcon(m_hIcon, FALSE);        // 设置小图标

    getTRTCCloud()->addCallback(this);

    // TODO: 在此添加额外的初始化代码
    UpdateRemoteViewInfo(IDC_REMOTE_VIDEO_VIEW1, "");
    UpdateRemoteViewInfo(IDC_REMOTE_VIDEO_VIEW2, "");
    UpdateRemoteViewInfo(IDC_REMOTE_VIDEO_VIEW3, "");

    ShowWindow(SW_NORMAL);

    CRect rtDesk, rtDlg;
    ::GetWindowRect(::GetDesktopWindow(), &rtDesk);
    GetWindowRect(&rtDlg);
    int iXPos = rtDesk.Width() / 2 - rtDlg.Width() / 2;
    int iYPos = rtDesk.Height() / 2 - rtDlg.Height() / 2;
    SetWindowPos(NULL, iXPos, iYPos, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

/**
* switchToMainThread + CCustomMessageWrapper提供跳转主线程执行task的能力。
* SDK内部会在异步线程回调callback。如果涉及UI更新等操作，必须切回主线程。
*/
void TRTCMainViewController::switchToMainThread(std::function<void(void)> func)
{
    CCustomMessageWrapper* msg = new CCustomMessageWrapper(func);
    PostMessage(WM_CUSTOM_MESSAGE, (WPARAM)msg, 0);
}

void TRTCMainViewController::onEnterRoom(uint64_t elapsed)
{
    //
    switchToMainThread([=] {
        CWnd *pLocalVideoView = GetDlgItem(IDC_LOCAL_VIDEO_VIEW);
        HWND hwnd = pLocalVideoView->GetSafeHwnd();
        getTRTCCloud()->setLocalViewFillMode(TRTCVideoFillMode_Fit);
        getTRTCCloud()->startLocalPreview(hwnd);


        std::vector<UserInfo> userInfos = TRTCGetUserIDAndUserSig::instance().getConfigUserIdArray();
        if (userInfos.empty())
            return;
        UserInfo info = userInfos[0];   // 登录第一个用户
        CWnd *pStatic = GetDlgItem(IDC_STATIC_LOCAL_USERID);
        pStatic->SetWindowTextW(UTF82Wide(info.userId).c_str());
        pStatic->SetFont(&newFont);
    });
}

void TRTCMainViewController::onExitRoom(int reason)
{
    switchToMainThread([=] {
        getTRTCCloud()->removeCallback(this);
        getTRTCCloud()->stopLocalPreview();
        getTRTCCloud()->stopAllRemoteView();
        
        CWnd *pStatic = GetDlgItem(IDC_STATIC_LOCAL_USERID);
        pStatic->SetWindowTextW(L"");

        UpdateRemoteViewInfo(IDC_REMOTE_VIDEO_VIEW1, "");
        UpdateRemoteViewInfo(IDC_REMOTE_VIDEO_VIEW2, "");
        UpdateRemoteViewInfo(IDC_REMOTE_VIDEO_VIEW3, "");

        //切换回登录界面
        ShowWindow(SW_HIDE);
        CWnd* pWnd = GetParent();
        if (pWnd)
        {
            pWnd->ShowWindow(SW_NORMAL);
            ::PostMessage(pWnd->GetSafeHwnd(), WM_CUSTOM_CLOSE_MAINVIEW, 0, 0);
        }
    });
}

void TRTCMainViewController::onUserEnter(const char* userId)
{
    std::string strUserId = userId;
    switchToMainThread([=] {
        int viewId = FindIdleRemoteVideoView();
        if (viewId != 0) 
        {
            UpdateRemoteViewInfo(viewId, strUserId);
            CWnd *pRemoteVideoView = GetDlgItem(viewId);
            HWND hwnd = pRemoteVideoView->GetSafeHwnd();
            getTRTCCloud()->setRemoteViewFillMode(strUserId.c_str(), TRTCVideoFillMode_Fit);
            getTRTCCloud()->startRemoteView(strUserId.c_str(), hwnd);
        }
        else
        {
            // no find view to render remote video
        }
    });
}

void TRTCMainViewController::onUserExit(const char* userId, int reason)
{
    std::string strUserId = userId;
    switchToMainThread([=] {
        int viewId = FindOccupyRemoteVideoView(strUserId);

        if (viewId != 0)
        {
            getTRTCCloud()->stopRemoteView(strUserId.c_str());
            UpdateRemoteViewInfo(viewId, "");
        }
    });
}

HBRUSH TRTCMainViewController::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
    if (nCtlColor == CTLCOLOR_STATIC)
    {
       
        if (pWnd->GetDlgCtrlID() == IDC_STATIC_LOCAL_BORD ||
            pWnd->GetDlgCtrlID() == IDC_STATIC_REMOTE_BORD1 ||
            pWnd->GetDlgCtrlID() == IDC_STATIC_REMOTE_BORD2 || 
            pWnd->GetDlgCtrlID() == IDC_STATIC_REMOTE_BORD3)
        {
            static CBrush brh(RGB(210, 210, 210));//静态画刷资源
            CRect rect;
            if (pWnd->GetDlgCtrlID() == IDC_STATIC_LOCAL_BORD)
                GetDlgItem(IDC_STATIC_LOCAL_BORD)->GetClientRect(rect);
            if (pWnd->GetDlgCtrlID() == IDC_STATIC_REMOTE_BORD1)
                GetDlgItem(IDC_STATIC_REMOTE_BORD1)->GetClientRect(rect);
            if (pWnd->GetDlgCtrlID() == IDC_STATIC_REMOTE_BORD2)
                GetDlgItem(IDC_STATIC_REMOTE_BORD2)->GetClientRect(rect);
            if (pWnd->GetDlgCtrlID() == IDC_STATIC_REMOTE_BORD3)
                GetDlgItem(IDC_STATIC_REMOTE_BORD3)->GetClientRect(rect);

            rect.InflateRect(-3, -7, -3, -3);
            pDC->FillRect(rect, &brh);//填充Groupbox矩形背景色
            return (HBRUSH)brh.m_hObject;//返回Groupbox背景画刷绘制画刷
        }

        if (pWnd->GetDlgCtrlID() == IDC_STATIC_LOCAL_USERID ||
            pWnd->GetDlgCtrlID() == IDC_STATIC_REMOTE_USERID1 ||
            pWnd->GetDlgCtrlID() == IDC_STATIC_REMOTE_USERID2 ||
            pWnd->GetDlgCtrlID() == IDC_STATIC_REMOTE_USERID3)
        {
            static CBrush brh(RGB(210, 210, 210));//静态画刷资源
            CRect rect;
            if (pWnd->GetDlgCtrlID() == IDC_STATIC_LOCAL_USERID)
                GetDlgItem(IDC_STATIC_LOCAL_USERID)->GetClientRect(rect);
            if (pWnd->GetDlgCtrlID() == IDC_STATIC_REMOTE_USERID1)
                GetDlgItem(IDC_STATIC_REMOTE_USERID1)->GetClientRect(rect);
            if (pWnd->GetDlgCtrlID() == IDC_STATIC_REMOTE_USERID2)
                GetDlgItem(IDC_STATIC_REMOTE_USERID2)->GetClientRect(rect);
            if (pWnd->GetDlgCtrlID() == IDC_STATIC_REMOTE_USERID3)
                GetDlgItem(IDC_STATIC_REMOTE_USERID3)->GetClientRect(rect);
            pDC->FillRect(rect, &brh);//填充Groupbox矩形背景色
            pDC->SetBkMode(TRANSPARENT);
            return (HBRUSH)brh.m_hObject;//返回Groupbox背景画刷绘制画刷
        }
    }
    return hbr;
}

int TRTCMainViewController::FindIdleRemoteVideoView()
{
    if (m_remoteUserInfo[IDC_REMOTE_VIDEO_VIEW1].compare("") == 0)
    {
        return IDC_REMOTE_VIDEO_VIEW1;
    }
    if (m_remoteUserInfo[IDC_REMOTE_VIDEO_VIEW2].compare("") == 0)
    {
        return IDC_REMOTE_VIDEO_VIEW2;
    }
    if (m_remoteUserInfo[IDC_REMOTE_VIDEO_VIEW3].compare("") == 0)
    {
        return IDC_REMOTE_VIDEO_VIEW3;
    }
    return 0;
}

int TRTCMainViewController::FindOccupyRemoteVideoView(std::string userId)
{
    if (m_remoteUserInfo[IDC_REMOTE_VIDEO_VIEW1].compare(userId.c_str()) == 0)
    {
        return IDC_REMOTE_VIDEO_VIEW1;
    }
    if (m_remoteUserInfo[IDC_REMOTE_VIDEO_VIEW2].compare(userId.c_str()) == 0)
    {
        return IDC_REMOTE_VIDEO_VIEW2;
    }
    if (m_remoteUserInfo[IDC_REMOTE_VIDEO_VIEW3].compare(userId.c_str()) == 0)
    {
        return IDC_REMOTE_VIDEO_VIEW3;
    }
    return 0;
}

void TRTCMainViewController::UpdateRemoteViewInfo(int id, std::string userId)
{
    if (id == IDC_REMOTE_VIDEO_VIEW1)
    {
        CWnd *pStatic = GetDlgItem(IDC_STATIC_REMOTE_USERID1);
        pStatic->SetWindowTextW(UTF82Wide(userId).c_str());
        pStatic->SetFont(&newFont);
        m_remoteUserInfo[id] = userId;
    }
    if (id == IDC_REMOTE_VIDEO_VIEW2)
    {
        CWnd *pStatic = GetDlgItem(IDC_STATIC_REMOTE_USERID2);
        pStatic->SetWindowTextW(UTF82Wide(userId).c_str());
        pStatic->SetFont(&newFont);
        m_remoteUserInfo[id] = userId;
    }
    if (id == IDC_REMOTE_VIDEO_VIEW3)
    {
        CWnd *pStatic = GetDlgItem(IDC_STATIC_REMOTE_USERID3);
        pStatic->SetWindowTextW(UTF82Wide(userId).c_str());
        pStatic->SetFont(&newFont);
        m_remoteUserInfo[id] = userId;
    }
}

LRESULT TRTCMainViewController::OnCustomMessage(WPARAM wParam, LPARAM lParam)
{
    CCustomMessageWrapper* msg = reinterpret_cast<CCustomMessageWrapper*>(wParam);
    if (msg)
    {
        msg->exec();
        delete msg;
    }

    return S_OK;
}

LRESULT TRTCMainViewController::OnMsgSettingViewClose(WPARAM wParam, LPARAM lParam)
{
    if (m_pTRTCSettingViewController != nullptr)
    {
        delete m_pTRTCSettingViewController;
        m_pTRTCSettingViewController = nullptr;
    }
    SetForegroundWindow();
    return LRESULT();
}

/**
* 加入视频房间：需要 TRTCNewViewController 提供的  TRTCVideoEncParam 函数
*/
void TRTCMainViewController::enterRoom(TRTCParams& params)
{
    // 大画面的编码器参数设置
    // 设置视频编码参数，包括分辨率、帧率、码率等等，这些编码参数来自于 TRTCSettingViewController 的设置
    // 注意（1）：不要在码率很低的情况下设置很高的分辨率，会出现较大的马赛克
    // 注意（2）：不要设置超过25FPS以上的帧率，因为电影才使用24FPS，我们一般推荐15FPS，这样能将更多的码率分配给画质
    TRTCVideoEncParam& encParams = TRTCStorageConfigMgr::GetInstance()->videoEncParams;
    TRTCQosMode& qosMode = TRTCStorageConfigMgr::GetInstance()->qosMode;
    TRTCVideoQosPreference qosPreference = TRTCStorageConfigMgr::GetInstance()->videoQosPreference;
    getTRTCCloud()->setLocalVideoQuality(encParams, qosMode, qosPreference);
    
    bool m_bPushSmallVideo = TRTCStorageConfigMgr::GetInstance()->bPushSmallVideo;
    bool m_bPlaySmallVideo = TRTCStorageConfigMgr::GetInstance()->bPlaySmallVideo;


    if (m_bPushSmallVideo)
    {
        //小画面的编码器参数设置
        //TRTC SDK 支持大小两路画面的同时编码和传输，这样网速不理想的用户可以选择观看小画面
        //注意：iPhone & Android 不要开启大小双路画面，非常浪费流量，大小路画面适合 Windows 和 MAC 这样的有线网络环境
        TRTCVideoEncParam param;
        param.videoFps = 15;
        param.videoBitrate = 100;
        param.videoResolution = TRTCVideoResolution_320_240;
        getTRTCCloud()->enableSmallVideoStream(true, param);
    }
    if (m_bPlaySmallVideo)
    {
        getTRTCCloud()->setPriorRemoteVideoStreamType(TRTCVideoStreamTypeSmall);
    }

    getTRTCCloud()->enterRoom(params);

    std::wstring title = format(L"TRTCDemo【房间ID: %d, 用户ID: %s】", params.roomId, Ansi2Wide(params.userId.c_str()).c_str());

    SetWindowText(title.c_str());

}

void TRTCMainViewController::OnBnClickedExitRoom()
{
    getTRTCCloud()->exitRoom();
    CWnd *pExitRoomBtn = GetDlgItem(IDC_EXIT_ROOM);
    pExitRoomBtn->EnableWindow(FALSE);
}

void TRTCMainViewController::OnBnClickedSetting()
{
    if (m_pTRTCSettingViewController == nullptr)
    {
        m_pTRTCSettingViewController = new TRTCSettingViewController(this);
        m_pTRTCSettingViewController->Create(IDD_DIALOG_SETTING, this);
        m_pTRTCSettingViewController->ShowWindow(SW_SHOW);
    }
}

void TRTCMainViewController::OnBnClickedLog()
{
    m_showDebugView++;
    int style = m_showDebugView % 3;
    if (getTRTCCloud())
        TRTCCloud::showDebugView(style);
}
